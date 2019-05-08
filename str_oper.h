#include <iostream>
#include <string>
#include <string.h>

using namespace std;

struct st_spl{
  string s[2];
};

st_spl split_str(string line, string delimeter){
  int cont{0};
  st_spl out;

  for(int i = 0; i < line.length(); i++){
    
    if(line.substr(i,delimeter.length()) != delimeter){
      out.s[0] += line[i];
      cont +=1;
      }

    else{
      break;
      }
  }

  for(int i = cont+delimeter.length(); i < line.length(); i++){
    out.s[1] += line[i];
    }

  return out;
}

void str_2_sh(string command){

  char shell[command.length()+1];
  strcpy(shell, command.c_str());
  system(shell);
}
