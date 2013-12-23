#include "Wrapper.h"

#include <string.h>

#include "Utils.h"

namespace jni {

void CJavaObject::NamedGetter(
  v8::Local<v8::String> prop,
  const v8::PropertyCallbackInfo<v8::Value>& info)
{
  CJavaObject& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  v8::String::Utf8Value name(prop);

  info.GetReturnValue().Set(env.GetMember(obj.m_obj, *name));
}

void CJavaObject::NamedSetter(
  v8::Local<v8::String> prop, v8::Local<v8::Value> value,
  const v8::PropertyCallbackInfo<v8::Value>& info)
{
  CJavaObject& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  v8::String::Utf8Value name(prop);

  info.GetReturnValue().Set(env.SetMember(obj.m_obj, *name, value));
}

void CJavaObject::NamedQuery(
  v8::Local<v8::String> prop,
  const v8::PropertyCallbackInfo<v8::Integer>& info)
{
  CJavaObject& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  v8::String::Utf8Value name(prop);

  if (env.HasMember(obj.m_obj, *name)) {
    info.GetReturnValue().Set(v8::Integer::New(v8::None));
  }
}

void CJavaObject::NamedEnumerator(
  const v8::PropertyCallbackInfo<v8::Array>& info)
{
  CJavaObject& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  info.GetReturnValue().Set(env.GetMembers(obj.m_obj));
}

void CJavaArray::NamedGetter(
  v8::Local<v8::String> prop,
  const v8::PropertyCallbackInfo<v8::Value>& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  v8::String::Utf8Value name(prop);

  if (strcmp("length", *name) == 0) {
    info.GetReturnValue().Set(v8::Uint32::New(obj.GetLength()));
  } else {
    __base__::NamedGetter(prop, info);
  }
}

void CJavaArray::NamedQuery(
  v8::Local<v8::String> prop,
  const v8::PropertyCallbackInfo<v8::Integer>& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  v8::String::Utf8Value name(prop);

  if (strcmp("length", *name) == 0) {
    info.GetReturnValue().Set(v8::Integer::New(v8::None));
  } else {
    __base__::NamedQuery(prop, info);
  }
}

void CJavaArray::IndexedGetter(
  uint32_t index,
  const v8::PropertyCallbackInfo<v8::Value>& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  static jmethodID mid = env.GetStaticMethodID(env.buildins.java.lang.reflect.Array, "get", "(Ljava/lang/Object;I)Ljava/lang/Object;");

  info.GetReturnValue().Set(env.Wrap(env->CallStaticObjectMethod(env.buildins.java.lang.reflect.Array, mid, obj.m_obj, index)));
}

void CJavaArray::IndexedSetter(
  uint32_t index, v8::Local<v8::Value> value,
  const v8::PropertyCallbackInfo<v8::Value>& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  static jmethodID mid = env.GetStaticMethodID(env.buildins.java.lang.reflect.Array, "set", "(Ljava/lang/Object;ILjava/lang/Object;)V");

  env->CallStaticVoidMethod(env.buildins.java.lang.reflect.Array, mid, obj.m_obj, index, env.Wrap(value));

  info.GetReturnValue().Set(value);
}

void CJavaArray::IndexedQuery(
  uint32_t index,
  const v8::PropertyCallbackInfo<v8::Integer>& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  if (index < obj.GetLength()) {
    info.GetReturnValue().Set(v8::Integer::New(v8::None));
  } else {
    __base__::IndexedQuery(index, info);
  }
}

void CJavaArray::IndexedEnumerator(
  const v8::PropertyCallbackInfo<v8::Array>& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  size_t length = obj.GetLength();
  v8::Handle<v8::Array> result = v8::Array::New(v8::Isolate::GetCurrent(), length);

  for (size_t i=0; i<length; i++)
  {
    result->Set(v8::Uint32::New(i), v8::Int32::New(i)->ToString());
  }

  info.GetReturnValue().Set(result);
}

CJavaFunction::CJavaFunction(JNIEnv *pEnv, jobject obj)
  : m_pEnv(pEnv)
{
  m_methods.push_back(std::make_pair(m_pEnv->NewGlobalRef(obj), GetParameterTypes(m_pEnv, obj)));
}
void CJavaFunction::ReleaseMethods(JNIEnv *pEnv, const methods_t& methods)
{
  for (size_t i=0; i<methods.size(); i++)
  {
    pEnv->DeleteGlobalRef(methods[i].first);

    const types_t& types = methods[i].second;

    for (size_t j=0; j<types.size(); j++)
    {
      pEnv->DeleteGlobalRef(types[j]);
    }
  }
}
const CJavaFunction::types_t CJavaFunction::GetParameterTypes(JNIEnv *pEnv, jobject method)
{
  jni::Env env(pEnv);

  static jmethodID mid = env.GetMethodID(env.buildins.java.lang.reflect.Method, "getParameterTypes", "()[Ljava/lang/Class;");

  jobjectArray classes = (jobjectArray) env->CallObjectMethod(method, mid);

  types_t types(env->GetArrayLength(classes));

  for (size_t j=0; j<types.size(); j++)
  {
    jobject type = env->GetObjectArrayElement(classes, j);

    types[j] = (jclass) env->NewGlobalRef(type);

    env->DeleteLocalRef(type);
  }

  return types;
}
jobject CJavaFunction::GetMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  jni::V8Env env(m_pEnv);

  jobject method = NULL;

  if (m_methods.size() > 1)
  {
    for (size_t i=0; i<m_methods.size(); i++)
    {
      const types_t& types = m_methods[i].second;

      if (types.size() == info.Length())
      {
        bool same = true;

        method = m_methods[i].first;

        for (size_t j=0; j<types.size(); j++)
        {
          if (!CanConvert(types[j], info[j]))
          {
            same = false;
            break;
          }
        }

        if (same) break;
      }
    }
  }
  else
  {
    method = m_methods[0].first;
  }

  return method;
}
bool CJavaFunction::CanConvert(jclass clazz, v8::Handle<v8::Value> value)
{
  jni::V8Env env(m_pEnv);

  if (value->IsTrue() || value->IsFalse() || value->IsBoolean())
  {
    return env.IsAssignableFrom(env.buildins.java.lang.Boolean, clazz);
  }
  else if (value->IsInt32() || value->IsUint32())
  {
    return env.IsAssignableFrom(env.buildins.java.lang.Long, clazz) ||
           env.IsAssignableFrom(env.buildins.java.lang.Integer, clazz) ||
           env.IsAssignableFrom(env.buildins.java.lang.Short, clazz) ||
           env.IsAssignableFrom(env.buildins.java.lang.Byte, clazz);
  }
  else if (value->IsNumber())
  {
    return env.IsAssignableFrom(env.buildins.java.lang.Double, clazz) ||
           env.IsAssignableFrom(env.buildins.java.lang.Float, clazz);
  }
  else if (value->IsString())
  {
    return env.IsAssignableFrom(env.buildins.java.lang.String, clazz);
  }
  else if (value->IsDate())
  {
    return env.IsAssignableFrom(env.buildins.java.util.Date, clazz);
  }
  else if (value->IsArray())
  {
    return env.IsAssignableFrom(env.buildins.lu.flier.script.V8Array, clazz);
  }
  else if (value.IsEmpty() || value->IsNull() || value->IsUndefined() || value->IsObject())
  {
    return true;
  }

  return false;
}

void CJavaFunction::Caller(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  CJavaFunction& func = *static_cast<CJavaFunction *>(v8::Handle<v8::External>::Cast(info.Data())->Value());

  jni::V8Env env(func.GetEnv());

  bool hasThiz = CManagedObject::IsWrapped(info.This()->ToObject());
  jobject thiz = hasThiz ? CManagedObject::Unwrap(info.This()->ToObject()).GetObject() : NULL;

  jobjectArray params = (jobjectArray) env.NewObjectArray(info.Length());

  for (size_t i=0; i<info.Length(); i++)
  {
    env->SetObjectArrayElement(params, i, env.Wrap(info[i]));
  }

  jobject method = func.GetMethod(info);
  static jmethodID mid = env.GetMethodID(env.buildins.java.lang.reflect.Method, "invoke", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");

  jobject result = env->CallObjectMethod(method, mid, thiz, params);

  info.GetReturnValue().Set(env.Wrap(result));
}

void CJavaBoundMethod::Caller(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  CJavaBoundMethod& func = *static_cast<CJavaBoundMethod *>(v8::Handle<v8::External>::Cast(info.Data())->Value());

  JNIEnv *pEnv = func.GetEnv();
  jobject thiz = func.GetThiz();
  jmethodID mid = func.GetMid();

  if (func.HasArgs())
  {
    jni::V8Env env(func.GetEnv());

    jobjectArray params = (jobjectArray) env.NewObjectArray(info.Length());

    for (size_t i=0; i<info.Length(); i++)
    {
      env->SetObjectArrayElement(params, i, env.Wrap(info[i]));
    }

    if (func.IsVoid())
    {
      pEnv->CallVoidMethod(thiz, mid, params);
      info.GetReturnValue().Set(v8::Null(v8::Isolate::GetCurrent()));
    }
    else
    {
      jobject result = pEnv->CallObjectMethod(thiz, mid, params);
      info.GetReturnValue().Set(env.Wrap(result));
    }
  }
  else
  {
    if (func.IsVoid())
    {
      pEnv->CallVoidMethod(thiz, mid);
      info.GetReturnValue().Set(v8::Null(v8::Isolate::GetCurrent()));
    }
    else
    {
      jni::V8Env env(func.GetEnv());
      jobject result = pEnv->CallObjectMethod(thiz, mid);
      info.GetReturnValue().Set(env.Wrap(result));
    }
  }
}
} // namespace jni
