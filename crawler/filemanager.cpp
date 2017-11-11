#include "filemanager.hpp"
using namespace std;
fileManager::fileManager(int threadId){ //Construtor
  stringstream ss;
  ss << "htmls/" << threadId; //Define o diretório de armazenamento
  path = ss.str();
  nFiles=0;
  pages=std::string();
}

void fileManager::formatHtml(string &html){ //Substitui os pipes por espaços
  for (int c=0; c<html.size(); c++){
    if (html[c] == '|') html[c] = ' ';
  }
}

string fileManager::generateFileName(){ //Gera o nome do arquivo, baseando-se no número de páginas já escritas
  stringstream ss;
  ss << path << "_" << nFiles/NHTML << ".txt";
  return ss.str();
}

void fileManager::writeHtml(string &url, string &html){
  if(nFiles%NHTML == 0){ //Caso tenha escrito 100 HTMLs, fecha o arquivo e limpa o buffer
    if (nFiles != 0){
      file << "|||";
      pages.clear();
      pages.shrink_to_fit();
      file.close();
    }
    fileName = generateFileName(); //Gera um novo nome e abre o arquivo
    file.open(fileName,fstream::app);
  }
  stringstream ss;
  formatHtml(html);
  ss << "||| " << url << " | " << html << " ";
  pages.append(ss.str()); //Adiciona a URL e o HTML no buffer
  nFiles++;
  if (nFiles%50 == 0){ //Caso seja múltiplo de 50, escreve no arquivo e limpa o buffer
    file << pages;
    pages.clear();
    pages.shrink_to_fit();
  }
}
