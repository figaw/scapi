/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey */

#ifndef _Included_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
#define _Included_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
#ifdef __cplusplus
extern "C" {
#endif
#undef edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_SCAPI_NATIVE_KEY_SIZE
#define edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_SCAPI_NATIVE_KEY_SIZE 16L
/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    createGarbledcircuit
 * Signature: (Ljava/lang/String;ZZZZ)J
 */
JNIEXPORT jlong JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_createGarbledcircuit
  (JNIEnv *, jobject, jstring, jboolean);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    getOutputIndicesArray
 * Signature: (J)[I
 */
JNIEXPORT jintArray JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_getOutputIndicesArray
  (JNIEnv *, jobject, jlong);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    getInputIndicesArray
 * Signature: (J)[I
 */
JNIEXPORT jintArray JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_getInputIndicesArray
  (JNIEnv *, jobject, jlong);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    getNumOfInputsForEachParty
 * Signature: (J)[I
 */
JNIEXPORT jintArray JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_getNumOfInputsForEachParty
  (JNIEnv *, jobject, jlong);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    setGarbleTables
 * Signature: (J[B)V
 */
JNIEXPORT void JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_setGarbleTables
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    getGarbleTables
 * Signature: (J)[B
 */
JNIEXPORT jbyteArray JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_getGarbleTables
  (JNIEnv *, jobject, jlong);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    getTranslationTable
 * Signature: (J)[B
 */
JNIEXPORT jbyteArray JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_getTranslationTable
  (JNIEnv *, jobject, jlong);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    setTranslationTable
 * Signature: (J[B)V
 */
JNIEXPORT void JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_setTranslationTable
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    garble
 * Signature: ([B[B[B[BJ)J
 */
JNIEXPORT jlong JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_garble
  (JNIEnv *, jobject, jbyteArray, jbyteArray, jbyteArray, jbyteArray, jlong);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    compute
 * Signature: (J[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_compute
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    verify
 * Signature: (J[B)Z
 */
JNIEXPORT jboolean JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_verify
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    internalVerify
 * Signature: (J[B[B)Z
 */
JNIEXPORT jboolean JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_internalVerify
  (JNIEnv *, jobject, jlong, jbyteArray, jbyteArray);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    translate
 * Signature: (J[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_translate
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    verifyTranslate
 * Signature: (J[B[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_verifyTranslate
  (JNIEnv *, jobject, jlong, jbyteArray, jbyteArray);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    verifyTranslationTable
 * Signature: (J[B)Z
 */
JNIEXPORT jboolean JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_verifyTranslationTable
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey
 * Method:    deleteCircuit
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_edu_biu_scapi_circuits_fastGarbledCircuit_ScNativeGarbledBooleanCircuitNoFixedKey_deleteCircuit
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif