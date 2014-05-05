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

#include <node.h>
#include <v8.h>
#include "pyobject2js.h"
#include "strings.h"

using namespace v8;

Persistent<Function> PyObject2JS::constructor;

PyObject2JS::PyObject2JS() : pyobject(NULL) {
}

PyObject2JS::~PyObject2JS() {
}

void PyObject2JS::Init() {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("PyObject"));

  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->InstanceTemplate()->Set(String::NewSymbol("isPyObject"),
                               Boolean::New(true));

  tpl->PrototypeTemplate()->Set(String::NewSymbol("dir"),
		         FunctionTemplate::New(dir)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("call"),
		         FunctionTemplate::New(call)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getAttr"),
		         FunctionTemplate::New(getAttr)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getItem"),
		         FunctionTemplate::New(getItem)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("hasAttr"),
		         FunctionTemplate::New(hasAttr)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("length"),
		         FunctionTemplate::New(length)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toBool"),
		         FunctionTemplate::New(toBool)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toNumber"),
		         FunctionTemplate::New(toNumber)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
		         FunctionTemplate::New(toString)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toRepr"),
		         FunctionTemplate::New(toRepr)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> PyObject2JS::New(const Arguments& args) {
  HandleScope scope;

  if (args.IsConstructCall()) {
    PyObject2JS * obj = new PyObject2JS();
    obj->Wrap(args.Holder());
    return args.Holder();
  } else {
    Local<Value> argv[0] = {};
    return scope.Close(constructor->NewInstance(0, argv));
  }
}

Handle<Value> PyObject2JS::NewInstance(const Arguments& args) {
  HandleScope scope;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}

Handle<Value>
PyObject2JS::FromPyObject(const Arguments& args, PyObject* pyobject) {
  HandleScope scope;
  Handle<Value> jsobject = PyObject2JS::NewInstance(args);
  PyObject2JS * obj =
	  node::ObjectWrap::Unwrap<PyObject2JS>(jsobject->ToObject());
  obj->pyobject = pyobject;
  return scope.Close(jsobject);
}

Handle<Value> PyObject2JS::call(const Arguments& args) {
  HandleScope scope;
  int argc = args.Length();
  PyObject2JS * obj = ObjectWrap::Unwrap<PyObject2JS>(args.Holder());
/*
  PyObject * pyargs;
  PyObject * pykwargs;
  if (argc == 0) {
    pyargs = PyTuple_New(0);
  } else {
    
  }
*/
  PyObject * list = PyTuple_New(argc);
  for (int i = 0; i < argc; i++) {
    PyObject2JS * arg = ObjectWrap::Unwrap<PyObject2JS>(args[i]->ToObject());
    PyTuple_SetItem(list, i, arg->pyobject);
  }
  return scope.Close(FromPyObject(args, PyObject_CallObject(obj->pyobject, list)));
}

Handle<Value> PyObject2JS::dir(const Arguments& args) {
  HandleScope scope;
  PyObject2JS * obj = ObjectWrap::Unwrap<PyObject2JS>(args.Holder());
  PyObject * attributes = PyObject_Dir(obj->pyobject);
  Py_ssize_t size = PyList_Size(attributes);
  Local<Array> jsarray = Array::New(size);

  for (int i=0; i < size; i++) {
    PyObject * item = PyList_GetItem(attributes, i);
    jsarray->Set(Number::New(i), String::New(PyString_AsString(item)));
  }
  return scope.Close(jsarray);
}

Handle<Value> PyObject2JS::getAttr(const Arguments& args) {
  HandleScope scope;
  PyObject2JS * obj = ObjectWrap::Unwrap<PyObject2JS>(args.Holder());
  return scope.Close(FromPyObject(args, PyObject_GetAttrString(obj->pyobject,
				  js2cstring(args[0]->ToString()))));
}

Handle<Value> PyObject2JS::getItem(const Arguments& args) {
  HandleScope scope;
  PyObject2JS * obj = ObjectWrap::Unwrap<PyObject2JS>(args.Holder());
  PyObject2JS * key = ObjectWrap::Unwrap<PyObject2JS>(args[0]->ToObject());
  return scope.Close(FromPyObject(args, PyObject_GetItem(obj->pyobject, key->pyobject)));
}

Handle<Value> PyObject2JS::hasAttr(const Arguments& args) {
  HandleScope scope;
  PyObject2JS * obj = ObjectWrap::Unwrap<PyObject2JS>(args.Holder());
  return scope.Close(Boolean::New(PyObject_HasAttrString(obj->pyobject,
				  js2cstring(args[0]->ToString()))));
}

Handle<Value> PyObject2JS::length(const Arguments& args) {
  HandleScope scope;
  PyObject2JS * obj = ObjectWrap::Unwrap<PyObject2JS>(args.Holder());
  return scope.Close(Number::New(PyObject_Length(obj->pyobject)));
}

Handle<Value> PyObject2JS::toBool(const Arguments& args) {
  HandleScope scope;
  PyObject2JS * obj = ObjectWrap::Unwrap<PyObject2JS>(args.Holder());
  return scope.Close(Boolean::New(PyObject_IsTrue(obj->pyobject)));
}

Handle<Value> PyObject2JS::toNumber(const Arguments& args) {
  HandleScope scope;
  PyObject2JS * obj = ObjectWrap::Unwrap<PyObject2JS>(args.Holder());
  return scope.Close(Number::New(PyFloat_AsDouble(
          PyNumber_Float(obj->pyobject))));
}

Handle<Value> PyObject2JS::toRepr(const Arguments& args) {
  HandleScope scope;
  PyObject2JS * obj = ObjectWrap::Unwrap<PyObject2JS>(args.Holder());
  return scope.Close(String::New(PyString_AsString(
				  PyObject_Repr(obj->pyobject))));
}

Handle<Value> PyObject2JS::toString(const Arguments& args) {
  HandleScope scope;
  PyObject2JS * obj = ObjectWrap::Unwrap<PyObject2JS>(args.Holder());
  return scope.Close(String::New(PyString_AsString(
				  PyObject_Str(obj->pyobject))));
}
