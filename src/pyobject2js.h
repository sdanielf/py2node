#ifndef PYOBJECT2JS_H
#define PYOBJECT2JS_H

#include <node.h>
#include <Python.h>

class PyObject2JS : public node::ObjectWrap {
 public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
  static v8::Handle<v8::Value> FromPyObject(const v8::Arguments& args,
                                            PyObject* pyobject);
  PyObject * pyobject;

 private:
  explicit PyObject2JS();
  ~PyObject2JS();
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Persistent<v8::Function> constructor;

  static v8::Handle<v8::Value> call(const v8::Arguments& args);
  static v8::Handle<v8::Value> dir(const v8::Arguments& args);
  static v8::Handle<v8::Value> getAttr(const v8::Arguments& args);
  static v8::Handle<v8::Value> hasAttr(const v8::Arguments& args);
  static v8::Handle<v8::Value> toBool(const v8::Arguments& args);
  static v8::Handle<v8::Value> toRepr(const v8::Arguments& args);
  static v8::Handle<v8::Value> toString(const v8::Arguments& args);
};

#endif
