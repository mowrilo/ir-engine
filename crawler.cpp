#include "crawler.hpp"

#define NTHREADS 5//80

scheduler crawler::sc;
mutex crawler::mutexQueue;
mutex crawler::mutexCrawled;
mutex crawler::mutexNPages;
vector<string> crawler::domainTypes;
int crawler::nPages;

using namespace std;

crawler::crawler(string &path){
  pathToStore = path;
  this->nPages = 0;
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
  return find;//find(br, url);
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
    string seedDomain = getUrlDomain(seedUrl);
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
    // cout << "thread got " << bla.getName() << "\n";
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

        for (int i=0; i<nOut; i++){
          CkString nxt;
          spider.GetOutboundLink(i, nxt);
          string nextUrl = nxt.getString();
          // cout << "aqui ele roda\n";
          // string nxtUrl = spider.canonicalizeUrl(nextUrl.c_str());
          if (nextUrl.back() != '/') nextUrl.push_back('/');
          string nxtDom = getUrlDomain(nextUrl);
          mutexCrawled.lock();
          bool isCrawled = sc.checkCrawled(nextUrl, nxtDom);
          mutexCrawled.unlock();
          if (!isCrawled){
            url prox(nextUrl, nxtDom);
            // cout << "Adding outbound " << nxtUrl << " of domain " << nxtDom << " on Outbound URLs\n";
            // cout << "aqui ele roda tambem!\n";
            mutexQueue.lock();
            sc.addOutbound(prox);
            // cout << "aqui?\n";
            mutexQueue.unlock();
          }
        }
        spider.ClearOutboundLinks();

        for (int i=0; i<nUnsp; i++){
          CkString nxt;
          spider.GetUnspideredUrl(0, nxt);
          string nxtUrl = nxt.getString();
          // string nxtUrl = spider.canonicalizeUrl(nextUrl.c_str());
          // cout << "segfault! " << nxtUrl <<" \n";
          if (nxtUrl.back() != '/') nxtUrl.push_back('/');
          string nxtDom = getUrlDomain(nxtUrl);
          // cout << "segfault!\n";
          mutexCrawled.lock();
          bool isCrawled = sc.checkCrawled(nxtUrl, nxtDom);
          mutexCrawled.unlock();
          if (!isCrawled){
            url prox(nxtUrl, nxtDom);
            // cout << "Adding inbound " << nxtUrl << " of domain " << nxtDom << " on Inbound URLs\n";
            mutexQueue.lock();
            sc.addInbound(prox);
            mutexQueue.unlock();
          }
          spider.SkipUnspidered(0);
        }

      }
      //cout << "oi beleza?\n";
      //spider.SleepMs(1000);
    }
  }
  file.close();
}

string crawler::getUrlDomain(string &url){
  // find third bar
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
  // cout << "posfim " << posFim << endl;
  // cout << "i e j: " << i << " " << j << endl;
  while ((url[i] != '.') && (i>0)) i--;
  // cout << "entrou1! \n";
  string subUrl = url.substr(i,(j-i));
  // cout << "entrou2! \n";
  bool isdmn = isDomain(subUrl);
  //cout << "suburl " << subUrl << "  " << isdmn << endl;
  if (isdmn){
    i--;
    while ((url[i] != '.') && (url[i] != '/')) i--;
  }
  i++;
  //cout << i << "  " << posFim << "\n";
  // cout << url.substr(i,(posFim-i-1)) << " " << posFim << " " << j << endl;
  return url.substr(i,(posFim-i-1));
}

bool crawler::isDomain(string &subUrl){
  for (int i=0;i<domainTypes.size();i++){
    //cout << "comparing " << subUrl << " with " << domainTypes[i] << endl;
    if (subUrl.compare(domainTypes[i]) == 0)  return true;
  }
  return false;
}

string crawler::normalizeUrl(string &name){

}


//
// string normalizeUrl(string &name){
//
// }
