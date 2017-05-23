#include "fileReader.hpp"

using namespace std;

fileReader::fileReader(string path, int initNum, int numThreads){
  fileNumber = initNum;
  nthreads = numThreads;
  path_to_files = path;
}

string fileReader::getNextHtml(){
  stringstream ss;
  char c;
  file.get(c);
  while (c == '|'){
    file.get(c);
  }
  while (c != '|'){
    cout << c;
    ss << c;
    file.get(c);
  }
  cout << "\n";
  //string url = ss.str();
  ss << c;
  ss.str("");
  file.get(c);

  while (c != '|'){
    ss << c;
    file.get(c);
  }
  //string html = ss.str();
  return ss.str();
}

bool fileReader::openNextFile(){
  string filename = "html_";
  filename.append(to_string(fileNumber));
  filename = path_to_files + filename;
  cout << "opened " << filename << "\n";
  fileNumber += nthreads;
  file.open(filename);
  if (file.is_open()) return true;
  else  return false;
}

bool fileReader::closeFile(){
  file.close();
}
