#include <iostream>
#include <unordered_set>
#include "scheduler.hpp"

#define MAXQUEUE 1000000

using namespace std;

scheduler::scheduler(){
  weights = genWeightsHash(); //Gera os pesos
}

url scheduler::getUrl(){
  clock_t timeSec = clock()/CLOCKS_PER_SEC; //Pega o tempo atual
    if (priQueue.empty()){ //Caso esteja vazia, retorna URL inválida
      string no = "";
      url notValid(no,no,0);
      return notValid;
    }
    url outb = priQueue.top();
    string dom = outb.getDomain();
    int key = hashFunc(dom);
    unordered_map<int, clock_t>::iterator it = crawledDomains.find(key);
    if ((it != crawledDomains.end()) && (timeSec < (it->second + 30))){ //Caso a URL exista e tenha sido acessada nos últimos 30 seg
        outb.increaseWeight(1001); //Aumenta-se o peso para afundar na fila
        priQueue.pop();
        priQueue.push(outb); //Insere-se de novo
        string no = "";
        url notValid(no,no,0); //Retorna URL inválida
        return notValid;
    }
    priQueue.pop();
    if (it != crawledDomains.end()) it->second = timeSec; //Atualiza o tempo de acesso para evitar pegar duas seguidas de mesmo dominio
    return outb;
}
vector<int> scheduler::genWeightsHash(){
  vector<int> wei;
  for (int i=0; i<MAXLEN; i++) wei.push_back(rand()%MAXLEN + 1); //Gera pesos aleatórios
  return wei;
}

int scheduler::hashFunc(string &name){// Função de transformação
  int n=0;
  int i=0;
  for (char c: name){
    n+=c*weights[i];
    i++;
  }
  return n;
}

bool scheduler::addToQueue(url a){
  if (priQueue.size() < MAXQUEUE){ //Caso a fila não esteja cheia, adiciona a URL
    priQueue.push(a);
    return true;
  }
  return false;
}

void scheduler::addCrawledDomain(string &domain){
  int keyDomain = hashFunc(domain);
  clock_t timeSec = clock()/CLOCKS_PER_SEC; //Calcula o tempo atual
  unordered_map<int, clock_t>::iterator it;
  it = crawledDomains.find(keyDomain);
  if (it != crawledDomains.end()) { //Caso já tenha acessado o dominio, atualiza o tempo de acesso
      it->second = timeSec;
  }
    else{
      crawledDomains.insert({keyDomain, timeSec}); //Caso contrário, adiciona na hash
    }
}

void scheduler::addCrawledUrl(string &name){
  int key = hashFunc(name);
  unordered_set<int>::iterator itPage = crawledPages.find(key);
  if (itPage == crawledPages.end()){//se a pagina nao existir na hash, adiciona
    crawledPages.insert(key);
  }
}

bool scheduler::checkCrawled(string &name){
  int key = hashFunc(name);
  unordered_set<int>::iterator it = crawledPages.find(key); //Apenas checa se a hash possui a chave
  if (it != crawledPages.end()) return true;
  return false;
}
