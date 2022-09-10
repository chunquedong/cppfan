/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CPPF_Ref_H
#define _CPPF_Ref_H

#include "cppfan/base/common.h"
#include "cppfan/base/Object.h"

CF_BEGIN_NAMESPACE

/**
 * Intrusive smart pointer. the T must be extend from Object
 */
template<typename T>
class Ref {
  T *pointer;
public:
  Ref() : pointer(NULL) {
    Object *p = pointer;
    p = NULL;
  }

  Ref(T *pointer) : pointer(pointer) {
  }

  Ref(const Ref &other) : pointer(other.pointer) {
    if (other.pointer) {
      bool ok = other.pointer->addRef();
      CF_UNUSED(ok);
      cf_assert(ok);
    }
  }

  virtual ~Ref() {
    clear();
  }

  Ref &operator=(const Ref& other) {
    if (other.pointer) {
      bool ok = other.pointer->addRef();
      CF_UNUSED(ok);
      cf_assert(ok);
    }
    if (pointer) {
      pointer->release();
    }
    pointer = other.pointer;
    return *this;
  }

  T *operator->() { return pointer; }

  T &operator*() { return *pointer; }

  T *get() { return pointer; }

  void set(T *p) { pointer = p; }

  bool isNull() { return pointer == NULL; }

  void clear() {
    if (pointer) {
      pointer->release();
      pointer = NULL;
    }
  }

  virtual long hashCode() const {
    if (pointer) {
      return pointer->hashCode();
    }
    return 0;
  }
  virtual bool equals(const Ref &other) const {
    if (pointer) {
      return pointer->equals(*const_cast<Ref&>(other).get());
    } else {
      return 0;
    }
  }
  virtual int compare(const Ref &other) const {
    if (pointer) {
      return pointer->compare(*const_cast<Ref&>(other).get());
    } else {
      return 0;
    }
  }
  bool operator==(const Ref &other) const { return equals(other); }
  bool operator<(const Ref &other) const { return compare(other) < 0; }
};

CF_END_NAMESPACE

#endif // Ref_H
