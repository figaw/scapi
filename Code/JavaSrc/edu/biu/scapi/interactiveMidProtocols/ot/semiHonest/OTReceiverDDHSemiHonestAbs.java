/**
* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
* 
* Copyright (c) 2012 - SCAPI (http://crypto.biu.ac.il/scapi)
* This file is part of the SCAPI project.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
* and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
* We request that any publication and/or code referring to and/or based on SCAPI contain an appropriate citation to SCAPI, including a reference to
* http://crypto.biu.ac.il/SCAPI.
* 
* SCAPI uses Crypto++, Miracl, NTL and Bouncy Castle. Please see these projects for any further licensing issues.
* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
* 
*/
package edu.biu.scapi.interactiveMidProtocols.ot.semiHonest;

import java.io.IOException;
import java.io.Serializable;
import java.math.BigInteger;
import java.security.SecureRandom;

import org.bouncycastle.util.BigIntegers;

import edu.biu.scapi.comm.Channel;
import edu.biu.scapi.exceptions.FactoriesException;
import edu.biu.scapi.exceptions.SecurityLevelException;
import edu.biu.scapi.generals.ScapiDefaultConfiguration;
import edu.biu.scapi.interactiveMidProtocols.ot.OTRBasicInput;
import edu.biu.scapi.interactiveMidProtocols.ot.OTRMessage;
import edu.biu.scapi.interactiveMidProtocols.ot.OTRInput;
import edu.biu.scapi.interactiveMidProtocols.ot.OTROutput;
import edu.biu.scapi.interactiveMidProtocols.ot.OTReceiver;
import edu.biu.scapi.interactiveMidProtocols.ot.OTSMessage;
import edu.biu.scapi.primitives.dlog.DlogGroup;
import edu.biu.scapi.primitives.dlog.GroupElement;
import edu.biu.scapi.securityLevel.DDH;
import edu.biu.scapi.tools.Factories.DlogGroupFactory;

/**
 * Abstract class for Semi-Honest OT assuming DDH receiver.
 * Semi-Honest OT have two modes: one is on ByteArray and the second is on GroupElement.
 * The different is in the input and output types and the way to process them. 
 * In spite that, there is a common behavior for both modes which this class is implementing.
 * 
 * @author Cryptography and Computer Security Research Group Department of Computer Science Bar-Ilan University (Moriya Farbstein)
 *
 */
abstract class OTReceiverDDHSemiHonestAbs implements OTReceiver{
	/*	
	 	This class runs the following protocol:
		 	SAMPLE random values alpha in Zq and h in the DlogGroup 
			COMPUTE h0,h1 as follows:
				1.	If sigma = 0 then h0 = g^alpha  and h1 = h
				2.	If sigma = 1 then h0 = h and h1 = g^alpha 
			SEND (h0,h1) to S
			WAIT for the message (u, v0,v1) from S
			COMPUTE kSigma = (u)^alpha							- in byte array scenario
				 OR (kSigma)^(-1) = u^(-alpha)					- in GroupElement scenario
			OUTPUT  xSigma = vSigma XOR KDF(|cSigma|,kSigma)	- in byte array scenario
				 OR xSigma = vSigma * (kSigma)^(-1) 			- in GroupElement scenario
	*/	
	
	protected DlogGroup dlog;
	private SecureRandom random;
	private BigInteger qMinusOne;
	
	/**
	 * Constructor that chooses default values of DlogGroup and SecureRandom.
	 */
	OTReceiverDDHSemiHonestAbs(){
		//Read the default DlogGroup name from a configuration file.
		String dlogName = ScapiDefaultConfiguration.getInstance().getProperty("DDHDlogGroup");
		DlogGroup dlog = null;
		try {
			//Create the default DlogGroup by the factory.
			dlog = DlogGroupFactory.getInstance().getObject(dlogName);
			
		} catch (FactoriesException e1) {
			// Should not occur since the dlog name in the configuration file is valid.
		}
		
		try {
			doConstruct(dlog, new SecureRandom());
		} catch (SecurityLevelException e1) {
			// Should not occur since the dlog in the configuration file is as secure as needed.
		}
	}

	/**
	 * Constructor that sets the given dlogGroup and random.
	 * @param dlog must be DDH secure.
	 * @param random
	 * @throws SecurityLevelException if the given dlog is not DDH secure.
	 */
	OTReceiverDDHSemiHonestAbs(DlogGroup dlog, SecureRandom random) throws SecurityLevelException{
		
		doConstruct(dlog, random);
	}
	
	/**
	 * Sets the given members.
	 * @param dlog must be DDH secure.
	 * @param random
	 * @throws SecurityLevelException if the given dlog is not DDH secure.
	 */
	private void doConstruct(DlogGroup dlog, SecureRandom random) throws SecurityLevelException {
		//The underlying dlog group must be DDH secure.
		if (!(dlog instanceof DDH)){
			throw new SecurityLevelException("DlogGroup should have DDH security level");
		}
		
		this.dlog = dlog;
		this.random = random;
		qMinusOne =  dlog.getOrder().subtract(BigInteger.ONE);
		
		// This protocol has no pre process stage.
		
	}
	
	/**
	 * Runs the part of the protocol where the receiver input is necessary.
	 * The transfer stage of OT protocol which can be called several times in parallel.
	 * In order to enable the parallel calls, each transfer call should use a different channel to send and receive messages.
	 * This way the parallel executions of the function will not block each other.
	 * The parameters given in the input must match the DlogGroup member of this class, which given in the constructor.
	 * @param input MUST be OTRBasicInput.
	 * @return OTROutput, the output of the protocol.
	 * @throws IOException if failed to send or receive a message.
	 * @throws ClassNotFoundException
	 */
	public OTROutput transfer(Channel channel, OTRInput input) throws IOException, ClassNotFoundException{
		//check if the input is valid.
		//If input is not instance of OTRBasicInput, throw Exception.
		if (!(input instanceof OTRBasicInput)){
			throw new IllegalArgumentException("input shoud contain sigma.");
		}
		
		byte sigma = ((OTRBasicInput) input).getSigma();
		//The given sigma should be 0 or 1.
		if ((sigma != 0) && (sigma!= 1)){
			throw new IllegalArgumentException("Sigma should be 0 or 1");
		}
		
		/* Run the protocol:
				SAMPLE random values alpha in  [0, . . . , q-1]  and h in the DlogGroup
				COMPUTE h0,h1 as follows:
					1.	If sigma = 0 then h0 = g^alpha  and h1 = h
					2.	If sigma = 1 then h0 = h and h1 = g^alpha 
				SEND (h0,h1) to S
				WAIT for the message (u, v0,v1) from S
				COMPUTE kSigma = (u)^alpha							- in byte array scenario
					OR (kSigma)^(-1) = u^(-alpha)					- in GroupElement scenario
				OUTPUT  xSigma = vSigma XOR KDF(|cSigma|,kSigma)	- in byte array scenario
					 OR xSigma = vSigma * (kSigma)^(-1) 			- in GroupElement scenario
		*/
		
		//Sample random alpha
		BigInteger alpha = BigIntegers.createRandomInRange(BigInteger.ZERO, qMinusOne, random);
		
		//Compute h0, h1
		OTRMessage tuple = computeTuple(alpha, sigma);
		
		//Send the tuple to sender
		sendTupleToSender(channel, tuple);
		
		//Wait for message from sender
		OTSMessage message = waitForMessageFromSender(channel);
		
		//Compute xSigma
		return computeFinalXSigma(sigma, alpha, message);
	
	}

	/**
	 * Runs the following lines from the protocol:
	 *  COMPUTE h0,h1 as follows:
	 *		1.	If sigma = 0 then h0 = g^alpha  and h1 = h
	 *		2.	If sigma = 1 then h0 = h and h1 = g^alpha"
	 * @param alpha random value sampled by the protocol
	 * @param sigma input for the protocol
	 * @return OTRSemiHonestMessage contains the tuple (h0, h1).
	 */
	private OTRMessage computeTuple(BigInteger alpha, byte sigma) {
		
		//Sample random h.
		GroupElement h = dlog.createRandomElement();
		
		//Calculate g^alpha.
		GroupElement g = dlog.getGenerator();
		GroupElement gAlpha = dlog.exponentiate(g, alpha);
				
		GroupElement h0 = null;
		GroupElement h1 = null;
		if (sigma == 0){
			h0 = gAlpha;
			h1 = h;
		}
		if (sigma == 1){
			h0 = h;
			h1 = gAlpha;
		}
		return new OTRMessage(h0.generateSendableData(), h1.generateSendableData());
	}
	
	/**
	 * Runs the following line from the protocol:
	 * "SEND (h0,h1) to S"
	 * @param channel 
	 * @param tuple contains (h0,h1)
	 * @throws IOException if failed to send the message.
	 */
	private void sendTupleToSender(Channel channel, OTRMessage tuple) throws IOException {
		try {
			channel.send(tuple);
		} catch (IOException e) {
			throw new IOException("failed to send the message. The thrown message is: " + e.getMessage());
		}
		
	}
	
	/**
	 * Runs the following line from the protocol:
	 * "WAIT for the message (u, v0,v1) from S"
	 * @param channel 
	 * @return OTSMessage contains (u, v0,v1)
	 * @throws ClassNotFoundException
	 * @throws IOException if failed to receive a message.
	 */
	private OTSMessage waitForMessageFromSender(Channel channel) throws ClassNotFoundException, IOException {
		Serializable message;
		try {
			message = channel.receive();
		} catch (IOException e) {
			throw new IOException("failed to receive message. The thrown message is: " + e.getMessage());
		}
		if (!(message instanceof OTSMessage)){
			throw new IllegalArgumentException("the given message should be an instance of OTSMessage");
		}
		return (OTSMessage) message;
	}
	
	/**
	 * Runs the following lines from the protocol:
	 * "COMPUTE kSigma = (u)^alpha							- in byte array scenario
			OR (kSigma)^(-1) = u^(-alpha)					- in GroupElement scenario
		OUTPUT  xSigma = vSigma XOR KDF(|cSigma|,kSigma)	- in byte array scenario
			 OR xSigma = vSigma * (kSigma)^(-1) 			- in GroupElement scenario
	 * @param sigma input for the protocol
	 * @param alpha random value sampled by the protocol
	 * @param message received from the sender
	 * @return OTROutput contains XSigma
	 */
	protected abstract OTROutput computeFinalXSigma(byte sigma, BigInteger alpha, OTSMessage message);
}
