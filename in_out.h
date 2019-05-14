#include "str_oper.h"
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <typeinfo>
#include <math.h>

using namespace std;

FILE *events;       //File to store the events

//To calculate the invariant mass
float Mass(float E, float px, float py, float pz){
  return E*E-px*px-py*py-pz*pz;
}

//To convert from CM frame to lab frame
void CM_2_lab(){

  long g{0};      //Gamma factor
  float pz;       //Momentum in the lab frame
  float E;        //Energy in the lab frame
  int index;
  int part;

  //If one of the initial particles is massless the we choose the another to be the target particle
  if(p_in[0].mass != 0){
    index = 0;      //Target particle
    part = 1;       //The no-target particle
    }

  else if(p_in[1].mass != 0){
    index = 1;
    part = 0;
    }

  //Initial particle different to the target
  g = p_in[index].energy/p_in[index].mass;               //Gamma factor
  float P = p_in[index].mom[2]/p_in[index].mass;         //For the tranformation rule
  E = (p_in[part].energy - p_in[part].mom[2]*P)*g;       //Energy in lab frame
  pz = (p_in[part].mom[2] - p_in[part].energy*P)*g;      //Momentum in the lab frame
  p_in[part].energy = E;
  p_in[part].mom[2] = pz;

  //Final particles
  for(int i = 0; i < Nparticles; i++){
    
    E = (p_out[i].energy - p_out[i].mom[2]*P)*g;
    pz = (p_out[i].mom[2] - p_out[i].energy*P)*g;
    p_out[i].energy = E;
    p_out[i].mom[2] = pz;
  }

  //Target particle
  p_in[index].energy = p_in[index].mass;
  p_in[index].mom[2] = 0;
}

void prepare(string path, string name){

  //Count files and folders beginning with run and store the number in runs.txt
  string nruns_command = "ls -d -1q " + path + "/" + "run*  | wc -l > " + path + "/runs.txt";
  str_2_sh(nruns_command);

  //Read the runs.txt
  string path_2_runs = path + "/runs.txt";
  string run;
  ifstream runs;
  runs.open (path_2_runs);
  getline(runs, run);

  //Number of runs, the minus 2 comes for the counting of the run_card and the runs.txt
  int n_runs = static_cast<int>(stof(run))-2;

  //Make the run folder
  string mk_fold = "mkdir  "+ path + "/" + "run" + to_string(n_runs+1);
  str_2_sh(mk_fold);

  string move = "mv " + path + "/process_card.txt " + path + "/" + "run" + to_string(n_runs+1);
  str_2_sh(move);

  //Creates the .lhe
  string lhe_file = path + "/" + "run" + to_string(n_runs+1) + "/" + name + "_run" + to_string(n_runs+1) + ".lhe";
  char lhe[lhe_file.length()+1];
  strcpy(lhe, lhe_file.c_str());

  events = fopen(lhe,"w");

  //Begins to print in the .lhe file following the LHE format
  fprintf(events, "<LesHouchesEvents version='1.0'>\n<header>\n</header>\n<init>\n");
  fprintf(events,"%3d %3d %0.4e %0.4e %d %d %d %d %d %d %d %d %d %d\n",p_in[0].name,p_in[1].name,p_in[0].energy,p_in[1].energy,
	  0,0,247000,247000,-4,1,0,0,0,1);
  fprintf(events,"</init>\n");
  }

//Function to write in the file .lhe everytime a new process is generated
void write_file(){

  fprintf(events,"<event>\n");
  fprintf(events,"%2d    %2d  %2d %2d %2d %2d\n", Nparticles+2,1,1,-1,-1,-1);

  float Mas{0};

  //To convert the CM values to the lab frame
  CM_2_lab();

  //Initial particles
  for(int j = 0; j < 2; j++){
    Mas = Mass(p_in[j].energy,p_in[j].mom[0],p_in[j].mom[1],p_in[j].mom[2]);         //Invariant mass
    
    fprintf(events,"      %3d %2d %2d %2d %2d %2d %+.4e %+.4e %+.6e %+.6e %+.6e %d %d\n",
	    p_in[j].name,-1,0,0,0,0,
	    p_in[j].mom[0],p_in[j].mom[1],p_in[j].mom[2],
	    p_in[j].energy,Mas,0,9);
  }

  //Final particles
  for(int i = 0; i < Nparticles; i++){
    Mas = Mass(p_out[i].energy,p_out[i].mom[0],p_out[i].mom[1],p_out[i].mom[2]);
    
    fprintf(events,"      %3d %2d %2d %2d %2d %2d %+.4e %+.4e %+.6e %+.6e %+.6e %d %d\n",
	    p_out[i].name,+1,1,2,0,0,
	    p_out[i].mom[0],p_out[i].mom[1],p_out[i].mom[2],
	    p_out[i].energy,Mas,0,9);
  }
  
  fprintf(events,"</event>\n");

}

//Close the file
void close_file(){
  fprintf(events,"</LesHouchesEvents>");
  fclose(events);
}
