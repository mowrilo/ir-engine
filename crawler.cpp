#include "crawler.hpp"

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
  crawl("http://www.uol.com.br");
}

void crawler::crawl(string seedUrl){
  CkSpider spider;
  scheduler sc;
  ofstream file;
  //string urlInit = "http://www.globo.com";
  string filename = "/htmls.txt";
	//file.open(pathToStore+filename);
  url pato(seedUrl);
  sc.addOutbound(pato);
  cout << "UIUIUI\n";
  while (true){
    url bla = sc.getOutbound();
    string a = bla.getName();
    CkString asd = spider.canonicalizeUrl(a.c_str());
    spider.Initialize(asd.getString());
    spider.AddUnspidered(a.c_str());
    if (spider.CrawlNext()){
      string andre = spider.lastUrl();
      string andDom = spider.getBaseDomain(andre.c_str());
      sc.addCrawled(andre, andDom);
      if (andre.back() == '/'){
        andre.pop_back();
        sc.addCrawled(andre, andDom);
      }
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
        if (!sc.checkCrawled(nextUrl, nxtDom)){
          url prox(nextUrl);
          sc.addInbound(prox);
        }
        spider.SkipUnspidered(0);
      }

      for (int i=0; i<nOut; i++){
        CkString nxt;
        spider.GetOutboundLink(i, nxt);
        string nextUrl = nxt.getString();
        string nxtUrl = spider.canonicalizeUrl(nextUrl.c_str());
        string nxtDom = spider.getBaseDomain(nxtUrl.c_str());
        if (!sc.checkCrawled(nextUrl, nxtDom)){
          url prox(nextUrl);
          sc.addOutbound(prox);
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
