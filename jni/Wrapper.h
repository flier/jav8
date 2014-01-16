#pragma once

#include <cassert>
#include <map>
#include <vector>
#include <string>

#ifdef __APPLE__
#include <pthread.h>
#endif

#include <jni.h>

#include <v8.h>

#include "Utils.h"

namespace jni {

class CManagedObject;

template <class T>
class ObjectTracer
{
  v8::Persistent<v8::Value> m_handle;
  std::auto_ptr<T> m_object;

  void MakeWeak(void)
  {
    m_handle.SetWeak(this, WeakCallback);
  }

  static void WeakCallback(const v8::WeakCallbackData<v8::Value, ObjectTracer<T> >& data)
  {
    std::auto_ptr< ObjectTracer<T> > tracer(data.GetParameter());
  }
public:
  ObjectTracer(v8::Handle<v8::Value> handle, T *object)
    : m_object(object)
  {
    m_handle.Reset(v8::Isolate::GetCurrent(), handle);
  }
  virtual ~ObjectTracer()
  {
    if (!m_handle.IsEmpty())
    {
      assert(m_handle.IsNearDeath());

      m_handle.ClearWeak();
      m_handle.Reset();

      m_object.reset();
    }
  }

  static ObjectTracer<T>& Trace(v8::Handle<v8::Value> handle, T *object)
  {
    ObjectTracer<T> *tracer = new ObjectTracer<T>(handle, object);

    tracer->MakeWeak();

    return *tracer;
  }
};

class CManagedObject
{
protected:
  JNIEnv *m_pEnv;
  jobject m_obj;

  CManagedObject(JNIEnv *pEnv, jobject obj)
    : m_pEnv(pEnv), m_obj(m_pEnv->NewGlobalRef(obj))
  {
  }
public:
  virtual ~CManagedObject(void)
  {
    m_pEnv->DeleteGlobalRef(m_obj);
  }

  JNIEnv *GetEnv(void) const { return m_pEnv; }
  jobject GetObject(void) const { return m_obj; }

  static bool IsWrapped(v8::Handle<v8::Object> obj)
  {
    return !obj.IsEmpty() && obj->InternalFieldCount() == 1;
  }

  static CManagedObject& Unwrap(v8::Handle<v8::Object> obj)
  {
    v8::HandleScope handle_scope(v8::Isolate::GetCurrent());

    return *static_cast<CManagedObject *>(v8::Handle<v8::External>::Cast(obj->GetInternalField(0))->Value());
  }
};

template <class T>
class CBaseJavaObject : public CManagedObject {
protected:
  typedef CManagedObject __base__;
  typedef v8::Persistent<v8::ObjectTemplate> template_t;

  static void NamedGetter(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {}

  static void NamedSetter(
    v8::Local<v8::String> property, v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<v8::Value>& info) {}

  static void NamedQuery(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Integer>& info) {}

  static void NamedDeleter(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Boolean>& info) { info.GetReturnValue().Set(v8::False(v8::Isolate::GetCurrent())); }

  static void NamedEnumerator(
    const v8::PropertyCallbackInfo<v8::Array>& info) {}

  static void IndexedGetter(
    uint32_t index,
    const v8::PropertyCallbackInfo<v8::Value>& info) {}

  static void IndexedSetter(
    uint32_t index, v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<v8::Value>& info) {}

  static void IndexedQuery(
    uint32_t index,
    const v8::PropertyCallbackInfo<v8::Integer>& info) {}

  static void IndexedDeleter(
    uint32_t index,
    const v8::PropertyCallbackInfo<v8::Boolean>& info) { info.GetReturnValue().Set(v8::False(v8::Isolate::GetCurrent())); }

  static void IndexedEnumerator(
    const v8::PropertyCallbackInfo<v8::Array>& info) {}

  static void Caller(const v8::FunctionCallbackInfo<v8::Value>& info) {}

private:
  static void SetupObjectTemplate(v8::Handle<v8::ObjectTemplate> clazz)
  {
    clazz->SetInternalFieldCount(1);
    clazz->SetNamedPropertyHandler(T::NamedGetter, T::NamedSetter, T::NamedQuery, T::NamedDeleter, T::NamedEnumerator);
    clazz->SetIndexedPropertyHandler(T::IndexedGetter, T::IndexedSetter, T::IndexedQuery, T::IndexedDeleter, T::IndexedEnumerator);
    clazz->SetCallAsFunctionHandler(T::Caller);
  }

  static v8::Handle<v8::ObjectTemplate> CreateObjectTemplate(void)
  {
    v8::EscapableHandleScope handle_scope(v8::Isolate::GetCurrent());

    v8::Local<v8::ObjectTemplate> clazz = v8::ObjectTemplate::New();

    SetupObjectTemplate(clazz);

    return handle_scope.Escape(clazz);
  }
protected:
  static v8::Local<v8::Object> InternalWrap(T *obj)
  {
    v8::EscapableHandleScope handle_scope(v8::Isolate::GetCurrent());
    v8::TryCatch try_catch;

#ifdef __APPLE__
    static pthread_key_t s_object_template_key = 0;

    if (!s_object_template_key) {
        pthread_key_create(&s_object_template_key, NULL);
    }

    template_t *ptr_s_template = (template_t *)pthread_getspecific(s_object_template_key);
    template_t s_template;

    if (!ptr_s_template) {
        s_template.Reset(v8::Isolate::GetCurrent(), CreateObjectTemplate());
        pthread_setspecific(s_object_template_key, ptr_s_template);
    } else {
        s_template.Reset(v8::Isolate::GetCurrent(), * (template_t *) ptr_s_template);
    }
#elif _MSC_VER
    // BUG: Multithreaded usage is (probably) broken on Windows
    static v8::Persistent<v8::ObjectTemplate> s_template(v8::Isolate::GetCurrent(), CreateObjectTemplate());
#else
	static __thread template_t *ptr_s_template = NULL;
  template_t s_template;

	if (!ptr_s_template) {
    ptr_s_template = new v8::Persistent<v8::ObjectTemplate>(v8::Isolate::GetCurrent(), CreateObjectTemplate());
  }

  s_template.Reset(v8::Isolate::GetCurrent(), *ptr_s_template);
#endif

    v8::Handle<v8::ObjectTemplate> l_template = v8::Local<v8::ObjectTemplate>::New(v8::Isolate::GetCurrent(), s_template);
    v8::Local<v8::Object> instance = l_template->NewInstance();

    ObjectTracer<T>::Trace(instance, obj);

    instance->SetInternalField(0, v8::External::New(v8::Isolate::GetCurrent(), obj));

    return handle_scope.Escape(instance);
  }
public:
  CBaseJavaObject(JNIEnv *pEnv, jobject obj) : __base__(pEnv, obj)
  {

  }

  static v8::Local<v8::Object> Wrap(JNIEnv *pEnv, jobject obj)
  {
    v8::EscapableHandleScope handle_scope(v8::Isolate::GetCurrent());

    return handle_scope.Escape(InternalWrap(new T(pEnv, obj)));
  }

  static T& Unwrap(v8::Handle<v8::Object> obj)
  {
    return static_cast<T&>(__base__::Unwrap(obj));
  }
};

class CJavaObject : public CBaseJavaObject<CJavaObject> {
  typedef CBaseJavaObject<CJavaObject> __base__;
public:
  CJavaObject(JNIEnv *pEnv, jobject obj) : __base__(pEnv, obj) {
  }

  static void NamedGetter(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void NamedSetter(
    v8::Local<v8::String> property, v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void NamedQuery(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Integer>& info);

  static void NamedEnumerator(
    const v8::PropertyCallbackInfo<v8::Array>& info);
};

class CJavaArray : public CBaseJavaObject<CJavaArray> {
  typedef CBaseJavaObject<CJavaArray> __base__;
public:
  CJavaArray(JNIEnv *pEnv, jobject obj) : __base__(pEnv, obj) {
  }

  size_t GetLength(void) { return m_pEnv->GetArrayLength((jarray) m_obj); }

  static void NamedGetter(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void NamedQuery(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Integer>& info);

  static void IndexedGetter(
    uint32_t index,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void IndexedSetter(
    uint32_t index, v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void IndexedQuery(
    uint32_t index,
    const v8::PropertyCallbackInfo<v8::Integer>& info);

  static void IndexedEnumerator(
    const v8::PropertyCallbackInfo<v8::Array>& info);
};

class CJavaFunction {
  typedef std::vector<jclass> types_t;
  typedef std::pair<jobject, types_t> method_t;
  typedef std::vector<method_t> methods_t;

  JNIEnv *m_pEnv;
  methods_t m_methods;

  bool CanConvert(jclass clazz, v8::Handle<v8::Value> value);
public:
  CJavaFunction(JNIEnv *pEnv, jobject obj);
  CJavaFunction(JNIEnv *pEnv, const methods_t& methods)
    : m_pEnv(pEnv), m_methods(methods)
  {
  }
  virtual ~CJavaFunction(void)
  {
    ReleaseMethods(m_pEnv, m_methods);
  }

  JNIEnv *GetEnv(void) { return m_pEnv; }
  jobject GetMethod(const v8::FunctionCallbackInfo<v8::Value>& info);

  static void ReleaseMethods(JNIEnv *pEnv, const methods_t& methods);
  static const types_t GetParameterTypes(JNIEnv *pEnv, jobject method);

  template <typename T>
  static v8::Local<v8::Value> Wrap(JNIEnv *pEnv, T methods)
  {
    jni::V8Env env(pEnv);

    v8::Handle<v8::FunctionTemplate> func_tmpl = v8::FunctionTemplate::New(v8::Isolate::GetCurrent());

    CJavaFunction *func = new CJavaFunction(pEnv, methods);

    func_tmpl->SetCallHandler(Caller, v8::External::New(v8::Isolate::GetCurrent(), func));

    v8::Local<v8::Function> instance = func_tmpl->GetFunction();

    ObjectTracer<CJavaFunction>::Trace(instance, func);

    return v8::EscapableHandleScope(v8::Isolate::GetCurrent()).Escape(instance);
  }

  static void Caller(const v8::FunctionCallbackInfo<v8::Value>& info);
};

class CJavaBoundMethod {
  JNIEnv *m_pEnv;
  jobject m_thiz;
  jmethodID m_mid;
  bool m_is_void;
  bool m_has_args;

  bool CanConvert(jclass clazz, v8::Handle<v8::Value> value);
public:
  CJavaBoundMethod(JNIEnv *pEnv, jobject thiz, jmethodID mid, bool is_void, bool has_args)
    : m_pEnv(pEnv), m_thiz(m_pEnv->NewGlobalRef(thiz)), m_mid(mid), m_is_void(is_void), m_has_args(has_args)
  {
  }

  virtual ~CJavaBoundMethod(void)
  {
    m_pEnv->DeleteGlobalRef(m_thiz);
  }

  JNIEnv *GetEnv(void) { return m_pEnv; }
  jobject GetThiz(void) { return m_thiz; }
  jmethodID GetMid(void) { return m_mid; }
  bool IsVoid(void) { return m_is_void; }
  bool HasArgs(void) { return m_has_args; }

  static v8::Handle<v8::Function> Wrap(JNIEnv *pEnv, jobject thiz, jmethodID mid, bool is_void, bool has_args)
  {
    jni::V8Env env(pEnv);

    v8::Handle<v8::FunctionTemplate> func_tmpl = v8::FunctionTemplate::New(v8::Isolate::GetCurrent());

    CJavaBoundMethod *func = new CJavaBoundMethod(pEnv, thiz, mid, is_void, has_args);

    func_tmpl->SetCallHandler(Caller, v8::External::New(v8::Isolate::GetCurrent(), func));

    v8::Local<v8::Function> instance = func_tmpl->GetFunction();

    ObjectTracer<CJavaBoundMethod>::Trace(instance, func);

    return v8::EscapableHandleScope(v8::Isolate::GetCurrent()).Escape(instance);
  }

  static void Caller(const v8::FunctionCallbackInfo<v8::Value>& info);
};


class CJavaContext : public CBaseJavaObject<CJavaContext> {
  typedef CBaseJavaObject<CJavaContext> __base__;
public:
  CJavaContext(JNIEnv *pEnv, jobject obj) : __base__(pEnv, obj) {
  }
public:
};

} // namespace jni
