#include <iostream>
#include <htmlcxx/html/ParserDom.h>
#include <regex>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <map>
#include "vocabulary.hpp"

using namespace std;
using namespace htmlcxx;
//pair<unordered_map<string,int>, pair<string> >
typedef struct parsedInfo{
  unordered_map<string,int> termFreq;
  unordered_map<string,vector<string> > linkTerm;
} info;

class parser{
  public:
    parser();
    vector<string> getTerms(string &text); //separa termos de um fragmento de texto
    bool isJS(string text); //checa se o texto é script
    void retiraAcentos(string &text);
    void cleanWord(string &term); //retira aspas e mantém apenas caracteres alfanuméricos
    void normalizeText(string &text);
    info parse(string htmlToParse); //recebe o html, faz o parsing e retorna uma hash com os termos e frequências
    void printVoc(); //printa o vocabulario na tela

  private:
    int ndoc;
    unordered_map<int,char> charsetHTML;
};
