/**
 * @class QW_Emitter
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2018/10/25$
 *
 *
 * @brief a Quantum Well (QW) Emitter is an Emitter with QW.
 *
 * It's mean that in each band their is energy levels with a probability of finding an Electron
 * With the Fabrice Philippe Formula (see @a Electron_presence) it's possible to calculate this probaility
 * knowing the electron number in Conduction Band (CB) thes probability are contained in the @a electron_presence
 * inititialated by the constructor if they not already exist.
 */

#ifndef QW_EMITTER
#define QW_EMITTER

#include "Emitter.h"
#include "Electron_presence.h"
#include <string>
#include <map>


/**
  * class QW_emitter
  * 
  */

class QW_emitter : public Emitter
{
public:
   /************************/
   /*Constructor/Destructor*/
   /************************/

  /**
   * @brief QW_emitter Constructor that overrite the construcotr of @a Emitter
   * @param pumping_local_rate represent the portion of the total pump
 * that occur in this @a Emitter (for exemple by a gaussian distribution).
   * @param q exponential_temperature represent the temperature at exponential form q=exp(-E/(KbT)), E is the energy lvl between two lvl
   *
   * @param laser_levels table that contain all laser level for each mode
   *
   * @param number_of_modes numer of modes in the laser
   *
   * @param electron_presence_map map that contain for each @a exponential_temperature a @a Electron_presence pointer
   * if the map contain already the @a Electron_presence conresponding to the @a exponential_temperature
   * this will be associated to the @a electron_presence, if no a new entry will be initialisate by @a Electron_presence constructor
   *
   */
   QW_emitter (double const pumping_local_rate, double const q, unsigned const int number_of_modes,
               const unsigned int *laser_levels, std::map<double,Electron_presence*> &electron_presence_map);

   virtual ~QW_emitter ();

   /*****************/
   /*Getters/Setters*/
   /*****************/

   /**
    * @return the pointer to @a Electron_presence associated
    */
   Electron_presence *getElectron_presence() const
   {
      return electron_presence;
   }


//============================================================================================================================

private:
   /*********************/
   /*Privates attributes*/
   /*********************/

  /**
   * @brief electron_presence pointer to the probability of finding an electron for each lasing and pumping level
   * that can be shared between some @a QW_Emitter
   */
   Electron_presence *electron_presence;

};// end of class Electron_presence

#endif // QW_EMITTER
