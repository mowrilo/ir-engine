#include "filemanager.hpp"
using namespace std;
fileManager::fileManager(thread::id threadId){
  stringstream ss;
  ss << "htmls/" << threadId;// << "/";
  path = ss.str();
  nFiles=0;
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
      file.close();
    }
    fileName = generateFileName();
    file.open(fileName,fstream::app);
  }
  // cout << "writing html on path " << path << " on file " << fileName << " of url " << url << endl;
  stringstream ss;
  formatHtml(html);
  ss << "||| " << url << " | " << html << " ";
  if (nFiles%50 == 49)  file << ss.str();
  nFiles++;
}
