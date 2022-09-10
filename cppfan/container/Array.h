//
//  Array.h
//  cppfan
//
//  Created by yangjiandong on 2017/1/8.
//  Copyright © 2017年 yangjiandong. All rights reserved.
//

#ifndef Array_h
#define Array_h

#include "cppfan/base/Object.h"
#include <vector>
#include <algorithm>
#include <functional>

CF_BEGIN_NAMESPACE

template<typename T>
class Array : public std::vector<T> {
public:
  
  void add(const T& t) {
    this->push_back(t);
  }
  
  T pop() {
    T temp = this->back();
    this->pop_back();
    return temp;
  }
  T &get(size_t i) {
      return this->at(i);
  }

  void removeAt(size_t i) {
    this->erase(this->begin()+i);
  }
  void insert(size_t i, const T& t) {
    this->insert(this->begin()+i, t);
  }
  void addAll(Array &other) {
    this->reserve(this->size() + other.size());
    for (int i=0; i<other.size(); ++i) {
      add(other[i]);
    }
  }
public:
  void deleteAll() {
    for (int i=0; i<this->size(); ++i) {
      cf_deleteIt((*this)[i]);
    }
    this->clear();
  }
  bool contains(const T &t) {
    for (int i=0; i<this->size(); ++i) {
      if ((*this)[i] == t) {
        return true;
      }
    }
    return false;
  }
  bool equals(Array &other) {
    if (this->size() != other.size()) return false;
    for (int i=0; i<other.size(); ++i) {
      if ((*this)[i]  != other[i]) {
        return false;
      }
    }
    return true;
  }
public:
  Array sort(std::function<bool(const T&, const T&)> less) {
    std::sort(this->begin(), this->end(), less);
  }
  
  void reverse() {
    std::reverse(this->begin(), this->end());
  }
  
  void unique() {
    std::unique(this->begin(), this->end());
  }
  
  void each(std::function<void(const T&)> func) {
    for (auto itr = this->begin(); itr != this->end(); ++itr) {
      func(*itr);
    }
  }
  
  template<typename O>
  Array<O> map(std::function<O(const T&)> func) {
    Array<O> list;
    list.reserve(this->size());
    for (auto itr = this->begin(); itr != this->end(); ++itr) {
      list.push_back(func(*itr));
    }
    return list;
  }
  
  Array filter(std::function<bool(const T&)> func) {
    Array list;
    for (auto itr = this->begin(); itr != this->end(); ++itr) {
      if (func(*itr)) {
        list.push_back(*itr);
      }
    }
    return list;
  }
  
  template<typename O>
  O reduce(O init, std::function<O(O, const T&)> func) {
    for (auto itr = this->begin(); itr != this->end(); ++itr) {
      init = func(init, *itr);
    }
    return init;
  }
  
};

CF_END_NAMESPACE

#endif /* Array_h */
