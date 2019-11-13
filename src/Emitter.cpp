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
 * @brief Reprensent the emitter that will put in the lactice of the @a 2D_laser
 *
 * This object is virtual and can be deffined as a Quantum Dot (DQ) laser or a Quantum Well (QW) laser.
 * Each  @a Emitter is composed by  :
 *
 * - the @a pumping_local_rate that represent the portion of the total pump
 * that occur in this @a Emitter (for exemple by a gaussian distribution).
 *
 * - the @a temperature represent the temperature in the laser
 *
 * - the @a neighboring_lasers regroup all neigbooring laser in direction @a UP, @a RIGHT, @a DOWN and @a LEFT
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

#include "../inc/Emitter.h"

//=====================================================================================================================================

/************************/
/*Constructor/Destructor*/
/************************/

Emitter::Emitter (double const pumping_local_rate, double const temperature, const unsigned int number_of_modes) {

   for(int i=0 ; i < Emitter::DIRECTION_NUMBER ; i++ )
   {
      this->neighboring_emitters[i]=nullptr;
      this->metropolis_critera_table[i]=0.0;
   }

   this->pumping_local_rate=pumping_local_rate;
   this->temperature=temperature;
   this->cavity_coupling = new double [number_of_modes];

}//Emitter::Emitter

Emitter::~Emitter () {
    delete [] this->cavity_coupling;
}

//=====================================================================================================================================

/******************/
/*Public functions*/
/******************/

/**
 * @brief init_neighboring_emitter set the new_Emitter at emitter_direction and add the probaility of electron transfert acceptation
 * the reverse association is done too
 * @param new_Emitter the neighbooring emitter to add
 * @param emitter_direction the @a direction to add
 */
void Emitter::init_neighboring_emitter(Emitter *new_Emitter, direction emitter_direction)
{
    const int oposite_direction = (emitter_direction + 2) % DIRECTION_NUMBER;
    this->neighboring_emitters[emitter_direction] = new_Emitter;
    new_Emitter->neighboring_emitters[oposite_direction] = this;//set the other emitter neigbboring in the oposite direction
    this->          metropolis_critera_table[emitter_direction] = metropolis_criteria(this->getTemperature(), new_Emitter->getTemperature());
    new_Emitter->   metropolis_critera_table[oposite_direction] = metropolis_criteria(new_Emitter->getTemperature(), this->getTemperature());

}

//=====================================================================================================================================

/*******************/
/*Private functions*/
/*******************/

/**
 * @brief Metropolis_criteria define a probability a an event to be accepted of exp(-DU/(k_B T))
 *  cf. [1] J. L. Guisado, F. Jiménez-Morales, and J. M. Guerra,
 * “Cellular automaton model for the simulation of laser dynamics,” Phys. Rev. E, vol. 67, no. 6, p. 066708, 2003.
 * @param origin_temperature Temperature of the orgin of the particle
 * @param destination_temperature Temperature to test the acceptation of the event.
 * @return the probability of an event to be accepted given the temperature input
 */
double Emitter::metropolis_criteria(double const origin_temperature, double const destination_temperature)
{
    if(origin_temperature > destination_temperature)
        return 1.0;
    else
    {
        return exp( 1 - destination_temperature / origin_temperature );
    }
}


//=====================================================================================================================================

