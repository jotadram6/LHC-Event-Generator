#include <map>
#include <iostream>
#include <vector>
#include <array>

using namespace std;

//Particles definition
struct particle
{
  int name;            //Particle label in the PDG format for lhe file
  float mass;             //Particle mass
  float charge;           //Electric charge
  float spin;             //Spin
  double energy;           //Energy
  double mom[3];           //Mommentum
  float angle[2];        //Angles theta and phi      
  };

//Masses in GeV, electric charges in e- units
//Charged leptons
particle ep{-11, 5e-4, +1, 0.5};
particle em{11,5e-4, -1, 0.5};

particle mup{-13, 0.105, +1, 0.5};
particle mum{13, 0.105, -1, 0.5};

particle taup{-15, 1.777, +1, 0.5};
particle taum{15, 1.777, -1, 0.5};

//Higgs
particle h{25, 125, 0, 0};

//Quarks
particle u{2, 3e-2, 2/3., 0.5};
particle ub{-2,3e-2, -2/3., 0.5};

particle d{1, 7e-2, -1/3., 0.5};
particle db{-1, 7e-2, 1/3., 0.5};

particle s{3, 0.12, -1/3., 0.5};
particle sb{-3, 0.12, 1/3., 0.5};

particle c{4, 1.2, 2/3., 0.5};
particle cb{-4, 1.2, -2/3., 0.5};

particle t{6, 174, 2/3., 0.5};
particle tb{-6, 174, -2/3., 0.5};

particle b{5, 4.3, -1/3., 0.5};
particle bb{-5, 4.3, 1/3., 0.5};

//Neutrinos
particle ve{12, 0, 0, 0.5};
particle vu{14, 0, 0, 0.5};
particle vt{16, 0, 0, 0.5};

//Vectorial bosons
particle a{22, 0, 0, 1}; //Photon
particle g{21, 0, 0, 1}; //Gluon

particle wp{24, 0.0804, +1, 1}; //W+
particle wm{-24, 0.0804, -1, 1}; //W-
particle z{23, 0.0919, 0, 1};   //Z

//Function to map a string name into a variable name
particle str_2_ptcle(string p_name){
  
  map<string,particle> p_map;

  p_map["e+"] = ep;
  p_map["e-"] = em;
  p_map["mu+"] = mup;
  p_map["mu-"] = mum;
  p_map["tau+"] = taup;
  p_map["tau-"] = taum;
  
  p_map["h"] = h;
  
  p_map["u"] = u;
  p_map["u~"] = ub;
  p_map["d"] = d;
  p_map["d~"] = db;
  p_map["c"] = c;
  p_map["c~"] = cb;
  p_map["s"] = s;
  p_map["s~"] = sb;
  p_map["t"] = t;
  p_map["t~"] = tb;
  p_map["b"] = b;
  p_map["b~"] = bb;
  
  p_map["a"] = a;
  p_map["g"] = g;
  
  p_map["w-"] = wm;
  p_map["w+"] = wp;
  p_map["z"] = z;

  p_map["ve"] = ve;
  p_map["vu"] = vu;
  p_map["vt"] = vt;

  if(p_map.find(p_name) == p_map.end()){
    cout << "The particle " << p_name <<" is not defined" << endl;
    cout << "Process stopped" << endl;
    exit(0);}
  
  return p_map[p_name];
}
