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
   const unsigned int lasing_level[2]={399,401};

   int photons[mode_number];
   int electrons[laser_number];

   double cavity_escape_rates[2]={0.5,0.5};
   double local_pump[laser_number];
   double temperature[laser_number];
   std::fill_n(local_pump, laser_number, 10);
   std::fill_n(temperature, laser_number, 300);

   std::fill_n(photons, mode_number, 1);
   std::fill_n(electrons, laser_number, 400);

   Electron_presence::initialize_size(mode_number,800);

   map<double,Electron_presence*> occ_map;

   Laser_2D::Laser_init_parameters parameters = {mode_number, laser_number, 1, 1, cavity_escape_rates,local_pump,temperature};
   Laser_2D *laser_2D= new Laser_2D(parameters,lasing_level,1,occ_map);

   cout << "mode number " << laser_2D->getMode_number()<<endl;


   Laser_transition_structure *trans_struct = new Laser_transition_structure(laser_2D);



   MarkovChain* chain = new MarkovChain(trans_struct);

   DiscreteDistribution* initial = trans_struct->initial_state(photons, electrons);

   chain->set_init_distribution(initial);

   int time=1;
   bool  	stats           = true;
   bool  	traj            = true;
   bool  	withIncrements  = true;
   bool  	trace           = true;

   chain->SimulateChainCT(time,stats,traj,withIncrements,trace);

   cout << "end of simulation" << endl;


   occ_map.clear();
   delete laser_2D;

   delete trans_struct;
   delete initial;
   delete chain;
   return 0;
}
