#include "../inc/Elementary_laser.h"
#include "../inc/Electron_presence.h"
#include "../inc/QW_elementary_laser.h"
#include "../inc/Laser_2D.h"
#include "../inc/LaserTransitionStructure.h"

#include "Distribution/dirac_distribution.h"
#include <string>
#include <map>


using namespace std;


/**
 * @brief main Testing software for occupancies and QW lasers
 * @return
 */
int main()
{
   const unsigned int laser_number=2;
   const unsigned int mode_number =2;
   const unsigned int lasing_level[2]={5,6};

   int photons[mode_number];
   int electrons[laser_number];

   double cavity_escape_rates[2]={0.9,0.9};
   double local_pump[laser_number];
   double temperature[laser_number];
   std::fill_n(local_pump, laser_number, 200);
   std::fill_n(temperature, laser_number, 300);

   std::fill_n(photons, mode_number, 1);
   std::fill_n(electrons, laser_number, 6);

   Electron_presence::initialize_size(mode_number,10);

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

   int time=10;
   bool  	stats           = false;
   bool  	traj            = true;
   bool  	withIncrements  = false;
   bool  	trace           = true;

   SimulationResult *result =  chain->SimulateChainCT(time,stats,traj,withIncrements,trace);




   trans_struct->print_trajectory(cout, result);

   cout << "transitions " << trans_struct->getEntry(1440,1441) << " and " << trans_struct->getEntry(1441,1452) << endl;

   cout << "end of simulation" << endl;


   occ_map.clear();
   delete laser_2D;

   //delete trans_struct;
   delete initial;
   delete chain;
   return 0;
}
