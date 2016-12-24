/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CPPF_HASHMAP_H_
#define _CPPF_HASHMAP_H_

#include <unordered_map>

#include "cppfan/base/Object.h"
#include "cppfan/container/LinkedList.h"
#include "cppfan/base/Ref.h"
#include "cppfan/text/String.h"
#include "cppfan/mem/MemPool.h"



/*========================================================================
 * Hash Function
 */
namespace std {
  template <> struct hash<cf::Object> {
    size_t operator()(const cf::Object &key) const {
      return key.hashCode();
    }
  };
  template <> struct hash<cf::StrRef> {
    size_t operator()(const cf::StrRef &key) const {
      return key.hashCode();
    }
  };
  template <> struct hash<cf::Str> {
    size_t operator()(const cf::Str &key) const {
      return key.hashCode();
    }
  };
}

CF_BEGIN_NAMESPACE

/*========================================================================
 * Hash Map
 */

template<typename K, typename V>
class HashMap : public Object, public std::unordered_map<K,V> {
public:
  HashMap(size_t tableSize)
    : std::unordered_map<K,V>(tableSize) {}

  ~HashMap() {
  }

  V &get(const K &key, const K &defVal) const {
    auto itr = find(key);
    if (itr == this->end()) {
      return defVal;
    }
    return itr->second;
  }
  
  V &getOrAdd(const K &key, std::function<V(const K&)> func) {
    auto itr = this->find(key);
    if (itr == this->end()) {
      V v = func(key);
      set(key, v);
      return (*this)[key];
    }
    return itr->second;
  }

  V &set(const K &key, V &val) {
    (*this)[key] = val;
    return val;
  }

  bool contains(const K &key) const {
    auto itr = this->find(key);
    return itr != this->end();
  }

  bool remove(const K &key) {
    auto itr = this->find(key);
    if (itr == this->end()) {
      return false;
    }
    this->erase(itr);
    return true;
  }
};

CF_END_NAMESPACE
#endif // HASHMAP_H
