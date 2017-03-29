#include <iostream>
#include <unordered_set>
#include <queue>
#include "url.hpp"
using namespace std;
//max size of url (8)

class scheduler{
  public:
        scheduler();
        void addInbound(url a);
        void addOutbound(url a);
        void addCrawled(string &name);
        url getInbound();
        url getOutbound();
        bool checkCrawled(string &name);

  private:
        unordered_set<string> crawled;
        priority_queue<url, vector<url>> inbound;
        priority_queue<url, vector<url>> outbound;
};
