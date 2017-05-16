#include <htmlcxx/html/ParserDom.h>
#include <bits/stdc++.h>


using namespace std;
using namespace htmlcxx;

int main(){
  ifstream fin;
  stringstream ss;
  fin.open("html");
  string html;
  char asd;
  fin.get(asd);
  while (!fin.eof()){
    ss << asd;
    fin.get(asd);
  }
  html = ss.str();
  // fin >> html;
  // fin >> html;
  //string html = "<html><body>hey</body></html>";
  HTML::ParserDom parser;
  tree<htmlcxx::HTML::Node> dom = parser.parseTree(html);
  //cout << "parse tree:\n" << dom << endl;
  //Dump all links in the tree
  tree<HTML::Node>::iterator it = dom.begin();
  tree<HTML::Node>::iterator end = dom.end();
  for (; it != end; ++it)
  {
  	//if (it->tagName() == "A")
  	//{
  		it->parseAttributes();
      pair<bool, string> a = it->attribute("content");
  		if (a.first)  cout << "attribute: "<< a.second << "\n";
  	//}
  }
  cout << "dump:\n";
  //Dump all text of the document
  it = dom.begin();
  end = dom.end();
  for (; it != end; ++it)
  {
  	if ((!it->isComment()))//(it->isTag()) &&
  	{
  		cout << it->tagName() << "\n";
  	}
  }
  fin.close();
  return 0;
}
