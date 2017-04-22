#include "crawler.hpp"

#define NTHREADS 80

//FAZER A FILA INTERNA

scheduler crawler::sc;
mutex crawler::mutexQueue;
mutex crawler::mutexCrawledDomains;
mutex crawler::mutexCrawledPages;
mutex crawler::mutexNPages;
vector<string> crawler::domainTypes;
vector<string> crawler::ignoreTypes;
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
  domainTypes = {".com",".fr",".it",".hn",".ca"".de",".uk",".jp",".cn","",".au",".us",".ar",".net",".gov",".org",".int",".edu",".mil",".blog",".info"};
  ignoreTypes = {"quora","{{",".tv",".mp3",".wma",".wav","stream","live","porn","sex","xxx","sexy","@"};
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
  clock_t tempo1 = clock()/CLOCKS_PER_SEC;
  //cout << "process terminated after " << tempo << "seconds\n";
  int nSeeds = seeds.size();
  for (int i=0; i<nSeeds; i++){
    thrds.push_back(thread(crawl, seeds[i], i));
  }

  for (int i=nSeeds; i<NTHREADS; i++){
    thrds.push_back(thread(crawl, none, i));
  }

  for (int i=0; i<NTHREADS; i++){
    thrds[i].join();
    cout << "joining thread " << i << "...\n";
  }
  clock_t tempo2 = clock()/CLOCKS_PER_SEC;
  cout << "process terminated after " << (tempo2-tempo1) << "seconds\n";
}

void crawler::crawl(string seedUrl, int id){
  CkSpider spider;
  int npgs = 0;
  int limQueue = 1;
  // priority_queue<url, deque<url>> inboundQueue;
  // priority_queue<url, deque<url>> outboundQeue;
  //cout << "thread " << id << " iniciou!\n";
  fileManager fm(id);
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
    // if (id<5) cout << "addseed\n";
  }

  while (true){
    mutexCrawledDomains.lock();
    mutexQueue.lock();
    for (int i=0;i<limQueue;i++){
      url bla = sc.getUrl();
      if (bla.checkValid()){
        shortTermScheduler.push(bla);
        //if (id<5) cout << bla.getName() << " entroutrue\n";
      }
    }
    mutexQueue.unlock();
    mutexCrawledDomains.unlock();
   // if (chamada == id){
    //   cout << "thread presente " << id << endl;
    //   chamada++;
    // }
    // cout << "o igot\n";
    while (!shortTermScheduler.empty()){//for (int i=0;i<20;i++){
      // cout << "got urls\n";
      url bla = shortTermScheduler.front();
      shortTermScheduler.pop();
      // bool got = bla.checkValid();
      // cout << "oi\n";
      // if (got){
        // cout << "not empty\n";
        string andre = bla.getName();
        string andDom = bla.getDomain();
        string andreNorm = normalizeUrl(andre);
        bool visited;
        mutexCrawledPages.lock();
        visited = sc.checkCrawled(andreNorm);
        mutexCrawledPages.unlock();
        if (!visited){
          // if (id == chamada%NTHREADS){
          mutexCrawledDomains.lock();
          mutexCrawledPages.lock();
          sc.addCrawledDomain(andDom);
          sc.addCrawledUrl(andreNorm);
          mutexCrawledPages.unlock();
          mutexCrawledDomains.unlock();
          //   cout << "thread " << id << " presente!\n";
          //   chamada++;
          //   cout << "novaChamada: " << chamada << endl;
          // }
          spider.Initialize(andre.c_str());//asd.getString());
          spider.AddUnspidered(andre.c_str());
          if (spider.CrawlNext()){
            // if (nPages%100 == 0){
              // mutexNPages.lock();
              // cout << "nPages: " << nPages << endl;
            // }
            // mutexNPages.lock();
            //nPages++;
            // mutexNPages.unlock();
            npgs++;
            if((npgs%10 == 0) && (limQueue<50))  limQueue++;
            // mutexNPages.unlock();
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
              bool isbra = (isBr(nextUrl) > 0);
              bool isSecure = mustIgnore(nextUrl);
              if ((nextSize < 100) && (nextSize > 10) && (!isSecure)){//} && (isbra)){// && (isBr(nextUrl) > 0)
                if (nextUrl.back() != '/') nextUrl.push_back('/');
                string nxtDom = getUrlDomain(nextUrl);
                if (nxtDom.size() > 0){
                  string nextUrlNorm = normalizeUrl(nextUrl);
                  mutexCrawledPages.lock();
                  bool isCrawled = sc.checkCrawled(nextUrlNorm);
                  // if (!isCrawled) sc.addCrawledUrl(nextUrlNorm);
                  mutexCrawledPages.unlock();
                  if (!isCrawled){
                    int wei = i*5;
                    if (!isbra)  wei+=99999999;
                    url prox(nextUrl, nxtDom, wei);
                    //sc.addCrawledUrl(nextUrl);
                    mutexQueue.lock();
                    sc.addOutbound(prox);
                    mutexQueue.unlock();
                  }
                }
              }
            }
            spider.ClearOutboundLinks();

            for (int i=0; i<nUnsp; i++){
              CkString nxt;
              spider.GetUnspideredUrl(0, nxt);
              string nxtUrl = nxt.getString();
              int nextSize = nxtUrl.size();
              bool isSecure = mustIgnore(nxtUrl);
              if ((nextSize < 100) && (nextSize > 10) && (!isSecure)){// && (isBr(nxtUrl) > 0)){
                if (nxtUrl.back() != '/') nxtUrl.push_back('/');
                string nxtDom = getUrlDomain(nxtUrl);
                if (nxtDom.size() > 0){
                  string nxtUrlNorm = normalizeUrl(nxtUrl);
                  mutexCrawledPages.lock();
                  bool isCrawled = sc.checkCrawled(nxtUrlNorm);
                  // if (!isCrawled) sc.addCrawledUrl(nxtUrlNorm);
                  mutexCrawledPages.unlock();
                  if (!isCrawled){
                    url prox(nxtUrl, nxtDom, i*100);
                    // mutexCrawledPages.lock();
                    // sc.addCrawledUrl(nxtUrl);
                    // mutexCrawledPages.unlock();
                    // cout << "adding " << nxtUrl << endl;
                    mutexQueue.lock();
                    sc.addOutbound(prox);
                    mutexQueue.unlock();
                  }
                }
              }
              spider.SkipUnspidered(0);
            }
          }
        // }
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
  if (i > url.length()) return "";
  string subUrl = url.substr(i,(j-i));
  bool isdmn = isDomain(subUrl);
  if (isdmn){
    i--;
    while ((url[i] != '.') && (url[i] != '/')) i--;
  }
  i++;
  // cout << url << " i: " << i << " posFim: " << posFim << endl;
  if (i > url.length()) return "";
  return url.substr(i,(posFim-i-1));
}

bool crawler::isDomain(string &subUrl){
  for (int i=0;i<domainTypes.size();i++){
    if (subUrl.compare(domainTypes[i]) == 0)  return true;
  }
  return false;
}

bool crawler::mustIgnore(string url){
  for (int i=0; i<ignoreTypes.size();i++){
    int find = url.find(ignoreTypes[i]);
    if (find != string::npos) return true;
  }
  return false;
}

string crawler::normalizeUrl(string name){
  //if (name.back() != '/') name.push_back('/');
  for (int i=0;i<name.size();i++)  tolower(name[i]);
  string www = "www.";
  int find = name.find(www);
  if (find != string::npos) name.erase(find,4);
  return name;
}
