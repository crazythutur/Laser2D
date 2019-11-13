/**
 * @class Analyse_tool
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 13/11/2019$
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


#ifndef ANALYSE_TOOL_H
#define ANALYSE_TOOL_H



#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <iterator>
#include <map>


#define ALLOCATION_BASE 128


//============================================================================================================================

/**
 * @brief The Analyse_init_params struct that reproup all parameters for the analysis of the trajectory
 */
struct Analyse_init_params {


    int SD_points_number; ///< spectral density points number
    double correlation_interval_duration; ///< time interval minimu for calculate correlation
    double correlation_max; ///< max range for the second order correlation function
    double end_time; ///< simulation time
    double stationnary_time; ///< time for stationnary research without aquiering data


    bool event_flag; ///< flag for recording all the trajectory

    bool g2_flag; ///< flag for second order correlation analysis

    bool histo_flag; ///< flag for recording all photon distribution

    bool sd_flag; ///< flag for recording spectral density

    bool verbose_flag; ///< flag for the verbose trajectory

    int seed; ///< seed already initialised


};//struct Laser_init_params

typedef struct Analyse_init_params Analyse_init_params;///< struture def


//========================================================================================================================================




/**
 * @brief The Analyse_tool struct regroup all analyse value for the laser
 * like average or variance for photon or electron
 */

//============================================================================================================================
struct Analyse_tool {

   Analyse_tool(unsigned int const number_of_modes, unsigned int const number_of_emitters);

   Analyse_tool(unsigned int const number_of_modes, unsigned int const number_of_emitters, Analyse_init_params parameters);

    ~Analyse_tool();

   /************************/
   /* Attributes           */
   /************************/

       unsigned int number_of_modes;  ///< number of modes in the laser
       unsigned int number_of_emitter; ///<number of emitters in the laser

       double* ave_photon;                              ///< photon average value for all modes
       double* ave_CB_electron;                         ///< CB electron average value for all Emitters
       double* ave_VB_electron;                         ///< VB electron average value for all Emitters

       double* var_photon;                              ///< photon variance value for all modes
       double* var_CB_electron;                         ///< CB electron variance value for all Emitters
       double* var_VB_electron;                         ///< VB electron variance value for all Emitters

       double var_total_photon;                         ///< total photon variance value

       double end_time; ///< simulation time

       unsigned int SD_points_number;///<  spectral density points number

       /** contain cosinus of Spectral Density for each @a SD_points_number  **/
       double ** cos_SD;
       /** contain sinus   of Spectral Density for each @a SD_points_number  **/
       double ** sin_SD;

       /** histogramm of photon number in the cavity each photon is represented in a box it must be a dynamic.
        *  It must be a dynamic table for prevent the error for a photon explosion             **/
       double ** photon_histogramm;

       /** size of histogramm must be incress if the photon number is too big                  **/
       int unsigned allocation_size;

       /** partials average photon for second order-correlation function                       **/
       double ** partial_average_photons;

       /** mark the time of correlation interval compared to instant_time                      **/
       double  correlation_time_mark;

       /** number of witch partial mean is calculated                                          **/
       int unsigned correlation_interval_number;

       /** time interval minimu for calculate correlation                                      **/
       double correlation_interval_duration;

       /** max range for the second order correlation function         **/
       double correlation_max_value;

       /** second order correlation function or g²(t) for each modes                           **/
       double **second_order_correlation;


       unsigned int *shutdown;                          ///< shutdown count for all modes
       std::map<std::string,unsigned int> event_counter;     ///< event counting

       bool analyse_flag; ///< flag for aquiering data, if the trajectory is not the stationnary reserach

       bool event_flag; ///< flag for recording all the trajectory

       bool g2_flag; ///<  flag for second order correlation analysis

       bool histo_flag; ///< flag for recording all photon distribution

       bool sd_flag; ///< flag for recording spectral density

       bool verbose_flag; ///< flag for the verbose trajectory

};//struct Analyse_tool

typedef struct Analyse_tool Analyse_tool;                ///< struture def


std::ostream& print_statistic(std::ostream& flux, const Analyse_tool *analyse_tool);

//========================================================================================================================================



double ** init_tab_dynamic( int x_size, int y_size );

void free_tab(double **tableau,size_t x_size );


#endif // ANALYSE_TOOL_H
