#include "crawler.hpp"

using namespace std;

crawler::crawler(){}

void crawler::begin(){
  CkSpider spider;
  scheduler sc;
  ofstream file;
	file.open("cod.html");
  //string urlInit = "http://www.globo.com/";
  //url pato(urlInit);
  //sc.addOutbound(pato);
  // sp.Initialize(urlInit.c_str());
  // string asd = "http://www.globo.com/";
  // spider.AddUnspidered(asd.c_str());

  //while (true){
    //url bla = sc.getOutbound();
    //string a = bla.getName();
    //CkString asd = sp.canonicalizeUrl(a.c_str());
    spider.Initialize("globo.com");
    spider.AddUnspidered("http://www.globo.com");//(a.c_str());
    //t = sp.get_NumUnspidered();
    //cout << t <<"\n";
    bool success;
    success = spider.CrawlNext();
    CkString html;
    spider.get_LastHtml(html);
    string htmls = html.getString();
    file << htmls;

    if (success){
      //cout << success;
      cout << spider.lastUrl() << "\n";
      //CkString p = sp.lastUrl();
      //string andre = p.getString();
      //sc.addCrawled(andre);
      //url macedo(andre);
      //sc.addOutbound(macedo);

      //joga tudo fora

      //adiciona tudo de novo
    }
    file.close();
  //}
}
