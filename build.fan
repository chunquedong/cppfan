/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-2-7  Jed Young  Creation
 */

using build

class Build : BuildGroup
{
  new make()
  {
    childrenScripts =
    [
      `cppfan/build.fan`,
      `cppfan/buildCfTest.fan`,
    ]
  }
}