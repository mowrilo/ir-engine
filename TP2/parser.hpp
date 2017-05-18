#include <iostream>
#include <htmlcxx>

using namespace std;
using namespace htmlcxx;

class parser{
  public:
    void setHtml();
    string getNextTerm();

  private:
    string html;
    int ndoc;
}
