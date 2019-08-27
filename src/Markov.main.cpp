#include "../inc/Emitter.h"
#include "../inc/Electron_presence.h"
#include "../inc/QW_emitter.h"
#include "../inc/Laser_2D.h"
#include "../inc/Markov_Chain.h"
#include "../inc/io.h"


#include <string>
#include <map>
#include<cstdlib>
#include <fstream>
#include <climits>



/**
 * @brief main Testing software for occupancies and QW lasers
 * @return
 */
int main()
{
   //srand(5);
    const unsigned int laser_number=1;
    const unsigned int mode_number =1;
    const unsigned int energies_levels=800;
    int charge_itiale = 800;

    const unsigned int lasing_level[mode_number]={400};//{energies_levels/2};


    unsigned int photons[mode_number];
    unsigned int CB_electrons[laser_number];
    unsigned int VB_electrons[laser_number];

    double cavity_escape_rates[mode_number];
    double local_pump[laser_number];
    double temperature[laser_number];
    double cavity_coupling[laser_number];

    double FWHM[mode_number] ;
    FWHM[0] =1; //0.31;
    //FWHM[1] =1;// 0.30;

    std::fill_n(local_pump, laser_number,35); //202.3/laser_number);
    std::fill_n(temperature, laser_number, 293.16);
    std::fill_n(cavity_escape_rates, mode_number, 0.6);
    std::fill_n(cavity_coupling, mode_number, 1);
    //local_pump[1]=0;

    /*cavity_escape_rates[0]=0.6225;
    cavity_escape_rates[1]=0.57;
    cavity_escape_rates[0]=0.6;
    cavity_escape_rates[1]=0.541;
    cout << cavity_escape_rates[0] << "and "<<  cavity_escape_rates[1] <<endl;
    */

    /****************************/
    /* initialisation chaine    */
    /****************************/

    Electron_presence::initialize_size(mode_number,energies_levels);

    std::fill_n(photons, mode_number, 333);
    //photons[0]=336;
    //photons[1]=364;

    std::fill_n(CB_electrons, laser_number, charge_itiale);
    std::fill_n(VB_electrons, laser_number, energies_levels - charge_itiale);

    map<double,Electron_presence*> occ_map;

    Laser_2D::Laser_init_parameters parameters;
    parameters.mode_number= mode_number;
    parameters.emitter_number = laser_number;
    parameters.pumps_phases = 1;
    parameters.beta = 1;
    parameters.electrical_coupling = 0;
    parameters.cavity_escape_rate = cavity_escape_rates;
    parameters.local_pump = local_pump;
    parameters.temperature = temperature;

    double time = 5000;

    Laser_2D *laser_2D= new Laser_2D(parameters,lasing_level,1,occ_map);

    //laser_2D->gaussian_coupling(FWHM, nullptr);
    laser_2D->uniform_coupling(FWHM);
    double assymetrie =1;

    if(laser_number > 1 && true){
        //laser_2D->getEmitter(1)->setCavity_coupling(0,1-assymetrie);
        //laser_2D->getEmitter(1)->setCavity_coupling(1,assymetrie);

        //laser_2D->getEmitter(0)->setCavity_coupling(0,assymetrie);
        //laser_2D->getEmitter(0)->setCavity_coupling(1,1-assymetrie);

        laser_2D->getEmitter(1)->setCavity_coupling(0,0);
        laser_2D->getEmitter(1)->setCavity_coupling(1,0);

        //CB_electrons[1] =0;
        //VB_electrons[1] = 1500;
    }

    Analyse_init_params analyse_init_params;

    analyse_init_params.SD_points_number=2;
    analyse_init_params.correlation_interval_duration=0;
    analyse_init_params.correlation_max =0;
    analyse_init_params.end_time        = time;
    analyse_init_params.event_flag      = false;
    analyse_init_params.g2_flag         = false;
    analyse_init_params.histo_flag      = false;
    analyse_init_params.sd_flag         = true;
    analyse_init_params.verbose_flag    = true;



    Markov_Chain *chain = new Markov_Chain(laser_2D,1, analyse_init_params);

    //chain->getAnalyse_tool()->verbose_flag = true;

    //chain->getAnalyse_tool()->event_flag = false;
    State *intialState = new State(mode_number, laser_number,photons, CB_electrons,VB_electrons );

    /****************************/
    /* Lancement simulation     */
    /****************************/



    cout << "simualtion begin"<< endl;
    cout << *intialState << endl;
    State *end_state = chain->simulate(time, intialState);
    cout << *end_state << endl;

    chain->getAnalyse_tool()->analyse_flag = true;
    cout << "initialisation ended, now it time to turn on  CPU." << endl;
    chain->simulate(time, end_state);
    cout << *end_state << endl;
    print_statistic(cout, chain->getAnalyse_tool());


    /********************/
    /*  delete objects  */
    /********************/

    occ_map.clear();

    delete intialState;

    delete chain;
    delete laser_2D;

    return 0;
  }
