/**
 * @class 2D_laser
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2018/10/29$
 *
 *
 * @brief a 2 dimmmentions (2d) laser represent a grind of elementary laser used previously[1].
 *
 * These @a Elementary_laser are putted in a square mesh. The electrons can be tranfert between
 * 2 @a Elementary_laser, with a rate that depend of the number of electron, the temperature
 * and a constant parameter that is proportional of the ratio of the velocity of the electrons
 * and the distance between 2 @a Elementary_laser.
 *
 * @a mode_number number of mode in the laser
 *
 * @a pumps_phases represent the number of phase in pumping, if 1 this correspond to no phase and so a poissonian pumping
 * the variance of the pumping is deffined by the ratio @a pump_rate / @a pumps_phases more you had phase more the pump
 * corespond to a quiet.
 *
 * @a beta proportion of photon emited in the laser mode(s)
 *
 * @a cavity_escape_rate optical cavity escape rate for each modes
 *
 * @a laser_table array of a number @a elementary_laser_number of @a Elementary_laser
 *
 *
 *
 * [1] : Vallet, A., Chusseau, L., Philippe, F., & Jean-Marie, A. (2019).
 * Markov model of quantum fluctuations at the transition to lasing of semiconductor nanolasers.
 * Physica E: Low-dimensional Systems and Nanostructures, 105, 97-104.
 */


#include "../inc/Laser_2D.h"

//=====================================================================================================================================

/************************/
/*Constructor/Destructor*/
/************************/

Laser_2D::Laser_2D (Laser_init_parameters const parameters) {

   this->mode_number             = parameters.mode_number;
   this->elementary_laser_number = parameters.elementary_laser_number;
   this->pumps_phases            = parameters.pumps_phases;
   this->beta                    = parameters.beta;
   this->cavity_escape_rate      = parameters.cavity_escape_rate;
}//Laser_2D::Laser_2D (Laser_init_parameters const parameters)

//=====================================================================================================================================

Laser_2D::Laser_2D (Laser_init_parameters const parameters, unsigned int const *laser_levels, double const energy_level_splitting,
                    std::map<double,Electron_presence*> &electron_presence_map) : Laser_2D(parameters)
{
   this->type=QW;

   double q;

   for(unsigned int laser_num=0 ; laser_num < this->mode_number ; laser_num++)
   {
      /*
       * we use (e/1000 / k_B) = 11.60451812 with e elementary charge and k_B boltzman constant
       * this permit to transform q=exp(-Epsilon/(Kb*T)) to q= exp(-11.60451812 * energy_level_splitting / temperature)
       * with temperature in °kelvin and energy_level_splitting in meV, KB = 1.380649e-23, e=1.602176634e-19
       */
      q=exp(-11.60451812 * energy_level_splitting / parameters.temperature[laser_num]);

      this->laser_table.push_back(QW_elementary_laser(parameters.local_pump[laser_num], q ,laser_levels, electron_presence_map));

   }
   organize_neighborhood();

}//Laser_2D::Laser_2D (Laser_init_parameters, unsigned int* , double, std::map<double,Electron_presence*> &)

//=====================================================================================================================================

Laser_2D::~Laser_2D ()
{}//Laser_2D::~Laser_2D (

//=====================================================================================================================================

/*******************/
/*Private functions*/
/*******************/

void  Laser_2D::organize_neighborhood(){

   unsigned const int height  = find_height(this->elementary_laser_number);
   unsigned const int width = this->elementary_laser_number/height;

   unsigned int position;

   //we make two loops for no care about the edge conditions
   /** First loop is for fill all lateral neigboor**/
   for(unsigned int it_y = 0 ; it_y < height ; it_y++ ){
      for(unsigned int it_x = 1 ; it_x < width ; it_x++ )
      {
         position = it_y * width + it_x;
         laser_table[position].setNeighboring_laser(laser_table[position - 1], Elementary_laser::LEFT);
      }
   }
   /** Second loop is for fill all vertical neigboor**/
   for(unsigned int it_y = 1 ; it_y < height ; it_y++ ){
      for(unsigned int it_x = 0 ; it_x < width ; it_x++ )
      {
         position = it_y * width + it_x;
         laser_table[position].setNeighboring_laser(laser_table[position - width], Elementary_laser::UP);
      }
   }
}// end of Laser_2D::organize_neighborhood()

//=====================================================================================================================================
//End of Class Laser_2D
//=====================================================================================================================================

/******************/
/*Other functions */
/******************/

unsigned int find_height(unsigned int area)
{
    /// we check first the sqrt of the area like a possible height
    unsigned int height = static_cast <unsigned int> (std::floor(std::sqrt(area)));

    /// while ths height is not a possible height for the rectangle we decrease it
    while(area % height > 0) height--;

    return height;
}//unsigned int find_height(unsigned int area)