#include "../inc/Elementary_laser.h"
#include "../inc/Electron_presence.h"
#include "../inc/QW_elementary_laser.h"
#include "../inc/Laser_2D.h"
#include "../inc/LaserTransitionStructure.h"

#include "Distribution/dirac_distribution.h"
#include <string>
#include <map>
#include<cstdlib>
#include <fstream>

using namespace std;


/**
 * @brief main Testing software for occupancies and QW lasers
 * @return
 */
int main()
{
   srand(0);
   const unsigned int laser_number=1;
   const unsigned int mode_number =1;
   const unsigned int energies_levels=801;

   Electron_presence::initialize_size(mode_number,energies_levels);

   const unsigned int lasing_level[mode_number]={energies_levels/2};

   int photons[mode_number];
   int electrons[laser_number];

   double cavity_escape_rates[mode_number];
   double local_pump[laser_number];
   double temperature[laser_number];
   std::fill_n(local_pump, laser_number, 35);
   std::fill_n(temperature, laser_number, 300);
   std::fill_n(cavity_escape_rates, mode_number, 0.6);


   std::fill_n(photons, mode_number, 58);
   std::fill_n(electrons, laser_number, lasing_level[0]);//fill until lasing level



   map<double,Electron_presence*> occ_map;

   Laser_2D::Laser_init_parameters parameters = {mode_number, laser_number, 1, 1, cavity_escape_rates,local_pump,temperature};
   Laser_2D *laser_2D= new Laser_2D(parameters,lasing_level,1,occ_map);

   cout << "mode number " << laser_2D->getMode_number()<<endl;

   QW_elementary_laser *qw =  (QW_elementary_laser*)laser_2D->getElementary_laser(0);

   cout << "electron presence "<<qw->getElectron_presence()->getEnergy_level_number() << endl;

   Laser_transition_structure *trans_struct = new Laser_transition_structure(laser_2D);



   MarkovChain* chain = new MarkovChain(trans_struct);

   DiscreteDistribution* initial = trans_struct->initial_state(photons, electrons);

   chain->set_init_distribution(initial);

   int      time            = 10000;
   bool  	stats           = false;
   bool  	traj            = true;
   bool  	withIncrements  = false;
   bool  	trace           = false;

   SimulationResult *result =  chain->SimulateChainCT(time,stats,traj,withIncrements,trace);

   string const nomFichier("../traj.csv");

    ofstream monFlux(nomFichier.c_str());

    if(monFlux)

    {
     trans_struct->print_trajectory(monFlux, result);
    }else
    {
      cerr << "ERREUR: Impossible d'ouvrir le fichier." << endl;
    }

   //For print Electron presence to debug
   //cout << *qw->getElectron_presence();

   //For print matrix to debug
   //trans_struct->print_matrix("../matrix.txt");

   cout << "end of simulation" << endl;

   //for test matrix
   //cout << "error number"  << trans_struct->test_Encoding() <<endl;

   occ_map.clear();
   delete laser_2D;

   //delete trans_struct;
   delete initial;
   delete chain;
   return 0;
}
