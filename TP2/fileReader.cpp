#include "fileReader.hpp"
#include <htmlcxx/html/utils.h>
#include <htmlcxx/html/CharsetConverter.h>

using namespace std;
using namespace htmlcxx;

fileReader::fileReader(string path, int initNum, int numThreads){
  fileNumber = initNum; //inicializa
  nthreads = numThreads;
  path_to_files = path;
}

vector<string> fileReader::getNextHtml(){
  vector<string> page; //utiliza dos limites do caracter pipe '|' para distinguir os HTMLs
  stringstream ss;
  char c;
  unsigned char c2;
  file.get(c);
  c2 = (unsigned char) c;
  while ((c2 == '|') && (!file.eof())){
    file.get(c);
    c2 = (unsigned char) c;
  }
  string url = "";
  while ((c2 != '|') && (!file.eof())){
    if (c2 != ' ')  url += c2;
    ss << c2;
    file.get(c);
    c2 = (unsigned char) c;
  }
  page.push_back(url);
  ss << c2;
  ss.str("");
  file.get(c);
  c2 = (unsigned char) c;

  while ((c2 != '|') && (!file.eof())){
    ss << c2;
    file.get(c);
    c2 = (unsigned char) c;
  }
  string html = ss.str();
  page.push_back(html); //retorna a URL e o HTML da página
  html= "";
  html.shrink_to_fit();
  return page;
}

bool fileReader::openNextFile(int nFile){
  // cout << "entrou no open next!\n";
  string filename = "html_";
  fileNumber = nFile;
  filename.append(to_string(fileNumber));
  filename = path_to_files + filename;
  fileNumber += nthreads;
  file.open(filename);//processa o nome e abre o arquivo
  if (file.is_open()) return true;
  else  return false;
}

bool fileReader::closeFile(){
  file.close();
}
