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

string input;             //Variable to store the user's input commands
string path;              //Variable to store the path of the folder in which the results will be saved
string process_name;      //Variable to store the folder name of the process
ofstream run_info;        //File to store the data of the run (Beam energy 1, beam energy 2, number of events)
ofstream process_info;    //File to store the info of the process generated
ofstream runs_num;        //File to store the number of runs
ofstream plot_info;
string path_mad;

//Load the params from the param_card
void load_params(){
  
  string line;
  ifstream input ("param_card.txt");
  
  if (input.is_open())
  {  
    getline(input, line);
    path = line;

    getline(input, line);
    path_mad = line;
    
    input.close();
  }

  else cout << "Unable to find param_card\n";
}

//Creates the run_card template and the runs.txt
void create_files(string road, string p_name){
  
  string run_card = road + "/" + p_name + "/run_card.txt";
  string plot_card = road + "/" + p_name + "/plot_card.txt";
  run_info.open(run_card);
  plot_info.open(plot_card);

  //plot_info << "import " + road + "/" + p_name + "/run01/" + p_name + "_run01.lhe";
  plot_info << "\n";
  plot_info << "set main.graphic_render = matplotlib\n";
  plot_info << "set main.normalize = none\n";
  plot_info << "#set main.lumi = 1000\n";
  plot_info << "plot ABSETA(e-[1]) 100 0 5\n";
  plot_info << "plot PT(e-[1]) 100 100 500 \n";
  plot_info << "submit\n";
  plot_info.close();

  run_info << "6500\n";
  run_info << "6500\n";
  run_info << "10000\n";
  run_info.close();

  string s3 = road + "/" + p_name + "/runs.txt";
  runs_num.open(s3);
  runs_num.close();

  cout << "The process card and the compiled program were put in " + road + "/" + p_name << endl;
}

//Create the process_card
//Compile generator.cpp in path/process_name/ with states 1 and 2
void compile(string path,string process_name, string state1, string state2){

  string process_card = path + "/" + process_name + "/process_card.txt";
  process_info.open(process_card);
  process_info << state1 << endl;
  process_info << state2 << endl;
  process_info.close();
 
  string path_to_compile = path + "/" + process_name;
  string comando = "g++ -DPATH=" + path_to_compile + " -DNAME=" + process_name + " -std=c++11 -o " + path_to_compile
    + "/" + process_name + " generator.cpp"; 
  str_2_sh(comando);
}

int main(){

  cout << "Welcome to the event generator, type help for instructions\n"; 

  //Initialize the shell
  while(true){

    load_params();
    
    cout << "event_generator>";
    std::getline(std::cin, input);

    //Creates the output folder
    if(split_str(input," ").s[0] == "output"){
      process_name = split_str(input," ").s[1];
      string order = "mkdir  "+ path + "/" + process_name;
      str_2_sh(order);
      create_files(path, process_name);
      cout << "A run card has been created in " + path + "/" + process_name + ". Check it to change the parameters of the run" << endl;
    }

    //Generate and compile the process
    else if(split_str(input," ").s[0] == "generate"){

      string state1;
      string state2;
      string states = split_str(input," ").s[1];
      state1 = split_str(states," > ").s[0];
      state2 = split_str(states," > ").s[1];
      compile(path, process_name, state1, state2);}

    //Launch the event generator of the process
    else if(split_str(input," ").s[0] == "launch"){
      string process = split_str(input," ").s[1];
      string launch = path + "/" + process + "/" + process;
      str_2_sh(launch);}

    else if(split_str(input," ").s[0] == "plot"){
      string folders = split_str(input," ").s[1];
      string folder1 = split_str(folders," ").s[0];
      string folder2 = split_str(folders," ").s[1];
      string comando = "sed -i -e '1i import " + path + "/" + folder1 + "/run" + folder2 + "/" + folder1 + "_run" + folder2 + ".lhe" + " as data\' "
	+ path + "/" + folder1 + "/plot_card.txt";
      str_2_sh(comando);
      
      //string str2 = "cd " + folder1 + "/run" + folder2 + "/";
      //str_2_sh(str2);
      
      string plot_str = path_mad + " " + path + "/" + folder1 +"/plot_card.txt";
      str_2_sh(plot_str);
      
      string str = "sed -i '/import/d' " + folder1 + "/" + "plot_card.txt"; 
      str_2_sh(str);

      string str3 = "mv ANALYSIS*/ " + path + "/" + folder1 + "/run" + folder2 + "/";
      str_2_sh(str3);
   
    }

    //CLose program
    else if(input == "quit"){
      cout << "Closing" << endl;
      exit(0);}

    //Launch the help
    else if(input == "help"){
      help();}

    //Prevent for bad command typing
    else{
      cout << "Command not found, type help for documentation" << endl;}
  }
  return 0;
}
