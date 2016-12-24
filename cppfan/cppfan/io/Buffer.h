//
//  Buffer.h
//  zip
//
//  Created by chunquedong on 15/6/26.
//  Copyright (c) 2015年 chunquedong. All rights reserved.
//

#ifndef __zip__Buffer__
#define __zip__Buffer__

#include <stdio.h>
#include <inttypes.h>
#include <string>

#include "cppfan/io/Stream.h"

CF_BEGIN_NAMESPACE

/**
 * ByteArray is memory buffer
 */
class Buffer : public Stream {
private:
  uint8_t* data;
  size_t _pos;
  size_t _size;
  bool owner;
  
public:
  Buffer();
  Buffer(size_t size);
  Buffer(uint8_t* data, size_t size, bool owner);
  ~Buffer();

  virtual ssize_t write(const char *buf, size_t size);
  virtual ssize_t read(char *buf, size_t size);

  void readSlice(Buffer &out, bool copy);

  void seek(int pos);
  size_t pos() { return _pos; }
  bool eof() { return _pos >= _size; }
  size_t size() { return _size; }
  size_t remaining() { return _size - _pos; }
  
  //read Data no copy
  unsigned char * readDirect(int len);
  unsigned char *getData() { return data; }
};

CF_END_NAMESPACE
#endif /* defined(__zip__Buffer__) */
