/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#include "cppfan/util/Config.h"
#include "cppfan/mem/StackTrace.h"

CF_USING_NAMESPACE

Config::Config(const char *path) : file(path),
  lastCheckTime(0), lastModifyTime(0) {
}

std::string Config::get(const char *key, const char *defVal) {
  MillisTime now = TimeUtil::currentTimeMillis();
  if (now - lastCheckTime > checkTime) {
    lastCheckTime = now;
    if (file.mtime() > lastModifyTime) {
      file.loadInfo();
      load();
    }
    lastModifyTime = file.mtime();
  }
  
  auto it = map.find(key);
  if (it == map.end()) {
    return defVal;
  }
  return it->second;
}

void Config::load() {
  const char *path = file.path().cstr();
  FILE *file;
  signed char c;//current char
  char key[1024];//key buffer
  char value[1024];//value buffer
  int pos;//current position
  int mode;//token mode, 0 is key, 1 is value;
  //CF_ENTRY_FUNC

  //open file
  file = fopen(path, "r");
  if (file == NULL) {
    //cf_HashMapSS_make(&self->map, 1);
    //CF_EXIT_FUNC
    return;
  }

  //make map
  //cf_HashMapSS_make(&self->map, 1024);

  c = fgetc(file);
  pos = 0;
  mode = 0;
  while (EOF != c) {
    if (c == '\r' || c == '\n') {
      if (mode == 1) {
        // deal new line
        mode = 0;
        value[pos] = 0;
        //cf_HashMapSS_set(&self->map, k, v, NULL, NULL);
        map[key] = value;
      }
      pos = 0;
    } else {
      if (mode == 0) {
        // read key
        if (c == '=') {
          //end read key
          mode = 1;
          key[pos] = 0;
          pos = 0;
        } else {
          key[pos++] = c;
        }
      } else {
        //read value
        value[pos++] = c;
      }
    }

    c = fgetc(file);
  }

  //last line
  if (pos > 0) {
    value[pos] = 0;
    map[key] = value;
  }

  fclose(file);

  //CF_EXIT_FUNC
  return;
}
