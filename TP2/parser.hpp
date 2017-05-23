#include <iostream>
#include <htmlcxx/html/ParserDom.h>
#include <regex>

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
    void setHtml(string htmlToSet);
    string getNextTerm();
    bool isJS(string text);
    void retiraAcentos(string &text);
    void normalizeText(string &text);
    void parse(string html);

  private:
    string html;
    int ndoc;
};
