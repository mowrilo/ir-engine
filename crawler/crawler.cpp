#include "crawler.hpp"

#define NTHREADS 100

scheduler crawler::sc; //Variáveis compartilhadas
mutex crawler::mutexQueue; //Exclusão mútua da fila de prioridades
mutex crawler::mutexCrawledDomains;//Exclusão mútua da hash dos domínios
mutex crawler::mutexCrawledPages;//Exclusão mútua da hash das páginas
vector<string> crawler::domainTypes;
vector<string> crawler::ignoreTypes;

using namespace std;

crawler::crawler(string &path){
  pathToStore = path;
  ifstream file;
  file.open("seeds"); //Lê as sementes e guarda em um vetor
  string seedUrl;
  while (file >> seedUrl){
    seeds.push_back(seedUrl);
  }
  domainTypes = {".com",".fr",".it",".hn",".ca"".de",".uk",".jp",".cn","",".au",".us",".ar",".net",".gov",".org",".int",".edu",".mil",".blog",".info"};
  ignoreTypes = {"{{",".tv",".mp3",".wma",".wav","stream","live","porn","sex","xxx","sexy","@"};
} //Define padrões de domínio e proibidos

int crawler::isBr(string &url){
  string br = ".br/"; //Checa se existe o padrão ".br" na URL
  int find = url.find(br);
  if (find == string::npos) find=-1;
  return find;
}

void crawler::begin(){
  vector<thread> thrds;
  string none = "";
  int nSeeds = seeds.size();
  for (int i=0; i<nSeeds; i++){ //Inicia as threads com URLs semente
    thrds.push_back(thread(crawl, seeds[i], i));
  }

  for (int i=nSeeds; i<NTHREADS; i++){ //Inicia as threads sem semente
    thrds.push_back(thread(crawl, none, i));
  }

  for (int i=0; i<NTHREADS; i++){
    thrds[i].join();
    cout << "joining thread " << i << "...\n";
  }
}

void crawler::crawl(string seedUrl, int id){
  CkSpider spider;
  int npgs = 0; //Contador local
  int limQueue = 1;
  fileManager fm(id);
  queue<url> shortTermScheduler; //Fila local
  if(seedUrl.size() != 0){  //Se a thread tiver recebido URL semente, adiciona na fila
    string seedDomain = getUrlDomain(seedUrl);
    url seed(seedUrl, seedDomain,0);
    mutexQueue.lock();
    sc.addToQueue(seed);
    mutexQueue.unlock();
  }

  while (true){ //Loop de coleção
    mutexCrawledDomains.lock();
    mutexQueue.lock();
    for (int i=0;i<limQueue;i++){ //Pega um bloco de URLs da fila
      url urlToCrawl = sc.getUrl();
      if (urlToCrawl.checkValid()){ //Só adiciona na fila local se for válida
        shortTermScheduler.push(urlToCrawl);
      }
    }
    mutexQueue.unlock();
    mutexCrawledDomains.unlock();
    while (!shortTermScheduler.empty()){
      url urlToCrawl = shortTermScheduler.front(); //Pega uma por uma na fila local
      shortTermScheduler.pop();
        string proxUrl = urlToCrawl.getName();
        string proxUrlDom = urlToCrawl.getDomain();
        string proxUrlNorm = normalizeUrl(proxUrl); //Normaliza para adicionar que foi visitada
        bool visited;
          mutexCrawledDomains.lock();
          mutexCrawledPages.lock();
          sc.addCrawledDomain(proxUrlDom); //Adiciona página e domínio nas tabelas hash
          sc.addCrawledUrl(proxUrlNorm);
          mutexCrawledPages.unlock();
          mutexCrawledDomains.unlock();
          spider.Initialize(proxUrl.c_str());
          spider.AddUnspidered(proxUrl.c_str()); //Adiciona a URL na fila do CkSpider
          if (spider.CrawlNext()){ //Caso a coleta tenha sido feita com sucesso
            npgs++;
            if((npgs%10 == 0) && (limQueue<50))  limQueue++; //Incrementa-se o limite da fila iterna
            cout << proxUrl << "\n";
            CkString html;
            spider.get_LastHtml(html);
            string htmlStr = html.getString();
            fm.writeHtml(proxUrl,htmlStr); //Dá a URL e o HTML para o fileManager escrever em disco

            int nOut = spider.get_NumOutboundLinks(); //Pega-se as filas do CkSpider
            int nUnsp= spider.get_NumUnspidered();
            for (int i=0; i<nOut; i++){
              CkString nxt;
              spider.GetOutboundLink(i, nxt); //Pega-se as Outbounds URLs da fila
              string nextUrl = nxt.getString();
              int nextSize = nextUrl.size();
              bool isbra = (isBr(nextUrl) > 0); //Checa se é ".br"
              bool isForbidden = mustIgnore(nextUrl); //Checa se contém padrão proibido
              if ((nextSize < 100) && (nextSize > 10) && (!isForbidden)){ //Caso o tamanho da URL esteja no limite e não seja proibida
                if (nextUrl.back() != '/') nextUrl.push_back('/'); //Adiciona uma barra ao final, caso nao tenha
                string nxtDom = getUrlDomain(nextUrl);
                if (nxtDom.size() > 0){ //Evita URLs inválidas
                  string nextUrlNorm = normalizeUrl(nextUrl); //Normaliza para checar na Hash
                  mutexCrawledPages.lock();
                  bool isCrawled = sc.checkCrawled(nextUrlNorm); //Checa na hash
                  mutexCrawledPages.unlock();
                  if (!isCrawled){ //Caso não tiver sido visitada, adiciona na fila
                    int wei = i*5;
                    if (!isbra)  wei+=99999999; //Se não for ".br", adiciona um peso muito grande para ir ao fundo da fila
                    url prox(nextUrl, nxtDom, wei);
                    mutexQueue.lock();
                    sc.addToQueue(prox); //Adiciona na fila
                    mutexQueue.unlock();
                  }
                }
              }
            }
            spider.ClearOutboundLinks(); //Limpa a fila do CkSpider

            for (int i=0; i<nUnsp; i++){ //Pega a unspidered list, de links internos
              CkString nxt;
              spider.GetUnspideredUrl(0, nxt);
              string nxtUrl = nxt.getString();
              int nextSize = nxtUrl.size();
              bool isForbidden = mustIgnore(nxtUrl); //Checa se é proibida
              if ((nextSize < 100) && (nextSize > 10) && (!isForbidden)){
                if (nxtUrl.back() != '/') nxtUrl.push_back('/'); //Adiciona a barra caso nao tenha
                string nxtDom = getUrlDomain(nxtUrl);
                if (nxtDom.size() > 0){
                  string nxtUrlNorm = normalizeUrl(nxtUrl); //Normaliza para checar na hash
                  mutexCrawledPages.lock();
                  bool isCrawled = sc.checkCrawled(nxtUrlNorm); //Checa na hash se já foi visitada
                  mutexCrawledPages.unlock();
                  if (!isCrawled){ // Se não foi visitada, adiciona na fila
                    url prox(nxtUrl, nxtDom, i*500); //A distância de cada inbound é de 500 na fila, para não se concentrarem
                    mutexQueue.lock();
                    sc.addToQueue(prox); //Adiciona na fila
                    mutexQueue.unlock();
                  }
                }
              }
              spider.SkipUnspidered(0); //Retira da fila do CkSpider
            }
          }
    }
}
}

string crawler::getUrlDomain(string &url){
  int posFim=0, nbarras=0;
  while (nbarras < 3){ //Vai até a posição da terceira barra, considerando que as duas primeiras estão em "http://"
    if (url[posFim] == '/')  nbarras++;
    posFim++;
  }
  int posBr = isBr(url);//Se for .br, a posição de referência passa a ser o .br
  int i=posFim-1;
  int j=posFim-1;
  if (posBr > 0){
    i = posBr;
    j = posBr;
  }
  i--;
  while ((url[i] != '.') && (i>0)) i--; //Anda-se para trás, até o próximo ponto '.'
  if (i > url.length()) return ""; //Retorna URL vazia para evitar exceções de limites da string
  string subUrl = url.substr(i,(j-i)); //Pega a próxima parte do domínio
  bool isdmn = isDomain(subUrl);        //e checa se faz parte dos padrões de domínio
  if (isdmn){
    i--;
    while ((url[i] != '.') && (url[i] != '/')) i--; //Vai até o próximo ponto ou barra
  }
  i++;
  if (i > url.length()) return "";//Retorna URL vazia para evitar exceções de limites da string
  return url.substr(i,(posFim-i-1)); //Retorna o domínio
}

bool crawler::isDomain(string &subUrl){
  for (int i=0;i<domainTypes.size();i++){ //Checa se a string está nos padrões de domínio
    if (subUrl.compare(domainTypes[i]) == 0)  return true;
  }
  return false;
}

bool crawler::mustIgnore(string url){
  for (int i=0; i<ignoreTypes.size();i++){ //Checa se a string está nos padrões proibidos
    int find = url.find(ignoreTypes[i]);
    if (find != string::npos) return true;
  }
  return false;
}

string crawler::normalizeUrl(string name){
  for (int i=0;i<name.size();i++)  tolower(name[i]); //Deixa tudo em letras minúsculas
  string www = "www."; //Retira "www.", caso tenha
  int find = name.find(www);
  if (find != string::npos) name.erase(find,4);
  return name;
}
