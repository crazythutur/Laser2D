#include "../inc/analyse_tool.h"

using namespace std;

Analyse_tool::Analyse_tool(unsigned int const number_of_modes, unsigned int const number_of_emitters)
{

   double* ave_photon;             ///< photon average value for all modes
   double* ave_CB_electron;        ///< CB electron average value for all emitters
   double* ave_VB_electron;        ///< VB electron average value for all emitters


   double* var_photon;             ///< photon variance value for all modes
   double* var_CB_electron;        ///< CB electron variance value for all emitters
   double* var_VB_electron;        ///< CB electron variance value for all emitters
   unsigned int *shutdown;         ///< shutdown number for all modes


   std::map <std::string, unsigned int> event_counter;

   ave_photon =        new double[number_of_modes];
   ave_CB_electron =   new double[number_of_emitters];
   ave_VB_electron =   new double[number_of_emitters];
   var_photon =        new double[number_of_modes];
   var_CB_electron =   new double[number_of_emitters];
   var_VB_electron =   new double[number_of_emitters];
   shutdown        =   new unsigned int[number_of_modes];

   fill_n(ave_photon,      number_of_modes,0);
   fill_n(ave_CB_electron, number_of_emitters,0);
   fill_n(ave_VB_electron, number_of_emitters,0);
   fill_n(var_photon,      number_of_modes,0);
   fill_n(var_CB_electron, number_of_emitters,0);
   fill_n(var_VB_electron, number_of_emitters,0);
   fill_n(shutdown,        number_of_modes,0);


   this->number_of_modes    = number_of_modes;
   this->number_of_emitter = number_of_emitters;


   this->ave_photon      = ave_photon;
   this->ave_CB_electron = ave_CB_electron;
   this->ave_VB_electron = ave_VB_electron;

   this->var_photon      = var_photon;
   this->var_CB_electron = var_CB_electron;
   this->var_VB_electron = var_VB_electron;

   this->shutdown    = shutdown;
   this->event_counter    = event_counter;

   this->analyse_flag=false;
   this->event_flag = false;
   this->g2_flag    = false;
   this->histo_flag = false;
   this->sd_flag = false;
   this->verbose_flag = false;


}

//========================================================================================================================================


Analyse_tool::Analyse_tool(unsigned int const number_of_modes, unsigned int const number_of_emitters,
                           Analyse_init_params parameters)
   :Analyse_tool( number_of_modes, number_of_emitters)
{


    this->event_flag    = parameters.event_flag;
    this->g2_flag       = parameters.g2_flag;
    this->histo_flag    = parameters.histo_flag;
    this->sd_flag       = parameters.sd_flag;
    this->verbose_flag  = parameters.verbose_flag;

    if(this->histo_flag)
    {
        this->photon_histogramm = init_tab_dynamic(ALLOCATION_BASE,number_of_modes);
        this->allocation_size=ALLOCATION_BASE;

    }

    if(this->g2_flag)
    {
        this->partial_average_photons = init_tab_dynamic(
                    ceil(parameters.end_time/parameters.correlation_interval_duration),number_of_modes);
        this->second_order_correlation = init_tab_dynamic(
                    floor(parameters.correlation_max / parameters.correlation_interval_duration ) +1 ,number_of_modes);

        this->correlation_time_mark=0;
        this->correlation_interval_number=0;
        this->correlation_interval_duration=parameters.correlation_interval_duration;
        this->correlation_max_value=parameters.correlation_max;
    }

   if(this->sd_flag)
   {
       this->SD_points_number = parameters.SD_points_number;
       this->cos_SD= init_tab_dynamic(number_of_modes,parameters.SD_points_number);
       this->sin_SD= init_tab_dynamic(number_of_modes,parameters.SD_points_number);

   }


   this->end_time = parameters.end_time;

   this->var_total_photon=0;        ///< total photon variance value
   this->allocation_size=ALLOCATION_BASE;


}

//========================================================================================================================================

Analyse_tool::~Analyse_tool()
{
    delete[] ave_photon;
    delete[] ave_CB_electron;
    delete[] ave_VB_electron;

    delete[] var_photon;
    delete[] var_CB_electron;
    delete[] var_VB_electron;

    if(this->g2_flag)
    {
        free_tab(this->partial_average_photons,
                 ceil(this->end_time / this->correlation_interval_duration));
        free_tab(this->second_order_correlation,
                 floor(this->correlation_max_value / this->correlation_interval_duration ) +1);

    }
    if(this->histo_flag)
    {
        free_tab(this->photon_histogramm, this->allocation_size);

    }
    if(this->sd_flag)
    {
        free_tab(cos_SD, this->number_of_modes);
        free_tab(sin_SD, this->number_of_modes);
    }




}


//========================================================================================================================================

std::ostream& print_statistic(std::ostream& flux, const Analyse_tool *analyse_tool)
{
    int total_average  = 0;
    int total_shutdown = 0;

    if(analyse_tool->number_of_modes == 1)
    {
        flux << "% Mode "
             << " : shutdown=" << analyse_tool->shutdown[0]
             << ",\t<m>="    << analyse_tool->ave_photon[0]
             << ",\tvar(m)=" << analyse_tool->var_photon[0]
             << endl;
    }else
    {
        for(unsigned int mode=0 ; mode < analyse_tool->number_of_modes ; mode++ )
        {
            flux << "% Mode " << mode+1
                 << " : shutdown=" << analyse_tool->shutdown[mode]
                 << ",\t<m>="    << analyse_tool->ave_photon[mode]
                 <<  ",\tvar(m)=" << analyse_tool->var_photon[mode]
                 << endl;
            total_average += analyse_tool->ave_photon[mode];
            total_shutdown += analyse_tool->shutdown[mode];
        }
        flux << "% Total \t"
             << " : shutdown="   << total_shutdown
             << ",\t<m>="        << total_average
             <<  ",\tvar(m)="     << analyse_tool->var_total_photon
             << endl;
    }


    if(analyse_tool->number_of_emitter == 1)
    {
        flux << "% Emitter "
             << " : <n_C>="            << analyse_tool->ave_CB_electron[0]
             << ",\tvar(n_C)="         << analyse_tool->var_CB_electron[0]
             << ",\t<n_V>="            << analyse_tool->ave_VB_electron[0]
             << ",\tvar(n_V)="         << analyse_tool->var_VB_electron[0]
             << endl;
    }else
    {
        for(unsigned int emitter = 0 ; emitter < analyse_tool->number_of_emitter ; emitter++ )
        {
            flux << "% Emitter "          << emitter + 1
                 << "\t: <n_C>="            << analyse_tool->ave_CB_electron[emitter]
                 << ",\tvar(n_C)="         << analyse_tool->var_CB_electron[emitter]
                 << ",\t<n_V>="            << analyse_tool->ave_VB_electron[emitter]
                 << ",\tvar(n_V)="         << analyse_tool->var_VB_electron[emitter]
                 << endl;
        }
    }






    return flux;
}



//========================================================================================================================================

/**
 * @brief init_tab_dynamic intialise a  dimentions dynamic tab that nedd a unique free
 * @param x_size
 * @param y_size
 * @return the tab initialised at 0
 */
double ** init_tab_dynamic( int x_size, int y_size )
{
    double** tableau;

    int i;
    int j;

    if(x_size <= 0 || y_size <= 0)
    {
        return NULL;
    }

    tableau = (double**) malloc(sizeof *tableau * x_size);
    if(tableau == NULL)
    {
        return NULL;
    }

    for(i=0;i<x_size;i++)
    {
        tableau[i] = (double *) malloc( sizeof *tableau[i] * y_size );
        if(tableau[i]==NULL)
        {
            return NULL;
        }
        for(j=0;j<y_size;j++){
            tableau[i][j]=0.;
        }
    }

    return tableau;
}


//========================================================================================================================================

/**
 * @brief free_tab a two dimention tab
 * @param tableau tab to free
 * @param x_size number of row of table
 */
void free_tab(double **tableau,size_t x_size ){

    size_t i;

    for(i = 0; i < x_size; i++)
    {
        free(tableau[i]);
    }
    free(tableau);
}


