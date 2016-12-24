/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#ifndef _CF_CONFIG_H_
#define _CF_CONFIG_H_

#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include "cppfan/io/File.h"

CF_BEGIN_NAMESPACE

/**
 * Tool for read config file.
 * the file format like Java's Properties file.
 * for example:
 *   key = value
 *
 */
class Config {
  File file;
  std::unordered_map<std::string, std::string> map;
  Time lastCheckTime;
  Time lastModifyTime;
  Time checkTime = TimeUtil::Minutes;

public:
  /**
   * load from file
   *
   */
  Config(const char *path);

  /**
   * get value by key
   *
   */
  std::string get(const char *key, const char *defVal = "");
  
private:
  void load();
};

CF_END_NAMESPACE

#endif
