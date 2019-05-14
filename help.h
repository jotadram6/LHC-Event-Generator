#include <iostream>

using namespace std;

void help(){
  
  system("clear");

  cout << "Particles are defined as:\n";
  cout << "Charged leptons and antileptons are: e- e+, mu- mu+, tau- tau+\n";
  cout << "Quarks and antiquarks are: u u~, d d~, c c~, s s~, t t~, b b~\n";
  cout << "Electronic, muonic and tauonic neutrinos are respectively: ve, vu, vt\n";
  cout << "Spin 1 bosons are: a, g, w+, w-, z\n";
  cout << "Higgs bosson is: h\n";
  cout << "The states are written as: particle1 particle2 particle3 ...\n";
  cout << "Set an output folder for a process typing 'output folder_name' without quotes\n";
  cout << "To generate and compile a process type 'generate state1 > state2' with spaces and without quotes\n";
  cout << "To launch a process type 'launch folder_name' without quotes\n";
  cout << "To launch the madanalysis type 'plot folder_name number_of_the_run' without quotes";
  cout << "Quit the program typing quit\n";
}
