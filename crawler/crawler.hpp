#include <iostream>
#include "scheduler.hpp"
#include "filemanager.hpp"
#include <CkSpider.h>
#include <CkString.h>
#include <cstring>
#include <fstream>
#include <thread>
#include <queue>
#include <mutex>
#include <string>
using namespace std;

class crawler{
  public:
        crawler(string &path); //Construtor
        void begin(); //Começa o processo de crawling
        static void crawl(string seedUrl, int id); //Rotina principal seguida por cada thread
        static string getUrlDomain(string &url); //Recebe a string da URL e retorna seu domínio
        static string normalizeUrl(string name); //Retira "www." e deixa as letras minúsculas
        static int isBr(string &domain); //Checa se a URL é ".br"
        static bool isDomain(string &subUrl); //Checa se a URL possui os padrões de domínio
        static bool mustIgnore(string url); //Checa se a URL possui os padrões proibidos

  private:
        string pathToStore; //Pasta para guardar informações das páginas
        static scheduler sc; //Escalonador
        static mutex mutexQueue;            //Variáveis de exclusão mútua para as variáveis
        static mutex mutexCrawledDomains;   //compartilhadas: fila e tabelas hash
        static mutex mutexCrawledPages;
        vector<string> seeds; //Vetor de URLs semente
        static vector<string> domainTypes; //Padrões de domínio
        static vector<string> ignoreTypes; //Padrões proibidos
};
