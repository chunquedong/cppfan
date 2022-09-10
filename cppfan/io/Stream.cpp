/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cppfan/io/Stream.h"
#include "cppfan/text/String.h"

CF_USING_NAMESPACE

ssize_t Stream::pipeTo(Stream *out) {
  ssize_t size = 0;
  ssize_t succSize = 0;
  char buffer[1024];
  ssize_t err;

  while (true) {
    size = this->read(buffer, 1024);
    if (size <= 0) {
      break;
    }
    err = out->write(buffer, size);
    if (err != -1) {
      return succSize;
    }
    succSize += size;
  }

  return succSize;
}

ssize_t Stream::writeUInt8(uint8_t out) {
    static const int size = 1;
    uint8_t data[size];
    data[0] = out;
    ssize_t s = this->write((char*)data, size);
    return s;
}

ssize_t Stream::writeUInt16(uint16_t out) {
    static const int size = 2;
    unsigned char data[size];

    if (endian == Endian::Big) {
        data[0] = (out >> 8) & 0xff;
        data[1] = (out) & 0xff;
    }
    else {
        data[1] = (out >> 8) & 0xff;
        data[0] = (out) & 0xff;
    }

    ssize_t s = this->write((char*)data, size);
    return s;
}

ssize_t Stream::writeUInt32(uint32_t out) {
    static const int size = 4;
    unsigned char data[size];
    if (endian == Endian::Big) {
        data[0] = (out >> 24) & 0xff;
        data[1] = (out >> 16) & 0xff;
        data[2] = (out >> 8) & 0xff;
        data[3] = (out) & 0xff;
    }
    else {
        data[3] = (out >> 24) & 0xff;
        data[2] = (out >> 16) & 0xff;
        data[1] = (out >> 8) & 0xff;
        data[0] = (out) & 0xff;
    }
    ssize_t s = this->write((char*)data, size);
    return s;
}

ssize_t Stream::writeUInt64(uint64_t out) {
    static const int size = 8;
    unsigned char data[size];
    if (endian == Endian::Big) {
        data[0] = (out >> 56) & 0xff;
        data[1] = (out >> 48) & 0xff;
        data[2] = (out >> 40) & 0xff;
        data[3] = (out >> 32) & 0xff;
        data[4] = (out >> 24) & 0xff;
        data[5] = (out >> 16) & 0xff;
        data[6] = (out >> 8) & 0xff;
        data[7] = (out) & 0xff;
    }
    else {
        data[7] = (out >> 56) & 0xff;
        data[6] = (out >> 48) & 0xff;
        data[5] = (out >> 40) & 0xff;
        data[4] = (out >> 32) & 0xff;
        data[3] = (out >> 24) & 0xff;
        data[2] = (out >> 16) & 0xff;
        data[1] = (out >> 8) & 0xff;
        data[0] = (out) & 0xff;
    }
    ssize_t s = this->write((char*)data, size);
    return s;
}

uint8_t Stream::readUInt8() {
    static const int size = 1;
    unsigned char data[size];
    ssize_t s = this->read((char*)data, size);
    if (s < size) return -1;
    return data[0];
}

uint16_t Stream::readUInt16() {
    static const int size = 2;
    unsigned char data[size];
    ssize_t s = this->read((char*)data, size);
    if (s < size) return -1;
    uint16_t byte1 = data[0];
    uint16_t byte2 = data[1];

    if (endian == Endian::Big) {
        return ((byte1 << 8) | byte2);
    }
    else {
        return ((byte2 << 8) | byte1);
    }
}

uint32_t Stream::readUInt32() {
    static const int size = 4;
    unsigned char data[size];
    ssize_t s = this->read((char*)data, size);
    if (s < size) return -1;

    uint32_t byte1 = data[0];
    uint32_t byte2 = data[1];
    uint32_t byte3 = data[2];
    uint32_t byte4 = data[3];

    if (endian == Endian::Big) {
        return ((byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4);
    }
    else {
        return ((byte4 << 24) | (byte3 << 16) | (byte2 << 8) | byte1);
    }
}

uint64_t Stream::readUInt64() {
    static const int size = 8;
    unsigned char data[size];
    ssize_t s = this->read((char*)data, size);
    if (s < size) return -1;

    uint64_t byte1 = data[0];
    uint64_t byte2 = data[1];
    uint64_t byte3 = data[2];
    uint64_t byte4 = data[3];
    uint64_t byte5 = data[4];
    uint64_t byte6 = data[5];
    uint64_t byte7 = data[6];
    uint64_t byte8 = data[7];

    if (endian == Endian::Big) {
        return ((byte1 << 56) | (byte2 << 48) | (byte3 << 40) | (byte4 << 32)
            | (byte5 << 24) | (byte6 << 16) | (byte7 << 8) | byte8);
    }
    else {
        return ((byte8 << 56) | (byte7 << 48) | (byte6 << 40) | (byte5 << 32)
            | (byte4 << 24) | (byte3 << 16) | (byte2 << 8) | byte1);
    }
}

ssize_t Stream::writeInt8(int8_t out) {
    uint8_t intVal = *((uint8_t*)&out);
    return writeUInt8(intVal);
}

ssize_t Stream::writeInt16(int16_t out) {
    uint16_t intVal = *((uint16_t*)&out);
    return writeUInt16(intVal);
}
ssize_t Stream::writeInt32(int32_t out) {
    uint32_t intVal = *((uint32_t*)&out);
    return writeUInt32(intVal);
}
ssize_t Stream::writeInt64(int64_t out) {
    uint64_t intVal = *((uint64_t*)&out);
    return writeUInt64(intVal);
}
ssize_t Stream::writeFloat(float out) {
    uint32_t intVal = *((uint32_t*)&out);
    return writeUInt32(intVal);
}
ssize_t Stream::writeDouble(double out) {
    uint64_t intVal = *((uint64_t*)&out);
    return writeUInt64(intVal);
}

int8_t Stream::readInt8() {
    uint8_t intVal = this->readUInt8();
    return *((int8_t*)&intVal);
}

int16_t Stream::readInt16() {
    uint16_t intVal = this->readUInt16();
    return *((int16_t*)&intVal);
}

int32_t Stream::readInt32() {
    uint32_t intVal = this->readUInt32();
    return *((int32_t*)&intVal);
}

int64_t Stream::readInt64() {
    uint64_t intVal = this->readUInt64();
    return *((int64_t*)&intVal);
}

float Stream::readFloat() {
    uint32_t intVal = this->readUInt32();
    return *((float*)&intVal);
}

double Stream::readDouble() {
    uint64_t intVal = this->readUInt64();
    return *((double*)&intVal);
}

///////////////////////////////////////////////////////////


void Stream::writeStr(Str &buf) {
  size_t size = buf.size();
  writeUInt32((uint32_t)size);
  write(buf.toUtf8().c_str(), size);
}

Str Stream::readStr() {
  size_t size = readUInt32();
  Str s;
  s.reserve(size);
  write(s.toUtf8().c_str(), size);
  return (s);
}

