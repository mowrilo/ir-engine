#include <iostream>
#include <unordered_set>
#include <queue>
#include <vector>
#include <deque>
#include <ctime>
#include <unordered_map>
#include "url.hpp"
using namespace std;

#define MAXLEN  80
#define MAXDOM  10
//max size of url (8)

class scheduler{
  public:
        scheduler();
        bool addInbound(url a);
        bool addOutbound(url a);
        void addCrawledUrl(string &name);
        void addCrawledDomain(string &domain);
        bool checkEmpty();
        vector<int> genWeightsHash();
        url getUrl();
        int hashFunc(string &name);
        bool checkCrawled(string &name);

  private:
        unordered_map<int, double> crawledDomains;
        unordered_set<int> crawledPages; //HASH PARA DOMINIO, COM NUM CRAWLED
        vector<int> weights;
        priority_queue<url, deque<url>> inbound;
        priority_queue<url, deque<url>> outbound;
        int whichUrl;
        int ctrlInOut;
};
