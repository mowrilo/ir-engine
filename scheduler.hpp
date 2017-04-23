#include <iostream>
#include <unordered_set>
#include <queue>
#include <vector>
#include <deque>
#include <ctime>
#include <unordered_map>
#include "url.hpp"
using namespace std;

#define MAXLEN  100 //Tamanho máximo considerado de URLs

class scheduler{
  public:
        scheduler(); //Construtor
        bool addToQueue(url a); //Adiciona URL na fila
        void addCrawledUrl(string &name); //Adiciona o nome da URL na Hash de páginas
        void addCrawledDomain(string &domain); //Adiciona o nome da URL na Hash de domínios
        vector<int> genWeightsHash(); //Gera os pesos do hashing polinomial
        url getUrl(); //Retorna uma URL da fila
        int hashFunc(string &name); //Função de transformação do hashing
        bool checkCrawled(string &name); //Checa se a página já foi visitada

  private:
        unordered_map<int, clock_t> crawledDomains; //Hash dos domínios e tempos de acesso
        unordered_set<int> crawledPages; //Hash de páginas
        vector<int> weights; //Vetor de pesos aleatórios
        priority_queue<url, deque<url>> priQueue; //Fila de prioridades
};
