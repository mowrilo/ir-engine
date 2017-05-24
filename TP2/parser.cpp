#include "parser.hpp"

using namespace std;
using namespace htmlcxx;

parser::parser(){
  ifstream chars;
  chars.open("charset");
  string aux;
  while (getline(chars,aux)){
    char c = aux[0];
    aux.erase(aux.begin());
    aux.erase(aux.begin());
    int code = atoi(aux.c_str());
    pair<int,char> entrada(code,c);
    charsetHTML.insert(entrada);
  }
  chars.close();
}

void parser::setHtml(string htmlToSet){
  html = htmlToSet;
}

string parser::getNextTerm(){

}

bool parser::isJS(string text){
  int find = text.find('{');
  if (find != string::npos) return true;
  find = text.find(" var ");
  if (find != string::npos) return true;
  return false;
}

void parser::retiraAcentos(string &text){ //hecho
  for (int i=0; i<text.size(); i++){
    unsigned char c = text[i];
    unordered_map<int,char>::iterator it = charsetHTML.find((int) c);
    if (it != charsetHTML.end()){
			text[i] = it->second;
		}
  }
}

void parser::normalizeText(string &text){ //tolower, tira plural, tira caracteres estranhos
  retiraAcentos(text);
  for (int i=0; i<text.size(); i++){
    text[i] = tolower(text[i]);
  }
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
      if (!isJS(text)){
        if (text[0] != ' '){
          if (text.length()) cout << "text: " << text << "\n";
        }
      }
      //conserta essa m
  	}
  }
}
