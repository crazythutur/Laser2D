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
   const unsigned int energies_levels=1501;

   Electron_presence::initialize_size(mode_number,energies_levels);

   const unsigned int lasing_level[mode_number]={748,752};//{energies_levels/2};

   int photons[mode_number];
   int electrons[laser_number];

   double cavity_escape_rates[mode_number];
   double local_pump[laser_number];
   double temperature[laser_number];
   std::fill_n(local_pump, laser_number, 210);
   std::fill_n(temperature, laser_number, 300);
   std::fill_n(cavity_escape_rates, mode_number, 0.6);
   cavity_escape_rates[0]=0.624;
   cavity_escape_rates[1]=0.576;



   //std::fill_n(photons, mode_number, 0);
   photons[0]=336;
   photons[1]=364;

   std::fill_n(electrons, laser_number, energies_levels/2);



   map<double,Electron_presence*> occ_map;

   Laser_2D::Laser_init_parameters parameters = {mode_number, laser_number, 1, 1, cavity_escape_rates,local_pump,temperature};
   Laser_2D *laser_2D= new Laser_2D(parameters,lasing_level,1,occ_map);

   cout << "mode number " << laser_2D->getMode_number()<< " laser number " << laser_2D->getElementary_laser_number() << endl;

   QW_elementary_laser *qw =  (QW_elementary_laser*)laser_2D->getElementary_laser(0);

   cout << "electron presence "<<qw->getElectron_presence()->getEnergy_level_number() << endl;

   Laser_transition_structure *trans_struct = new Laser_transition_structure(laser_2D);

   if(trans_struct->getSet()->Cardinal() < LONG_MAX)
   {
       cout << "int max : " << LONG_MAX << "\tcardinal : "
            << trans_struct->getSet()->Cardinal() <<endl;
   }
   else
   {
       cerr << "error 2 Bg 4 u !!!!" <<endl;
   }


   MarkovChain* chain = new MarkovChain(trans_struct);

   DiscreteDistribution* initial = trans_struct->initial_state(photons, electrons);

   chain->set_init_distribution(initial);

   int      time            = 250000;
   bool  	stats           = false;
   bool  	traj            = true;
   bool  	withIncrements  = false;
   bool  	trace           = false;
   bool     my_print_traj   = false;
   bool     analyse   = true;

   //For print Electron presence to debug
   //cout << *qw->getElectron_presence();

   //for test matrix
   //cout << "error number"  << trans_struct->test_Encoding() <<endl;

   cout << "begin of simulation" << endl;
   SimulationResult *result;
   if(analyse)
   {
     result =  chain->SimulateChainCT_AllOpt(time,stats,traj,withIncrements,trace,CACHE_NONE);
   }

   cout << "end   of simulation" << endl;


   if(my_print_traj)
   {
        string const nomFichier("../traj.csv");

        ofstream monFlux(nomFichier.c_str());

        if(monFlux)
         trans_struct->print_trajectory(monFlux, result);
        else
            cerr << "ERREUR: Impossible d'ouvrir le fichier." << endl;

     }

    if(analyse)
    {
        Laser_transition_structure::Analyse_tool tool;
        tool = trans_struct->analyse_trajectory(result,10000);
        cout << " Average values " << endl;
        double tot_ave_photon = 0;
        double tot_ave_electron = 0;
        for (unsigned int mode=0; mode < mode_number ; mode++)
        {
            cout << "m" << mode+1 << " = " << tool.ave_photon[mode] << endl;
            cout << "var(m" << mode+1 << ") = " << tool.var_photon[mode] << endl;

            tot_ave_photon += tool.ave_photon[mode];
        }
        cout << "mtot = " << tot_ave_photon << endl;
        cout << "var(mtot) = " << tool.var_total_photon << endl;

        for(unsigned int laser_num = 0 ; laser_num < laser_number ; laser_num ++)
        {
            cout << "n" << laser_num+1 << " = " << tool.ave_CB_electron[laser_num] << endl;
            tot_ave_electron += tool.ave_CB_electron[laser_num];
            cout << "var(n" << laser_num+1 << ") = " << tool.var_CB_electron[laser_num] << endl;
        }
        cout << "ntot = " << tot_ave_electron << endl;
        cout << "var(ntot) = " << tool.var_total_CB_electron << endl;
    }




     //For print matrix to debug
     //trans_struct->print_matrix("../matrix.txt");
    int testphoton[2] ={50,50};
    int testphoton2[2] ={49,50};
    int testphoton3[2] ={50,49};
    int testelecton[1] ={750};
    int testelecton2[1]= {751};


    int teststate =  trans_struct->myGetIndex(testphoton,testelecton);
    int teststate2 =  trans_struct->myGetIndex(testphoton2,testelecton);
    int teststate3 =  trans_struct->myGetIndex(testphoton,testelecton2);



    cout <<trans_struct->getEntry(teststate,teststate2) <<endl;
    cout <<trans_struct->getEntry(teststate,teststate3) <<endl;


     occ_map.clear();
     delete laser_2D;

     //delete trans_struct;
     delete initial;
     delete chain;
     return 0;
  }
