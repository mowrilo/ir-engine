#include "parser.hpp"

using namespace std;
using namespace htmlcxx;

parser::parser(){
  ifstream chars;
  chars.open("charset");
  string aux;
  while (getline(chars,aux)){ //faz uma tabela hash com o charset de html, para retirar acentos e outros caracteres
    char c = aux[0];
    aux.erase(aux.begin());
    aux.erase(aux.begin());
    int code = atoi(aux.c_str());
    pair<int,char> entrada(code,c);
    charsetHTML.insert(entrada);
  }
  chars.close();
}

void parser::cleanWord(string &term){
  int last=term.size()-1;
  if ((term[0] == '\'') || (term[0] == '\"'))  term.erase(term.begin()); //retira aspas
  if ((term[last] == '\'') || (term[last] == '\"'))  term.pop_back();
  int pos=0;
  while ((pos < term.size()) && ((term[pos] >= 'a' && term[pos] <= 'z') || (term[pos] >= '0' && term[pos] <= '9'))){
    pos++; //mantém caracteres alfanuméricos
  }
  term = term.substr(0,pos);
  if ((term.size() > 4) && (term.back() == 's'))  term.pop_back(); //tira 's' do fim de palavras com mais de 4 letras
}

vector<string> parser::getTerms(string &text){ //separa as palavras de um segmento de texto
  vector<string> rtrn;
  while(text.size() > 0){
    while (text[0] == ' ')  text.erase(text.begin());
    string sub;
    int pos=0;
    // for (int i=0; i<text.size(); i++) cout << "letra " << i << ":" << text[i] << "!\n";
    while ((text[pos] != ' ') && (pos < text.size())){
      //cout << "aqui!!!\n";
      pos++;
    }
    sub = text.substr(0,pos);
    text.erase(0,pos);
    cleanWord(sub);
    // cout << "text: \n" << text << "\nsub:\n" << sub << "\npos:\n" << pos << "\ntextpos:\n" << text[pos] << "\n";
    // cout << "text size:\n" << text.size() << "\ntext space:\n" << (text[0] == ' ') << "\n";
    if ((sub.size() > 1) && (sub.size() < 30)){
      rtrn.push_back(sub);
    }
  }
  return rtrn;
}

bool parser::isJS(string text){ //checa se o texto é script
  int find = text.find('{');
  if (find != string::npos) return true;
  find = text.find(" var ");
  if (find != string::npos) return true;
  return false;
}

void parser::retiraAcentos(string &text){ //baseia-se no charset inicializado
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

info parser::parse(string htmlToParse){//pair<, vector<string> >
  HTML::ParserDom parserInst; //instância do analizador de html
  vector<string> termVec;
  tree<htmlcxx::HTML::Node> dom = parserInst.parseTree(htmlToParse); //cria a árvore de parsing
  // cout << "criou arvore\n";
  tree<HTML::Node>::iterator it = dom.begin();
  tree<HTML::Node>::iterator end = dom.end();
  unordered_map<string,int> termFreqs;
  unordered_map<string,vector<string> > linkTerms;
  for (; it != end; ++it){
  		it->parseAttributes(); //faz o parsing na árvore
  }
  // cout << "fez parsing na arvore\n";
  it = dom.begin();
  end = dom.end();
  for (; it != end; ++it){
    // cout << "entrou for1\n";
    // cout << "texto: \n" << *it << "\n";
  	if ((!it->isTag()) && (!it->isComment())) //caso o texto não seja uma tag ou um comentário,
  	{                                        //normaliza e obtém seus termos e frequências
      // cout << "notag and no comment\n";
      string text = it->text();
      if (!isJS(text)){
        // cout << "noJS";
        termVec = getTerms(text);
        for (int i=0; i<termVec.size(); i++){
          // cout << "entrou for2\n";
          unordered_map<string,int>::iterator it = termFreqs.find(termVec[i]);
          if (it != termFreqs.end()){
            it->second++;
          }
          else{
            pair<string,int> foo(termVec[i],1);
            termFreqs.insert(foo);
          }
        }
        // cout << "saiu for2\n";
      }
  	}
    if (it->isTag()){
      string aroldo = it->tagName();
      if (aroldo.compare("a") == 0){
        HTML::Node nod = *it;
        nod.parseAttributes();
        // cout << "text: " << it->text() << "\n";
        map<string,string> att = nod.attributes();
        // cout << nod.attributes() << "\n";
        vector<string> termos;
        string url = "";
        for (map<string,string>::iterator iit = att.begin(); iit != att.end(); ++iit){
          //pair<string,vector<string> > links;
          // cout << "attribute: " << iit->first << "  " << iit->second << "\n";
          if (iit->first.compare("title") == 0){
            string texto = iit->second;
            termos = getTerms(texto);
          }
          else if (iit->first.compare("href") == 0){
            url = iit->second;
          }
        }
        if ((url.size() < 80) && (url.size() > 10)){
          string first4 = url.substr(0,4);
          if (first4.compare("http") == 0){
            // if (url.size() == 0)  cout << "ALO ALO MARCIANO\n";
            pair<string,vector<string> > links(url,termos);
            linkTerms.insert(links);
          }
        }
      }
    }
  }
  info retorno;
  retorno.termFreq = termFreqs;
  retorno.linkTerm = linkTerms;
  // cout << "saiu for1\n";
  return retorno;
}

//vector<string> parseLinks
