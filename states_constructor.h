#include <iostream>
#include <vector>
#include <typeinfo>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

vector<string> state_out;       //Vector with the names of particles in the final state 
vector<string> state_in;        //Vector with the names of particles in the initial state
vector<particle> p_in;         //Vector with particles in the initial state
vector<particle> p_out;        //Vector with particles in the final state
int counter{0};                     //Counter for the number of particles
int Nparticles;                    //Number of particles in the final state
long double E1{0};             //Energy of the beam 1
long double E2{0};             //Energy of the beam 2
int n_events{0};               //Number of events

void states(string path, string name){

  string state1;
  string state2;

  string state;

  //Read the process from process_card
  string proces_card = name + "/" + "process_card.txt";  
  ifstream states (proces_card);

  if (states.is_open())
  {
    
  getline(states, state);
  state1 = state;
  getline(states, state);
  state2 = state;}

  //Prevent if there is no process_card
  else{
    cout << "Error reading the process_card" << endl;
  }
  
  int len1 = state1.size();   //Number of characteres in the initial state
  int len2 = state2.size();   //Number of characteres in the final state

  //Read the parameters of the run
  string run_car = name + "/" + "run_card.txt";
  ifstream params (run_car);
  string param;

  if (params.is_open())
  {
    
  getline(params, param);
  E1 = stof(param);
  getline(params, param);
  E2 = stof(param);
  getline(params, param);
  n_events = static_cast<int>(stof(param));}

  //Prevent if here is no run_card
  else{
    cout << "Error reading the run_card" << endl;
  }
    
  //Counter for the particles in the final state, since the states are written leaving " " between particles.
  for(int i = 0; i < len2; i++){
    if(state2[i] == ' '){
      counter += 1;
    }
  }

  //If the state ends with a " " the number of particles is the number of spaces.
  if(state2[len2-1] == ' '){
    Nparticles = counter;
  }

  //If the state ends in a character the number of particles is the number of spaces plus one
  else{
    Nparticles = counter + 1;
    }

  //Resize the state vectors
  state_out.resize(Nparticles);
  state_in.resize(2);

  //Contruct the vector with the labels of particles in the initial state
  int count{0};
  for(int i = 0; i < 2; i++){
    for(int ii = count; ii < len1; ii++){

      //Adds a characters to the string state1[ii] until a space appears
      if(state1[ii] == ' '){
	count += 1;
	break;
      }

      //Add a particle to the vector state
      state_in[i] += state1[ii];
      count += 1;
}
  }

  //Contruct the vector with the labels of particles in the final state
  count = 0;
  for(int i = 0; i < Nparticles; i++){
    for(int ii = count; ii < len2; ii++){

      //Adds a characters to the string state2[ii] until a space appears
      if(state2[ii] == ' '){
	count += 1;
	break;
      }

      //Add a particle to the vector state
      state_out[i] += state2[ii];
      count += 1;
}
  }

  //Now we map the particles names to the particles defined in particle_content.h
  p_in.resize(2);
  p_out.resize(Nparticles);

  for(int i = 0; i < 2; i++){
    p_in[i] = str_2_ptcle(state_in[i]);
  }

  for(int i = 0; i < Nparticles; i++){
    p_out[i] = str_2_ptcle(state_out[i]);
  }
}
