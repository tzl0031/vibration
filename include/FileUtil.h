#include <fstream>
#include <iostream>
#include <json/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

#ifndef FILEUTIL_H
#define FILEUTIL_H
class FileUtil {
public:
  FileUtil();
  int getPort();
  string getHost();
  string getRpiID();

private:
  string host;
  int port;
  string rpi_id;
};

#endif

