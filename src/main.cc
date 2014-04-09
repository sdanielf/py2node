#include <Python.h>
#include <node.h>
#include <v8.h>

#include "pyobject2js.h"
#include "strings.h"

using namespace v8;

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

void init(Handle<Object> exports) {
    Py_Initialize();
    PyObject2JS::Init();
    exports->Set(String::NewSymbol("import"),
		 FunctionTemplate::New(Import)->GetFunction());
}

NODE_MODULE(py2node, init)
