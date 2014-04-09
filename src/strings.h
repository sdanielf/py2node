#ifndef STRINGS_H
#define STRINGS_H

#include <v8.h>
#include <node.h>

char * js2cstring(v8::Handle<v8::String> jsstring);

#endif
