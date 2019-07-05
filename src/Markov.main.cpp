#include "../inc/Emitter.h"
#include "../inc/Electron_presence.h"
#include "../inc/QW_emitter.h"
#include "../inc/Laser_2D.h"
#include "../inc/Markov_Chain.h"


#include <string>
#include <map>
#include<cstdlib>
#include <fstream>
#include <climits>

using namespace std;


/**
 * @brief main Testing software for occupancies and QW lasers
 * @return
 */
int main()
{
   srand(5);
   const unsigned int laser_number=1;
   const unsigned int mode_number =2;
   const unsigned int energies_levels=10000;

   Electron_presence::initialize_size(mode_number,energies_levels);

   const unsigned int lasing_level[mode_number]={4999,5002};//{energies_levels/2};

   unsigned int photons[mode_number];
   unsigned int CB_electrons[laser_number];
   unsigned int VB_electrons[laser_number];

   double cavity_escape_rates[mode_number];
   double local_pump[laser_number];
   double temperature[laser_number];
   std::fill_n(local_pump, laser_number, 202.3);
   std::fill_n(temperature, laser_number, 300);
   std::fill_n(cavity_escape_rates, mode_number, 0.6);
   cavity_escape_rates[0]=0.6075;
   cavity_escape_rates[1]=0.57;



   std::fill_n(photons, mode_number, 333);
   //photons[0]=336;
   //photons[1]=364;

   std::fill_n(CB_electrons, laser_number, 9000);
   std::fill_n(VB_electrons, laser_number, 1000);


   map<double,Electron_presence*> occ_map;

   Laser_2D::Laser_init_parameters parameters = {mode_number, laser_number, 1, 1, 1, cavity_escape_rates,local_pump,temperature};
   Laser_2D *laser_2D= new Laser_2D(parameters,lasing_level,1,occ_map);


   Markov_Chain *chain = new Markov_Chain(laser_2D,1);
   State *intialState = new State(mode_number, laser_number,photons, CB_electrons,VB_electrons );

   cout << "simualtion begin"<< endl;
   State *end_state = chain->simulate(10000, intialState);

   chain->getAnalyse_tool()->analyse_flag = true;
    cout << "initialisation ended, now it time to turn on  CPU." << endl;
   chain->simulate(10000, end_state);

   print_statistic(cout, chain->getAnalyse_tool());

   cout << chain->getAnalyse_tool()->ave_photon[0] << endl;
   cout << chain->getAnalyse_tool()->ave_CB_electron[0] << endl;

    occ_map.clear();

    delete intialState;

    delete chain;
    delete laser_2D;

    return 0;
  }
