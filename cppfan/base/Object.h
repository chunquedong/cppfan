/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CPPF_OBJECT_H_
#define _CPPF_OBJECT_H_

#include "cppfan/base/common.h"
#include "cppfan/mem/Memory.h"

CF_BEGIN_NAMESPACE

#define CF_OBJECT(Type) public: virtual const char *type(){ return #Type; }\
        static const char *staticType(){ return #Type; } private:

class NoCopyable {
public:
  NoCopyable() = default;
private:
  NoCopyable(const NoCopyable&) = delete;
  NoCopyable& operator = (const NoCopyable &) = delete;
};

/**
 * Base class for all class.
 * support reference count to manager memory.
 */
class Object {
  CF_OBJECT(Object)
protected:
  /**
   * current reference count, base 0
   */
  int refCount;

public:
  CF_OVERRIDE_NEW
  Object() : refCount(0) {}

  virtual ~Object() {}

  /**
   * increase reference count
   * @return true if success. return false if pointer be deleted.
   */
  bool addRef();

  /**
   * will call by release to cleanup.
   */
  virtual void dispose() {}

  /**
   * decrease reference count.
   * if no any reference will call delete self.
   */
  virtual Object *release();

  /**
   * get current reference count, base 1.
   */
  int getRefCount() const { return refCount+1; }
  
  bool operator==(const Object &other) const { return equals(other); }
  bool operator!=(const Object &other) const { return !equals(other); }
  bool operator<(const Object &other) const { return compare(other) < 0; }

  virtual size_t hashCode() const { return (size_t)this; }
  virtual int compare(const Object &other) const { return (int)((long long)this - (long long)&other); }
  virtual bool equals(const Object &other) const { return (long long)this == (long long)&other; }

};

template<class T>
inline void cf_deleteIt(T& o) {
  CF_UNUSED(o);
}

template<class T>
inline void cf_deleteIt(T* o) {
  if (o == nullptr) return;
  if (std::is_base_of<Object, T>::value) {
    Object * obj = dynamic_cast<Object*>(o);
    if (obj) obj->release();
  }
  else delete o;
}


CF_END_NAMESPACE
#endif // OBJECT_H
