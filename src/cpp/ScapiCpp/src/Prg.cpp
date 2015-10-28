#include "../include/prg.hpp"

void ScPrgFromPrf::setKey(SecretKey secretKey) {
	prf->setKey(secretKey); //Sets the key to the underlying prf.
	
	ctr = vector<byte>(prf->getBlockSize());
	// Initializes the counter to 1.
	ctr[ctr.size() - 1] = 1;
	this->isKeySet = true;
}

void ScPrgFromPrf::getPRGBytes(vector<byte> & outBytes, int outOffset, int outLen) {
	if (!isKeySet())
		throw IllegalStateException("secret key isn't set");

	int numGeneratedBytes = 0;	// Number of current generated bytes.
	vector<byte> generatedBytes = vector<byte>(ctr.size());

	while (numGeneratedBytes < outLen) {
		try {
			// If the prf can output any length (for example, IteratedPrfVarying) call the computeBlock with the outputLen.
			prf->computeBlock(ctr, 0, ctr.size(), outBytes, outOffset + numGeneratedBytes, outLen);
			numGeneratedBytes += outLen;
		}
		catch (out_of_range& e) {
			try {
				// if the prf can receive any input length (for example, Hmac) call the computeBlock with the ctr length.
				// the output is written to a new array because there is no guarantee that output array is long enough to hold the next output block.
				prf->computeBlock(ctr, 0, ctr.size(), generatedBytes, 0);
				//Copy the right number of generated bytes.
				if (numGeneratedBytes + generatedBytes.size() <= outLen)
					outBytes.insert(outBytes.begin() + outOffset + numGeneratedBytes, generatedBytes.begin(), generatedBytes.end());
				else
					System.arraycopy(generatedBytes, 0, outBytes, outOffset + numGeneratedBytes, outLen - numGeneratedBytes);

				// increases the number of generated bytes.
				numGeneratedBytes += ctr.length;
			}
			catch (IllegalBlockSizeException e1) {
				try {
					//If the prf can receive fixed input length (for example, AES) call the computeBlock without the input length.
					//The output is written to a new array because there is no guarantee that output array is long enough to hold the next output block.
					prf.computeBlock(ctr, 0, generatedBytes, 0);
					//Copy the right number of generated bytes.
					if (numGeneratedBytes + generatedBytes.length <= outLen) {
						System.arraycopy(generatedBytes, 0, outBytes, outOffset + numGeneratedBytes, generatedBytes.length);
					}
					else {
						System.arraycopy(generatedBytes, 0, outBytes, outOffset + numGeneratedBytes, outLen - numGeneratedBytes);
					}
					//Increases the number of generated bytes.
					numGeneratedBytes += ctr.length;
				}
				catch (IllegalBlockSizeException e2) {
					// TODO Auto-generated catch block
					e2.printStackTrace();
				}
			}
		}
		//Increases the counter.
		increaseCtr();
	}

}