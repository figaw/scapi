#include "../../include/primitives/TrapdoorPermutations.hpp"

/*************************************************/
/*TrapdoorPermutationAbs                         */
/*************************************************/
byte TrapdoorPermutationAbs::hardCorePredicate(TPElement * tpEl) {
	if (!isKeySet())
		throw IllegalStateException("keys aren't set");
	/*
	*  We use this implementation both in RSA permutation and in Rabin permutation.
	* Thus, We implement it in TrapdoorPermutationAbs and let derived classes override it if needed.
	*/
	//gets the element value as byte array
	biginteger elementValue = tpEl->getElement();
	byte* bytesValue=NULL; 
	int bytesSize = allocateAndEncodeBigInteger(elementValue, bytesValue);

	// returns the least significant bit (byte, as we said above)
	byte res = bytesValue[bytesSize - 1];
	delete bytesValue;
	return res;
}

byte* TrapdoorPermutationAbs::hardCoreFunction(TPElement * tpEl) {
	if (!isKeySet())
		throw IllegalStateException("keys aren't set");
	/*
	* We use this implementation both in RSA permutation and in Rabin permutation.
	* Thus, We implement it in TrapdoorPermutationAbs and let derived classes override it if needed.
	*/
	// gets the element value as byte array
	biginteger elementValue = tpEl->getElement();
	byte* bytesValue=NULL;
	int bytesSize = allocateAndEncodeBigInteger(elementValue, bytesValue);

	// the number of bytes to get the log (N) least significant bits
	
	double logBits = NumberOfBits(modulus) / 2.0;  //log N bits
	int logBytes = (int)ceil(logBits / 8); //log N bites in bytes

	// if the element length is less than log(N), the return byte[] should be all the element bytes
	int size = min(logBytes, bytesSize);
	byte* leastSignificantBytes = new byte[size];
	
	// copies the bytes to the output array
	for (int i = 0; i < size; i++)
		leastSignificantBytes[i] = bytesValue[bytesSize - size + i];
	return leastSignificantBytes;
}

/*************************************************/
/*RSAElement                                     */
/*************************************************/

RSAElement::RSAElement(biginteger modN){
	/*
	* samples a number between 1 to n-1
	*/
	mt19937 generator = get_seeded_random();
	biginteger randNumber;
	int numbit = NumberOfBits(modN);
	biginteger expo = mp::pow(biginteger(2), numbit-1);
	boost::random::uniform_int_distribution<biginteger> ui(0, expo);
	do {
		randNumber = ui(generator); // samples a random BigInteger with modN.bitLength()+1 bits
	} while (randNumber > (modN - 2)); // drops the element if it's bigger than mod(N)-2
	// gets a random biginteger between 1 to modN-1
	randNumber += 1;
	// sets it to be the element
	element = randNumber;
}

RSAElement::RSAElement(biginteger modN, biginteger x, bool check) {
	if (!check)
		element = x;
	else {
		/*
		* checks if the value is valid (between 1 to (mod n) - 1).
		* if valid - sets it to be the element
		* if not valid - throws exception
		*/
		if (x > 0 && x < modN)
			element = x;
		else 
			throw invalid_argument("element out of range");
	}
}


