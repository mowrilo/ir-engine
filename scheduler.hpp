#include <iostream>
#include <unordered_set>
#include <queue>
#include "url.hpp"
using namespace std;

#define MAXLEN 80
//max size of url (8)

class scheduler{
  public:
        scheduler();
        bool addInbound(url a);
        bool addOutbound(url a);
        void addCrawled(string &name);
        array<int,MAXLEN> genWeightsHash();
        int hashFunc(string &name);
        url getInbound();
        url getOutbound();
        bool checkCrawled(string &name);

  private:
        unordered_set<int> crawled;
        array<int,MAXLEN> weights;
        priority_queue<url, vector<url>> inbound;
        priority_queue<url, vector<url>> outbound;
};
