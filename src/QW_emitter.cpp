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
 * It's mean that in each band their is energy levels with a probability of finding an Electron
 * With the Fabrice Philippe Formula (see @a Electron_presence) it's possible to calculate this probaility
 * knowing the electron number in Conduction Band (CB) thes probability are contained in the @a electron_presence
 * inititialated by the constructor if they not already exist.
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

#include "../inc/QW_emitter.h"


using namespace std;

/************************/
/*Constructor/Destructor*/
/************************/

QW_emitter::QW_emitter(const double pumping_local_rate, const double q, const unsigned int number_of_modes,
                       const unsigned int *laser_levels, map<double, Electron_presence*> &electron_presence_map)
    : Emitter(pumping_local_rate, q, number_of_modes)
{
   if(!electron_presence_map.count(q))
   {
      electron_presence_map[q]=new Electron_presence(laser_levels, q);
   }
   this->electron_presence =  electron_presence_map[q];

}//QW_Emitter::QW_Emitter


QW_emitter::~QW_emitter () {
}








