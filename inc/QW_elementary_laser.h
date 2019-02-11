/**
 * @class QW_elementary_laser
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2018/10/25$
 *
 *
 * @brief a Quantum Well (QW) elementary laser is an elementary laser with QW.
 *
 * It's mean that in each band their is energy levels with a probability of finding an Electron
 * With the Fabrice Philippe Formula (see @a Electron_presence) it's possible to calculate this probaility
 * knowing the electron number in Conduction Band (CB) thes probability are contained in the @a electron_presence
 * inititialated by the constructor if they not already exist.
 */

#ifndef QW_ELEMENTARY_LASER_H
#define QW_ELEMENTARY_LASER_H

#include "Elementary_laser.h"
#include "Electron_presence.h"
#include <string>
#include <map>


/**
  * class QW_elementary_laser
  * 
  */

class QW_elementary_laser : public Elementary_laser
{
public:
   /************************/
   /*Constructor/Destructor*/
   /************************/

  /**
   * @brief QW_elementary_laser Constructor that overrite the construcotr of @a Elementary_laser
   * @param pumping_local_rate represent the portion of the total pump
 * that occur in this @a Elementary_laser (for exemple by a gaussian distribution).
   * @param q exponential_temperature represent the temperature at exponential form q=exp(-E/(KbT)), E is the energy lvl between two lvl
   * @param laser_levels table that contain all laser level for each mode
   *
   * @param electron_presence_map map that contain for each @a exponential_temperature a @a Electron_presence pointer
   * if the map contain already the @a Electron_presence conresponding to the @a exponential_temperature
   * this will be associated to the @a electron_presence, if no a new entry will be initialisate by @a Electron_presence constructor
   *
   */
   QW_elementary_laser (double const pumping_local_rate, double const q,
                        const unsigned int *laser_levels, std::map<double,Electron_presence*> &electron_presence_map);

   virtual ~QW_elementary_laser ();

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
   * that can be shared between some @a QW_elementary_laser
   */
   Electron_presence *electron_presence;

};// end of class Electron_presence

#endif // QW_ELEMENTARY_LASER_H
