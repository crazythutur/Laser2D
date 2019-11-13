/**
 * @class 2D_laser
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2018/10/29$
 *
 */

#ifndef LASER_2D_H
#define LASER_2D_H

#include <vector>
#include <cmath>
#include "Emitter.h"
#include "QW_emitter.h"
//#include "QD_Emitter.h"

using namespace std;

/**
 * @brief The Laser_2D class is a 2 dimmmentions (2d) laser represent a grind of emitter used previously[1].
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

      unsigned int emitter_number;  ///< Emitter_number is the number of @a Emitter

      unsigned int pumps_phases;             ///< pumps_phases is the number of phase in pumping

      double beta;                           ///< beta is the proportion of photon emited in the laser mode(s)

      double electrical_coupling;            ///< coupling factor for electron transfert between two emitters

      double *cavity_escape_rate;            ///< cavity_escape_rate is the optical cavity escape rate for each modes

      double *local_pump;                    ///< pump_rate is a table of @a Emitter_number values that contain all local pumps

      double *temperature;                   ///< temperature is a table of @a Emitter_number elements of @a Emitter temperature

      double number_of_photonic_phases;     ///< number of phases for imitate the spectral hole burning

      double weigth_of_photonic_phases;     ///< weight of phases for imitate the spectral hole burning

      unsigned int *lasing_level;           ///< list of the lasing levels for each modes


   };//struct Laser_init_parameters


   typedef struct Laser_init_parameters Laser_init_parameters;///< struture def

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
    * @param energy_level_splitting energy that sperate 2 energy levels  in meV
    * @param electron_presence_map map of all @a Electron_presence. This permit to not recalculate the @a Electron_presence
    * a new entry will be added if the value are not already calculated
    */
   Laser_2D (Laser_init_parameters const parameters,
             double const energy_level_splitting, std::map<double, Electron_presence *> &electron_presence_map);

   virtual ~Laser_2D ();


   /*****************/
   /*Getters/Setters*/
   /*****************/

   /**
    * @return the type of the laser can be QW(Quantum Wells) or QD(Quantum dots)
    */
   laser_type getType() const
   {
      return type;
   }

   /**
    * @return th mode number of the laser
    */
   unsigned int getMode_number() const
   {
      return mode_number;
   }

   /**
    * @return the number of @a Emitter that contain the object
    */
   unsigned int getEmitter_number() const
   {
     return emitter_number;
   }

   /**
    * @return the pump phase numbers
    */
   unsigned int getPumps_phases () const
   {
    return pumps_phases;
   }

   /**
    * @return the proportion of the spontaneous emision that emit in the lasing mode in ]0,1]
    */
   double getBeta ()  const
   {
    return beta;
   }

   /**
    * @return the coupling factor for electron transfert between two emitters
    */
   double getElectical_coupling ()  const
   {
    return electical_coupling;
   }


   /**
    * @param mode lasing mode
    * @return the cavity escape rate of the cavity for the given lasiing mode
    */
   double getCavity_escape_rate(unsigned int mode) const
   {
      return cavity_escape_rate[mode];
   }

   /**
    * @param laser_number the number of the laser wanted
    * @return @a Emitter of the table corresponding to the number input
    */
   Emitter *getEmitter(unsigned int laser_number)
   {
      return laser_table[laser_number];
   }


//============================================================================================================================


public:

   /**
    * @brief gaussian_coupling create a coupling factor for all emiter for all mode depending of the parameters input
    * @param FWHM_x Full width at half maximum(FWHM) for the x axis for all modes
    * @param FWHM_y Full width at half maximum(FWHM) for the y axis for all modes
    */
   void gaussian_coupling(double *FWHM_x, double *FWHM_y);

   /**
    * @brief uniform_coupling apply a uniform gain medium for each mode with the given
    * @param value values table
    */
   void uniform_coupling(double *value);



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
    * @brief Emitter_number number of @a Emitter
    */
   unsigned int emitter_number;

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
   * @brief electical_coupling represent the coupling factor for electron transfert between two emitters
   */
  double electical_coupling;

  /**
   * @brief cavity_escape_rate optical cavity escape rate for each modes
   */
  double *cavity_escape_rate;

  /**
   * @brief height of the Laser 2D
   */
  unsigned int height;

  /**
   * @brief width of the Laser 2D
   */
  unsigned int width;


  double number_of_photonic_phases;///< number of phases for imitate the spectral hole burning

  double weigth_of_photonic_phases;///< weight of phases for imitate the spectral hole burning

  /**
   * @brief laser_table array of a number @a Emitter_number of @a Emitter pointer
   */
  vector<Emitter *> laser_table;

//============================================================================================================================

  /********************/
  /*Privates function */
  /********************/

  /**
    * @brief organize_neighborhood initialize all @a Emitter.neighboring_lasers
    *
    * all direction are initialisated with @a Emitter.setNeighboring_laser we make two loop
    * one for the width and the other for the height
    */
   void organize_neighborhood();

};// end of class Laser_2D
//============================================================================================================================

/**
 * @brief find_width return the height of a rectangle with the more square shape possible
 *
 * @param area of the most square shape
 * @return the height of a rectangle with area input
 */
unsigned int find_height(unsigned int area);

#endif // LASER_2D_H
