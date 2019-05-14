#include <iostream>
#include <vector>
#include <typeinfo>
#include <string>

using namespace std;

//Spin conservation
void spin_check(){
  
  float sum_spin1{0};   //Total initial spin
  float sum_spin2{0};   //Total final spin

  //Initial states always have two particles
  for(int i = 0; i < 2; i++){
    sum_spin1 += p_in[i].spin;
  }

  //Nparticles is calculated by states_constructor, is the number of particles in the final state
  for(int i = 0; i < Nparticles; i++){
    sum_spin2 += p_out[i].spin;
  }

  //Spin check, if is violated kill the process
  float frac1 = sum_spin1 - int(sum_spin1);
  float frac2 = sum_spin2 - int(sum_spin2);

  if(frac1 == 0 && frac2 != 0 && sum_spin2 < sum_spin1){
    cout << "The process violates spin conservation" << endl;
    cout << "Process stopped" << endl;
    exit(0);
  }

  if(frac1 != 0 && frac2 == 0 && sum_spin1 < sum_spin2){
    cout << "The process violates helicity conservation" << endl;
    cout << "Process stopped" << endl;
    exit(0);
  }
}

//Charge conservation
void charge_check(){
  
  float sum_charge1{0};    //Total initial charge
  float sum_charge2{0};    //Total final charge

  //Initial states always have two particles
  for(int i = 0; i < 2; i++){
    sum_charge1 += p_in[i].charge;
  }

  //Nparticles is calculated by states_constructor, is the number of particles in the final state
  for(int i = 0; i < Nparticles; i++){
    sum_charge2 += p_out[i].charge;
  }
  
  //Spin check, if is violated kill the process
  if(sum_charge1 != sum_charge2){
    cout << "The process violates charge conservation" << endl;
    cout << "Process stopped" << endl;
    exit(0);
  }
}

void check_mom(){
  
  float sum_mom1[3]{0,0,0};
  float sum_mom2[3]{0,0,0};

    //Initial states always have two particles
  for(int i = 0; i < 2; i++){
    sum_mom1[0] += p_in[i].mom[0];
    sum_mom1[1] += p_in[i].mom[1];
    sum_mom1[2] += p_in[i].mom[2];
  }

  //Nparticles is calculated by states_constructor, is the number of particles in the final state
  for(int i = 0; i < Nparticles; i++){
    sum_mom2[0] += p_out[i].mom[0];
    sum_mom2[1] += p_out[i].mom[1];
    sum_mom2[2] += p_out[i].mom[2];
  }
  
  //mommentum check
  for(int j = 0; j < 3; j++){
    if(sum_mom1[j] != sum_mom2[j]){
      cout << "The process has violated the momentum conservation in the " << j << "component." << endl;
      cout << "calculation ended" << endl;
      break;
    }
  }
}

//Check that E1 + E2 > sum of masses in the final state
void kin_check(float E1, float E2){
  float en_in = E1 + E2;
  float sum_mass{0};

  for(int i = 0; i < Nparticles; i++){
    sum_mass += p_out[i].mass;
  }

  if(en_in < sum_mass){
    cout << "The process is kinematically forbidden" << endl;
    cout << "Process stopped" << endl;
    exit(0);
  }
}
