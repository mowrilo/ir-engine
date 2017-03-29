#include <iostream>
#include <unordered_set>
#include "scheduler.hpp"
using namespace std;

scheduler::scheduler(){}

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
  url inb = inbound.top();
  inbound.pop();
  return inb;
}

url scheduler::getOutbound(){
  url outb = outbound.top();
  outbound.pop();
  return outb;
}

bool scheduler::checkCrawled(string &name){
  return crawled.count(name) > 0;
}
