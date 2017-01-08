/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cppfan/util/CsvReader.h"

CF_USING_NAMESPACE

using namespace std;

void fgetcsvline(vector<string> &csv_databuf, FILE *fhead)
{
  bool isNewStart = true;
  char preChar = 0;
  int doubleQuot = 0;
  bool startWithQuot = false;
  string word = "";
  char c;
  int partCount = 0;

  while (true)
  {
    c = fgetc(fhead);

    if (c == '"')
    {
      if (isNewStart)
      {
        isNewStart =false;
        doubleQuot = false;
        startWithQuot = true;//!
      }
      else
      {
        if (!startWithQuot)
        {
          word += '"';
          doubleQuot = false;
        }
        else if (preChar == '"')
        {
          if (doubleQuot)
          {
            word += '"';
            doubleQuot = false;
          }
          else
          {
            if (partCount == 2 && startWithQuot)
            {
            }
            else
            {
              doubleQuot = true;
            }
          }
        }
        else
        {

        }
      }
    }
    else if (c == '\r' || c == '\n')
    {
      if (startWithQuot && (preChar != '"' || doubleQuot))
      {
        word += c;
      }
      else if (partCount == 2 && preChar == '"')
      {
        word += "\\n";
      }
      else
      {
        //END VALUE MODEL
        csv_databuf.push_back(word);
        isNewStart = true;
        word = "";
        startWithQuot = false;
        partCount = 0;
        break;
      }

      doubleQuot = false;
    }
    else if (c == EOF)
    {
      if (isNewStart) break;
      else
      {
        if (startWithQuot && doubleQuot)
        {
          word += '"';
        }

        //END VALUE MODEL
        csv_databuf.push_back(word);
        isNewStart = true;
        word = "";
        partCount = 0;
        startWithQuot = false;
      }
      break;
    }
    else if (c == ',')
    {
      if (startWithQuot)
      {
        if (preChar == '"' && partCount >2)
        {
          if (doubleQuot)
          {
            word += '"';
            doubleQuot = false;
            word += c;
          }
          else
          {
            //END VALUE MODEL
            csv_databuf.push_back(word);
            isNewStart = true;
            word = "";
            partCount = 0;
            startWithQuot = false;
          }
        }
        else
        {
          word += c;
        }
      }
      else
      {
        //END VALUE MODEL
        csv_databuf.push_back(word);
        isNewStart = true;
        word = "";
        partCount = 0;
        startWithQuot = false;
      }
      doubleQuot = false;
    }
    else
    {
      if (doubleQuot)
      {
        word += '"';
        doubleQuot = false;
      }
      word += c;
      doubleQuot = false;
      isNewStart = false;
    }

    preChar = c;
    ++partCount;
  }

}

//////////////////////////////////////////////////////////////////////////
// Member
//////////////////////////////////////////////////////////////////////////

bool CsvReader::next()
{
  csv_data.clear();
  if (feof(fp_head)) return false;

  fgetcsvline(csv_data, fp_head);
  dump();
  return true;
}

void CsvReader::dump()
{
  size_t n = csv_data.size();
  for(int i=0; i<n ; ++i)
  {
    printf("%s\n", csv_data[i].c_str());
  }
}

bool CsvReader::open(string csvFileName)
{
  fp_head = fopen( csvFileName.c_str(), "rt" );
  return fp_head != NULL;
}

void CsvReader::close()
{
  fclose(fp_head);
}


string CsvReader::getValue(int i)
{
  return csv_data[i];
}
size_t CsvReader::getSize()
{
  return csv_data.size();
}
