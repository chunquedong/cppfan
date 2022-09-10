/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CPPF_CACHE_H
#define _CPPF_CACHE_H

#include "cppfan/base/Object.h"
#include "cppfan/container/HashMap.h"
#include "cppfan/container/LinkedList.h"

CF_BEGIN_NAMESPACE

/**
 * LRU Cache
 */
template<typename K, typename V>
class Cache : public Object, NoCopyable {
  struct CacheItem {
    K key;
    V val;
    CacheItem *previous;
    CacheItem *next;
  };
  HashMap<K,CacheItem*> map;
  LinkedList<CacheItem> list;
  CF_FIELD(long, maxSize)
public:
  Cache(long maxSize) : map(maxSize), _maxSize(maxSize) {
  }

  ~Cache() {
    clear();
  }

  V &get(K &key) {
    CacheItem *item = map[key];
    list.remove(item);
    list.insertFirst(item);
    return item->val;
  }

  V &_get(K &key) {
    CacheItem *item = map[key];
    return item->val;
  }

  void set(K &key, V &val) {
    CacheItem *item = new CacheItem();
    item->key = key;
    item->val = val;
    map.set(key, item);
    list.insertFirst(item);

    clearUp(maxSize());
  }

  bool contains(K &key) const {
    return map.contains(key);
  }

  long size() const { return map.size(); }

  void clear() {
    clearUp(0);
  }

protected:
  virtual void onRemove(K &key, V &val) {
    CF_UNUSED(key);
    cf_deleteIt(val);
  }

private:
  void clearUp(long max) {
    while (map.size() > max) {
      CacheItem *item = list.last();
      map.remove(item->key);
      list.remove(item);
      onRemove(item->key, item->val);
      delete item;
    }
  }
};

CF_END_NAMESPACE

#endif // CACHE_H
