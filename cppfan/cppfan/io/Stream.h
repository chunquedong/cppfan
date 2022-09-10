/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CPPF_STREAM_H
#define _CPPF_STREAM_H

#include "cppfan/base/common.h"
#include "cppfan/base/Object.h"

CF_BEGIN_NAMESPACE

class String;
/**
 * ByteOrder
 */
class Endian {
public:
  enum Type {
    Big,
    Little
  };
  
  static inline Type hostEndian() {
    static const int __one = 1;
    return (*(char *)(&__one)==0) ? Endian::Big : Endian::Little;
  }
  
  static inline void swap16p(void *lpMem) {
    char * p = (char*)lpMem;
    p[0] = p[0] ^ p[1];
    p[1] = p[0] ^ p[1];
    p[0] = p[0] ^ p[1];
  }
  
  static inline void swap32p(void *lpMem) {
    char * p = (char*)lpMem;
    p[0] = p[0] ^ p[3];
    p[3] = p[0] ^ p[3];
    p[0] = p[0] ^ p[3];
    p[1] = p[1] ^ p[2];
    p[2] = p[1] ^ p[2];
    p[1] = p[1] ^ p[2];
  }
  
  static inline void swap64p(void *lpMem) {
    char * p = (char*)lpMem;
    p[0] = p[0] ^ p[7];
    p[7] = p[0] ^ p[7];
    p[0] = p[0] ^ p[7];
    p[1] = p[1] ^ p[6];
    p[6] = p[1] ^ p[6];
    p[1] = p[1] ^ p[6];
    p[2] = p[2] ^ p[5];
    p[5] = p[2] ^ p[5];
    p[2] = p[2] ^ p[5];
    p[3] = p[3] ^ p[4];
    p[4] = p[3] ^ p[4];
    p[3] = p[3] ^ p[4];
  }

};

#ifndef ssize_t
  #define ssize_t signed long
#endif

class Stream : public Object, NoCopyable {
public:
  Endian::Type endian = Endian::Big;
  virtual ssize_t write(const char *buf, size_t size) = 0;

  /**
   * @return the total number of bytes read into the buffer,
   * or -1 if there is no more data because the end of the stream has been reached.
   */
  virtual ssize_t read(char *buf, size_t size) = 0;
  
  virtual void flush() {}
  
  virtual void close() {}

  virtual ssize_t pipeTo(Stream *out);

  ssize_t writeInt8(int8_t out);
  ssize_t writeInt16(int16_t out);
  ssize_t writeInt32(int32_t out);
  ssize_t writeInt64(int64_t out);
  ssize_t writeUInt8(uint8_t out);
  ssize_t writeUInt16(uint16_t out);
  ssize_t writeUInt32(uint32_t out);
  ssize_t writeUInt64(uint64_t out);
  ssize_t writeFloat(float out);
  ssize_t writeDouble(double out);

  uint8_t readUInt8();
  uint16_t readUInt16();
  uint32_t readUInt32();
  uint64_t readUInt64();
  int8_t readInt8();
  int16_t readInt16();
  int32_t readInt32();
  int64_t readInt64();
  float readFloat();
  double readDouble();
  
  void writeStr(String &buf);
  String readStr();
};

CF_END_NAMESPACE

#endif // STREAM_H
