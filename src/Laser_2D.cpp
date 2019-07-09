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
 * @brief a 2 dimmmentions (2d) laser represent a grind of emitter used previously[1].
 *
 * These @a Emitter are putted in a square mesh. The electrons can be tranfert between
 * 2 @a Emitter, with a rate that depend of the number of electron, the temperature
 * and a constant parameter that is proportional of the ratio of the velocity of the electrons
 * and the distance between 2 @a Emitter.
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
 * @a laser_table array of a number @a Emitter_number of @a Emitter
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
   this->emitter_number          = parameters.emitter_number;
   this->pumps_phases            = parameters.pumps_phases;
   this->beta                    = parameters.beta;
   this->electical_coupling      = parameters.electrical_coupling;
   this->cavity_escape_rate      = parameters.cavity_escape_rate;
}//Laser_2D::Laser_2D (Laser_init_parameters const parameters)

//=====================================================================================================================================

Laser_2D::Laser_2D (Laser_init_parameters const parameters, unsigned int const *laser_levels,
                    double const energy_level_splitting, std::map<double,Electron_presence*> &electron_presence_map)
    : Laser_2D(parameters)
{
   this->type=QW;

   double q;

   for(unsigned int laser_num=0 ; laser_num < this->emitter_number ; laser_num++)
   {
       QW_emitter *emitter;

      /*
       * we use (e/1000 / k_B) = 11.60451812 with e elementary charge and k_B boltzman constant
       * this permit to transform q=exp(-Epsilon/(Kb*T)) to q= exp(-11.60451812 * energy_level_splitting / temperature)
       * with temperature in °kelvin and energy_level_splitting in meV, KB = 1.380649e-23, e=1.602176634e-19
       */
      q=exp(-11.60451812 * energy_level_splitting / parameters.temperature[laser_num]);

      emitter = new QW_emitter(parameters.local_pump[laser_num], q , this->mode_number,
                               laser_levels, electron_presence_map);
      emitter->setLaser_num(laser_num);

      this->laser_table.push_back(emitter);
   }
   organize_neighborhood();

}//Laser_2D::Laser_2D (Laser_init_parameters, unsigned int* , double, std::map<double,Electron_presence*> &)

//=====================================================================================================================================

Laser_2D::~Laser_2D ()
{
    this->laser_table.clear();
}//Laser_2D::~Laser_2D (

//=====================================================================================================================================


void Laser_2D::gaussian_coupling(double *FWHM_x, double *FWHM_y)
{

    double x0 = 0.5;
    double y0 = 0.5;

    unsigned int position;
    double value_x, x;
    double value_y, y;

    if(width > 1)
    {
        for(unsigned int mode=0 ; mode < this->getMode_number() ; mode ++)
        {
            for(unsigned int it_x = 0 ; it_x < width ; it_x++ )
            {

                x= it_x * 1.0 / (width - 1.0);
                value_x = 1/sqrt(2*M_PI*FWHM_x[mode]*FWHM_x[mode])*exp(-(x-x0)*(x-x0)/(2*FWHM_x[mode]*FWHM_x[mode]));
                if(height>1)
                {
                    for(unsigned int it_y = 0 ; it_y < height ; it_y++ )
                    {
                        position = it_y * width + it_x;
                        y = it_y / (height - 1 );

                        value_y = 1/sqrt(2*M_PI*FWHM_y[mode]*FWHM_y[mode])*exp(-(y-y0)*(y-y0)/(2*FWHM_y[mode]*FWHM_y[mode]));
                        laser_table[position]->setCavity_coupling(mode, value_x * value_y);

                    }
                }
                else
                {
                    position =   it_x;
                    laser_table[position]->setCavity_coupling(mode, value_x);

                }
            }
        }


    }
    else
    {
        for(unsigned int mode=0 ; mode < this->getMode_number() ; mode ++)
        {
            laser_table[0]->setCavity_coupling(mode, 1.0);
        }
    }



}
//=====================================================================================================================================

/*******************/
/*Private functions*/
/*******************/

void  Laser_2D::organize_neighborhood(){

   unsigned const int height  = find_height(this->emitter_number);
   unsigned const int width = this->emitter_number/height;

   this->height = height;
   this->width = width;

   unsigned int position;
   if(this->emitter_number > 1)
   {
       //we make two loops for no care about the edge conditions
       /** First loop is for fill all lateral neigboor**/
       for(unsigned int it_y = 0 ; it_y < height ; it_y++ ){
          for(unsigned int it_x = 1 ; it_x < width ; it_x++ )
          {
             position = it_y * width + it_x;
             laser_table[position]->init_neighboring_emitter(laser_table[position - 1], Emitter::LEFT);
          }
       }
       /** Second loop is for fill all vertical neigboor**/
       for(unsigned int it_y = 1 ; it_y < height ; it_y++ ){
          for(unsigned int it_x = 0 ; it_x < width ; it_x++ )
          {
             position = it_y * width + it_x;
             laser_table[position]->init_neighboring_emitter(laser_table[position - width], Emitter::UP);
          }
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
