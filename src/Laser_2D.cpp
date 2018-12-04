#include "../inc/Laser_2D.h"

// Constructors/Destructors
//  

Laser_2D::Laser_2D (Laser_init_parameters const parameters) {

   this->mode_number             = parameters.mode_number;
   this->elementary_laser_number = parameters.elementary_laser_number;
   this->pumps_phases            = parameters.pumps_phases;
   this->beta                    = parameters.beta;
   this->cavity_escape_rate      = parameters.cavity_escape_rate;
}


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

}

Laser_2D::~Laser_2D ()
{}

unsigned int gcd(unsigned int a,unsigned int b)
{
    return b ?  gcd(b,a%b) : a;
}


void  Laser_2D::organize_neighborhood(){

   unsigned const int width  = gcd(this->elementary_laser_number,1);
   unsigned const int height = this->elementary_laser_number/width;

   unsigned int position;

   //we make two loops for no care about the edge conditions
   for(unsigned int it_y = 0 ; it_y < height ; it_y++ ){
      for(unsigned int it_x = 1 ; it_x < width ; it_x++ )
      {
         position = it_y * width + it_x;
         laser_table[position].setNeighboring_laser(laser_table[position - 1], Elementary_laser::LEFT);
      }
   }

   for(unsigned int it_y = 1 ; it_y < height ; it_y++ ){
      for(unsigned int it_x = 0 ; it_x < width ; it_x++ )
      {
         position = it_y * width + it_x;
         laser_table[position].setNeighboring_laser(laser_table[position - width], Elementary_laser::DOWN);
      }
   }
}// end of Laser_2D::organize_neighborhood()




