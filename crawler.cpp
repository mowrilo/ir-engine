#include "crawler.hpp"

#define NTHREADS 100

//FAZER A FILA INTERNA

scheduler crawler::sc;
mutex crawler::mutexQueue;
mutex crawler::mutexCrawled;
mutex crawler::mutexNPages;
vector<string> crawler::domainTypes;
int crawler::nPages;
int crawler::chamada;

using namespace std;

crawler::crawler(string &path){
  pathToStore = path;
  this->nPages = 0;
  this->chamada = 0;
  ifstream file, file2;
  file.open("seeds");
  string seedUrl;
  while (file >> seedUrl){
    seeds.push_back(seedUrl);
  }
  domainTypes = {".com",".net",".gov",".org",".int",".edu",".mil",".blog",".info"};
}

int crawler::isBr(string &url){
  string br = ".br/";
  int find = url.find(br);
  if (find == string::npos) find=-1;
  // cout << "find: " << find << "\n";
  return find;//find(br, url);
}

void crawler::begin(){
  vector<thread> thrds;
  string none = "";
  int nSeeds = seeds.size();
  for (int i=0; i<nSeeds; i++){
    thrds.push_back(thread(crawl, seeds[i], i));
  }

  for (int i=nSeeds; i<NTHREADS; i++){
    thrds.push_back(thread(crawl, none, i));
  }

  for (int i=0; i<NTHREADS; i++){
    thrds[i].join();
  }
}

void crawler::crawl(string seedUrl, int id){
  CkSpider spider;
  //cout << "thread " << id << " iniciou!\n";
  fileManager fm(this_thread::get_id());
  //signal(SIGPIPE, SIG_IGN);
  queue<url> shortTermScheduler;
  // ofstream file;
  // string filename = "/htmls.txt";
  if(seedUrl.size() != 0){
    string seedDomain = getUrlDomain(seedUrl);
    url pato(seedUrl, seedDomain,0);
    mutexQueue.lock();
    sc.addOutbound(pato);
    mutexQueue.unlock();
  }

  while (true){
    for (int i=0;i<20;i++){
      mutexCrawled.lock();
      mutexQueue.lock();
      url bla = sc.getUrl();
      mutexQueue.unlock();
      mutexCrawled.unlock();
      shortTermScheduler.push(bla);
    }
    while (!shortTermScheduler.empty()){//for (int i=0;i<20;i++){
      url bla = shortTermScheduler.front();
      shortTermScheduler.pop();
      bool got = bla.checkValid();
      // cout << "oi\n";
      if (got){
        // cout << "oigot\n";
        string andre = bla.getName();
        string andDom = getUrlDomain(andre);

        mutexCrawled.lock();
        sc.addCrawledDomain(andDom);
        mutexCrawled.unlock();
        // if (id == chamada%NTHREADS){
        //   cout << "thread " << id << " presente!\n";
        //   chamada++;
        //   cout << "novaChamada: " << chamada << endl;
        // }
        spider.Initialize(andre.c_str());//asd.getString());
        spider.AddUnspidered(andre.c_str());
        if (spider.CrawlNext()){
          mutexNPages.lock();
          nPages++;
          cout << "nPages: " << nPages << endl;
          mutexNPages.unlock();
          //file << andre << "\n";
          cout << andre << "\n";// << " threadid: " << this_thread::get_id() << "\n";
          CkString html;
          spider.get_LastHtml(html);
          string htmlStr = html.getString();
          fm.writeHtml(andre,htmlStr);
          //file << html.getString();

          int nOut = spider.get_NumOutboundLinks();
          int nUnsp= spider.get_NumUnspidered();
          //cout << "nUnsp: " << nUnsp << " nOut: " << nOut << endl;
          for (int i=0; i<nOut; i++){
            CkString nxt;
            spider.GetOutboundLink(i, nxt);
            string nextUrl = nxt.getString();
            int nextSize = nextUrl.size();
            if ((nextSize < 100) && (nextSize > 10)){// && (isBr(nextUrl) > 0)){// && (isBr(nextUrl) > 0)
              if (nextUrl.back() != '/') nextUrl.push_back('/');
              string nxtDom = getUrlDomain(nextUrl);
              mutexCrawled.lock();
              if (!sc.checkCrawled(nextUrl)){
                url prox(nextUrl, nxtDom, i);
                sc.addCrawledUrl(nextUrl);
                mutexQueue.lock();
                sc.addOutbound(prox);
                mutexQueue.unlock();
              }
              mutexCrawled.unlock();
            }
          }
          spider.ClearOutboundLinks();

          for (int i=0; i<nUnsp; i++){
            CkString nxt;
            spider.GetUnspideredUrl(0, nxt);
            string nxtUrl = nxt.getString();
            int nextSize = nxtUrl.size();
            if ((nextSize < 100) && (nextSize > 10)){// && (isBr(nxtUrl) > 0)){
              if (nxtUrl.back() != '/') nxtUrl.push_back('/');
              string nxtDom = getUrlDomain(nxtUrl);
              mutexCrawled.lock();
              if (!sc.checkCrawled(nxtUrl)){
                url prox(nxtUrl, nxtDom, i*10);
                mutexQueue.lock();
                sc.addCrawledUrl(nxtUrl);
                sc.addOutbound(prox);
                mutexQueue.unlock();
              }
              mutexCrawled.unlock();
            }
            spider.SkipUnspidered(0);
          }
        }
      }
    }
}
  // file.close();
}

string crawler::getUrlDomain(string &url){
  int posFim=0, nbarras=0;
  while (nbarras < 3){
    if (url[posFim] == '/')  nbarras++;
    posFim++;
  }
  int posBr = isBr(url);
  int i=posFim-1;
  int j=posFim-1;
  if (posBr > 0){
    i = posBr;
    j = posBr;
  }
  i--;
  while ((url[i] != '.') && (i>0)) i--;
  // cout << url << " i: " << i << " j: " << j << endl;
  string subUrl = url.substr(i,(j-i));
  bool isdmn = isDomain(subUrl);
  if (isdmn){
    i--;
    while ((url[i] != '.') && (url[i] != '/')) i--;
  }
  i++;
  // cout << url << " i: " << i << " posFim: " << posFim << endl;
  return url.substr(i,(posFim-i-1));
}

bool crawler::isDomain(string &subUrl){
  for (int i=0;i<domainTypes.size();i++){
    if (subUrl.compare(domainTypes[i]) == 0)  return true;
  }
  return false;
}

string crawler::normalizeUrl(string &name){

}
