#include "parser.hpp"

using namespace std;
using namespace htmlcxx;

void parser::setHtml(string htmlToSet){
  html = htmlToSet;
}

string parser::getNextTerm(){

}

bool parser::isJS(string text){
  int find = text.find('{');
  if (find != string::npos) return true;
  return false;
}

void parser::retiraAcentos(string &text){ //hecho
  for (int i=0; i<text.size(); i++){
    char a;
  }
}

void parser::normalizeText(string &text){ //tolower, tira plural, tira caracteres estranhos

}

void parser::parse(string html){
  HTML::ParserDom parser;
  tree<htmlcxx::HTML::Node> dom = parser.parseTree(html);
  tree<HTML::Node>::iterator it = dom.begin();
  tree<HTML::Node>::iterator end = dom.end();
  for (; it != end; ++it)
  {
  		it->parseAttributes();
  }
  it = dom.begin();
  end = dom.end();
  for (; it != end; ++it)
  {
  	if ((!it->isTag()) && (!it->isComment()))//(it->isTag()) &&
  	{
      string text = it->text();
      cout << text;
      //conserta essa m
  	}
  }
}
