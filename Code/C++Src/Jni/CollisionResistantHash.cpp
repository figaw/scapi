#include "jni.h" 
#include "stdafx.h"
#include <string>
#include "CollisionResistantHash.h"
#include "sha.h"
#include "cryptlib.h"

using namespace std;
using namespace CryptoPP;



/* function createHash : This function creates a hash and returns a pointer to the created hash. The function 
 * param hashName	   : The name of the hash we wish to create
 * return			   : A pointer to the created hash.
 */
JNIEXPORT jlong JNICALL Java_edu_biu_scapi_primitives_hash_cryptopp_CryptoPpCollResHash_createHash
  (JNIEnv *env, jobject, jstring hashName){

	HashTransformation *hashPtr = NULL;

	//get the string from java
	const char* str = env->GetStringUTFChars( hashName, NULL );

	//supports all of the SHA hashes. Get the name of the required hash and instanciate that hash.
	if(strcmp (str,"SHA1") == 0)
		hashPtr = new SHA1;
	else if(strcmp (str,"SHA224") == 0)
		hashPtr = new SHA224;
	else if(strcmp (str,"SHA256") == 0)
		hashPtr = new SHA256;
	else if(strcmp (str,"SHA384") == 0)
		hashPtr = new SHA384;
	else if(strcmp (str,"SHA512") == 0)
		hashPtr = new SHA512;

	//return a pointer to the created hash.
	return (jlong)hashPtr;

}


/* function algName : This function gets the name of the hash function of the passed pointer to hash 
 * param hashPtr	: The actual hash object pointer to get the name from
 * return			: The name of the passed hash algorithm
 */
JNIEXPORT jstring JNICALL Java_edu_biu_scapi_primitives_hash_cryptopp_CryptoPpCollResHash_algName
(JNIEnv *env, jobject, jlong hashPtr){

	//cast to HashTransformation wich is the base class of all hash function. 
	//the function AlgorithmName is defined there (actually in a base class of it)
	string shaName = ((HashTransformation *)hashPtr)->AlgorithmName();

	//return a string that Java can understand with the nama of the algorithm.
	return env->NewStringUTF(shaName.c_str());
}


/* function updateHash : This function updates the hash function with the byte array data
 * param hashPtr	   : The actual hash object pointer to update
 * param data		   : the byte array to translate to c++ and update the hash
 * param len		   : the length of the byte array
 */
JNIEXPORT void JNICALL Java_edu_biu_scapi_primitives_hash_cryptopp_CryptoPpCollResHash_updateHash
(JNIEnv *env, jobject, jlong hashPtr, jbyteArray data, jlong len){

	//declare a byte array in c++
	jbyte *carr;

	//get to carr the elements of the input byte array data
	carr = env->GetByteArrayElements(data, 0);

	//invoke the update function after casting to HashTransformation that defines this function for all the derived hash
	//algorithms to implement
	((HashTransformation *)hashPtr)->Update((const byte *)carr, len);

	//make sure to release the memory created in c++. The JVM will not release it automatically.
	env->ReleaseByteArrayElements(data,carr,0);
}

/* function finalHash : This function completes the hash computation
 * param hashPtr	   : The actual hash object pointer 
 * param input		   : the byte array to put the result in
 * param size		   : the length of the byte array. This will be different for different hash functions
 */
JNIEXPORT void JNICALL Java_edu_biu_scapi_primitives_hash_cryptopp_CryptoPpCollResHash_finalHash
(JNIEnv *env, jobject, jlong hashPtr, jbyteArray output, jlong size){

	//allocate a new byte array with the size of the specific hash algorithm.
	byte *ret = new byte[size]; 

	//perform the final function
	((HashTransformation *)hashPtr)->Final(ret);

	//put the result of the final computation in the output array passed from java
	env->SetByteArrayRegion(output, 0, size, (jbyte*)ret); 

	//make sure to release the dynamically allocated memory. Will not be deleted by the JVM.
	delete ret;


}

/* function deleteHash : This function deletes the hash dynamically allocated pointer that was created in c++. This
 *						 memory allocation will not be deleted by the JVM.
 * param hashPtr	   : The actual hash object pointer 
 */
JNIEXPORT void JNICALL Java_edu_biu_scapi_primitives_hash_cryptopp_CryptoPpCollResHash_deleteHash
(JNIEnv *, jobject, jlong hashPtr){
}