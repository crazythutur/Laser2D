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

#include "../inc/QW_elementary_laser.h"


using namespace std;

/************************/
/*Constructor/Destructor*/
/************************/

QW_elementary_laser::QW_elementary_laser(const double pumping_local_rate, const double temperature, const double q,
                                         const unsigned int *laser_levels, map<double, Electron_presence*> &electron_presence_map) :
   Elementary_laser(pumping_local_rate, temperature,  q)
{
   if(!electron_presence_map.count(q))
   {      
      electron_presence_map[q]=new Electron_presence(laser_levels, q);
   }
   this->electron_presence =  electron_presence_map[q];

}//QW_elementary_laser::QW_elementary_laser


QW_elementary_laser::~QW_elementary_laser () {
}








