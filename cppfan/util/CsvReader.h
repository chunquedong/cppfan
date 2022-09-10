/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CEDIT_UTIL_CSVREADER_H_
#define _CEDIT_UTIL_CSVREADER_H_

#include <stdio.h>

#include <vector>
#include <map>
#include <string>

#include "cppfan/base/common.h"

CF_BEGIN_NAMESPACE

/**
 * Read CSV(Comma Separated Value) file
 */
class CsvReader
{
private:
  FILE  *fp_head;
  std::vector<std::string> csv_data;

public:
  std::string getValue(int i);
  size_t getSize();
  void dump();

public:
  bool next();
  bool open(std::string path);
  void close();
};

CF_END_NAMESPACE

#endif
