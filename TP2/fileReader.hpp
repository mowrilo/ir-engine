#include <iostream>
#include <fstream>
#include <stringstream>

using namespace std;

class fileReader{
  public:
    string getNextHtml();
    bool openNewFile();
    bool closeFile();

  private:
    ifstream file;
    string path_to_files;
    string html;
};
