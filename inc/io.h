/**
 * @class io
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

#ifndef IO_H
#define IO_H


/*====================================================================== */
/* includes ============================================================ */
/*====================================================================== */

#include "Laser_2D.h"
#include "rate_array.h"
#include "state.h"
#include "analyse_tool.h"
#include "Markov_Chain.h"
#include "../lib/argtable3.h"


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_math.h>
#include <iomanip>
#include <ctime>

/*====================================================================== */
/* macros ============================================================== */
/*====================================================================== */

#define MAX_MODES 3
#define MAX_EMITTERS 25
#define MAX_SEED 20

using namespace std;

//============================================================================================================================


/**
 * @brief The IO class regroup the input output function and the structure to initialise the software
 */
class IO
{
public:   

    /********************/
    /*Public Enum/Struc */
    /********************/

   /**
    * @brief The CSV_LOOP_BREAKER enum the number values of all CSV breaker
    */
   enum CSV_LOOP_BREAKER{
       TOTAL_BREAKER=0,
       HISTO_BREAKER,
       SD_BREAKER,
       G2_BREAKER,
       EVENT_BREAKER,
       MAX_BREAKER

   };

   /**
    * @brief The Input_parameters struct that regroup all the initialisation parameter from the command line
    */
   struct Input_parameters {

        Input_parameters();
        /**
         * @brief Input_parameters initialise all tables for the input parameters
         * @param number_of_modes
         * @param number_of_emitters
         */
        Input_parameters(const unsigned int number_of_modes, const unsigned int number_of_emitters);
        ~Input_parameters();

        unsigned int number_of_modes;               ///< number_of_modes number of mode in the laser

        unsigned int number_of_emitters;            ///< Emitter_number is the number of @a Emitter

        unsigned int pump_phases;                   ///< number of phases in pumping

        unsigned int SD_point_numbers;              ///< number of point to calculate the Spectral density

        unsigned int energies_levels;               ///< number of energy level

        unsigned int *lasing_levels;                ///< list of the lasing levels

        unsigned int *initial_photons_number;       ///< number of the photons for each modes at the initial states

        unsigned int *initial_CBelectrons_number;   ///< number of the electron in conduction band for each emitters at the initial states

        unsigned int *initial_VBelectrons_number;   ///< number of the electron in conduction band for each emitters at the initial states


        unsigned int* seed;                         ///< all seed for the simulation


        double time;                                ///< simulation time

        double stationnary_time;                    ///< time without analyse the trajectory

        double electrical_coupling;                 ///< electrical coupling between emitters

        double beta;                                ///< proportion of the photon emit in the coherent mode (default is 1, all spontaneous photon are in the mode)

        double * pump;                              ///< local pump for each emitters

        double *temerature;                         ///< local temperature for each emitters in Kelvin

        double * cavity_escape_rate;                ///< inverse of photon life time for each emitters

        double *cavity_coupling;                    ///< coupling





        string output_file;                         ///< where to write all the analyssys data



        bool verbose;                               ///< verbose output

        bool g2_flag;                               ///< calculate the second order correlation

        bool histo_flag;                            ///< calculate and display in ouput the histogramms of photons for each modes

        bool sd_flag;                               ///< calculate the spectral density

        bool event_flag;                            ///< recor all the trajectory

        bool help_flag;                             ///< show help menu
   };//struct input_parameters
   typedef struct Input_parameters Input_parameters;///< struture def




   /************************/
   /*Constructor/Destructor*/
   /************************/



//============================================================================================================================





//============================================================================================================================


   /*******************/
   /*Public functions */
   /*******************/

   /**
    * @brief write_command the input command butwith some reductions
    * @param flux input flux
    * @param parameters
    * @param analyse_init_params
    * @param laser
    * @param input_parameters
    * @return the input flux updated
    */
   static ostream & write_command(ostream &flux, Laser_2D::Laser_init_parameters *parameters,
                                  Analyse_init_params *analyse_init_params, Laser_2D *laser,
                                  Input_parameters *input_parameters);

   static std::ostream& write_csv(std::ostream& flux, Analyse_tool *tool);

   static Input_parameters *read_argtable(int argc, char *argv[]);

   static long unsigned  init_seed(unsigned int *input_seeds);



//============================================================================================================================

   /*************************/
   /*Pure Virtuals function */
   /*************************/




//============================================================================================================================


private:

   /*********************/
   /*Privates attributes*/
   /*********************/





//============================================================================================================================

  /********************/
  /*Privates function */
  /********************/

   template <typename T>
   /**
    * @brief write_table write multiple value for the command shape looking ( --parameters={1,2,3} )
    * @param flux input flux
    * @param table table of values
    * @param size number of values to plot
    * @return
    */
   static ostream & write_table(ostream &flux, T table[], unsigned int size);



};// end of class IO
//============================================================================================================================

#endif // IO_H
