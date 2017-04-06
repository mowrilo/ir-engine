#include "crawler.hpp"

#define NTHREADS 80

scheduler crawler::sc;
mutex crawler::mutexQueue;
mutex crawler::mutexCrawled;
mutex crawler::mutexNPages;
int crawler::nPages;

using namespace std;

crawler::crawler(string &path){
  pathToStore = path;
  this->nPages = 0;
  ifstream file;
  file.open("seeds");
  string seedUrl;
  while (file >> seedUrl){
    seeds.push_back(seedUrl);
  }
}

bool crawler::isBr(string &domain){
  int siz = domain.size();
  string end = domain.substr(siz-2);
  return end.compare("br");
}

void crawler::begin(){
  vector<thread> thrds;
  string none = "";
  int nSeeds = seeds.size();
  for (int i=0; i<nSeeds; i++){
    thrds.push_back(thread(crawl, seeds[i]));
  }

  for (int i=nSeeds; i<NTHREADS; i++){
    thrds.push_back(thread(crawl, none));
  }

  for (int i=0; i<NTHREADS; i++){
    thrds[i].join();
  }
}

void crawler::crawl(string seedUrl){
  CkSpider spider;
  queue<url> shortTermScheduler;
  //scheduler sc;
  ofstream file;
  //cout << "hello from thread " << this_thread::get_id() << endl;
  //string urlInit = "http://www.globo.com";
  string filename = "/htmls.txt";
	//file.open(pathToStore+filename);
  if(seedUrl.size() != 0){
    //cout << "OIOIOI\n";
    string seedDomain = spider.getBaseDomain(seedUrl.c_str());
    url pato(seedUrl, seedDomain);
    mutexQueue.lock();
    //cout << "mutexQueue locked by some thread\n";
    sc.addInbound(pato);
    //cout << "mutexQueue to be unlocked by some thread\n";
    mutexQueue.unlock();
  }

  // bool a = true;
  // while (a){
  //   mutexQueue.lock();
  //   a = sc.checkEmpty();
  //   mutexQueue.unlock();
  // }

  while (true){
    mutexQueue.lock();
    url bla = sc.getUrl();
    mutexQueue.unlock();
    //cout << "name got: " << bla.getName() << " valid: " << bla.checkValid() << endl;
    bool got = bla.checkValid();
    //cout << "got " << got << "\n";
    if (got){
      string a = bla.getName();
      //cout << "before canonicalize: " << a << "\n";
      CkString asd = spider.canonicalizeUrl(a.c_str());
      //cout << "after canonicalize: " << asd.getString() << "\n";
      spider.Initialize(asd.getString());
      spider.AddUnspidered(a.c_str());
      if (spider.CrawlNext()){
        mutexNPages.lock();
        nPages++;
        cout << "nPages: " << nPages << endl;
        mutexNPages.unlock();
      //cout << "\noi beleza? o teste passou flw\n";
        string andre = spider.lastUrl();
        file << andre << "\n";
        string andDom = spider.getBaseDomain(andre.c_str());
        cout << andre << "\n";
        //cout << "\nPASSOU\n";
        mutexCrawled.lock();
        sc.addCrawled(andre, andDom);
        //cout << "\nPASSOU\n";
        mutexCrawled.unlock();
        //cout << "DEU BOM " << andre << "\n";

        CkString html;
        spider.get_LastHtml(html);

        //TRATA HTML

        file << html.getString();

        int nOut = spider.get_NumOutboundLinks();
        int nUnsp= spider.get_NumUnspidered();
        cout << "nUnsp: " << nUnsp << " nOut: " << nOut << endl;

        for (int i=0; i<nUnsp; i++){
          CkString nxt;
          spider.GetUnspideredUrl(0, nxt);
          string nextUrl = nxt.getString();
          string nxtUrl = spider.canonicalizeUrl(nextUrl.c_str());
          string nxtDom = spider.getBaseDomain(nxtUrl.c_str());
          mutexCrawled.lock();
          bool isCrawled = sc.checkCrawled(nxtUrl, nxtDom);
          mutexCrawled.unlock();
          if (!isCrawled){
            url prox(nxtUrl, nxtDom);
            cout << "Adding " << nxtUrl << " of domain " << nxtDom << " on Inbound URLs\n";
            mutexQueue.lock();
            sc.addInbound(prox);
            mutexQueue.unlock();
          }
          spider.SkipUnspidered(0);
        }

        for (int i=0; i<nOut; i++){
          CkString nxt;
          spider.GetOutboundLink(i, nxt);
          string nextUrl = nxt.getString();
          string nxtUrl = spider.canonicalizeUrl(nextUrl.c_str());
          string nxtDom = spider.getBaseDomain(nxtUrl.c_str());
          mutexCrawled.lock();
          bool isCrawled = sc.checkCrawled(nxtUrl, nxtDom);
          mutexCrawled.unlock();
          if (!isCrawled){
            url prox(nxtUrl, nxtDom);
            cout << "Adding " << nxtUrl << " of domain " << nxtDom << " on Outbound URLs\n";
            mutexQueue.lock();
            sc.addOutbound(prox);
            mutexQueue.unlock();
          }
        }
        spider.ClearOutboundLinks();
      }
      //cout << "oi beleza?\n";
      //spider.SleepMs(1000);
    }
  }
  file.close();
}

string crawler::getUrlDomain(string &url){

}

string crawler::normalizeUrl(string &name){

}


//
// string normalizeUrl(string &name){
//
// }
