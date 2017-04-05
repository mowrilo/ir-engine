#include <iostream>
#include "scheduler.hpp"
#include <CkSpider.h>
#include <CkString.h>
#include <cstring>
#include <fstream>
#include <thread>
#include <mutex>
using namespace std;

class crawler{
  public:
        crawler(string &path);
        static void begin();
        static void crawl(string seedUrl);
        // string normalizeUrl(string &name);
        // bool isBr(string &name);

  private:
        string pathToStore;
        static scheduler sc;
        static mutex mutexQueue;
        static mutex mutexCrawled;
};
