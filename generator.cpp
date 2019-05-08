#include "particle_content.h"       //Particle definitions
#include "states_constructor.h"     //Returns an initial and final state vector named state_in and state_out
#include "conservation.h"           //Checks the conservation of spin and charge and E1 + E2 > m1 + m2 + m3 +...+
#include "in_out.h"
//#include "str_oper.h"
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <chrono>
#include <thread> 

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define fpath TOSTRING(PATH)
#define fname TOSTRING(NAME)
#define plot TOSTRING(PLOT)

using namespace std;

string path = fpath;
string name = fname;

int main(){

  states(path, name);           //Ask for the states and return the vectors p_in and p_out with the particles in the states initial and final

  p_in[0].energy = E1;
  p_in[1].energy = E2;
  p_in[0].mom[2] = sqrt(E1*E1-p_in[0].mass*p_in[0].mass);
  p_in[1].mom[2] = -sqrt(E2*E2-p_in[1].mass*p_in[1].mass);
  
  prepare(path, name);
  srand(time(NULL));  //Seed for the random numbers, it varies every time the script is executed

  float s_sqr = E1 + E2;
  
  spin_check();       //Check that the process conserves spin
  charge_check();     //Check that the process conserves charge
  kin_check(E1, E2);  //Check that the process is kinematically allowed

  cout << "Generating events" << endl;
  for(int i = 0; i < n_events; i++){
    cout << (100*i)/n_events << "%\r";
    std::cout.flush();
    //this_thread::sleep_for(chrono::miiseconds(1));
    
    long  aux = s_sqr;
    long  p{0};
    float phi{0};
    long  En{0};
    float theta{0};
    long  suma1{0};
    float suma2{0};

    if(Nparticles == 2){
      p_out[0].energy = (E1+E2)/2.0;
      p_out[1].energy = (E1+E2)/2.0;
	
      theta = 2 * M_PI * (static_cast<float>(rand()))/(static_cast<float>(RAND_MAX));
	
      p = sqrt(p_out[0].energy*p_out[0].energy - p_out[0].mass*p_out[0].mass);
      p_out[0].mom[0] = p * sin(theta);
      p_out[0].mom[2] = p * cos(theta);

      p_out[1].mom[0] = -p * sin(theta);
      p_out[1].mom[2] = -p * cos(theta);
    }

    else{
      for(int j = 0; j < Nparticles-1; j++){
      
      En = (aux - p_out[j].mass) * (static_cast<float>(rand()))/(static_cast<float>(RAND_MAX)) + p_out[j].mass;
      theta = M_PI * (static_cast<float>(rand()))/(static_cast<float>(RAND_MAX)) - 0.5 * M_PI;
      p_out[j].energy = En;
      p = sqrt(En*En - p_out[j].mass*p_out[j].mass);
      p_out[j].mom[0] = p * sin(theta);
      p_out[j].mom[2] = p * cos(theta);
      suma1 += p * sin(theta);
      suma2 += p * cos(theta);
      aux = aux - En;
    }

    p_out[Nparticles-1].mom[0] = -suma1;
    p_out[Nparticles-1].mom[2] = -suma2;
    p_out[Nparticles-1].energy = sqrt(suma2*suma2+suma1*suma1 + p_out[Nparticles-1].mass*p_out[Nparticles-1].mass);
    }
  write_file();
  }
  close_file();
  return 0;
}

