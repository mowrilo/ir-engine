#include <iostream>
#include <unordered_set>
#include "scheduler.hpp"

#define MAXURL 1000
#define MAXQUEUE 3000

using namespace std;

scheduler::scheduler(){
  weights = genWeightsHash();
  whichUrl = 0;
  ctrlInOut = 0;
}

bool scheduler::addInbound(url a){
  if (inbound.size() < MAXQUEUE){
    inbound.push(a);
    return true;
  }
  return false;
}

url scheduler::getUrl(){
  int timeSec = clock()/CLOCKS_PER_SEC;
  // ctrlInOut = 1;
  if (ctrlInOut%3 == 0){// || (whichUrl < 10))
    if (inbound.empty()){
      string no = "";
      // cout << "Returning not valid because of empty heap\n";
      url notValid(no,no,0);
      ctrlInOut++;
      return notValid;
    }
    url inb = inbound.top();
    string dom = inb.getDomain();
    // cout << "trying inbound domain " << dom << endl;
    int key = hashFunc(dom);
    unordered_map<int, double>::iterator it = crawledDomains.find(key);
    if ((it != crawledDomains.end()) && (timeSec < it->second + 30)){
      inb.increaseWeight();
      string no = "";
      // cout << "Returning not valid because of time\n";
      url notValid(no,no,0);
      ctrlInOut++;
      return notValid;
    }
    inbound.pop();
    whichUrl++;
    ctrlInOut = rand()%3;
    return inb;
  }
  else{
    if (outbound.empty()){
      string no = "";
      // cout << "Returning not valid because of empty heap\n";
      url notValid(no,no,0);
      ctrlInOut++;
      return notValid;
    }
    url outb = outbound.top();
    string dom = outb.getDomain();
    // cout << "trying outbound domain " << dom << endl;
    int key = hashFunc(dom);
    unordered_map<int, double>::iterator it = crawledDomains.find(key);
    if ((it != crawledDomains.end()) && (timeSec < (it->second + 30))){
        outb.increaseWeight();
        outbound.pop();
        outbound.push(outb);
        string no = "";
        // cout << "Returning not valid because of time\n";
        url notValid(no,no,0);
        ctrlInOut++;
        return notValid;
    }
    outbound.pop();
    whichUrl++;
    ctrlInOut = rand()%3;
    return outb;
  }
}
vector<int> scheduler::genWeightsHash(){
  vector<int> wei;
  for (int i=0; i<MAXLEN; i++) wei.push_back(rand()%80 + 1);
  return wei;
}

int scheduler::hashFunc(string &name){
  int n=0;
  int i=0;
  for (char c: name){
    n+=c*weights[i];
    i++;
  }
  return n;
}

bool scheduler::addOutbound(url a){
  if (outbound.size() < MAXQUEUE){
    outbound.push(a);
    return true;
  }
  return false;
}

void scheduler::addCrawledDomain(string &domain){
  int keyDomain = hashFunc(domain);
  // cout << "dominio: " << domain << " chave: " << keyDomain << endl;
  int timeSec = clock()/CLOCKS_PER_SEC;
  unordered_map<int, double>::iterator it;
  it = crawledDomains.find(keyDomain);
  if (it != crawledDomains.end()) { //se o dominio existir na hash
      it->second = timeSec;
  }
    else{
      crawledDomains.insert({keyDomain, timeSec});
    }
}

void scheduler::addCrawledUrl(string &name){
  int key = hashFunc(name);
  unordered_set<int>::iterator itPage = crawledPages.find(key);
  if (itPage == crawledPages.end()){//se a pagina nao existir na hash
    crawledPages.insert(key);
  }
}

bool scheduler::checkEmpty(){
  return outbound.empty();
}


bool scheduler::checkCrawled(string &name){
  int key = hashFunc(name);
  return crawledPages.find(key) != crawledPages.end();
}
