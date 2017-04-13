#include "filemanager.hpp"
using namespace std;
fileManager::fileManager(int threadId){
  stringstream ss;
  ss << "htmls/" << threadId;// << "/";
  path = ss.str();
  nFiles=0;
  pages=std::string();
  // stringstream ind;
  // ind <<
  // indexName =
}

void fileManager::formatHtml(string &html){
  for (int c=0; c<html.size(); c++){
    if (html[c] == '|') html[c] = ' ';
  }
}

string fileManager::generateFileName(){
  stringstream ss;
  ss << path << "_" << nFiles/NHTML << ".txt";
  return ss.str();
}

void fileManager::writeHtml(string &url, string &html){
  if(nFiles%NHTML == 0){
    if (nFiles != 0){
      file << "|||";
      //ss.str( std::string() );
      pages.clear();
      pages.shrink_to_fit();
      file.close();
    }
    fileName = generateFileName();
    file.open(fileName,fstream::app);
    // file.open()
  }
  // cout << "writing html on path " << path << " on file " << fileName << " of url " << url << endl;
  stringstream ss;
  formatHtml(html);
  ss << "||| " << url << " | " << html << " ";
  pages.append(ss.str());
  if (nFiles%50 == 49)  file << pages;
  nFiles++;
  // cout << "numero de urls salvas pela thread " << threadId << ": " nFiles << "\n";
}
