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
 * - the @a exponential_temperature represent the temperature at exponential form q=exp(-E/(KbT)), E is the energy lvl between two lvl
 *
 * - the @a temperature is used for the electron tranferts rates
 *
 * - the @a neighboring_lasers regroup all neigbooring laser in direction @a UP, @a RIGHT, @a DOWN and @a LEFT
 */

#include "../inc/Elementary_laser.h"

//=====================================================================================================================================

/************************/
/*Constructor/Destructor*/
/************************/

Elementary_laser::Elementary_laser (double const pumping_local_rate, const double temperature, double const q) {

   for(int i=0 ; i < DIRECTION_NUMBER ; i++ )
   {
      this->neighboring_lasers[i]=nullptr;
   }

   this->temperature=temperature;
   this->pumping_local_rate=pumping_local_rate;
   this->exponential_temperature=q;

}//Elementary_laser::Elementary_laser

Elementary_laser::~Elementary_laser () { }





