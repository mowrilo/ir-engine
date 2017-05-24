#include "fileReader.hpp"
#include <htmlcxx/html/utils.h>
#include <htmlcxx/html/CharsetConverter.h>

using namespace std;
using namespace htmlcxx;

fileReader::fileReader(string path, int initNum, int numThreads){
  fileNumber = initNum;
  nthreads = numThreads;
  path_to_files = path;
}

vector<string> fileReader::getNextHtml(){
  vector<string> page;
  stringstream ss;
  char c;
  unsigned char c2;
  file.get(c);
  c2 = (unsigned char) c;
  // cout << "entrou while1\n";
  while ((c2 == '|') && (!file.eof())){
    file.get(c);
    c2 = (unsigned char) c;
  }
  // cout << "entrou while2\n";
  string url = "";
  while ((c2 != '|') && (!file.eof())){
    if (c2 != ' ')  url += c2;
    ss << c2;
    file.get(c);
    c2 = (unsigned char) c;
  }
  page.push_back(url);
  // cout << "\n";
  //string url = ss.str();
  ss << c2;
  ss.str("");
  file.get(c);
  c2 = (unsigned char) c;

  // cout << "entrou while3\n";
  while ((c2 != '|') && (!file.eof())){
    // if ((int) c2 == 195){
  	// 	file.get(c);
    //   c2 = (unsigned char) c;
  	// 	cout << (int) c2 << "\n";
  	// }
    ss << c2;
    file.get(c);
    c2 = (unsigned char) c;
  }
  string html = ss.str();
  //cout << html;
  //CharsetConverter csc("ISO8859-1","ascii");
  html = HTML::decode_entities(html);
  page.push_back(html);
  // csc.convert(html);
  return page;
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
