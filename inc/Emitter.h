/**
 * @class Emitter
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2018/10/26$
 *
 *
 * @brief Reprensent the Emitter that will put in the lactice of the @a 2D_laser
 *
 * This object is virtual and can be deffined as a Quantum Dot (DQ) laser or a Quantum Well (QW) laser.
 * Each  @a Emitter is composed by  :
 *
 * - the @a laser_num represent the number of the laser in the @a Laser_2D
 *
 * - the @a pumping_local_rate that represent the portion of the total pump
 * that occur in this @a Emitter (for exemple by a gaussian distribution).
 *
 * - the @a temperature represent the temperature in the laser
 *
 * - the @a cavity_coupling define the coupling optical between the emiter and the cavity for each mode
 *
 * - the @a neighboring_lasers regroup all neigbooring laser in direction @a UP, @a RIGHT, @a DOWN and @a LEFT
 */

#ifndef EMITTER_H
#define EMITTER_H

#include <math.h>

class Emitter
{
public:

    /********************/
    /*Public enumeration*/
    /********************/

    /**
     * @brief The direction enum represent the 4 directions for the @a neighboring_lasers
     * it can be use for exemple with neighboring_lasers[UP]
     */
    enum direction{UP, RIGHT, DOWN, LEFT, DIRECTION_NUMBER};

   /************************/
   /*Constructor/Destructor*/
   /************************/

  Emitter ();

  /**
   * @brief Emitter Constructor to initialise the private parameters
   *
   * @param pumping_local_rate represent the portion of the total pump
   * that occur in this @a Emitter (for exemple by a gaussian distribution).
   * @param temperature represent the temperature at non exponential form
   *
   */
   Emitter (double const pumping_local_rate, double const temperature, const unsigned int number_of_modes);

   virtual ~Emitter ();

   /*****************/
   /*Getters/Setters*/
   /*****************/

   /**
    * @return the number of the laser in the @a Laser2D
    */
   unsigned int getEmitter_num() const
   {
       return emitter_num;
   }

   /**
    * @return the pumping_local_rate
    */
   double getPumping_local_rate() const
   {
       return pumping_local_rate;
   }

   /**
    * @return temperature non exponential for electrons tranfers
    */
   double getTemperature() const
   {
      return temperature;
   }

   /**
    * @return the coupling optical between the emiter and the cavity
    */
   double getCavity_coupling(const int mode) const
   {
      return cavity_coupling[mode];
   }

   /**
    * @return the coupling optical between the emiter and the cavity
    */
   double* getCavity_coupling() const
   {
      return cavity_coupling;
   }


   /**
    * @param value direction of the neigboring emitter using @a direction enum
    * @return the pointer of the neigbooring emitter
    */
   Emitter getNeighboring_emitter (direction const value)   const{
      return *neighboring_emitters[value];
   }

   /**
    * @param value direction of the neigboring emitter without using if @a direction enum
    * @return the pointer of the neigbooring emitter
    */
   Emitter *getNeighboring_emitter (int const value)   const{
      return neighboring_emitters[value];
   }

   /**
    * @brief getMetropolis_criteria return the event accetation probability of electron transfert
    *  following Metropolis criteria see function Metropolis_criteria
    * @param direction of the electron transfert
    * @return
    */
   double getMetropolis_criteria(int const direction) const{
       return metropolis_critera_table[direction];
   }



//=====================================================================================================================================
   /**
    * @brief setLaser_num set the number of the laser at given value
    * @param value new number of the laser
    */
   void setLaser_num(unsigned int const value)
   {
      emitter_num = value;
   }

   void setCavity_coupling( unsigned int const mode, double const value)
   {
      this->cavity_coupling[mode] = value;
   }



//============================================================================================================================

   /******************/
   /*Public functions*/
   /******************/

   /**
    * @brief init_neighboring_emitter set the new_Emitter at emitter_direction,
    * the reverse association is done too
    * @param new_Emitter the neighbooring laser to add
    * @param emitter_direction the @a direction to add
    */
   void init_neighboring_emitter(Emitter *new_Emitter, Emitter::direction emitter_direction);



//============================================================================================================================

private :
   /*********************/
   /*Privates attributes*/
   /*********************/

   /**
     * @brief emitter_num represent the number of the laser in the @a Laser_2D
     */
    unsigned int emitter_num;

    /**
     * @brief pumping_local_rate : Local pumping rate deffined by a portion of the total pump, for exemple by gaussian distribution.
     */
    double pumping_local_rate;

    /**
     * @brief temperature represent the temperature in the @a Emitter
     */
    double temperature;

    /**
     * @brief cavity_coupling define the coupling optical between the emiter and the cavity for each mode
     */
    double *cavity_coupling;

    /**
     * @brief neighboring_emitters regroup all neigbooring laser in direction @a UP, @a RIGHT, @a DOWN and @a LEFT
     */
    Emitter *neighboring_emitters [DIRECTION_NUMBER] ;

    /**
     * @brief Metropolis_critera_table contain the allowed probability of an electronic transfert for each directions
     * folowing the Metropolis criteria a acceptation with a probability exp(-DU/(k_B T)) these probabilities are intialised in
     *
     * cf. [1] J. L. Guisado, F. Jiménez-Morales, and J. M. Guerra,
     * “Cellular automaton model for the simulation of laser dynamics,” Phys. Rev. E, vol. 67, no. 6, p. 066708, 2003.
     */
    double metropolis_critera_table[DIRECTION_NUMBER];

//============================================================================================================================

    /*******************/
    /*Private functions*/
    /*******************/

    static double metropolis_criteria(double origin_temperature, double destination_temperature);


//============================================================================================================================
};// end of class Emitter

#endif // EMITTER_H
