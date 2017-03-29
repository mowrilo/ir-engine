#include <iostream>
#include <unordered_set>
#include "url.hpp"
using namespace std;

//max size of url (8)

class scheduler{
  private:
        unordered_set<string> crawled;
        auto compare = [](url a, url b) { return (a.getSize()) < (b.getSize());};
        priority_queue<url, vector<url>, decltype(compare)> inbound(compare);
        priority_queue<url, vector<url>, decltype(compare)> outbound(compare);

  public:
        void addInbound(url);
        void addOutbound(url);
        void addCrawled(string);
        url getInbound();
        url getOutbound();
        bool checkCrawled(string);
}
