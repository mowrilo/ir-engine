#include <iostream>
#include <unordered_set>
#include "scheduler.hpp"

#define MAXURL 1000
#define MAXQUEUE 3000

using namespace std;

scheduler::scheduler(){
  weights = genWeightsHash();
  whichUrl = 0;
}

bool scheduler::addInbound(url a){
  //cout << "Inserting url " << a.getName() << " on heap of size " << inbound.size() << "...\n";
  if (inbound.size() < MAXQUEUE){
    inbound.push(a);
    //cout << "Success.\n";
    return true;
  }
  //cout << "Failed.\n";
  return false;
}

url scheduler::getUrl(){
  int timeSec = clock()/CLOCKS_PER_SEC;
//  if (timeSec)
  if (whichUrl%3 == 0) {// || (whichUrl < 10)
    // cout << "Trying to get inbound URL on heap of size"<< inbound.size() << "...\n";
    // cout << "Inbound it is.\n";
    if (inbound.empty()){
      // cout << "Returning not valid because of empty heap" << rand()%10 << ".\n";
      string no = "";
      url notValid(no,no);
      return notValid;
    }
    url inb = inbound.top();
    string dom = inb.getDomain();
    int key = hashFunc(dom);
    //cout << "trying domain: " << dom << " on heap of size " << inbound.size() << endl;
    unordered_map<int, double>::iterator it = crawledDomains.find(key);
    if ((it != crawledDomains.end()) && (timeSec < it->second + 30)){
      inb.increaseWeight();
      // cout << "Returning not valid because of time.\n";
      string no = "";
      url notValid(no,no);
      return notValid;
    }
    inbound.pop();
    whichUrl++;
    // cout << "Inbound it is.\n";
    // cout << "Returning valid URL: " << inb.getName() << "\n";
    return inb;
  }
  else{
    // cout << "Trying to get URL on heap of size"<< outbound.size() << "...\n";
    // cout << "Outbound it is.\n";
    if (outbound.empty()){
      string no = "";
      //cout << "Returning not valid because of empty heap" << rand()%10 << ".\n";
      url notValid(no,no);
      return notValid;
    }
    url outb = outbound.top();
    outbound.pop();
    string dom = outb.getDomain();
    int key = hashFunc(dom);
    unordered_map<int, double>::iterator it = crawledDomains.find(key);
    if ((it != crawledDomains.end()) && (timeSec < (it->second + 30))){
      outb.increaseWeight();
      //cout << "Returning not valid because of time.\n";
      string no = "";
      url notValid(no,no);
      return notValid;
    }
    outbound.pop();
    whichUrl++;
    // cout << "Outbound it is.\n";
    // cout << "Returning valid URL: " << outb.getName() << "\n";
    return outb;
  }
  //return true;
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

void scheduler::addCrawled(string &name, string &domain){
  int key = hashFunc(name);
  int keyDomain = hashFunc(domain);
  int timeSec = clock()/CLOCKS_PER_SEC;
  //cout << "\nHello from scheduler\n";
  unordered_map<int, double>::iterator it;
  if (!crawledDomains.empty()){
    it = crawledDomains.find(keyDomain);
    //cout << "Getting numCrawled";
    if (it != crawledDomains.end()) {
      unordered_set<int>::iterator itPage = crawledPages.find(key);
      if (itPage == crawledPages.end()){
        it->second = timeSec;
        crawledPages.insert(key);
      }
    }
    else{
      crawledDomains.insert({keyDomain, timeSec});
      crawledPages.insert(key);
    }
  }
  else{
    crawledDomains.insert({keyDomain,timeSec});
    crawledPages.insert(key);
  }
  //cout << "\nGoodbye from scheduler\n";
}

bool scheduler::checkEmpty(){
  return outbound.empty();
}

// url scheduler::getInbound(){
//
// }
//
// url scheduler::getOutbound(){
//
// }

bool scheduler::checkCrawled(string &name, string &domain){
  int key = hashFunc(name);
  int keyDomain = hashFunc(domain);
  unordered_map<int, double>::iterator it = crawledDomains.find(keyDomain);
  if (it == crawledDomains.end())  return false;
  else{
    return crawledPages.find(key) != crawledPages.end();
  }
}
