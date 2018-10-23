#include "../inc/Electron_presence.h"

using namespace std;



/**
 * @brief get_occupancy permit to obtain the probability to find an electron
 *
 * at laser lvl  with electron in BC done for each mode
 * We use the fabrice formula that is iterative formula we calculate the probability table in two times
 * because of the error propagation.
 * the general formule for an occupancy (at stationary state) at level k is
 * \f$ occ_k(N+1) = (q^k (1-occ_k(N)) \frac{1-q^{N+1}}{q^N-q^B}\f$
 * @todo add ref to formula here
 *
 * we can use the symetry of the band \f$occ_k(N)=1-occ_{B-1-k}(B-N)\f$
 *
 * to obtain
 * \f$ occ_k(B-N-2) = 1-q^{B-1-k}occ_k(B-N-1)\frac{1-q^{N+1}}{q^N-q^B}\f$
 * B is abreviation for @a Markov_Parameters.energy_level_number
 * k is abreviation for @a laser_levels in CB, NB : teir is the same number of laser level in CB than modes numbers
 * N is abreviation for the energy level we use this incrementation variable for loop all the energy level
 *@param occupancies tab to put the occupancy
 *  first dimention for witch number of electrons in cavity, and the second dimention is for each laser mode and the pumping level.
 *
* @param energy_level_number number of energy states
* @param laser_levels table that contain all laser level for each mode
* @param q represent the temperature at exponential form q=exp(-E/(KbT)), E is the energy lvl between two lvl
                in our unity E=1meV, Kb is Boltzmann constant 1,38064852.10^-23 and T temperature in °K
*/



Electron_presence::Electron_presence(unsigned int const mode_number, std::size_t const energy_level_number,
                                     unsigned int const *laser_levels, double const q)
{


   this->probability = new double*[energy_level_number+1];

   for(size_t energy_level=0 ; energy_level <= energy_level_number ; energy_level++)//occupancies are deffiened in [0,B]
   {
      this->probability[energy_level] = new double[mode_number + 1];
   }

   this->mode_number=mode_number;
   this->energy_level_number = energy_level_number;

   this->initialise_occuancies(laser_levels, q);
}

void Electron_presence::initialise_occuancies(unsigned int const *laser_levels, double const q)

{
   size_t energy_lvl, inv_energy_lvl, mode; //incrementations variables

   unsigned int middle=(this->energy_level_number - 1) / 2; // represent the middle band we need to calculate the two band half separely
   double factor;

   /** We use in this function for formula length reduction the convention q_x that represent q^x                       **/

   double q_B;
   double q_k[this->mode_number];
   double q_invk[this->mode_number];    // k_invk is symetric value of  laser_levels k
   double q_N;

   unsigned int PUMPING_LEVEL = this->mode_number;/** @todo pumping level defined in public class at = mode_number**/

   q_B =    pow(q,this->energy_level_number);
   for(mode=0;mode<this->mode_number ;mode++)
   {
      q_k[mode]= pow(q,laser_levels[mode]);
      q_invk[mode]= pow(q,this->energy_level_number- 1 - laser_levels[mode]);
      /** initialisation of the occupancy with 0 electron in CB the probability to find an electron at laser level is 0 **/
      this->probability[0][mode]=0.;
      this->probability[this->energy_level_number][mode]=1.;//the electron cas be in [0,B] !
   }


   /*******************************/
   /* Occupancies at laser level  */
   /*******************************/
   /** calcul of occupancy have a big error propagation so we calculate the second part with the first **/
   for(energy_lvl = 0 ; energy_lvl <= middle ; energy_lvl++)
   {
      q_N=pow(q,energy_lvl);
      factor = (1-q_N* q )/(q_N - q_B);


      for(mode=0;mode<this->mode_number ;mode++)
      {

         this->probability[energy_lvl+1][mode] = q_k[mode] *  (1.- this->probability[energy_lvl][mode]) * factor ;

         /** We use the symetry of the band to calcultae the second part
    *  this evit the error propagation and this accelerate to the calcul.                              **/
         inv_energy_lvl = this->energy_level_number - energy_lvl;

         this->probability[inv_energy_lvl-1][mode] = 1. - q_invk[mode] *  ( this->probability[inv_energy_lvl][mode]) * factor ;
      }

      /*******************************/
      /* Occupancy at pumping level  */
      /*******************************/
      /** first part of occupancy pump  level we use the last mode dimention to get this occupancy**/
      this->probability[energy_lvl][PUMPING_LEVEL]   =  q_B / q_N *(1.0 - q_N)/(1.0-q_B);

   }

   /** second part of occupancy pump  level**/
   for(energy_lvl = middle+1 ; energy_lvl <= this->energy_level_number ; energy_lvl++)
   {
      q_N=pow(q,energy_lvl);
      this->probability[energy_lvl][PUMPING_LEVEL]   =  q_B / q_N * (1.0 - q_N)/(1.0-q_B);


   }
}


Electron_presence::~Electron_presence(){
   //the electron cas be in [0,B] !
   for(size_t energy_level=0 ; energy_level <= this->energy_level_number ; energy_level++)
   {
      delete this->probability[energy_level];
   }
   delete this->probability;
}

unsigned int Electron_presence::getMode_number() const
{
   return mode_number;
}

unsigned int Electron_presence::getEnergy_level_number() const
{
   return energy_level_number;
}

double **Electron_presence::getProbability() const
{
   return probability;
}

ostream& operator<<(ostream& flux, Electron_presence const& electron_presence)

{

   electron_presence.affiche(flux);
   return flux;

}

void Electron_presence::affiche(ostream& flux) const

{
   flux << "Electron_number\t";
   for(size_t mode=0;mode<this->mode_number;mode++){
      flux << "mode " << mode +1 << "\t";
   }
   flux << "pump lvl" << endl ;

   for( size_t level=0; level<=this->energy_level_number; level ++)//the electron cas be in [0,B] !
   {
      flux << level <<"\t" ;
      for(size_t mode=0;mode<=this->mode_number;mode++)
      {
         flux <<this->probability[level][mode] <<"\t" ;
      }
      flux << endl;
   }
}


/**
 * @brief main Testing
 * @return
 */
int main()
{

    const unsigned int lasing_level[2]={399,401};
    Electron_presence occ (2,800,lasing_level,0.962090);
    cout << occ;
    double** tab=occ.getProbability();
    cout << tab[795][2] << endl;
    return 0;
}
