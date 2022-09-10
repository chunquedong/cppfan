/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#include "cppfan/io/File.h"

CF_USING_NAMESPACE

bool File::mkdirs(){
  const char * path = _path.toUtf8().c_str();
  size_t len = strlen(path);
  char temp[1024];
  int lastSep = 0;
  int i;
  bool err = false;
  strcpy(temp, path);
  
  for (i = 2; i < len; ++i) {
    if (path[i] == '/' || path[i] == '\\'){
      lastSep = i;
      if(i > 0){
        temp[i] = 0;
        File file(temp);
        err = file.createDir();
      }
      temp[i] = '/';
    }
  }

  //last dir
  if (lastSep + 1 < len) {
    File file(temp);
    err = file.createDir();
  }
  return err;
}

bool File::rename(Str &name) {
  int rc = ::rename(_path.toUtf8().c_str(), name.toUtf8().c_str());
  return rc == 0;
}

Str File::getBaseName(Str &_path) {
    int i = _path.index(Str("/"));
    int j = _path.index(Str("."));
    if (i == -1 || j==-1 || i>=j) {
        return "";
    }
    return _path.substr(i+1, j-i-1);
}

Str File::getName(Str &_path) {
    int i = _path.index(Str("/"));
    if (i == -1) {
        return _path;
    }
    return _path.substr(i+1);
}

Str File::getParentPath(Str &_path) {
    int i = _path.index(Str("/"));
    if (i == -1) {
        return "";
    }
    return _path.substr(0, i);
}

Str File::getExtName(Str &_path) {
    int j = _path.index(Str("."));
    if (j==-1) {
        return "";
    }
    return _path.substr(j+1);
}

#ifdef CF_WIN

#include <Windows.h>

bool File::loadInfo() {
  HANDLE hFile;
  FILETIME ftCreate, ftAccess, ftWrite;
  DWORD lpFileSizeHigh;
  DWORD lpFileSizeLow;
  DWORD rc;
  unsigned __int64 ll;
  
  rc = GetFileAttributesA(_path.cstr());
  if (rc == INVALID_FILE_ATTRIBUTES) {
    goto error;
  }
  
  if (rc == FILE_ATTRIBUTE_DIRECTORY) {
    _isDir = true;
    _exists = true;
    _size = 0;
    _mtime = (time_t)0;
    return true;
  } else {
    _isDir = false;
  }
  
  
  hFile = CreateFileA(_path.cstr(), GENERIC_READ, FILE_SHARE_READ, NULL,
                      OPEN_EXISTING, 0, NULL);
  
  if(hFile == INVALID_HANDLE_VALUE) {
    goto error;
  }
  
  // Retrieve the file times for the file.
  if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
    goto error;
  }
  
  lpFileSizeLow = GetFileSize(hFile, &lpFileSizeHigh);
  if (lpFileSizeLow == INVALID_FILE_SIZE) {
    goto error;
  }
  
  _size = (size_t)lpFileSizeLow;
  
  //FILETIME to time_t
  ll = (((unsigned __int64)ftWrite.dwHighDateTime) << 32) + ftWrite.dwLowDateTime;
  _mtime = (time_t)((unsigned __int64)(ll - 116444736000000000) / 10000);
  _exists = true;
  
  CloseHandle(hFile);
  return true;
  
error:
  _exists = false;
  return false;
}

bool File::createDir() {
  if (CreateDirectoryA(_path.cstr(), NULL) != 0) {
    return true;
  }
  return false;
}

bool File::remove() {
  if (DeleteFileA(_path.cstr())) {
    return true;
  }
  return false;
}

std::vector<File> File::list() {
  std::vector<File> flist;
  
  WIN32_FIND_DATAA *findFileData;
  HANDLE hFind;
  char *regex = (char*)cf_malloc(strlen(_path.cstr())+3);
  strcpy(regex, _path.cstr());
  strcat(regex, "\\*");
  
  findFileData = (WIN32_FIND_DATAA *)cf_malloc(sizeof(WIN32_FIND_DATAA)*2);
  hFind = FindFirstFileA(regex, findFileData);
  cf_free(regex);
  if (hFind == INVALID_HANDLE_VALUE)
  {
    cf_free(findFileData);
    //self->second = NULL;
    printf ("FindFirstFile failed (%d)\n", (int)GetLastError());
    return flist;
  }

  if (hFind == 0) return flist;
  findFileData[1] = findFileData[0];
  
  while (FindNextFileA(hFind, findFileData))
  {
    File f(findFileData[1].cFileName);
    flist.push_back(f);
  }
  FindClose(hFind);
  cf_free(findFileData);
  return flist;
}

#else

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#if !defined(__IOS__) && !defined(__ANDROID__)
#include <sys/dir.h>
#endif
#include <dirent.h>

bool File::loadInfo() {
  struct stat stbuf;
  if (stat(_path.toUtf8().c_str(), &stbuf) == -1) {
    _exists = false;
    return false;
  }
  _size = stbuf.st_size;
  _isDir = S_ISDIR(stbuf.st_mode);
  _mtime = stbuf.st_mtime * 1000;
  _exists = true;
  return true;
}

bool File::createDir() {
  if (mkdir(_path.toUtf8().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
    return false;
  }
  return true;
}

bool File::remove() {
  if (::remove(_path.toUtf8().c_str()) == 0) {
    return true;
  }
  return false;
}

std::vector<File> File::list() {
  std::vector<File> flist;
  DIR *dir = opendir(_path.toUtf8().c_str());
  if (dir == NULL) return flist;
  
  struct dirent *ent = NULL;
  while (NULL != (ent = readdir(dir))) {
    File f(ent->d_name);
    flist.push_back(f);
  }
  
  closedir(dir);
  return flist;
}

#endif

