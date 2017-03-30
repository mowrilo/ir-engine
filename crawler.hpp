#include <iostream>
#include "scheduler.hpp"
#include <CkSpider.h>
#include <CkString.h>
#include <cstring>
#include <fstream>
using namespace std;

class crawler{
  public:
        crawler();
        void begin();
        string normalizeUrl(string &name);
        bool isBr(string &name);
};
