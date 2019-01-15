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

#ifndef LASER_2D_H
#define LASER_2D_H

#include <vector>
#include <cmath>
#include "Elementary_laser.h"
#include "QW_elementary_laser.h"
//#include "QD_elementary_laser.h"

using namespace std;

class Laser_2D
{
public:   

   /********************/
   /*Public Enum/Struc */
   /********************/

   /**
    * @brief The Laser_init_parameters struct regroup all general parameters for intitialisate the laser
    */
   struct Laser_init_parameters {

      unsigned int mode_number;              ///< mode_number number of mode in the laser

      unsigned int elementary_laser_number;  ///< elementary_laser_number is the number of @a Elementary_laser

      unsigned int pumps_phases;             ///< pumps_phases is the number of phase in pumping

      double beta;                           ///< beta is the proportion of photon emited in the laser mode(s)

      double *cavity_escape_rate;            ///< cavity_escape_rate is the optical cavity escape rate for each modes

      double *local_pump;                    ///< pump_rate is a table of @a elementary_laser_number values that contain all local pumps

      double *temperature;                   ///< temperature is a table of @a elementary_laser_number element of @a Elementary_laser temperature

   };//struct Laser_init_parameters

   typedef struct Laser_init_parameters Laser_init_parameters;

   /**
    * @brief type of the laser can be Quantum Well(QW), or Quantum Dot (QD)
    */
   enum laser_type{ QW, QD };

   /************************/
   /*Constructor/Destructor*/
   /************************/
   /**
    * @brief Laser_2D base constructor
    * @param parameters all paramerters needed to initialisate the laser
    */
   Laser_2D (Laser_init_parameters const parameters);


   /**
    * @brief Laser_2D Constructor for a 2D laser QW
    * @param parameters classics parameter to initialise the laser
    * @param laser_levels array of each lasing level
    * @param energy_level_splitting energy that sperate 2 energy levels  in meV
    * @param electron_presence_map map of all @a Electron_presence. This permit to not recalculate the @a Electron_presence
    * a new entry will be added if the value are not already calculated
    */
   Laser_2D (Laser_init_parameters const parameters, unsigned int const *laser_levels, double const energy_level_splitting,
             std::map<double, Electron_presence *> &electron_presence_map);

   virtual ~Laser_2D ();


   /*****************/
   /*Getters/Setters*/
   /*****************/

   laser_type getType() const
   {
      return type;
   }

   unsigned int getMode_number() const
   {
      return mode_number;
   }

   unsigned int getElementary_laser_number() const
   {
     return elementary_laser_number;
   }

   unsigned int getPumps_phases () const
   {
    return pumps_phases;
   }

   double getBeta ()  const
   {
    return beta;
   }

   double getCavity_escape_rate(unsigned int mode) const
   {
      return cavity_escape_rate[mode];
   }

   Elementary_laser *getElementary_laser(unsigned int laser_number)
   {
      return laser_table[laser_number];
   }

//============================================================================================================================

   void setMode_number(unsigned int value)
   {
      mode_number = value;
   }

   void setPumps_phases (unsigned int new_var)   {
       pumps_phases = new_var;
   }

   void setBeta (double new_var)   {
       beta = new_var;
   }

   void setCavity_escape_rate(unsigned int mode, double value)
   {
      cavity_escape_rate[mode] = value;
   }

//============================================================================================================================

private:
   /*********************/
   /*Privates attributes*/
   /*********************/

   /**
    * @brief type @a laser_type of a laser can be Quantum Well(QW), or Quantum Dot (QD)
    */
   laser_type type;

   /**
    * @brief mode_number number of mode in the laser
    */
   unsigned int mode_number;

   /**
    * @brief elementary_laser_number number of @a Elementary_laser
    */
   unsigned int elementary_laser_number;

   /**
    * @brief pumps_phases represent the number of phase in pumping, if 1 this correspond to no phase and so a poissonian pumping
    * the variance of the pumping is deffined by the ratio @a pump_rate / @a pumps_phases more you had phase more the pump corespond to a quiet
    */
   unsigned int pumps_phases;

  /**
   * @brief beta proportion of photon emited in the laser mode(s)
   */
  double beta;

  /**
   * @brief cavity_escape_rate optical cavity escape rate for each modes
   */
  double *cavity_escape_rate;

  /**
   * @brief laser_table array of a number @a elementary_laser_number of @a Elementary_laser pointer
   */
  vector<Elementary_laser *> laser_table;

//============================================================================================================================

  /********************/
  /*Privates function */
  /********************/

  /**
    * @brief organize_neighborhood initialize all @a Elementary_laser.neighboring_lasers
    *
    * all direction are initialisated with @a Elementary_laser.setNeighboring_laser we make two loop
    * one for the width and the other for the height
    */
   void organize_neighborhood();

};// end of class Laser_2D
//============================================================================================================================

/**
 * @brief find_width return the height of a rectangle with the more square shape possible
 *
 *
 * @return the height of a rectangle with area of @param area
 */
unsigned int find_height(unsigned int area);

#endif // LASER_2D_H
