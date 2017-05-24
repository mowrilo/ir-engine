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

vector<string> parser::getTerms(string &text){
  vector<string> rtrn(1);
  // cout << text << "\n";
  // cout << "initial size: " << text.size() << "\n";
  while(text.size() > 0){
    while (text[0] == ' ')  text.erase(text.begin());
    string sub;
    int pos=0;
    while ((text[pos] != ' ') && (text[pos] != '\0'))  pos++;
    sub = text.substr(0,pos);
    // cout << pos << " " << sub << "\n";
    // cout << "antes: " << text << "\n";
    text.erase(0,pos);
    // cout << "depois: " << text << "\n";
    rtrn.push_back(sub);
  }
  return rtrn;
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
  vector<string> termVec;
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
        termVec = getTerms(text);
        for (int i=0; i<termVec.size(); i++){
          cout << termVec[i] << "\n";
        }
      }
      //conserta essa m
  	}
  }
}
