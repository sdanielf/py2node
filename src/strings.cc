#include <node.h>
#include "strings.h"

char * js2cstring(v8::Handle<v8::String> jsstring) {
  int len = jsstring->Utf8Length();
  char * cstring = new char[len];
  jsstring->WriteUtf8(cstring);
  return cstring;
}
