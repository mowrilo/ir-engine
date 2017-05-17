#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class fileReader{
  public:
    fileReader(string path, int initNum, int numThreads);
    string getNextHtml();
    bool openNextFile();
    bool closeFile();

  private:
    ifstream file;
    string path_to_files;
    string html;
    int fileNumber;
    int nthreads;
};
