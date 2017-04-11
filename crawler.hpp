#include <iostream>
#include "scheduler.hpp"
#include "kmp.hpp"
#include "filemanager.hpp"
#include <CkSpider.h>
#include <CkString.h>
#include <cstring>
#include <fstream>
#include <thread>
#include <queue>
#include <mutex>
#include <string>
using namespace std;

class crawler{
  public:
        crawler(string &path);
        void begin();
        static void crawl(string seedUrl, int id);
        static string getUrlDomain(string &url);
        static string normalizeUrl(string &name);
        static int isBr(string &domain);
        static bool isDomain(string &subUrl);

  private:
        static int nPages;
        string pathToStore;
        static scheduler sc;
        static mutex mutexQueue;
        static mutex mutexCrawledDomains;
        static mutex mutexCrawledPages;
        static mutex mutexNPages;
        vector<string> seeds;
        static vector<string> domainTypes;
        static int chamada;
};
