/**
 * @class Electron_presence
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2018/10/25$
 *
 *
 * @brief The Electron_presence class represent the probability of finding an electron at different level for a @a QW_elementary_laser
 * for different number of electron in Conduction Band (CB)
 * this probability is deffined for each lasing mode by  by @a probability[CB_Electron_number][mode]
 * or for pumping level by @a probability[CB_Electron_number][PUMPING_LEVEL]
 * the @a mode_number
 * @a energy_level_number  represent the maximum electron capacity of the CB that correspond to the energy level number
 */

#include "../inc/Electron_presence.h"

using namespace std;

unsigned int Electron_presence::energy_level_number;
unsigned int Electron_presence::mode_number;
unsigned int Electron_presence::PUMPING_LEVEL;


//=====================================================================================================================================

/************************/
/*Constructor/Destructor*/
/************************/

Electron_presence::Electron_presence(unsigned int const *laser_levels, double const q)
{

   this->probability = new double*[Electron_presence::energy_level_number+1];// size declaration number of mode +pumping level

   for(size_t energy_level=0 ; energy_level <= energy_level_number ; energy_level++)//occupancies are deffiened in [0,B]
   {
      this->probability[energy_level] = new double[mode_number + 1];
   }

   this->initialise_occuancies(laser_levels, q);
}//Electron_presence::Electron_presence

//=====================================================================================================================================

Electron_presence::~Electron_presence(){
   //the electron cas be in [0,B] !
   for(size_t energy_level=0 ; energy_level <= Electron_presence::energy_level_number ; energy_level++)
   {
      delete this->probability[energy_level];
   }
   delete this->probability;
}//Electron_presence::~Electron_presence

//=====================================================================================================================================

/******************/
/*Public functions*/
/******************/

void Electron_presence::affiche(ostream& flux) const

{
   flux << "Electron_number\t";
   for(size_t mode=0;mode<Electron_presence::mode_number;mode++){
      flux << "mode " << mode +1 << "\t";
   }
   flux << "pump lvl" << endl ;

   for( size_t level=0; level<=Electron_presence::energy_level_number; level ++)//the electron cas be in [0,B] !
   {
      flux << level <<"\t" ;
      for(size_t mode=0;mode<=Electron_presence::mode_number;mode++)
      {
         flux <<this->probability[level][mode] <<"\t" ;
      }
      flux << endl;
   }
}//void Electron_presence::affiche

//=====================================================================================================================================

void Electron_presence::initialize_size(const unsigned int mode_number, const unsigned int energy_level_number)
{

   Electron_presence::mode_number = mode_number;
   Electron_presence::energy_level_number = energy_level_number;
   Electron_presence::PUMPING_LEVEL = mode_number;
}//void static Electron_presence::initialize_size

//=====================================================================================================================================

ostream& operator<<(ostream& flux, const Electron_presence& electron_presence)

{

   electron_presence.affiche(flux);
   return flux;

}//ostream& operator<<

//=====================================================================================================================================

/*******************/
/*Private functions*/
/*******************/

void Electron_presence::initialise_occuancies(unsigned int const *laser_levels, double const q)

{
   size_t energy_lvl, inv_energy_lvl, mode; //incrementations variables

   unsigned int middle=(Electron_presence::energy_level_number - 1) / 2; // represent the middle band we need to calculate the two band half separely
   double factor;

   /** We use in this function for formula length reduction the convention q_x that represent q^x                       **/

   double q_B;
   double *q_k = new double[mode_number];
   double *q_invk= new double[mode_number];    // k_invk is symetric value of  laser_levels k
   double q_N;

   q_B =    pow(q,Electron_presence::energy_level_number);
   for(mode=0;mode<Electron_presence::mode_number ;mode++)
   {
      q_k[mode]= pow(q,laser_levels[mode]);
      q_invk[mode]= pow(q,Electron_presence::energy_level_number- 1 - laser_levels[mode]);
      /** initialisation of the occupancy with 0 electron in CB the probability to find an electron at laser level is 0 **/
      this->probability[0][mode]=0.;
      this->probability[Electron_presence::energy_level_number][mode]=1.;//the electron cas be in [0,B] !
   }



   // Occupancies at laser level  //
   //-----------------------------//
   /** calcul of occupancy have a big error propagation so we calculate the second part with the first **/
   for(energy_lvl = 0 ; energy_lvl <= middle ; energy_lvl++)
   {
      q_N=pow(q,energy_lvl);
      factor = (1-q_N* q )/(q_N - q_B);


      for(mode=0;mode<Electron_presence::mode_number ;mode++)
      {

         this->probability[energy_lvl+1][mode] = q_k[mode] *  (1.- this->probability[energy_lvl][mode]) * factor ;

         /** We use the symetry of the band to calculate the second part
    *  this evit the error propagation and this accelerate to the calcul.                              **/
         inv_energy_lvl = Electron_presence::energy_level_number - energy_lvl;

         this->probability[inv_energy_lvl-1][mode] = 1. - q_invk[mode] *  ( this->probability[inv_energy_lvl][mode]) * factor ;
      }


      // Occupancy at pumping level //
      //----------------------------//
      /** first part of occupancy pump  level we use the last mode dimention to get this occupancy**/
      this->probability[energy_lvl][PUMPING_LEVEL]   =  q_B / q_N *(1.0 - q_N)/(1.0-q_B);

   }

   /** second part of occupancy pump  level**/
   for(energy_lvl = middle+1 ; energy_lvl <= Electron_presence::energy_level_number ; energy_lvl++)
   {
      q_N=pow(q,energy_lvl);
      this->probability[energy_lvl][PUMPING_LEVEL]   =  q_B / q_N * (1.0 - q_N)/(1.0-q_B);


   }
   delete  q_k;
   delete q_invk;

} // Electron_presence::initialise::occuancies

//=====================================================================================================================================
//                             E N D   O F   F I L E                                                                                 //
//=====================================================================================================================================
