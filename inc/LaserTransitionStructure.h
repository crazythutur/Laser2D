/**
 * @class Laser_transition_structure
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2019/01/03$
 *
 *
 * @brief extention of of the Marmote @a TransitionStructure For Laser simulation
 *
 * it was composed princialyt by  two @a MarmoteBox :
 *
 * - @a photon        :represent the number of Photon for each mode
 * - @a CB_electrons  :reprensent the electron number in conduction band for all @a Elementary_laser
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
 */

#ifndef LASER_TRANSITION_STRUCTURE_H
#define LASER_TRANSITION_STRUCTURE_H


/** Safety warper to MARMOTE included for avoid warning messages **/
#ifndef ROOTHEADERS_MARMOTE_INCLUDED
#define ROOTHEADERS_MARMOTE_INCLUDED
#ifdef __GNUC__
#pragma GCC system_header
#endif
#include "markovChain/markov_chain.h"
#include "Distribution/discrete_distribution.h"
#include "transitionStructure/transition_structure.h"
#include "Set/marmote_box.h"
#include "Distribution/dirac_distribution.h"
#endif

//Other includes
#include "Laser_2D.h"
#include "rate_array.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//============================================================================================================================

class Laser_transition_structure : public TransitionStructure
{
public:   

   /************************/
   /*Constructor/Destructor*/
   /************************/

    /**
    * @brief Laser_transition_structure create a transition structure based on the @param laser
    * @param laser laser that contain all information for calculate the generator infinitesimal generator
    */
   Laser_transition_structure (Laser_2D *laser);

   virtual ~Laser_transition_structure ();

//============================================================================================================================


   /******************/
   /*Public function */
   /******************/

   /**
    * @brief myGetIndex permit with the number of photon and eletrons on input to obtain the num of the state
    * @param photons table of all photon for each mode
    * @param electron table of electron in conduction band
    * @return the index of the state defined by the parameters input
    */
   int myGetIndex(int *photons, int *electron);

   /**
    * @brief initial_state return an initial state for the @a Laser_transition_structure
    * @param photon_distr table of all photon for each mode
    * @param CB_electrons_distrof electron in conduction band
    * @return @a DiscreteDistribution that represent a deterministic state of the number of photon and electron gave in input
    */
   DiscreteDistribution *initial_state(int *photon_distr, int *CB_electrons_distr);

   /**
    * @brief decode_laser_state permit to decode the state number @param state_num and put the result table input
    * @param photon array of number of photon of each mode should be initialisated with
    *   a size this->laser->getMode_number()
    * @param CB_electrons array of number of electron in conduction band for each @a Elementary_laser
    *   should be initialisated with a size this->laser->getMode_number()
    * @param state_num number of the event
    */
   void decode_laser_state(int *photon, int *CB_electrons, int state_num);

   /**
    * @brief print_trajectory print the trajectory with a readable format
    * @param flux output flux
    * @param result the result of the @a MarkovChain
    * @return
    */
   std::ostream& print_trajectory(std::ostream& flux, SimulationResult *result);

   /**
    * @brief print_event print the @param event_number state with a proper definition
    * @param flux output flux
    * @param size size of the dates and states
    * @param dates array of all dates of the events
    * @param states array of all states
    * @param event_number number of the event in the trace
    *
    * @return the output with the added string
    */
   std::ostream& print_event(std::ostream& flux, int size, double *dates, int *states, int event_number);

   /**
    * @brief print_header print the header of the trace of a chain for the @a print_event
    * @param flux outpt flux
    * @return the output with the added string
    */
   std::ostream& print_header(std::ostream& flux);

   /**
    * @brief test_Encoding test if the encoding and decoding is reciproque to use for debug
    * @return number of error
    */
   int test_Encoding();

   /**
    * @brief print_matrix print the matrix in the file input in a format to be used for Marmote Prossesing
    * @todo complete format by adding continus the size and the stop marker
    * @param filename name of the file with the extention
    */
   void print_matrix(string filename);

//============================================================================================================================

   /*************************/
   /*Pure Virtuals function */
   /*************************/
   /**
    * @brief TransDistrib function overwrited from @a TransitionStructure
    *
    * This function gave from the index number @param i all transition possibles
    * then the simulation will call it from each state for knowing the next state
    *  without knowing all the infinitesimal generator
    *
    * @return the @a DiscreteDistribution that reprensent all accessible state from state @param i
    */
   virtual DiscreteDistribution* TransDistrib(int i);

   /**
    * @brief getEntry return the rate associated with the transition from state @param i to @param j
    *
    * by convention the rate from i to i is minus the sum of all others rates
    *
    * @return the rate from @param i to @param j
    */
   virtual double getEntry(int i, int j);

   /**
    * All these pure virtual Functions Are Not Implemented
    * @todo To Implement
    */
   virtual bool setEntry(int i, int j, double val);

   virtual int getNbElts(int i);
   virtual int getCol(int i, int k);
   virtual double getEntryByCol(int i, int k);
   virtual double RowSum(int i);
   virtual TransitionStructure* Copy();
   virtual TransitionStructure* Uniformize();
   virtual TransitionStructure* Embed();
   virtual void EvaluateValue(double* v, double* res);

//============================================================================================================================

private:

   /*********************/
   /*Privates attributes*/
   /*********************/

   /**
    * @brief photon_max limit of number of photon this limit is just for simplicity
    * in practice this value should be modify until the probability of having @a photon_max is negligeable
    */
   static const int photon_max = 1000;

   /**
    * @brief electron_dim number of values that electron can be so is the size of a band +1
    * because the band can be full of empty
    */
   unsigned int electron_dim;
   /**
    * @brief n_dims numbers of dimentions for the Markov chain is the sum of @a photon and @a CB_electrons dimentions
    */
   int n_dims;



   /**
    * @brief set represent the Carthesian product of the two Set @a photon and @a CB_electrons
    * @a MarmoteSet permit to reprensent a system with only a one dimention with the attribuation of a number for each state
    * D
    */
   MarmoteSet *set;

   /**
    * @brief photon renpresent the number of Photon for each mode
    * @a MarmoteBox is a @a MarmoteSet with the state space rectangular
    * The state space of @a photon is a table of dimention the mode number of the laser and the photon_max
    */
   MarmoteBox *photon;

   /**
    * @brief CB_electrons reprensent the electron number in conduction band for all @a Elementary_laser
    * @a MarmoteBox is a @a MarmoteSet with the state space rectangular
    * The state space of @a CB_electrons is a table of dimention the Elementary_laser number and the level number of bands
    */
   MarmoteBox *CB_electrons;

   /**
    * @brief laser is used for calculates all transitions rates. The object contain all informations for calculates this rates
    */
   Laser_2D *laser;

//============================================================================================================================

  /********************/
  /*Privates function */
  /********************/

   /**
    * @brief init_state permit copy the @param photon_prev_state and @param elec_prev_state
    *  in @param photon_next_state and @param elec_next_state
    */
   void init_state(int *photon_prev_state, int *photon_next_state, int *elec_prev_state, int *elec_next_state);

   /**
    * @brief print_state print the state decoded
    * @param flux flux to put the output
    * @param state state to write the corresponding staet
    * @return the flux
    */
   std::ostream& print_state(std::ostream& flux, int state);

   /**
    * @brief transition_probabilities return the transition probability from a state @param i
    * neccessary to implement getEntry and Trans_distrib functions from @a Transition_structure
    *
    * The function possed all events
    * the sheme is simple we test if the event is possible
    * then make a copy of the state
    * calculate the rate
    * add the application of the rate for the new state
    * add the rate and the new rates for the return value
    */
   Rate_array *transition_probabilities(int i);



};// end of class Laser_transition_structure
//============================================================================================================================

#endif // LASER_TRANSITION_STRUCTURE_H
