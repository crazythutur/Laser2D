/**
 * @class Elementary_laser
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2018/10/26$
 *
 *
 * @brief Reprensent the elementary laser that will put in the lactice of the @a 2D_laser
 *
 * This object is virtual and can be deffined as a Quantum Dot (DQ) laser or a Quantum Well (QW) laser.
 * Each  @a Elementary_laser is composed by  :
 *
 * - the @a pumping_local_rate that represent the portion of the total pump
 * that occur in this @a Elementary_laser (for exemple by a gaussian distribution).
 *
 * - the @a temperature represent the temperature in the laser
 *
 * - the @a neighboring_lasers regroup all neigbooring laser in direction @a UP, @a RIGHT, @a DOWN and @a LEFT
 */

#include "../inc/Elementary_laser.h"

//=====================================================================================================================================

/************************/
/*Constructor/Destructor*/
/************************/

Elementary_laser::Elementary_laser (double const pumping_local_rate, double const temperature) {

   for(int i=0 ; i < DIRECTION_NUMBER ; i++ )
   {
      this->neighboring_lasers[i]=nullptr;
   }

   this->pumping_local_rate=pumping_local_rate;
   this->temperature=temperature;

}//Elementary_laser::Elementary_laser

Elementary_laser::~Elementary_laser () { }





