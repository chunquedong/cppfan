/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include <stdio.h>
#include "cppfan/cppfan.h"


CF_USING_NAMESPACE

/**
 * execute the test.
 *
 */
int main(int argc, char **argv) {
  const char *name;
  name = argc >= 2 ? argv[1] : "";


  //run test

  cf_Test_run(name, 0);


  cf_dumpMem();

  puts("END");
  return 0;
}

