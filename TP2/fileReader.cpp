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

string fileReader::getNextHtml(){
  stringstream ss;
  char c;
  unsigned char c2;
  file.get(c);
  c2 = (unsigned char) c;
  while (c2 == '|'){
    file.get(c);
    c2 = (unsigned char) c;
  }
  while (c2 != '|'){
    cout << c2;
    ss << c2;
    file.get(c);
    c2 = (unsigned char) c;
  }
  cout << "\n";
  //string url = ss.str();
  ss << c2;
  ss.str("");
  file.get(c);
  c2 = (unsigned char) c;

  while (c2 != '|'){
    if ((int) c2 == 195){
  		file.get(c);
      c2 = (unsigned char) c;
  		cout << (int) c2 << "\n";
  	}
    ss << c2;
    file.get(c);
    c2 = (unsigned char) c;
  }
  string html = ss.str();
  //cout << html;
  if (HTML::detect_utf8(html.c_str(), html.size())) cout << "ALO ALO MARCIANO \n";
  //CharsetConverter csc("ISO8859-1","ascii");
  html = HTML::decode_entities(html);
  // csc.convert(html);
  return html;
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
