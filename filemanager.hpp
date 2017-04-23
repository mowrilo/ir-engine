#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>

#define NHTML 100

using namespace std;

class fileManager{
public:
    fileManager(int threadId); //Construtor
    void formatHtml(string &html); //Troca os pipes '|' por espaços ' '
    string generateFileName(); //Gera nome de arquivo para guardar HTMLs
    void writeHtml(string &url, string &html); //Escreve a URL e o HTML em disco

private:
    int nFiles; //Controle de quantas páginas foram escritas
    string path; //Caminho para o diretório onde escrever
    string fileName; //Nome do arquivo atual
    fstream file; //Arquivo para escrita
    string pages; //Buffer para escrever nos arquivos
};
