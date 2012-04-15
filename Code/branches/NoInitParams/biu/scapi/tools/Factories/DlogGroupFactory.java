/*
 * DlogGroupFactory has a member of type FactoriesUtility to which it delegates the actual creation of the object. 
 * This ensures proper code re-use.The 
 * factories have two getObject methods to retrieve an algorithm compatible with the type; 
 * one specifies the provider and the other one relies on a default provider.
 */

package edu.biu.scapi.tools.Factories;

import edu.biu.scapi.exceptions.FactoriesException;
import edu.biu.scapi.primitives.dlog.DlogGroup;

/**
 * DlogGroupFactory is in charge of creating instances of different Dlog groups. Its implemented as a singleton.
 * Once you have an instance of the factory, you can create a Dlog group by means of calling one of the two getObject methods provided.
 * @author Moriya
 *
 */
public final class DlogGroupFactory {
	
	private FactoriesUtility factoriesUtility;
	private static DlogGroupFactory instance = new DlogGroupFactory();

	
	/*
	 * Private constructor since this class is of the singleton pattern. 
         * It creates an instance of FactoriesUtility and passes a predefined file names to the constructor
         * of FactoriesUtility.
	 * 
	 */
	private DlogGroupFactory() {

		//create an instance of FactoriesUtility with the predefined file names.
		factoriesUtility = new FactoriesUtility("DlogDefault.properties", "Dlog.properties");
		
	}
	
	
	/**
	 * This function creates and returns a DlogGroup object from a specified provider.
	 * @param algName is the name of a specific DlogGroup.A list of possible names follows: <p>
 	 * 	   	  For Elliptic Curves:   DlogECFp, DlogECF2m. <p>
	 *		  For Dlog groups:	 DlogZpSafePrime 		  
	 * @param provider the required provider name
	 * @return an object of type DlogGroup class that was determined by the algName + provider
	 * @throws FactoriesException 
	 */
	public DlogGroup getObject(String algName, String provider) throws FactoriesException {
		
		return (DlogGroup) factoriesUtility.getObject(provider, algName);
	}

	/** 
	 * This function creates and returns a DlogGroup object from a default provider chosen by SCAPI.
	 * @param algName is the name of a specific DlogGroup.A list of possible names follows: <p>
 	 * 	   	  For Elliptic Curves:   DlogECFp, DlogECF2m. <p>
	 *		  For Dlog groups:	 DlogZpSafePrime 		  
	 *
	 * @return an object of type DlogGroup class that was determined by the algName + the default provider for that algorithm
	 * @throws FactoriesException 
	 */
	public DlogGroup getObject(String algName) throws FactoriesException {
		
		return (DlogGroup) factoriesUtility.getObject(algName);
	}

	/** 
	 * This function creates (if needed) and returns an instance of this factory.
	 * @return the singleton instance.
	 */
	public static DlogGroupFactory getInstance() {
		return instance;
	}
}
