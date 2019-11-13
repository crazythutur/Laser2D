
/**
 * main file
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 18/07/2019$
 *
 * This file is part of Laser2D.
 *
 *   Laser2D is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Laser2D is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Laser2D.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


#include "../inc/Emitter.h"
#include "../inc/Electron_presence.h"
#include "../inc/QW_emitter.h"
#include "../inc/Laser_2D.h"
#include "../inc/Markov_Chain.h"
#include "../inc/io.h"

#include <iostream>
#include <string>
#include <map>
#include<cstdlib>
#include <fstream>
#include <climits>


/**
 * @brief main Testing software for occupancies and QW lasers
 * @return
 */
int main(int argc, char* argv[])
{
    IO::Input_parameters *input_parameters;
    try {
        input_parameters = IO::read_argtable(argc, argv);
    }
    catch(int help_flag)
    {

        if(help_flag)
        {
            exit(EXIT_SUCCESS);
        }else
        {
            fprintf(stderr,"error parsing \n");
            exit(EXIT_FAILURE);
        }
    }
    catch(...) {
        std::cerr << "error: unknown exceptions" << std::endl;
        return 1;
      }


    /***********************/
    /* Argtable parsing    */
    /***********************/
    /** argtable is reading and all values are parsing for the simualtion   **/


    long unsigned seed = IO::init_seed(input_parameters->seed);



    /****************************/
    /* initialisation chaine    */
    /****************************/

    Electron_presence::initialize_size(input_parameters->number_of_modes, input_parameters->energies_levels);

    map<double,Electron_presence*> occ_map;

    Laser_2D::Laser_init_parameters parameters;
    parameters.mode_number              = input_parameters->number_of_modes;
    parameters.emitter_number           = input_parameters->number_of_emitters;
    parameters.pumps_phases             = input_parameters->pump_phases;
    parameters.beta                     = input_parameters->beta;
    parameters.electrical_coupling      = input_parameters->electrical_coupling;
    parameters.cavity_escape_rate       = input_parameters->cavity_escape_rate;
    parameters.local_pump               = input_parameters->pump;
    parameters.temperature              = input_parameters->temerature;
    parameters.number_of_photonic_phases= 0;
    parameters.weigth_of_photonic_phases= 0;
    parameters.lasing_level             = input_parameters->lasing_levels;


    Laser_2D *laser_2D= new Laser_2D(parameters,1,occ_map);
    unsigned int *intial_VBelectrons_number = new unsigned int [parameters.emitter_number];

    //laser_2D->gaussian_coupling(FWHM, nullptr);
    //laser_2D->uniform_coupling(FWHM);
    //double assymetrie =1;
    for(unsigned int emitter = 0 ; emitter < parameters.emitter_number ; emitter++ )
    {
        for(unsigned int mode = 0 ; mode < parameters.mode_number ; mode++ )
        {
            laser_2D->getEmitter(emitter)->setCavity_coupling(
                        mode ,input_parameters->cavity_coupling[emitter*parameters.mode_number + mode]);

        }
        intial_VBelectrons_number[emitter] = input_parameters->energies_levels - input_parameters->initial_CBelectrons_number[emitter];

    }

    Analyse_init_params analyse_init_params;


    analyse_init_params.SD_points_number=input_parameters->SD_point_numbers;
    analyse_init_params.correlation_interval_duration=0;
    analyse_init_params.correlation_max =0;
    analyse_init_params.end_time        = input_parameters->time;
    analyse_init_params.stationnary_time= input_parameters->stationnary_time;
    analyse_init_params.event_flag      = input_parameters->event_flag;
    analyse_init_params.g2_flag         = input_parameters->g2_flag;
    analyse_init_params.histo_flag      = input_parameters->histo_flag;
    analyse_init_params.sd_flag         = input_parameters->sd_flag;
    analyse_init_params.verbose_flag    = input_parameters->verbose;
    analyse_init_params.seed = seed;





    Markov_Chain *chain = new Markov_Chain(laser_2D, seed, analyse_init_params);

    //chain->getAnalyse_tool()->verbose_flag = true;

    //chain->getAnalyse_tool()->event_flag = false;
    State *intialState = new State(parameters.mode_number, parameters.emitter_number,
                                   input_parameters->initial_photons_number, input_parameters->initial_CBelectrons_number,
                                    intial_VBelectrons_number);

    /****************************/
    /* Lancement simulation     */
    /****************************/


    if(analyse_init_params.verbose_flag)
    {
    cout << "simualtion begin"<< endl;
    cout << *intialState << endl;
    }
    State *end_state = chain->simulate(analyse_init_params.stationnary_time, intialState);

    if(analyse_init_params.verbose_flag)

    cout << *end_state << endl;

    chain->getAnalyse_tool()->analyse_flag = true;

    if(analyse_init_params.verbose_flag)
    cout << "initialisation ended, now it time to turn on  CPU." << endl;

    chain->simulate(analyse_init_params.end_time, end_state);

    if(analyse_init_params.verbose_flag)
    {
        cout << *end_state << endl;
        print_statistic(cout, chain->getAnalyse_tool());
    }
    ofstream flux;
    streambuf * buf;
    if(input_parameters->output_file.empty())
    {
       buf = std::cout.rdbuf();

    }else
    {
        flux.open(input_parameters->output_file.c_str());
        buf=flux.rdbuf();
    }
    std::ostream out(buf);
    if(out)
    {
        IO::write_command(out, &parameters, &analyse_init_params, laser_2D, input_parameters);
        IO::write_csv(out, chain->getAnalyse_tool());

    }else
        cerr << "error wrinting file " <<endl;

    /********************/
    /*  delete objects  */
    /********************/

    for (map<double, Electron_presence*>::iterator it = occ_map.begin(); it != occ_map.end(); it++)
    {
        delete it->second;
    }
    occ_map.clear();

    delete intialState;
    delete input_parameters;
    delete[] intial_VBelectrons_number;
    delete chain;
    delete laser_2D;

    return 0;
  }
