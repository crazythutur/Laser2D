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


struct Analyse_init_params {


    int SD_points_number;
    double correlation_interval_duration;
    double correlation_max;
    double end_time;


    bool event_flag;

    bool g2_flag;

    bool histo_flag;

    bool sd_flag;

    bool verbose_flag;

    int seed;


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

       unsigned int number_of_modes;
       unsigned int number_of_emitter;

       double* ave_photon;                              ///< photon average value for all modes
       double* ave_CB_electron;                         ///< CB electron average value for all Emitters
       double* ave_VB_electron;                         ///< VB electron average value for all Emitters

       double* var_photon;                              ///< photon variance value for all modes
       double* var_CB_electron;                         ///< CB electron variance value for all Emitters
       double* var_VB_electron;                         ///< VB electron variance value for all Emitters

       double var_total_photon;                         ///< total photon variance value

       double end_time;

       unsigned int SD_points_number;

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

       bool analyse_flag;

       bool event_flag;

       bool g2_flag;

       bool histo_flag;

       bool sd_flag;

       bool verbose_flag;

};//struct Analyse_tool

typedef struct Analyse_tool Analyse_tool;                ///< struture def


std::ostream& print_statistic(std::ostream& flux, const Analyse_tool *analyse_tool);

//========================================================================================================================================



double ** init_tab_dynamic( int x_size, int y_size );

void free_tab(double **tableau,size_t x_size );


#endif // ANALYSE_TOOL_H
