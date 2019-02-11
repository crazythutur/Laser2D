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
 * this probability is deffined for each lasing mode by  by @a getLasing_lvl_prob([)CB_Electron_number, mode)
 * or for pumping level by @a pumping_lvl_prob[CB_Electron_number]
 * the @a mode_number
 */


#ifndef ELECTRON_PRESENCE_H
#define ELECTRON_PRESENCE_H

#include <iostream>
#include <cmath>

//============================================================================================================================

class Electron_presence
{
 public:
   /************************/
   /*Constructor/Destructor*/
   /************************/

    /**
    * @brief Electron_presence Constructor for the table of probability to find a electron
    * @param laser_levels lasing levels, need to be the same size as mode_number
    * @param exponential_temperature represent the temperature at exponential form q=exp(-E/(KbT)), E is the energy lvl between two lvl
                   in our unity E=1meV, Kb is Boltzmann constant 1,38064852.10^-23 and T temperature in °K
    */
   Electron_presence(unsigned int const *laser_levels, double const exponential_temperature);

   ~Electron_presence();

   /*****************/
   /*Getters/Setters*/
   /*****************/

   /**
    * @return the numbers of mode in the laser
    */
   static unsigned int getMode_number()
   {
      return mode_number;
   }//unsigned int Electron_presence::getMode_number


   /**
    * @return the the number of energy levels
    */
   static unsigned int getEnergy_level_number()
   {
      return energy_level_number;
   }//unsigned int Electron_presence::getEnergy_level_number


   /**
    * @brief getLasing_lvl_prob get the probability of finding an electron at pumping level
    * @param electron_number in the conduction band
    * @param mode of the laser
    * @return probaility corresponding at lasing level to parrameters
    */
   double getLasing_lvl_prob( const unsigned int electron_number, const unsigned int mode) const
   {
      return lasing_lvl_prob[electron_number * mode_number  + mode ];
   }//double Electron_presence::getLasing_lvl_prob

   /**
    * @brief getPumping_lvl_prob get the probability of finding an electron at pumping level
    * @param electron_number in the conduction band
    * @return the probability corresponding to the electron_number
    */
   double getPumping_lvl_prob( const unsigned int electron_number) const
   {
      return pumping_lvl_prob[electron_number];
   }//double Electron_presence::getLasing_lvl_prob

   //============================================================================================================================

   /**
    * @brief setLasing_lvl_prob set the lasing level probability of finding an electron
    * @param electron_number electron number in the CB
    * @param mode lasing mode to consider
    * @param value corresponding value
    */
   void setLasing_lvl_prob(const unsigned int electron_number, const unsigned int mode,const double value)
   {
      lasing_lvl_prob[electron_number * mode_number + mode ] = value;
   }//void Electron_presence::setLasing_lvl_prob

   /**
    * @brief setPumping_lvl_prob set the pumping level probability of finding an electron
    * @param electron_number electron number in the CB
    * @param value corresponding value
    */
   void setPumping_lvl_prob(const unsigned int electron_number ,const double value)
   {
      pumping_lvl_prob[electron_number] = value;
   }//void Electron_presence::setLasing_lvl_prob

//============================================================================================================================

   /******************/
   /*Public functions*/
   /******************/   

   /**
    * @brief init_size initialise the size of the @a lasing_lvl_prob and pumping_lvl_prob tab and so the static parameters if the class
    * @param mode_number number of lasing modes
    * @param energy_level_number represent the maximum electron capacity of the CB that correspond to the energy level number
    */
   static void initialize_size(unsigned int const mode_number, unsigned int const energy_level_number);

//============================================================================================================================

private:
   /*********************/
   /*Privates attributes*/
   /*********************/
   /**
   * @brief lasing_level probability of finding an electron at different level for a @a QW_elementary_laser
   * for different number of electron in Conduction Band (CB)
   * this probability is deffined for each lasing mode by vector defined by lasing_level[electron_number * mode_number + mode ]
   * accesible by @a getLasing_lvl_prob(electron_number, mode). This table is reduce in   dimmention table for optimsation and simplicity
   */
   double *lasing_lvl_prob;

   /**
    * @brief pumping_level probability of finding an electron at pumping level for differents in CB
    */
   double *pumping_lvl_prob;

   /**
    * @brief mode_number the number of mode in the laser
    */
   static unsigned int mode_number;
   /**
    * @brief energy_level_number represent the maximum electron capacity of the CB that correspond to the energy level number
    */
   static unsigned int energy_level_number;

//============================================================================================================================

   /*******************/
   /*Private functions*/
   /*******************/
   /**
   * @brief initialise_occuancies permit to obtain the probability to find an electron
   * in lasing level @a lasing_lvl_prob and pumping level @a pumping_lvl_prob
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
   * For the pumping level probability it's a simpler formula :
   * \f$ occ_B(N) = \frac{q^B}{q^N}\frac{1-q^N}{1-q^B}\f$
   *
   *
   * B is abreviation for @a energy_level_number
   * k is abreviation for @a laser_levels in CB, NB : their is the same number of laser level in CB than modes numbers
   * N is abreviation for the energy level we use this incrementation variable for loop all the energy level
   *
   * all result are contain in @a lasing_lvl_prob for lasing level and @a pumping_lvl_prob for pumping level
   * first dimention for witch number of electrons in CB, and the second dimention is for each laser mode and the pumping level.
   * @param laser_levels table that contain all laser levels for each mode
   * @param exponential_temperature represent the temperature at exponential form q=exp(-E/(KbT)), E is the energy lvl between two lvl
                   in our unity E=1meV, Kb is Boltzmann constant 1,38064852.10^-23 and T temperature in °K
    */
   void initialise_occuancies(unsigned int const *laser_levels, double const exponential_temperature) ;

   /**
    * @brief operator << surcharge of the opperator << to be able of read the the @a probability table
    * @param flux input stream
    * @param electron_presence the object to read te table
    * @return
    */
   friend std::ostream& operator<<(std::ostream& flux, const Electron_presence& electron_presence);

};// end of class Electron_presence



#endif // ELECTRON_PRESENCE_H
