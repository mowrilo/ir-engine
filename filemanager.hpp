#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>

#define NHTML 100

using namespace std;

class fileManager{
public:
    fileManager(int threadId);
    void formatHtml(string &html);
    string generateFileName();
    void writeHtml(string &url, string &html);

private:
    int nFiles;
    string path;
    string fileName;
    // string indexName;
    fstream file;
    // fstream index;
    string pages;
};
