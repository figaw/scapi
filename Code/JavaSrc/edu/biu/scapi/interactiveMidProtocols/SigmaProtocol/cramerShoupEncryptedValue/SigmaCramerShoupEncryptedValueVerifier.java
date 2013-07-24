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
package edu.biu.scapi.interactiveMidProtocols.SigmaProtocol.cramerShoupEncryptedValue;

import java.io.IOException;
import java.math.BigInteger;
import java.security.SecureRandom;
import java.util.ArrayList;

import edu.biu.scapi.exceptions.InvalidDlogGroupException;
import edu.biu.scapi.interactiveMidProtocols.SigmaProtocol.DlogBasedSigma;
import edu.biu.scapi.interactiveMidProtocols.SigmaProtocol.SigmaVerifierComputation;
import edu.biu.scapi.interactiveMidProtocols.SigmaProtocol.dhExtended.SigmaDHExtendedInput;
import edu.biu.scapi.interactiveMidProtocols.SigmaProtocol.dhExtended.SigmaDHExtendedVerifier;
import edu.biu.scapi.interactiveMidProtocols.SigmaProtocol.utility.SigmaProtocolInput;
import edu.biu.scapi.interactiveMidProtocols.SigmaProtocol.utility.SigmaProtocolMsg;
import edu.biu.scapi.midLayer.asymmetricCrypto.keys.CramerShoupPublicKey;
import edu.biu.scapi.midLayer.ciphertext.CramerShoupOnGroupElementCiphertext;
import edu.biu.scapi.primitives.dlog.DlogGroup;
import edu.biu.scapi.primitives.dlog.GroupElement;
import edu.biu.scapi.primitives.dlog.cryptopp.CryptoPpDlogZpSafePrime;
import edu.biu.scapi.primitives.dlog.miracl.MiraclDlogECF2m;
import edu.biu.scapi.primitives.hash.CryptographicHash;
import edu.biu.scapi.primitives.hash.cryptopp.CryptoPpSHA1;

/**
 * Concrete implementation of Sigma Protocol verifier computation. <p>
 * 
 * This protocol is used to prove that the value encrypted under Cramer-Shoup in the ciphertext (u1,u2,e,v) 
 * with public-key g1,g2,c,d,h is x. <p>
 * The protocol is for the case that the prover knows the randomness used to encrypt.
 * 
 * @author Cryptography and Computer Security Research Group Department of Computer Science Bar-Ilan University (Moriya Farbstein)
 *
 */
public class SigmaCramerShoupEncryptedValueVerifier implements SigmaVerifierComputation, DlogBasedSigma{

	/*	
	  This class uses an instance of SigmaDHExtendedVerifier with:
	  
	  		�	Common DlogGroup
			�	Common input: (g1,g2,g3,g4,h1,h2,h3,h4) = (g1,g2,h,cd^w,u1,u2,e/x,v)
	*/	
	
	private SigmaDHExtendedVerifier sigmaDH;		//underlying SigmaDHExtendedVerifier to use.
	private DlogGroup dlog;							//We save the dlog because we need it to calculate the input for the underlying Sigma verifier.
	private CryptographicHash hash;					//Underlying hash function that used in the CramerShoup cryptosystem.
	
	/**
	 * Constructor that gets the underlying DlogGroup, CryptographicHash, soundness parameter and SecureRandom.
	 * @param dlog DlogGroup used in CramerShoup encryption scheme.
	 * @param hash CryptographicHash used in CramerShoup encryption scheme. 
	 * @param t Soundness parameter in BITS.
	 * @param random
	 * @throws InvalidDlogGroupException if the given dlog is invalid.
	 */
	public SigmaCramerShoupEncryptedValueVerifier(DlogGroup dlog, CryptographicHash hash, int t, SecureRandom random) throws InvalidDlogGroupException {
		
		setParameters(dlog, hash, t, random);
	}
	
	/**
	 * Default constructor that chooses default values for the parameters.
	 */
	public SigmaCramerShoupEncryptedValueVerifier() {
	
		DlogGroup dlogTemp;
		try {
			//Create Miracl Koblitz 233 Elliptic curve.
			dlogTemp = new MiraclDlogECF2m("K-233");
		} catch (IOException e) {
			//If there is a problem with the elliptic curves file, create Zp DlogGroup.
			dlogTemp = new CryptoPpDlogZpSafePrime();
		}
		
		try {
			setParameters(dlogTemp, new CryptoPpSHA1(), 80, new SecureRandom());
		} catch (InvalidDlogGroupException e) {
			// Can not occur since the DlogGroup is valid.
		}
	}
	
	/**
	 * Sets the given parameters.
	 * @param dlog DlogGroup used in CramerShoup encryption scheme.
	 * @param hash CryptographicHash used in CramerShoup encryption scheme.
	 * @param t Soundness parameter in BITS.
	 * @param random
	 * @throws InvalidDlogGroupException if the given DlogGroup is not valid.
	 */
	private void setParameters(DlogGroup dlog, CryptographicHash hash, int t, SecureRandom random) throws InvalidDlogGroupException {
		//Creates the underlying SigmaDHExtendedVerifier object with the given parameters.
		sigmaDH = new SigmaDHExtendedVerifier(dlog, t, random);
		this.dlog = dlog;
		this.hash = hash;
	}
	
	/**
	 * Returns the soundness parameter for this Sigma protocol.
	 * @return t soundness parameter
	 */
	public int getSoundness(){
		//Delegates to the underlying sigmaDHExtended verifier.
		return sigmaDH.getSoundness();
	}

	/**
	 * Sets the input for this Sigma protocol.
	 * @param input MUST be an instance of SigmaCramerShoupEncryptedValueInput.
	 * @throws IllegalArgumentException if input is not the expected.
	 */
	public void setInput(SigmaProtocolInput in) {
		if (!(in instanceof SigmaCramerShoupEncryptedValueInput)){
			throw new IllegalArgumentException("the given input must be an instance of SigmaCramerShoupEncryptedValueInput");
		}
		
		//Gets the input values.
		SigmaCramerShoupEncryptedValueInput input = (SigmaCramerShoupEncryptedValueInput) in;
		CramerShoupPublicKey publicKey = input.getPublicKey();
		CramerShoupOnGroupElementCiphertext cipher = input.getCipher();
		GroupElement x = input.getX();
		
		//Prepare the input for the underlying SigmaDHExtendedVerifier.
		ArrayList<GroupElement> gArray = new ArrayList<GroupElement>();
		ArrayList<GroupElement> hArray = new ArrayList<GroupElement>();
		
		//Converts the given input to the necessary input to the underlying SigmaDHExtendedVerifier.
		//(g1,g2,g3,g4,h1,h2,h3,h4) = (g1,g2,h,cd^w,u1,u2,e/x,v)
		
		//add the input for the gArray:
		gArray.add(publicKey.getGenerator1()); //add g1 = g1.
		gArray.add(publicKey.getGenerator2()); //add g2 = g2.
		gArray.add(publicKey.getH());		   //add g3 = h.
		
		//Compute w = H(u1,u2,e).
		BigInteger q = dlog.getOrder();
		BigInteger w = calcW(cipher.getU1(), cipher.getU2(), cipher.getE()).mod(q);
		//Compute cd^w. such that w = H(u1,u2,e).
		GroupElement dToW = dlog.exponentiate(publicKey.getD(), w);
		GroupElement g4 = dlog.multiplyGroupElements(publicKey.getC(), dToW);
		gArray.add(g4);		   				   //add g4 = cd^w.
		
		//add the input for the hArray:
		hArray.add(cipher.getU1());			   //add h1 = u1.
		hArray.add(cipher.getU2());			   //add h2 = u2.
		//Compute e/x = e*x^(-1)
		GroupElement xInverse = dlog.getInverse(x);
		GroupElement h3 = dlog.multiplyGroupElements(cipher.getE(), xInverse);
		hArray.add(h3);			   			   //add h3 = e/x.
		hArray.add(cipher.getV());			   //add h4 = v.
		
		//Create an input object to the underlying sigma DHExtended verifier.
		SigmaDHExtendedInput underlyingInput = new SigmaDHExtendedInput(gArray, hArray);
		sigmaDH.setInput(underlyingInput);
		
	}
	
	/**
	 * Receives three byte arrays and calculates the hash function on their concatenation.
	 * @param u1ToByteArray
	 * @param u2ToByteArray
	 * @param eToByteArray
	 * @return the result of hash(u1ToByteArray+u2ToByteArray+eToByteArray) as BigInteger.
	 */
	protected BigInteger calcW(GroupElement u1, GroupElement u2, GroupElement e) {
		
		byte[] u1ToByteArray = dlog.mapAnyGroupElementToByteArray(u1);
		byte[] u2ToByteArray = dlog.mapAnyGroupElementToByteArray(u2);
		byte[] eToByteArray = dlog.mapAnyGroupElementToByteArray(e);
		
		//Concatenates u1, u2 and e into msgToHash.
		int lengthOfMsgToHash =  u1ToByteArray.length + u2ToByteArray.length + eToByteArray.length;
		byte[] msgToHash = new byte[lengthOfMsgToHash];
		System.arraycopy(u1ToByteArray, 0, msgToHash, 0, u1ToByteArray.length);
		System.arraycopy(u2ToByteArray, 0, msgToHash, u1ToByteArray.length, u2ToByteArray.length);
		System.arraycopy(eToByteArray, 0, msgToHash, u2ToByteArray.length+u1ToByteArray.length, eToByteArray.length);
		
		//Calculates the hash of msgToHash.
		
		//Calls the update function in the Hash interface.
		hash.update(msgToHash, 0, msgToHash.length);

		//Gets the result of hashing the updated input.
		byte[] alpha = new byte[hash.getHashedMsgSize()];
		hash.hashFinal(alpha, 0);
		
		return new BigInteger(alpha);
	}
	
	/**
	 * Samples the challenge e <- {0,1}^t
	 */
	public void sampleChallenge(){
		//Delegates to the underlying Sigma DHExtended verifier.
		sigmaDH.sampleChallenge();
	}
	
	/**
	 * Sets the given challenge.
	 * @param challenge
	 */
	public void setChallenge(byte[] challenge){
		//Delegates to the underlying Sigma DHExtended verifier.
		sigmaDH.setChallenge(challenge);
	}
	
	/**
	 * Returns the sampled challenge.
	 * @return the challenge.
	 */
	public byte[] getChallenge(){
		//Delegates to the underlying Sigma DHExtended verifier.
		return sigmaDH.getChallenge();
	}

	/**
	 * Verifies the proof.
	 * @param z second message from prover
	 * @return true if the proof has been verified; false, otherwise.
	 * @throws IllegalArgumentException if the first message of the prover is not an instance of SigmaDHExtendedMsg
	 * @throws IllegalArgumentException if the second message of the prover is not an instance of SigmaBIMsg
	 */
	public boolean verify(SigmaProtocolMsg a, SigmaProtocolMsg z) {
		//Delegates to the underlying Sigma DHExtended verifier.
		return sigmaDH.verify(a, z);
	}

}