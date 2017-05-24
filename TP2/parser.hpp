#include <iostream>
#include <htmlcxx/html/ParserDom.h>
#include <regex>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <vector>

/*
- tirar acentos
- tudo minusculo
- substituir \n . , por espaço em branco
- se tem caracter nao-alfanumerico, ja elvis
*/

using namespace std;
using namespace htmlcxx;

class parser{
  public:
    parser();
    void setHtml(string htmlToSet);
    vector<string> getTerms(string &text);
    bool isJS(string text);
    void retiraAcentos(string &text);
    void normalizeText(string &text);
    void parse(string html);

  private:
    string html;
    int ndoc;
    unordered_map<int,char> charsetHTML;
};
