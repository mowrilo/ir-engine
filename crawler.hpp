#include <iostream>
#include "scheduler.hpp"
#include <CkSpider.h>
#include <CkString.h>
#include <cstring>
#include <fstream>
#include <thread>
#include <queue>
#include <mutex>
using namespace std;

class crawler{
  public:
        crawler(string &path);
        void begin();
        static void crawl(string seedUrl);
        // string normalizeUrl(string &name);
        bool isBr(string &domain);

  private:
        static int nPages;
        string pathToStore;
        static scheduler sc;
        static mutex mutexQueue;
        static mutex mutexCrawled;
        static mutex mutexNPages;
        vector<string> seeds;
};
