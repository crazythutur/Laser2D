/**
 * @class Electron_presence
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2018/10/25$
 *
 *
 * @brief The Electron_presence class represent the probability of finding an electron at different level for a @a QW_elementary_laser
 * for different number of electron in Conduction Band (CB)
 * this probability is deffined for each lasing mode by  by @a probability[CB_Electron_number][mode]
 * or for pumping level by @a probability[CB_Electron_number][PUMPING_LEVEL]
 * the @a mode_number
 * @a energy_level_number  represent the maximum electron capacity of the CB that correspond to the energy level number
 */


#ifndef ELECTRON_PRESENCE_H
#define ELECTRON_PRESENCE_H

#include <iostream>
#include <cmath>

class Electron_presence
{
 public:
   /************************/
   /*Constructor/Destructor*/
   /************************/

   Electron_presence(unsigned int const *laser_levels, double const q);

   ~Electron_presence();

   /*****************/
   /*Getters/Setters*/
   /*****************/

   unsigned int getMode_number() const
   {
      return mode_number;
   }//unsigned int Electron_presence::getMode_number


   unsigned int getEnergy_level_number() const
   {
      return energy_level_number;
   }//unsigned int Electron_presence::getEnergy_level_number


   double **getProbability() const
   {
      return probability;
   }//double **Electron_presence::getProbability

   /******************/
   /*Public functions*/
   /******************/

   /**
   * @brief affiche print the electron presence tab
   * @param flux
   */
   void affiche(std::ostream &flux) const;


   /**
    * @brief init_size initialise the size of the @a probability tab and so the static parameters if the class
    * @param mode_number number of lasing modes
    * @param energy_level_number represent the maximum electron capacity of the CB that correspond to the energy level number
    */
   static void initialize_size(unsigned int const mode_number, unsigned int const energy_level_number);

   /*********************/
   /*Public attributes*/
   /*********************/
   /**
    * @brief PUMPING_LEVEL permit to obtain the probability of having an electron with @a probability[CB_Electron_number][PUMPING_LEVEL]
    */
   static unsigned int PUMPING_LEVEL;


//============================================================================================================================

private:
   /*********************/
   /*Privates attributes*/
   /*********************/
   /**
   * @brief probability probability of finding an electron at different level for a @a QW_elementary_laser
   * for different number of electron in Conduction Band (CB)
   * this probability is deffined for each lasing mode by  by @a probability[CB_Electron_number][mode]
   * or for pumping level by @a probability[CB_Electron_number][PUMPING_LEVEL]
   */
   double **probability;
   /**
    * @brief mode_number the number of mode in the laser
    */
   static unsigned int mode_number;
   /**
    * @brief energy_level_number represent the maximum electron capacity of the CB that correspond to the energy level number
    */
   static unsigned int energy_level_number;


   /**
    * @brief initialise_occuancies inititialise the electron presence probability for each lasing mode and puming level
    * for modes by by @a probability[][mode]and for the pumping lvl accecible by @a probability[][PUMPING_LEVEL]
    * @param laser_levels
    * @param q
    */

   /*******************/
   /*Private functions*/
   /*******************/
   /**
   * @brief initialise_occuancies permit to obtain the probability to find an electron in @a probability
   *
   * at laser lvl  with electron in BC done for each mode
   * We use the fabrice formula that is iterative formula we calculate the probability table in two times
   * because of the error propagation.
   * the general formule for an occupancy (at stationary state) at level k is
   * \f$ occ_k(N+1) = (q^k (1-occ_k(N)) \frac{1-q^{N+1}}{q^N-q^B}\f$
   *
   * see Vallet, A., Chusseau, L., Philippe, F., & Jean-Marie, A. (2019).
   * Markov model of quantum fluctuations at the transition to lasing of semiconductor nanolasers.
   *  Physica E: Low-dimensional Systems and Nanostructures, 105, 97-104.
   *
   * we can use the symetry of the band \f$occ_k(N)=1-occ_{B-1-k}(B-N)\f$
   *
   * to obtain
   * \f$ occ_k(B-N-2) = 1-q^{B-1-k}occ_k(B-N-1)\frac{1-q^{N+1}}{q^N-q^B}\f$
   *
   * B is abreviation for @a energy_level_number
   * k is abreviation for @a laser_levels in CB, NB : their is the same number of laser level in CB than modes numbers
   * N is abreviation for the energy level we use this incrementation variable for loop all the energy level
   *
   * all result are contain in @a probability
   * first dimention for witch number of electrons in CB, and the second dimention is for each laser mode and the pumping level.
   * @param laser_levels table that contain all laser level for each mode
   * @param q represent the temperature at exponential form q=exp(-E/(KbT)), E is the energy lvl between two lvl
                   in our unity E=1meV, Kb is Boltzmann constant 1,38064852.10^-23 and T temperature in °K
    */
   void initialise_occuancies(unsigned int const *laser_levels, double const q) ;


};// end of class Electron_presence

/**
 * @brief operator << surcharge of the opperator << to be able of read the the @a probability table
 * @param flux input stram
 * @param electron_presence the object to read te table
 * @return
 */
std::ostream& operator<<(std::ostream& flux, const Electron_presence& electron_presence);

#endif // ELECTRON_PRESENCE_H
