# LHC-Event-Generator

## Description 

An event generator for LHC processes without any physics of the standard model, only the laws of conservation (energy, momentum, helicity, charge) and it's based on monte carlo methods to generate de events. However, the generator doesn't allow to treat protons or any other hadron, just the fundamental particles of the standard model.

## The cards

The program have a few cards to control some aspects of the program and parameters of the runs. The first one is the param_card in which is placed the path of the folder where the data and the analysis will be saved (first line) and the path to the top folder of madanalysis. The second one is the run_card which is responsible for the parameters of the runs, in the first two lines are the beams energies and in the third lines is the number of events to be generated.

## How to use

To start de program you must launch the executable called ev_gen, then a command line or shell is opened. To see how the particles are defined, how to write states and other useful information type "help" without quotes and to leave the program type "quit". Now, to calculate a process you have to open a folder in which the information and the results of the process will be stored, to do that type "output folder_name" with folder_name, obviously, being the name of the folder to be created. Next, type "generate state_1 > state_2" with all the spaces included and the states are written as "particle_1 particle_2 particle_3 ...". The initial states is always a two particles state and the final is a n>1 state. 

To generate the events of the process type "launch folder_name", this will create a folder inside "folder_name" with the name "runn" with n being the number of times that process has been launched. Finally, to plot the results with madanalysis type "plot folder_name number_of_the_run" and when it finishes type "exit" to leave madanalysis and return to the event generator. The results of the analysis are stored in folder_name/runn/ with the name ANALYSYS_0.

