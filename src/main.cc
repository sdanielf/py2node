#include <Python.h>
#include <node.h>
#include <v8.h>

#include "pyobject2js.h"
#include "strings.h"

using namespace v8;

Handle<Value> Float2Py(const Arguments& args) {
  HandleScope scope;
  double number;

  if (args.Length() > 1) {
    ThrowException(Exception::TypeError(String::New("Too many arguments")));
    return scope.Close(Undefined());
  } else if (args.Length() == 1) {
    number = args[0]->ToNumber()->Value();
  } else {
    number = 0;
  }

  PyObject * pymodule = PyFloat_FromDouble(number);

  return scope.Close(PyObject2JS::FromPyObject(args, pymodule));
}

Handle<Value> Int2Py(const Arguments& args) {
  HandleScope scope;
  Py_ssize_t number;

  if (args.Length() > 1) {
    ThrowException(Exception::TypeError(String::New("Too many arguments")));
    return scope.Close(Undefined());
  } else if (args.Length() == 1) {
    number = args[0]->ToNumber()->Value();
  } else {
    number = 0;
  }

  PyObject * pymodule = PyInt_FromSsize_t(number);

  return scope.Close(PyObject2JS::FromPyObject(args, pymodule));
}

Handle<Value> Import(const Arguments& args) {
  HandleScope scope;
  if (args.Length() < 1) {
    ThrowException(Exception::TypeError(
                        String::New("Name of module is required")));
      return scope.Close(Undefined());
  }
  if (args.Length() > 1) {
    ThrowException(Exception::TypeError(
        String::New("Returning multiple modules is not supported")));
    return scope.Close(Undefined());
  }
  if (!args[0]->IsString()) {
      ThrowException(Exception::TypeError(
			      String::New("Module name must be a string")));
      return scope.Close(Undefined());
  }

  PyObject * pymodule = PyImport_ImportModule(js2cstring(args[0]->ToString()));

  return scope.Close(PyObject2JS::FromPyObject(args, pymodule));
}

Handle<Value> String2Py(const Arguments& args) {
  HandleScope scope;
  char * cstring;

  if (args.Length() > 1) {
    ThrowException(Exception::TypeError(String::New("Too many arguments")));
    return scope.Close(Undefined());
  } else if (args.Length() == 1) {
    cstring = js2cstring(args[0]->ToString());
  } else {
    cstring = new char[0];
  }

  PyObject * pymodule = PyString_FromString(cstring);

  return scope.Close(PyObject2JS::FromPyObject(args, pymodule));
}

void init(Handle<Object> exports) {
  Py_Initialize();
  PyObject2JS::Init();
  exports->Set(String::NewSymbol("Int"),
   FunctionTemplate::New(Int2Py)->GetFunction());
  exports->Set(String::NewSymbol("import"),
   FunctionTemplate::New(Import)->GetFunction());
  exports->Set(String::NewSymbol("Float"),
   FunctionTemplate::New(Float2Py)->GetFunction());
  exports->Set(String::NewSymbol("String"),
   FunctionTemplate::New(String2Py)->GetFunction());
}

NODE_MODULE(py2node, init)
