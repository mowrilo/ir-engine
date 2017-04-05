#include "crawler.hpp"
scheduler crawler::sc;
mutex crawler::mutexQueue;
mutex crawler::mutexCrawled;

using namespace std;

crawler::crawler(string &path){
  pathToStore = path;
}

void crawler::begin(){
  // for (int i=0; i<5; i++){
  //     if (i==0) thread t(crawl,"http://www.globo.com");
  //     else if (i==1) thread t(crawl,"http://www.uol.com.br");
  //     else if (i==2) thread t(crawl,"http://www.uai.com.br");
  //     else if (i==3) thread t(crawl,"http://www.ufmg.br");
  //     else if (i==4) thread t(crawl,"http://www.pbh.gov.br");
  // }
  string seed1 = "http://brasilecrawler::scola.uol.com.br/biologia/";
  thread t(crawl,seed1);
  t.join();
}

void crawler::crawl(string seedUrl){
  CkSpider spider;
  cout << "oi beleza?\n";
  //crawler::scheduler crawler::sc;
  ofstream file;
  //string urlInit = "http://www.globo.com";
  string filename = "/htmls.txt";
	//file.open(pathToStore+filename);
  url pato(seedUrl);
  crawler::mutexQueue.lock();
  crawler::sc.addOutbound(pato);
  crawler::mutexQueue.unlock();
  while (true){
    crawler::mutexQueue.lock();
    url bla = crawler::sc.getOutbound();
    crawler::mutexQueue.unlock();
    string a = bla.getName();
    CkString asd = spider.canonicalizeUrl(a.c_str());
    spider.Initialize(asd.getString());
    spider.AddUnspidered(a.c_str());
    cout << a;
    if (spider.CrawlNext()){
      string andre = spider.lastUrl();
      string andDom = spider.getBaseDomain(andre.c_str());
      cout << andre << "   " << andDom;
      cout << "\nPASSOU\n";
      crawler::mutexCrawled.lock();
      crawler::sc.addCrawled(andre, andDom);
      cout << "\nPASSOU\n";
      if (andre.back() == '/'){
        andre.pop_back();
        crawler::sc.addCrawled(andre, andDom);
      }
      crawler::mutexCrawled.unlock();
      cout << "DEU BOM " << andre << "\n";

      CkString html;
      spider.get_LastHtml(html);

      //TRATA HTML

      //file << html.getString();

      int nOut = spider.get_NumOutboundLinks();
      int nUnsp= spider.get_NumUnspidered();
      cout << "nUnsp: " << nUnsp << " nOut: " << nOut << endl;

      for (int i=0; i<nUnsp; i++){
        CkString nxt;
        spider.GetUnspideredUrl(0, nxt);
        string nextUrl = nxt.getString();
        string nxtUrl = spider.canonicalizeUrl(nextUrl.c_str());
        string nxtDom = spider.getBaseDomain(nxtUrl.c_str());
        crawler::mutexCrawled.lock();
        bool isCrawled = crawler::sc.checkCrawled(nextUrl, nxtDom);
        crawler::mutexCrawled.unlock();
        if (!isCrawled){
          url prox(nextUrl);
          crawler::mutexQueue.lock();
          crawler::sc.addInbound(prox);
          crawler::mutexQueue.unlock();
        }
        spider.SkipUnspidered(0);
      }

      for (int i=0; i<nOut; i++){
        CkString nxt;
        spider.GetOutboundLink(i, nxt);
        string nextUrl = nxt.getString();
        string nxtUrl = spider.canonicalizeUrl(nextUrl.c_str());
        string nxtDom = spider.getBaseDomain(nxtUrl.c_str());
        crawler::mutexCrawled.lock();
        bool isCrawled = crawler::sc.checkCrawled(nextUrl, nxtDom);
        crawler::mutexCrawled.unlock();
        if (!isCrawled){
          url prox(nextUrl);
          crawler::mutexQueue.lock();
          crawler::sc.addOutbound(prox);
          crawler::mutexQueue.unlock();
        }
      }
      spider.ClearOutboundLinks();
    }
    spider.SleepMs(1000);
  }
  file.close();
}



//
// string crawler::normalizeUrl(string &name){
//
// }
