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
  if (inbound.size() < MAXQUEUE){
    inbound.push(a);
    return true;
  }
  return false;
}

url scheduler::getUrl(){
  int timeSec = clock()/CLOCKS_PER_SEC;
//  if (timeSec)
  if ((whichUrl%3 == 0) || (whichUrl < 10)) {
    if (inbound.empty()){
      string no = "none";
      url notValid(no,no);
      return notValid;
    }
    url inb = inbound.top();
    string dom = inb.getDomain();
    int key = hashFunc(dom);
    unordered_map<int, double>::iterator it = crawledDomains.find(key);
    if ((it != crawledDomains.end()) && (timeSec < it->second + 30)){
      inb.increaseWeight();
      string no = "";
      url notValid(no,no);
      return notValid;
    }
    inbound.pop();
    whichUrl++;
    return inb;
  }
  else{
    if (outbound.empty()){
      string no = "";
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
      string no = "";
      url notValid(no,no);
      return notValid;
    }
    outbound.pop();
    whichUrl++;
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
