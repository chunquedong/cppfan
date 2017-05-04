/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#ifndef _CF_FILE_H_
#define _CF_FILE_H_

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "cppfan/text/String.h"
#include "cppfan/util/Time.h"

CF_BEGIN_NAMESPACE

/**
 * Represents File Info or Dir Info
 *
 */
class File {
  Str _path;
  bool _isDir;
  bool _exists;
  size_t _size;
  MillisTime _mtime;//modified time

public:
  /**
   * constructor
   *
   */
  File() : _exists(false) {}
  File(const char *path) : _path(path) {}
    
  File(Str &path) : _path(path) {}
  
  Str &path() { return _path; }

  /**
   * load file infomation.
   * Must call this before get the file info.
   *
   */
  bool loadInfo();

  /**
   * return file size.
   *
   */
  size_t size() {
    return _size;
  }

  /**
   * is directory
   *
   */
  bool isDir() {
    return _isDir;
  }

  bool exists() {
    return _exists;
  }

  /**
   * create directory
   *
   */
  bool createDir();
  bool mkdirs();

  /**
   * remove file
   *
   */
  bool remove();
  
  bool rename(Str &name);

  /**
   * get file modify time
   *
   */
  MillisTime mtime() {
    return _mtime;
  }

  /**
   * Dir
   */
  std::vector<File> list();
    
    
  static Str getBaseName(Str &uri);
    
  static Str getName(Str &uri);
    
  static Str getParentPath(Str &uri);
    
  static Str getExtName(Str &uri);
  
  Str baseName() { return getBaseName(_path); }

  Str name() { return getName(_path); }
  
  File parent() { Str p = getParentPath(_path); return File(p); }
  
  Str extName() { return getExtName(_path); }

};


CF_END_NAMESPACE

#endif
