#include "particle_content.h"       //Particle definitions
#include "states_constructor.h"     //Returns an initial and final state vector named state_in and state_out
#include "conservation.h"           //Checks the conservation of spin and charge and E1 + E2 > m1 + m2 + m3 +...+
#include "in_out.h"
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

//To pass the path and the name as macros in the compilation
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define fpath TOSTRING(PATH)
#define fname TOSTRING(NAME)

using namespace std;

//For the macros
string path = fpath;
string name = fname;

int main(){

  states(path, name);           //Read the process_card and return two vectors with the particles in the initial and final state (p_in and p_out)

  //Initial conditions
  p_in[0].energy = E1;
  p_in[1].energy = E2;
  p_in[0].mom[2] = sqrt(E1*E1-p_in[0].mass*p_in[0].mass);
  p_in[1].mom[2] = -sqrt(E2*E2-p_in[1].mass*p_in[1].mass);
  
  prepare(path, name);          //Creates the folder of the run and open the .lhe file to store the events
  srand(time(NULL));            //Seed for the random numbers, it varies every time the script is executed

  float s_sqr = E1 + E2;        //Total energy
  
  spin_check();         //Check that the process conserves helicity
  charge_check();       //Check that the process conserves charge
  kin_check(E1, E2);    //Check that the process is kinematically allowed

  cout << "Generating events" << endl;

  for(int i = 0; i < n_events; i++){
    //Print progress
    cout << (100*i)/n_events << "%\r";
    std::cout.flush();
    
    long  aux = s_sqr;  //For the max energy value allowed 
    long  p{0};         //A p magnitude
    long  En{0};        //A energy
    float theta{0};     //An angle
    long  suma1{0};     //To store the sum of momemtum in x direction
    float suma2{0};     //To store the sum of momentum in z direction

    //Case with 2 particles in the final state
    if(Nparticles == 2){

      //The energy is splitted in equal parts
      p_out[0].energy = (E1+E2)/2.0;
      p_out[1].energy = (E1+E2)/2.0;

      //Random angle between 0 and 2*pi
      theta = 2 * M_PI * (static_cast<float>(rand()))/(static_cast<float>(RAND_MAX));

      //Momentum
      p = sqrt(p_out[0].energy*p_out[0].energy - p_out[0].mass*p_out[0].mass);
      p_out[0].mom[0] = p * sin(theta);
      p_out[0].mom[2] = p * cos(theta);

      p_out[1].mom[0] = -p * sin(theta);
      p_out[1].mom[2] = -p * cos(theta);
    }

    //Case with more than 2 particles in the final state
    else{

      //Loop over all particles except the last one
      for(int j = 0; j < Nparticles-1; j++){

	//Random energy between the particle's mass and the max energy allowed (aux). Aux initialize in E1 + E2
	En = (aux - p_out[j].mass) * (static_cast<float>(rand()))/(static_cast<float>(RAND_MAX)) + p_out[j].mass;
	
	//Random angle between 0 and 2*pi
	theta = 2*M_PI * (static_cast<float>(rand()))/(static_cast<float>(RAND_MAX));

	//Save values
	p_out[j].energy = En;
	p = sqrt(En*En - p_out[j].mass*p_out[j].mass);
	p_out[j].mom[0] = p * sin(theta);
	p_out[j].mom[2] = p * cos(theta);

	//Sums of momentum in x and z direction
	suma1 += p * sin(theta);
	suma2 += p * cos(theta);

	//The max value of the energy allowed is shifted subtracting the result of the random En to the actual max value
	aux = aux - En;
    }

    //For the last particle, we calculate the momentum from the equations of conservation in x and z direction.
    p_out[Nparticles-1].mom[0] = -suma1;
    p_out[Nparticles-1].mom[2] = -suma2;
    p_out[Nparticles-1].energy = sqrt(suma2*suma2+suma1*suma1 + p_out[Nparticles-1].mass*p_out[Nparticles-1].mass);
    }

    //Save values in the .lhe
  write_file();
  }
  //Close the .lhe
  close_file();
  return 0;
}

