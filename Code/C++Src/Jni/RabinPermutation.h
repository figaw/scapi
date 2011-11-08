/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation */

#ifndef _Included_edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation
#define _Included_edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation
 * Method:    initRabinAll
 * Signature: ([B[B[B[B[B[B)J
 */
JNIEXPORT jlong JNICALL Java_edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation_initRabinAll
  (JNIEnv *, jobject, jbyteArray, jbyteArray, jbyteArray, jbyteArray, jbyteArray, jbyteArray);

/*
 * Class:     edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation
 * Method:    initRabinNRS
 * Signature: ([B[B[B)J
 */
JNIEXPORT jlong JNICALL Java_edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation_initRabinNRS
  (JNIEnv *, jobject, jbyteArray, jbyteArray, jbyteArray);

/*
 * Class:     edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation
 * Method:    initRabinKeySize
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation_initRabinKeySize
  (JNIEnv *, jobject, jint);

/*
 * Class:     edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation
 * Method:    loadRabinName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation_loadRabinName
  (JNIEnv *, jobject, jlong);

/*
 * Class:     edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation
 * Method:    getRabinModulus
 * Signature: (J)[B
 */
JNIEXPORT jbyteArray JNICALL Java_edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation_getRabinModulus
  (JNIEnv *, jobject, jlong);

/*
 * Class:     edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation
 * Method:    checkRabinValidity
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation_checkRabinValidity
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation
 * Method:    computeRabin
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation_computeRabin
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation
 * Method:    invertRabin
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation_invertRabin
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation
 * Method:    deleteRabin
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_edu_biu_scapi_primitives_trapdoorPermutation_cryptopp_CryptoPpRabinPermutation_deleteRabin
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
