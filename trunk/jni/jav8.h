/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class lu_flier_script_V8ScriptEngineFactory */

#ifndef _Included_lu_flier_script_V8ScriptEngineFactory
#define _Included_lu_flier_script_V8ScriptEngineFactory
#ifdef __cplusplus
extern "C" {
#endif
/* Inaccessible static: names */
/* Inaccessible static: mimeTypes */
/* Inaccessible static: extensions */
/*
 * Class:     lu_flier_script_V8ScriptEngineFactory
 * Method:    getParameter
 * Signature: (Ljava/lang/String;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8ScriptEngineFactory_getParameter
  (JNIEnv *, jobject, jstring);

/*
 * Class:     lu_flier_script_V8ScriptEngineFactory
 * Method:    initialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8ScriptEngineFactory_initialize
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class lu_flier_script_V8CompiledScript */

#ifndef _Included_lu_flier_script_V8CompiledScript
#define _Included_lu_flier_script_V8CompiledScript
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     lu_flier_script_V8CompiledScript
 * Method:    internalCompile
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_lu_flier_script_V8CompiledScript_internalCompile
  (JNIEnv *, jobject, jstring);

/*
 * Class:     lu_flier_script_V8CompiledScript
 * Method:    internalRelease
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8CompiledScript_internalRelease
  (JNIEnv *, jobject, jlong);

/*
 * Class:     lu_flier_script_V8CompiledScript
 * Method:    internalExecute
 * Signature: (JLjavax/script/ScriptContext;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8CompiledScript_internalExecute
  (JNIEnv *, jobject, jlong, jobject);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class lu_flier_script_V8Context */

#ifndef _Included_lu_flier_script_V8Context
#define _Included_lu_flier_script_V8Context
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     lu_flier_script_V8Context
 * Method:    getEntered
 * Signature: ()Llu/flier/script/V8Context;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8Context_getEntered
  (JNIEnv *, jclass);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    getCurrent
 * Signature: ()Llu/flier/script/V8Context;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8Context_getCurrent
  (JNIEnv *, jclass);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    getCalling
 * Signature: ()Llu/flier/script/V8Context;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8Context_getCalling
  (JNIEnv *, jclass);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    inContext
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_lu_flier_script_V8Context_inContext
  (JNIEnv *, jclass);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    internalCreate
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_lu_flier_script_V8Context_internalCreate
  (JNIEnv *, jclass);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    internalRelease
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8Context_internalRelease
  (JNIEnv *, jobject, jlong);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    internalEnter
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8Context_internalEnter
  (JNIEnv *, jobject, jlong);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    internalLeave
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8Context_internalLeave
  (JNIEnv *, jobject, jlong);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    internalGetGlobal
 * Signature: (J)Llu/flier/script/V8Object;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8Context_internalGetGlobal
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class lu_flier_script_V8Object */

#ifndef _Included_lu_flier_script_V8Object
#define _Included_lu_flier_script_V8Object
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     lu_flier_script_V8Object
 * Method:    internalRelease
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8Object_internalRelease
  (JNIEnv *, jobject, jlong);

/*
 * Class:     lu_flier_script_V8Object
 * Method:    internalGetKeys
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_lu_flier_script_V8Object_internalGetKeys
  (JNIEnv *, jobject);

/*
 * Class:     lu_flier_script_V8Object
 * Method:    size
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_lu_flier_script_V8Object_size
  (JNIEnv *, jobject);

/*
 * Class:     lu_flier_script_V8Object
 * Method:    isEmpty
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_lu_flier_script_V8Object_isEmpty
  (JNIEnv *, jobject);

/*
 * Class:     lu_flier_script_V8Object
 * Method:    clear
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8Object_clear
  (JNIEnv *, jobject);

/*
 * Class:     lu_flier_script_V8Object
 * Method:    containsKey
 * Signature: (Ljava/lang/Object;)Z
 */
JNIEXPORT jboolean JNICALL Java_lu_flier_script_V8Object_containsKey
  (JNIEnv *, jobject, jobject);

/*
 * Class:     lu_flier_script_V8Object
 * Method:    get
 * Signature: (Ljava/lang/Object;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8Object_get
  (JNIEnv *, jobject, jobject);

/*
 * Class:     lu_flier_script_V8Object
 * Method:    put
 * Signature: (Ljava/lang/String;Ljava/lang/Object;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8Object_put
  (JNIEnv *, jobject, jstring, jobject);

/*
 * Class:     lu_flier_script_V8Object
 * Method:    remove
 * Signature: (Ljava/lang/Object;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8Object_remove
  (JNIEnv *, jobject, jobject);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class lu_flier_script_V8Function */

#ifndef _Included_lu_flier_script_V8Function
#define _Included_lu_flier_script_V8Function
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     lu_flier_script_V8Function
 * Method:    internalInvoke
 * Signature: (JLjava/lang/Long;[Ljava/lang/Object;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8Function_internalInvoke
  (JNIEnv *, jobject, jlong, jobject, jobjectArray);

#ifdef __cplusplus
}
#endif
#endif
