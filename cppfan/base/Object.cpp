#include "cppfan/base/Object.h"
#include "cppfan/util/System.h"

CF_USING_NAMESPACE

bool Object::addRef() {
  int oldVal = System::increase(&refCount);
  return oldVal >= 0;
}

Object *Object::release() {
  System::decrease(&refCount);
  if (refCount < 0) {
    dispose();
    delete this;
    return NULL;
  }
  return this;
}
