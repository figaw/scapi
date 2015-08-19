#include <iostream>
#include "../include/Dlog.hpp"

using namespace std;


void DlogGroupAbs::GroupElementsExponentiations::prepareExponentiations(biginteger size)
{
	//find log of the number - this is the index of the size-exponent in the exponentiation array 
//	float res = boost::multiprecision::log<biginteger>(size);
	float index; //XXX

	/* calculates the necessary exponentiations and put them in the exponentiations vector */
	for (int i = exponentiations.size(); i <= index; i++) {
		GroupElement * multI = parent->exponentiate(exponentiations[i - 1], biginteger(2));
		exponentiations.push_back(multI);
	}
}

DlogGroupAbs::GroupElementsExponentiations::GroupElementsExponentiations(DlogGroupAbs * parent_, GroupElement * base_) {
	base = base_;
	parent = parent_;
	// build new vector of exponentiations
	exponentiations.push_back(base); // add the base - base^1

	biginteger two(2);
	for (int i = 1; i<4; i++) {
		GroupElement * multI;
		multI = parent->exponentiate(exponentiations[i - 1], two);
		exponentiations.push_back(multI);
	}
}

GroupElement * DlogGroupAbs::GroupElementsExponentiations::getExponentiation(biginteger size) {
	/**
	* The exponents in the exponents vector are all power of 2.
	* In order to achieve the exponent size, we calculate its closest power 2 in the exponents vector
	* and continue the calculations from there.
	*/
	// find the the closest power 2 exponent
	//int index = size.bitLength() - 1;
	int index; //XXX

	GroupElement * exponent = NULL;
	/* if the requested index out of the vector bounds, the exponents have not been calculated yet, so calculates them.*/
	if (exponentiations.size() <= index)
		prepareExponentiations(size);

	exponent = exponentiations[index]; //get the closest exponent in the exponentiations vector
									   /* if size is not power 2, calculates the additional multiplications */
	biginteger lastExp = boost::multiprecision::pow(biginteger(2), index);
	biginteger difference = size - lastExp;
	if (difference > 0) {
		GroupElement * diff = getExponentiation(size - lastExp);
		exponent = parent->multiplyGroupElements(diff, exponent);
	}

	return exponent;
}

bool DlogGroupAbs::isPrimeOrder() {
	return (miller_rabin_test(getOrder(), 40, prime_gen));
}

bool DlogGroupAbs::isOrderGreaterThan(int numBits) {
	if (getOrder() > boost::multiprecision::pow(biginteger(2), numBits))
		return true;
	else return false;
}

GroupElement * DlogGroupAbs::createRandomElement() {
		//This is a default implementation that is valid for all the Dlog Groups and relies on mathematical properties of the generators.
		//However, if a specific Dlog Group has a more efficient implementation then is it advised to override this function in that concrete
		//Dlog group. For example we do so in CryptoPpDlogZpSafePrime.
	boost::random::uniform_int_distribution<biginteger> ui(0, groupParams->getQ() - 1);
	biginteger randNum = ui(random_element_gen);
	// compute g^x to get a new element
	return exponentiate(generator, randNum);
}

GroupElement * DlogGroupAbs::createRandomGenerator() {
	// in prime order groups every element except the identity is a generator.
	// get a random element in the group
	GroupElement * randGen = createRandomElement();
	// if the given element is the identity, get a new random element
	while (randGen->isIdentity() == true) {
		randGen = createRandomElement();
	}
	return randGen;
}

GroupElement * DlogGroupAbs::computeLoop(vector<biginteger> exponentiations, int w, int h, vector<vector<GroupElement *>> preComp, GroupElement * result, int bitIndex){
	int n = groupElements.length; //number of bases and exponents
	vector<GroupElement> exponentsResult = new vector<GroupElement>(n); //holds the exponentiations result

	// raises each element to the corresponding power
	for (int i = 0; i < n; i++) {
		exponentsResult[i] = exponentiate(groupElements[i], exponentiations[i]);
	}

	GroupElement * result = NULL; //holds the multiplication of all the exponentiations
	result = getIdentity(); //initialized to the identity element

	//multiplies every exponentiate
	for (int i = 0; i<n; i++) {
		result = multiplyGroupElements(exponentsResult[i], result);
	}

	//return the final result
	return result;
}

vector<vector<GroupElement *>> DlogGroupAbs::createLLPreCompTable(vector<vector<GroupElement *>> createLLPreCompTable(vector<GroupElement *> groupElements, int w, int h){
	int n = groupElements.length; //number of bases and exponents

									//get the biggest exponent
	BigInteger bigExp = BigInteger.ZERO;
	for (int i = 0; i<exponentiations.length; i++)
		if (bigExp.compareTo(exponentiations[i])<0)
			bigExp = exponentiations[i];

	int t = bigExp.bitLength(); //num bits of the biggest exponent.
	int w = 0; //window size

				//choose w according to the value of t
	w = getLLW(t);

	//h = n/w
	int h;
	if ((n % w) == 0) {
		h = n / w;
	}
	else {
		h = ((int)(n / w)) + 1;
	}

	//create pre computation table
	GroupElement[][] preComp = createLLPreCompTable(groupElements, w, h);

	/*for (int i=0; i<h; i++)
	for (int j=0; j<Math.pow(2, w); j++)
	System.out.println(((ECElement) preComp[i][j]).getX());
	*/
	GroupElement result = null; //holds the computation result
	result = getIdentity();

	//computes the first loop of the algorithm. This loop returns in the next part of the algorithm with one single tiny change. 
	result = computeLoop(exponentiations, w, h, preComp, result, t - 1);

	//computes the third part of the algorithm
	for (int j = t - 2; j >= 0; j--) {
		//Y = Y^2
		result = exponentiate(result, new BigInteger("2"));

		//computes the inner loop
		result = computeLoop(exponentiations, w, h, preComp, result, j);
	}

	return result;
}


GroupElement DlogGroupAbs::computeLoop(BigInteger[] exponentiations, int w, int h, GroupElement[][] preComp, GroupElement result, int bitIndex) {
	int e = 0;
	for (int k = 0; k<h; k++) {
		for (int i = k*w; i<(k * w + w); i++) {
			if (i < exponentiations.length) {
				//if the bit is set, change the e value
				if (exponentiations[i].testBit(bitIndex) == true) {
					int twoPow = (int)(Math.pow(2, i - k*w));
					e += twoPow;
				}
			}
		}
		//multiply result with preComp[k][e]
		result = multiplyGroupElements(result, preComp[k][e]);

		e = 0;
	}

	return result;
}

private GroupElement[][] DlogGroupAbs::createLLPreCompTable(GroupElement[] groupElements, int w, int h) {
	int twoPowW = (int)(Math.pow(2, w));
	//create the pre-computation table of size h*(2^(w))
	GroupElement[][] preComp = new GroupElement[h][twoPowW];

	GroupElement base = null;
	int baseIndex;

	//fill the table
	for (int k = 0; k<h; k++) {
		for (int e = 0; e<twoPowW; e++) {
			preComp[k][e] = getIdentity();

			for (int i = 0; i<w; i++) {
				baseIndex = k*w + i;
				if (baseIndex < groupElements.length) {
					base = groupElements[baseIndex];
					//if bit i in e is set, change preComp[k][e]
					if ((e & (1 << i)) != 0) { //bit i is set
						preComp[k][e] = multiplyGroupElements(preComp[k][e], base);
					}
				}
			}
		}
	}

	return preComp;

}

int DlogGroupAbs::getLLW(int t) {
		int w;
		//choose w according to the value of t
		if (t <= 10) {
			w = 2;
		}
		else if (t <= 24) {
			w = 3;
		}
		else if (t <= 60) {
			w = 4;
		}
		else if (t <= 144) {
			w = 5;
		}
		else if (t <= 342) {
			w = 6;
		}
		else if (t <= 797) {
			w = 7;
		}
		else if (t <= 1828) {
			w = 8;
		}
		else {
			w = 9;
		}
		return w;
}

GroupElement DlogGroupAbs::exponentiateWithPreComputedValues(GroupElement groupElement, BigInteger exponent) {
	//extracts from the map the GroupElementsExponentiations object corresponding to the accepted base
	GroupElementsExponentiations exponentiations = exponentiationsMap.get(groupElement);

	// if there is no object that matches this base - create it and add it to the map
	if (exponentiations == null) {
		exponentiations = new GroupElementsExponentiations(groupElement);
		exponentiationsMap.put(groupElement, exponentiations);
	}
	// calculates the required exponent
	return exponentiations.getExponentiation(exponent);
}

