#include "str_oper.h"
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <typeinfo>
#include <math.h>

using namespace std;

FILE *events;

float Mass(float E, float px, float py, float pz){
  return E*E-px*px-py*py-pz*pz;
}

void CM_2_lab(){

  float g{0};
  float pz;
  float E;
  
  g = p_in[1].energy/p_in[1].mass;
  float P = p_in[1].mom[2]/p_in[1].mass;
  E = (p_in[0].energy - p_in[0].mom[2]*P)*g;
  pz = (p_in[0].mom[2] - p_in[0].energy*P)*g;
  p_in[0].energy = E;
  p_in[0].mom[2] = pz;

  for(int i = 0; i < Nparticles; i++){
    
    E = (p_out[i].energy - p_out[i].mom[2]*P)*g;
    pz = (p_out[i].mom[2] - p_out[i].energy*P)*g;
    p_out[i].energy = E;
    p_out[i].mom[2] = pz;
  }

  p_in[1].energy = p_in[1].mass;
  p_in[1].mom[2] = 0;
}

void prepare(string path, string name){

  string nruns_command = "ls -d -1q " + path + "/" + "run*  | wc -l > " + path + "/runs.txt";
  str_2_sh(nruns_command);

  string path_2_runs = path + "/runs.txt";

  string run;
  ifstream runs;
  runs.open (path_2_runs);
  getline(runs, run);
  
  int n_runs = static_cast<int>(stof(run))-2;
  
  string mk_fold = "mkdir  "+ path + "/" + "run" + to_string(n_runs+1);
  str_2_sh(mk_fold);
 
  string lhe_file = path + "/" + "run" + to_string(n_runs+1) + "/" + name + "_run" + to_string(n_runs+1) + ".lhe";
  char lhe[lhe_file.length()+1];
  strcpy(lhe, lhe_file.c_str());

  events = fopen(lhe,"w");

  fprintf(events, "<LesHouchesEvents version='1.0'>\n<header>\n</header>\n<init>\n");
  fprintf(events,"%3d %3d %0.4e %0.4e %d %d %d %d %d %d %d %d %d %d\n",p_in[0].name,p_in[1].name,p_in[0].energy,p_in[1].energy,
	  0,0,247000,247000,-4,1,0,0,0,1);
  fprintf(events,"</init>\n");

  }

void write_file(){

  fprintf(events,"<event>\n");
  fprintf(events,"%2d    %2d  %2d %2d %2d\n", Nparticles+2,1,1,-1,-1,-1);

  float Mas{0};

  CM_2_lab();
  
  for(int j = 0; j < 2; j++){
    Mas = Mass(p_in[j].energy,p_in[j].mom[0],p_in[j].mom[1],p_in[j].mom[2]);
    
    fprintf(events,"      %3d %2d %2d %2d %2d %2d %+.4e %+.4e %+.6e %+.6e %+.6e %d %d\n",
	    p_in[j].name,-1,0,0,0,0,
	    p_in[j].mom[0],p_in[j].mom[1],p_in[j].mom[2],
	    p_in[j].energy,Mas,0,9);
  }
  for(int i = 0; i < Nparticles; i++){
    Mas = Mass(p_out[i].energy,p_out[i].mom[0],p_out[i].mom[1],p_out[i].mom[2]);
    
    fprintf(events,"      %3d %2d %2d %2d %2d %2d %+.4e %+.4e %+.6e %+.6e %+.6e %d %d\n",
	    p_out[i].name,+1,1,2,0,0,
	    p_out[i].mom[0],p_out[i].mom[1],p_out[i].mom[2],
	    p_out[i].energy,Mas,0,9);
  }
  
  fprintf(events,"</event>\n");

}

void close_file(){
  fprintf(events,"</LesHouchesEvents>");
  fclose(events);
}
