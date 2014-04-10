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

#ifndef PYOBJECT2JS_H
#define PYOBJECT2JS_H

#include <v8.h>
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
  static v8::Handle<v8::Value> toNumber(const v8::Arguments& args);
  static v8::Handle<v8::Value> toRepr(const v8::Arguments& args);
  static v8::Handle<v8::Value> toString(const v8::Arguments& args);
};

#endif
