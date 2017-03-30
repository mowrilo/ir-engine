#include "crawler.hpp"

using namespace std;

crawler::crawler(){}

void crawler::begin(){
  CkSpider spider;
  scheduler sc;
  ofstream file;
	file.open("cod.html");
  string urlInit = "http://www.9gag.com/";
  url pato(urlInit);
  sc.addOutbound(pato);
  //sp.Initialize(urlInit.c_str());
  //string asd = "http://www.globo.com/";
  //spider.AddUnspidered(asd.c_str());
  while (true){
    cout << "OI\n";
    url bla = sc.getOutbound();
    string a = bla.getName();
    CkString asd = spider.canonicalizeUrl(a.c_str());
    spider.Initialize(asd.getString());
    spider.AddUnspidered(a.c_str());//(a.c_str());
    //t = sp.get_NumUnspidered();
    //cout << t <<"\n";
    //CkString html;
    // spider.get_LastHtml(html);
    // string htmls = html.getString();
    // file << htmls;

    if (spider.CrawlNext()){
      //cout << success;
      cout << spider.lastUrl() << "\n";
      string andre = spider.lastUrl();
      //string andre = p.getString();
      sc.addCrawled(andre);
      if (andre.back() == '/'){
        andre.pop_back();
        sc.addCrawled(andre);
      }
      //url macedo(andre);
      //sc.addOutbound(macedo);

      //joga tudo fora
      int nUnsp = spider.get_NumUnspidered();
      cout << "nUnsp: " << nUnsp << endl;
      for (int i=0; i<nUnsp; i++){
        CkString nxt;
        spider.GetUnspideredUrl(0, nxt);
        string nextUrl = nxt.getString();
        cout << "new url: " << nextUrl << endl;
        if (!sc.checkCrawled(nextUrl)){
          url prox(nextUrl);
          sc.addOutbound(prox);
        }
        spider.SkipUnspidered(0);
      }
      spider.SleepMs(1000);
      //adiciona tudo de novo
    }
  }
  file.close();
}
