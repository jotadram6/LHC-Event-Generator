#include "help.h"
#include "str_oper.h"
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <map>
#include <functional>
#include <fstream>

using namespace std;

string input;
string path;
string process_name;
ofstream run_info;
ofstream process_info;
ofstream runs_num;

void load_params(){
  
  string line;
  ifstream input ("param_card.txt");
  
  if (input.is_open())
  {  
    getline(input, line);
    path = line;
    
    input.close();
  }

  else cout << "Unable to find param_card\n";
}

void create_files(string road, string p_name){
  
  string run_card = road + "/" + p_name + "/run_card.txt";
  run_info.open(run_card);
  
  run_info << "6500\n";
  run_info << "6500\n";
  run_info << "10000\n";
  run_info.close();

  string s3 = road + "/" + p_name + "/runs.txt";
  runs_num.open(s3);
  runs_num.close();

  cout << "The process card and the compiled program were put in " + s3 << endl;
}

void compile(string path,string process_name, string state1, string state2){

  string process_card = path + "/" + process_name + "/process_card.txt";
  process_info.open(process_card);
  process_info << state1 << endl;
  process_info << state2 << endl;
  process_info.close();
 
  string path_to_compile = path + "/" + process_name;
  string comando = "g++ -DPATH=" + path_to_compile + " -DNAME=" + process_name + " -o " + path_to_compile
    + "/" + process_name + " generator.cpp"; 
  str_2_sh(comando);

}

int main(){

  cout << "Welcome to the event generator, type help for instructions\n"; 

  while(true){

    cout << "event_generator>";
    std::getline(std::cin, input);

    if(split_str(input," ").s[0] == "output"){
      load_params();
      process_name = split_str(input," ").s[1];
      string order = "mkdir  "+ path + "/" + process_name;
      str_2_sh(order);
      create_files(path, process_name);
      cout << "A run card has been created in " + path + "/" + process_name + ". Check it to change the parameters of the run" << endl;
    }

    else if(split_str(input," ").s[0] == "generate"){

      string state1;
      string state2;
      string states = split_str(input," ").s[1];
      state1 = split_str(states," > ").s[0];
      state2 = split_str(states," > ").s[1];
      compile(path, process_name, state1, state2);}

    else if(split_str(input," ").s[0] == "launch"){
      string process = split_str(input," ").s[1];
      string launch = path + "/" + process + "/" + process;
      str_2_sh(launch);}
   
    else if(input == "quit"){
      cout << "Closing" << endl;
      exit(0);}
    
    else if(input == "help"){
      help();}

    else{
      cout << "Command not found, type help for documentation" << endl;}
  }
  return 0;
}
