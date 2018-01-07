/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CPPF_FILESTREAM_H
#define _CPPF_FILESTREAM_H

#include "cppfan/io/Stream.h"
#include <stdio.h>

CF_BEGIN_NAMESPACE

class FileStream : public Stream {
public:
  FILE *file;
public:
  FileStream(FILE *file) : file(file) {
    cf_assert(file);
  }

  static FileStream *make(const char *path, const char *modes) {
    FILE *f = fopen(path, modes);
    if (f == NULL) return NULL;
    return _new FileStream(f);
  }

  virtual ssize_t read(char *out, size_t n) override {
    ssize_t c = fread(out, 1, n, file);
    return c;
  }

  virtual ssize_t write(const char *m, size_t n) override {
    ssize_t c = fwrite(m, n, 1, file);
    return c;
  }

  virtual void flush() override {
    fflush(file);
  }

  virtual void close() override {
    int c = fclose(file);
    CF_UNUSED(c);
    cf_assert(c == 0);
  }

  bool seek(int64_t pos) {
    //const fpos_t p = pos;
    int c = fseek(file, (long)pos, SEEK_SET);
    return c == 0;
  }

  int64_t pos() {
    //fpos_t p;
    //fgetpos(file, &p);
    long p = ftell(file);
    return p;
  }

  bool toEnd() {
    int c = fseek(file, 0, SEEK_END);
    return c == 0;
  }
  
  bool eof() {
    return feof(file);
  }

  void rewind() {
    ::rewind(file);
  }

  bool peek(int *c) {
    *c = fgetc(file);
    int nc = ungetc(*c, file);
    return (*c == nc && nc != EOF);
  }

  bool getError() {
    if (ferror(file)) {
      return false;
    }
    return true;
  }
    
  String readLine() {
    //char *fgets( char *str, int count, FILE *stream );
    wchar_t buf[7693];
    const wchar_t *s = fgetws(buf, 7693, file);
    if (!s) return Str();
    return Str(s);
  }
};


CF_END_NAMESPACE

#endif // FILESTREAM_H
