/*
 * Copyright (C) Daniel Francis, 2014
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <Python.h>
#include <node.h>
#include <v8.h>

#include "pyobject2js.h"
#include "strings.h"

using namespace v8;

Handle<Value> Bool2Py(const Arguments& args) {
  HandleScope scope;
  bool boolean;

  if (args.Length() > 1) {
    ThrowException(Exception::TypeError(String::New("Too many arguments")));
    return scope.Close(Undefined());
  } else if (args.Length() == 1) {
    boolean = args[0]->ToBoolean()->Value();
  } else {
    boolean = false;
  }

  PyObject * pyboolean;
  pyboolean = boolean ? Py_True : Py_False;

  return scope.Close(PyObject2JS::FromPyObject(args, pyboolean));
}

Handle<Value> Dict2Py(const Arguments& args) {
  HandleScope scope;
  Local<Object> object;

  if (args.Length() > 1) {
    ThrowException(Exception::TypeError(String::New("Too many arguments")));
    return scope.Close(Undefined());
  } else if (args.Length() == 1) {
    object = args[0]->ToObject();
  } else {
    object = Object::New();
  }

  Local<Array> keys = object->GetOwnPropertyNames();
  int length = keys->Length();
  PyObject * dict = PyDict_New();
  for (int i = 0; i < length; i++) {
    Handle<Value> key = keys->Get(i);
    char * ckey = js2cstring(key->ToString());
    Handle<Value> value = object->Get(key);
    PyObject2JS * pyvalue = node::ObjectWrap::Unwrap<PyObject2JS>(
        value->ToObject());
    PyDict_SetItemString(dict, ckey, pyvalue->pyobject);
  }

  return scope.Close(PyObject2JS::FromPyObject(args, dict));
}

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

  PyObject * pyfloat = PyFloat_FromDouble(number);

  return scope.Close(PyObject2JS::FromPyObject(args, pyfloat));
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

  PyObject * pyint = PyInt_FromSsize_t(number);

  return scope.Close(PyObject2JS::FromPyObject(args, pyint));
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

Handle<Value> List2Py(const Arguments& args) {
  HandleScope scope;
  Local<Array> array;

  if (args.Length() > 1) {
    ThrowException(Exception::TypeError(String::New("Too many arguments")));
    return scope.Close(Undefined());
  } else if (args.Length() == 1) {
    array = Local<Array>::Cast(args[0]);
  } else {
    array = Array::New();
  }

  int length = array->Length();
  PyObject * list = PyList_New(0);
  for (int i = 0; i < length; i++) {
    Local<Value> element = array->Get(i);
    PyObject2JS * item = node::ObjectWrap::Unwrap<PyObject2JS>(
        element->ToObject());
    PyList_Append(list, item->pyobject);
  }

  return scope.Close(PyObject2JS::FromPyObject(args, list));
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

  PyObject * pystring = PyString_FromString(cstring);

  return scope.Close(PyObject2JS::FromPyObject(args, pystring));
}

Handle<Value> Tuple2Py(const Arguments& args) {
  HandleScope scope;
  Local<Array> array;

  if (args.Length() > 1) {
    ThrowException(Exception::TypeError(String::New("Too many arguments")));
    return scope.Close(Undefined());
  } else if (args.Length() == 1) {
    array = Local<Array>::Cast(args[0]);
  } else {
    array = Array::New();
  }

  int length = array->Length();
  PyObject * tuple = PyTuple_New(length);
  for (int i = 0; i < length; i++) {
    Local<Value> element = array->Get(i);
    PyObject2JS * item = node::ObjectWrap::Unwrap<PyObject2JS>(
        element->ToObject());
    PyTuple_SetItem(tuple, i, item->pyobject);
  }

  return scope.Close(PyObject2JS::FromPyObject(args, tuple));
}

void init(Handle<Object> exports) {
  Py_Initialize();
  PyObject2JS::Init();
  exports->Set(String::NewSymbol("Bool"),
   FunctionTemplate::New(Bool2Py)->GetFunction());
  exports->Set(String::NewSymbol("Dict"),
   FunctionTemplate::New(Dict2Py)->GetFunction());
  exports->Set(String::NewSymbol("Float"),
   FunctionTemplate::New(Float2Py)->GetFunction());
  exports->Set(String::NewSymbol("Int"),
   FunctionTemplate::New(Int2Py)->GetFunction());
  exports->Set(String::NewSymbol("import"),
   FunctionTemplate::New(Import)->GetFunction());
  exports->Set(String::NewSymbol("List"),
   FunctionTemplate::New(List2Py)->GetFunction());
  exports->Set(String::NewSymbol("String"),
   FunctionTemplate::New(String2Py)->GetFunction());
  exports->Set(String::NewSymbol("Tuple"),
   FunctionTemplate::New(List2Py)->GetFunction());
}

NODE_MODULE(py2node, init)
