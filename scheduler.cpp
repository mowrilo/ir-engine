#include <iostream>
#include <unordered_set>
#include "url.hpp"
#include "scheduler.hpp"
using namespace std;

void scheduler::addInbound(url a){
  inbound.push(a);
}

void scheduler::addOutbound(url a){
  outbound.push(a);
}

void scheduler::addCrawled(string &name){
  crawled.insert(name);
}

url scheduler::getInbound(){
  return inbound.top();
}

url scheduler::getOutbound(){
  return outbound.top();
}

bool scheduler::checkCrawled(string &name){
  return crawled.count(name) > 0;
}
