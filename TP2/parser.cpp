#include "parser.hpp"

vocabulary parser::voc;

using namespace std;
using namespace htmlcxx;

parser::parser(){
  // ntriples=0;
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
//
// void parser::setHtml(string htmlToSet){
//   html = htmlToSet;
// }

void parser::cleanWord(string &term){
  int last=term.size()-1;
  if ((term[0] == '\'') || (term[0] == '\"'))  term.erase(term.begin());
  if ((term[last] == '\'') || (term[last] == '\"'))  term.pop_back();
  int pos=0;
  while ((pos < term.size()) && ((term[pos] >= 'a' && term[pos] <= 'z') || (term[pos] >= '0' && term[pos] <= '9'))){
    pos++;
  }
  term = term.substr(0,pos);
  if ((term.size() > 4) && (term.back() == 's'))  term.pop_back();
}

vector<string> parser::getTerms(string &text){
  vector<string> rtrn;
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
    cleanWord(sub);
    if ((sub.size() > 1) && (sub.size() < 51)){
      rtrn.push_back(sub);
      // cout << "pushed " << sub << "\n";
    }
  }
  // cout << "size antes: " << rtrn.size() << "\n";
  return rtrn;
}

bool parser::isJS(string text){
  int find = text.find('{');
  if (find != string::npos) return true;
  find = text.find(" var ");
  if (find != string::npos) return true;
  return false;
}

void parser::retiraAcentos(string &text){
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
    if (text[i] == '\\')  text[i] = ' ';
  }
}

unordered_map<int,int> parser::parse(string htmlToParse){
  HTML::ParserDom parser;
  vector<string> termVec;
  tree<htmlcxx::HTML::Node> dom = parser.parseTree(htmlToParse);
  tree<HTML::Node>::iterator it = dom.begin();
  tree<HTML::Node>::iterator end = dom.end();
  unordered_map<int,int> termFreqs;
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
          voc.addTerm(termVec[i]);
          int termId = voc.getTermID(termVec[i]);
          unordered_map<int,int>::iterator it = termFreqs.find(termId);
          if (it != termFreqs.end()){
            it->second++;
          }
          else{
            pair<int,int> foo(termId,1);
            termFreqs.insert(foo);
          }
        }
      }
  	}
  }
  return termFreqs;
}

void parser::printVoc(){
  voc.print();
}
