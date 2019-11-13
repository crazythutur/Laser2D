/**
 * @class Markov_Chain
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 02/07/2019$
 *
 *
 * @brief extention of of the Marmote @a TransitionStructure For Laser simulation
 *
 * it was composed princialyt by  two @a MarmoteBox :
 *
 * - @a photon        :represent the number of Photon for each mode
 * - @a CB_electrons  :reprensent the electron number in conduction band for all @a Emitter
 *
 * @a MarmoteBox is a @a MarmoteSet with the state space rectangular
 * @a MarmoteSet permit to reprensent a system with only a one dimention with the attribuation of a number for each state
 *
 * then for simulate we need only one @a MarmoteSet so we use a carthesian product of the 2 previous @a MarmoteBox
 * this value in stocked on @a set
 *
 * then for being simulated By MarmoteCore the function TransDistrib is overwrited all the events are inside
 * For using this object it should be used in the object @a MarkovChain then initialisated by the function initial_state
 * Before being simulated.
 *
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
 */

#ifndef MARKOV_CHAIN_H
#define MARKOV_CHAIN_H


/*====================================================================== */
/* includes ============================================================ */
/*====================================================================== */

#include "Laser_2D.h"
#include "rate_array.h"
#include "state.h"
#include "analyse_tool.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_math.h>
#include <iomanip>

/*====================================================================== */
/* macros ============================================================== */
/*====================================================================== */

#define RANDOM_NUMBERS 2


using namespace std;

//============================================================================================================================

class Markov_Chain
{
public:   

    /********************/
    /*Public Enum/Struc */
    /********************/

    /**
     * @brief Group that represent all information to describe a transition
     *
     * the name can be  : O for cavity output, follow by a number corresponding the mode.
     * E for Cohérent emission, A for absorbtion,  and U for uncoherent emission followed by
     * the mode number and emitter number.
     * J for pump follow by emitter number
     * C and V for conduction of valence eletron transfert follow by the origin emitter number
     * and destination emitter number.
     */
    struct Transition {

       string name; ///< name of the transition areviated see function description
       double rate; ///< rate of the transition
       State state; ///< next state after the transition



    };//struct Transition

    typedef struct Transition Transition;///< struture def



   /************************/
   /*Constructor/Destructor*/
   /************************/

    /**
    * @brief Markov_Chain create a transition structure based on the @param laser
    * @param laser laser that contain all information for calculate the generator infinitesimal generator
    * @param seed  of the trajectory
    */
   Markov_Chain (Laser_2D *laser, unsigned long seed);


   /**
    * @brief Markov_Chain create a transition structure based on the @param laser and the @param parameters given
    * @param laser laser that contain all information for calculate the generator infinitesimal generator
    * @param seed  of the trajectory
    * @param parameters parameters for the analysis
    */
   Markov_Chain (Laser_2D *laser, unsigned long seed, Analyse_init_params parameters);


   virtual ~Markov_Chain ();

//============================================================================================================================

   /*****************/
   /*Getters/Setters*/
   /*****************/

   /**
    * @return the laser of the chain
    */
   Laser_2D *getLaser() const
   {
       return this->laser;
   }

   /**
    * @return the analysis tool
    */
   Analyse_tool *getAnalyse_tool() const
   {
       return this->analyse_tool;
   }



//============================================================================================================================


   /*******************/
   /*Public functions */
   /*******************/



   /**
    * @brief print_event print the event
    * @param flux output flux
    * @param transition transition of the event to print
    * @param date date of the evet
    *
    * @return the output with the added string
    */
   std::ostream& print_event(std::ostream& flux, const Transition *transition, const double date);

   /**
    * @brief print_header print the header of the trace of a chain for the @a print_event
    * @param flux outpt flux
    * @return the output with the added string
    */
   std::ostream& print_header(std::ostream& flux);

   /**
    * @brief print_state print the state decoded
    * @param flux flux to put the output
    * @param state the state to write
    * @return the flux
    */
   std::ostream& print_state(std::ostream& flux, const State *state);



   State * simulate(double max_time, State *initial_state);

   /**
    * @brief write_csv the csv file representing the chain
    * @param flux to put the output
    * @return the flux
    */
   std::ostream& write_csv(std::ostream& flux);


//============================================================================================================================

   /*************************/
   /*Pure Virtuals function */
   /*************************/




//============================================================================================================================


private:

   /*********************/
   /*Privates attributes*/
   /*********************/


   /**
    * @brief laser is used for calculates all transitions rates. The object contain all informations for calculates this rates
    */
   Laser_2D *laser;

   /**
    * @brief analyse_tool regroup all data for the anlasis of the chain
    */
   Analyse_tool *analyse_tool;

   /**
    * @brief rng_generator for all random values
    */
   gsl_rng *rng_generator[RANDOM_NUMBERS];



//============================================================================================================================

  /********************/
  /*Privates function */
  /********************/

   /**
    * @brief rates_sum make the sum of all rates from a @param transition_structure
    * @return the sum of all rates
    */
   double rates_sum( std::vector<Transition> *transition_structure);

   /**
    * @brief init_state copy the previous state to new state
    * @param photon_prev_state number of photons for each mode in the previous state
    * @param photon_next_state number of photons for each mode in the next state
    * @param CBelec_prev_state number of electron in CB for each emitter in the previous state
    * @param CBelec_next_state number of electron in CB for each emitter in the next state
    * @param VBelec_prev_state number of electron in VB for each emitter in the previous state
    * @param VBelec_next_state number of electron in VB for each emitter in the next state
    */
   void init_state(int *photon_prev_state, int *photon_next_state,
                   int *CBelec_prev_state, int *CBelec_next_state,
                   int *VBelec_prev_state, int *VBelec_next_state );


   /**
    * @brief transition_probabilities return the transition probability from a state @param current_state
    * neccessary to implement getEntry and Trans_distrib functions from @param transitions
    *
    * The function possed all events
    * the sheme is simple we test if the event is possible
    * then make a copy of the state
    * calculate the rate
    * add the application of the rate for the new state
    * add the rate and the new rates for the return value
    */
   std::vector<Markov_Chain::Transition> *transition_structure(
           std::vector<Markov_Chain::Transition> * transitions, State  * const current_state);
   void init_random(const long unsigned int seed);

   void trajectory_analysis(const State *current_state, const Markov_Chain::Transition *transition,
                            const double waiting_time, const double current_time);


   void update_stat(const State *state, const double waiting_time);

   void post_calcul_analyse(const double end_time);

   void update_SD(int mode, double instant_time);


   /**
    * @brief calcul_histogramme update the histogramme analysis
    * @param state state input for the update
    * @param waiting_time time during the state will not change
    */
   void calcul_histogramme(const State *state, const double waiting_time);


   /**
    * @brief correlation_update update the correlations
    * @param state state input for the update
    * @param waiting_time  time during the state will not change
    */
   void correlation_update(const State *state, const double waiting_time);



};// end of class Markov_Chain
//============================================================================================================================

#endif // MARKOV_CHAIN_H
