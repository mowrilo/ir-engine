#include <iostream>
using namespace std;

class url{
  public:
        url (string &nm, string &dm, int offset); //Construtor
        int getWeight(); //Retorna o peso da URL
        int setWeight(string &name, int offset); //Calcula e seta o peso da URL, de acordo com o nome e o Offset
        string getName(); //Retorna o nome da URL
        string getDomain();// Retorna o domínio da URL
        void setValid(string &name); //Decide se a URL é válida ou não, baseado no nome
        bool checkValid(); //Retorna se é válida ou não
        void increaseWeight(int i); //Aumenta o peso da URL
        bool operator<(const url& a) const; //Define operador de comparação para a fila de prioridades

  private:
        string name; //Nome
        string domain; //Domínio
        int weight; //Peso
        bool isValid; //Booleano que guarda a validade
};
