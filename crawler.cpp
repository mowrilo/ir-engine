#include "crawler.hpp"

using namespace std;

crawler::crawler(){}

void crawler::begin(){
  CkSpider spider;
  scheduler sc;
  ofstream file;
	file.open("globo.com");
  string urlInit = "http://www.globo.com";
  url pato(urlInit);
  sc.addOutbound(pato);
  //sp.Initialize(urlInit.c_str());
  //string asd = "http://www.globo.com/";
  //spider.AddUnspidered(asd.c_str());
  while (true){
    //cout << "OI\n";
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
    // int i;
    // for (i = 0; i <= 30; i++) {
    if (spider.CrawlNext()){
      //cout << success;
      string andre = spider.lastUrl();
      // int nUnsp = spider.get_NumUnspidered();
      //cout << "nUnsp: " << nUnsp << endl;
      // for (int j = 0; j<nUnsp; j++){
      // CkString nxt;
      // spider.GetUnspideredUrl(j, nxt);
      //nxt.saveToFile("/home/murilo/Documentos/ri/file.txt","utf-8");
      //nxt = spider.canonicalizeUrl(nxt);//nxt.getString();
      // string nextUrl = nxt.getString();
      // cout << "proxs: " << nextUrl << endl;
    // }
      //string andre = p.getString();
      sc.addCrawled(andre);
      if (andre.back() == '/'){
        andre.pop_back();
        sc.addCrawled(andre);
      }
      cout << "DEU BOM " << andre << "\n";
    // }
    // else  cout << "DEU RUIM\n";
      //url macedo(andre);
      //sc.addOutbound(macedo);

      //joga tudo fora
      int nOut = spider.get_NumOutboundLinks();
      int nUnsp= spider.get_NumUnspidered();
      cout << "nUnsp: " << nUnsp << "nOut: " << nOut << endl;
      for (int i=0; i<nOut; i++){
        CkString nxt;
        spider.GetOutboundLink(i, nxt);
        // nxt.saveToFile("/home/murilo/Documentos/ri/file.txt","utf-8");
        //nxt = spider.canonicalizeUrl(nxt);//nxt.getString();
        string nextUrl = nxt.getString();
        string nxtUrl = spider.canonicalizeUrl(nextUrl.c_str());
        if (!sc.checkCrawled(nextUrl)){
          url prox(nextUrl);
          //cout << "new url: " << prox.getName() << endl;
          sc.addOutbound(prox);
        }
        spider.SkipUnspidered(0);
      }
      // spider.SleepMs(1000);
      //adiciona tudo de novo
      //for (int i=0; i<nUnsp; i++){
      //url teste = sc.getOutbound();
      //cout << teste.getName() << endl;}
    //}
    }

  }
  file.close();
}

string crawler::normalizeUrl(string &name){

}
