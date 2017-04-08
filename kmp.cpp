#include "kmp.hpp"

using namespace std;
//
// vector<int> preprocess(string &pattern){
//
//   return prefixes;
// }

int find(string &pattern, string &text){
  int n = text.length();
  int m = pattern.length();
  vector<int> prefixes(m);
  int i,j=0;
  prefixes[0] = 0;
  for (i=0; i<m;i++){
    while ((j > 0) && (pattern[j] != pattern[i])){
			j = prefixes[j-1];
		}
		if (pattern[j] == pattern[i]){
			j++;
		}
    prefixes[i] = j;
  }
  //vector<int> prefixes = preprocess(pattern);
  //cout << "aroldo";
  j=0;
  for (i=0; i<n; i++){
		while((j>0) && (text[i] != pattern[j])){
			j = prefixes[j];
		}
		if(text[i] == pattern[j]){
			j++;
		}
		if(j == m){
			return (i-m+1);
		}
  }
  return -1;
}
