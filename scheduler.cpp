#include <iostream>
#include <unordered_set>
#include "scheduler.hpp"

#define MAXURL 1000
#define MAXCADA 700
#define PRIME 9999

using namespace std;

scheduler::scheduler(){
  weights = genWeightsHash();
}

bool scheduler::addInbound(url a){
  if (inbound.size() < MAXCADA){
    inbound.push(a);
    return true;
  }
  return false;
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
  if (outbound.size() < MAXCADA){
  outbound.push(a);
  return true;
  }
  return false;
}

void scheduler::addCrawled(string &name, string &domain){
  int key = hashFunc(name);
  int keyDomain = hashFunc(domain);
  unordered_map<int, unordered_set<int> >::iterator it = crawledDomains.find(keyDomain);
  it->second.insert(key);
  //crawledDomains.insert(key);
}

url scheduler::getInbound(){
  url inb = inbound.top();
  inbound.pop();
  return inb;
}

url scheduler::getOutbound(){
  url outb = outbound.top();
  outbound.pop();
  return outb;
}

bool scheduler::checkCrawled(string &name, string &domain){
  int key = hashFunc(name);
  int keyDomain = hashFunc(domain);
  unordered_map<int, unordered_set<int> >::iterator it = crawledDomains.find(keyDomain);
  if (it == crawledDomains.end())  return false;
  else{
    unordered_set<int>::iterator itSet = it->second.find(key);
    return crawledDomains.find(key) != crawledDomains.end();
  }
}
