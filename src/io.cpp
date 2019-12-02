/**
 * @class io
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 18/07/2019$
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
 *
 *
 */

#include "../inc/io.h"

using namespace std;


//=====================================================================================================================================

/************************/
/*Constructor/Destructor*/
/************************/



//=====================================================================================================================================

/******************/
/*Public function */
/******************/
ostream & IO::write_command(ostream &flux, Laser_2D::Laser_init_parameters *parameters,
                            Analyse_init_params *analyse_init_params, Laser_2D *laser, IO::Input_parameters *input_parameters)
{
    flux << "% laser2D.exe";

    if(analyse_init_params->histo_flag)
        flux << " --histo";
    if(analyse_init_params->event_flag)
        flux << " --event";
    if(analyse_init_params->sd_flag)
        flux << " --sd";
    if(analyse_init_params->g2_flag)
        flux << " --g2";
    if(analyse_init_params->verbose_flag)
        flux << " --verbose";

    flux << " --seed=" << analyse_init_params->seed;

    flux << " --modes=" << parameters->mode_number;

    flux << " --emitters=" << parameters->emitter_number;

    flux << " --pump_phases=" << parameters->pumps_phases;

    flux << " --sd_points=" << analyse_init_params->SD_points_number;

    flux << " --energy_levels=" << Electron_presence::getEnergy_level_number();

    flux << " --electrical_coupling="    << parameters->electrical_coupling;
    flux << " --beta=" << parameters->beta;

    flux << " --pump=";
    write_table(flux, parameters->local_pump, parameters->emitter_number);

    flux << " --temperature=";
    write_table(flux, parameters->temperature, parameters->emitter_number);


    for(unsigned int emitter = 0 ; emitter < parameters->emitter_number ; emitter ++)
    {
        flux << " --cavity_coupling" << "=" ;
        write_table(flux, laser->getEmitter(emitter)->getCavity_coupling(), parameters->mode_number);

    }
     flux << " --lasing_lvl=";
     write_table(flux, parameters->lasing_level, parameters->mode_number);

     flux << " --cavity=" ;
     write_table(flux, parameters->cavity_escape_rate, parameters->mode_number);






    if(!input_parameters->output_file.empty())
    {
            flux << " --output=" << input_parameters->output_file;
    }

    flux << " --photons=";
    write_table(flux, input_parameters->initial_photons_number, parameters->mode_number);

    flux << " --electrons=";
    write_table(flux, input_parameters->initial_CBelectrons_number, parameters->emitter_number);

    flux << " --stationnary=" << analyse_init_params->stationnary_time ;
    flux << " --time=" << analyse_init_params->end_time << endl;
    return flux;
}

//=====================================================================================================================================


ostream & IO::write_csv(ostream &flux, Analyse_tool *tool)
{
   char col_sep=',';   // colomns sepearator for csv file
   bool hist_breaker=false;
   long unsigned hist_iterator = 0;//first value fix at min
   long unsigned SD_iterator =0;
   int breaker_iterator;
   map<string, unsigned int>::iterator event_iterator = tool->event_counter.begin();
   int unsigned g2_iterator=0;
   long unsigned hist_hight_value = tool->allocation_size ;//highter value fix at maximum
   bool csv_end[MAX_BREAKER]={false};

   double frequency_precision_min = 2 * M_PI / tool->end_time;
   double histogramm_delimiter = 0;//For ignore small values in histogramm. 0 if no ignores values
   double cos_SD_somme;
   double sin_SD_somme;

   unsigned int total_cavity_output_events=0;
   unsigned int correlation_max =  floor(tool->correlation_max_value  /
                                         tool->correlation_interval_duration ) +1;
   // number of correlation values wanted


   /****************************/
   /* breakers initialisation  */
   /****************************/
   /** if the flags are absant fix the breaker of differents cases **/
   for(breaker_iterator=0;breaker_iterator<MAX_BREAKER;breaker_iterator++)
   {
       csv_end[breaker_iterator]=0;
   }
   if(!tool->histo_flag)
   {
       csv_end[HISTO_BREAKER]=1;
   }
   if(!tool->sd_flag)
   {
       csv_end[SD_BREAKER]=1;
   }
   if(!tool->g2_flag)
   {
       csv_end[G2_BREAKER]=1;
   }


   /*****************************/
   /* Histogram delimitation    */
   /*****************************/
   /** Determining the useful part of the histogram  putted on 0 value for no ignores values  **/
   if(tool->histo_flag)
   {


       /** fix the lower histogramm value to the first value of histogramm **/
       while(!hist_breaker && hist_iterator < tool->allocation_size)
       {

           for(unsigned int mode = 0 ; mode < tool->number_of_modes && !hist_breaker; mode++)
           {
                  hist_breaker= tool->photon_histogramm[hist_iterator][mode] > histogramm_delimiter ;
           }
           hist_iterator ++;
       }
       hist_iterator--;// value is +1 per rapport at lower value. we can not initialise at -1 because of unsigned

       /** fix the highter histogramm value to the last value of histogramm **/
       hist_breaker=false;
       while(!hist_breaker && (hist_hight_value > 0))
       {
           hist_hight_value --;
           for(unsigned int mode = 0 ; mode < tool->number_of_modes  && !hist_breaker; mode++)
           {
                  hist_breaker= tool->photon_histogramm[hist_hight_value][mode]> histogramm_delimiter;

           }

       }
   }

   /************************/
   /* write header area    */
   /************************/
   /** write input command **/
   //write_input_command( parameters,  parameters->out_file);

   /** statistics description  **/
   print_statistic(flux, tool);


   /** time of calculation are writing **/
   //fprintf( parameters->out_file, "%% Execution time : %.0f s\n", difftime(time(NULL), start_time));

   /** header of CSV writing, attribute the column's head in function of flags**/
   if(tool->histo_flag)
   {
      flux << "m" << col_sep;
       for(unsigned int mode = 0 ; mode < tool->number_of_modes  ; mode++)
       {
          flux << "P(m_" << mode << ")" << col_sep;
       }

   }
   if(tool->sd_flag)
   {
      flux << "Omega" << col_sep;
       for(unsigned int mode = 0 ; mode < tool->number_of_modes  ; mode++)
       {
          flux << "S_" << mode << "(Omega)" << col_sep ;
       }
       if(tool->number_of_modes >1)
       {

          flux << "S_T" << "(Omega)" << col_sep ;
       }
   }

   if(tool->g2_flag)
   {
      flux << 't' << col_sep;
       if(tool->number_of_modes ==1)
       {
          flux <<  "g2(t)" << col_sep;
       }else
       {
           for(unsigned int mode = 0 ; mode < tool->number_of_modes  ; mode++)
           {
              flux << "g2_" << mode << "(t)" << col_sep;
           }
       }
   }


   /** for all cases  print the event label**/
   flux << "Event_name" << col_sep << "Event_count" << endl ;


   /********************/
   /* write CSV loop   */
   /********************/
   /** loop for each line in the output file test if the data is already completely wrinting and then
    * if all datas are completely wrinting **/
   do
   {
       csv_end[TOTAL_BREAKER]=false;

       /***********************/
       /* histogram writing   */
       /***********************/
       /** Histogram values writing            **/
       if(tool->histo_flag)
       {

           if(csv_end[HISTO_BREAKER])
           {
               for(unsigned int mode = 0 ; mode < tool->number_of_modes  +1; mode++)
               {//+1 because we use this loop to pass photon values too
                   flux << col_sep;
               }
           }else
           {
              flux << hist_iterator << col_sep;
               for(unsigned int mode = 0 ; mode < tool->number_of_modes  ; mode++)
               {
                  flux << tool->photon_histogramm[hist_iterator][mode] / tool->end_time << col_sep;

               }
               hist_iterator++;
               csv_end[HISTO_BREAKER] = (hist_iterator > hist_hight_value);
           }
       }

       /******************************/
       /* Spectral Density writing   */
       /******************************/
       /** Spectral densities values writing            **/
       if(tool->sd_flag)
       {

           if(csv_end[SD_BREAKER])
           {
               for(unsigned int mode = 0 ; mode < tool->number_of_modes  +1; mode++)
               {
                   //+1 because we use this loop to pass omega values too
                   flux << col_sep;
               }
           }else
           {
               sin_SD_somme = 0.;
               cos_SD_somme = 0.;
               /**     Frequency values writing            **/
               flux <<  frequency_precision_min * (SD_iterator+1) << col_sep ;

               for(unsigned int mode = 0 ; mode < tool->number_of_modes  ; mode++)
               {
                   /**     Spectral Density writing for each modes                **/
                  flux <<  (tool->cos_SD[mode][SD_iterator]*tool->cos_SD[mode][SD_iterator] +
                            tool->sin_SD[mode][SD_iterator]*tool->sin_SD[mode][SD_iterator])/
tool->event_counter["O" + std::to_string(mode)] << col_sep;


                   cos_SD_somme+=tool->cos_SD[mode][SD_iterator];
                   sin_SD_somme+=tool->sin_SD[mode][SD_iterator];
                   total_cavity_output_events += tool->event_counter["O" +  std::to_string(mode)];
               }
               /**     Total Spectral Density writing                 **/
               if(tool->number_of_modes >1)
               {
                  flux << (cos_SD_somme*cos_SD_somme + sin_SD_somme*sin_SD_somme) /total_cavity_output_events << col_sep;
               }

               SD_iterator++;
               csv_end[SD_BREAKER] = (SD_iterator >= tool->SD_points_number);
           }
       }

       /**************************************/
       /* second order correlation writing   */
       /**************************************/
       /** second order correlation values writing            **/
       if(tool->g2_flag)
       {

           if(csv_end[G2_BREAKER])
           {
               for(unsigned int mode = 0 ; mode < tool->number_of_modes  +1; mode++)
               {
                   //+1 because we use this loop to pass omega values too
                   flux << col_sep;
               }
           }else
           {
              flux << (g2_iterator) * tool->correlation_interval_duration << col_sep;
               for(unsigned int  mode = 0 ; mode < tool->number_of_modes  ; mode++)
               {
                   /**     Spectral Density writing for each modes                **/
                  flux << tool->second_order_correlation[g2_iterator][mode] << col_sep;

               }


               g2_iterator++;
               csv_end[G2_BREAKER] = (g2_iterator >= correlation_max);
           }
       }

       /********************/
       /* Event  writing   */
       /********************/
       /** Events count values are wrinting             **/
       if(csv_end[EVENT_BREAKER])
       {

           flux << col_sep;
       }else
       {
          flux << event_iterator->first << col_sep << event_iterator->second ;
           event_iterator++;
           csv_end[EVENT_BREAKER] = (event_iterator == tool->event_counter.end());
       }

       // end of line
       flux << endl;

       /** test if all breaker are declanched  **/
       csv_end[TOTAL_BREAKER]=1;
       for(breaker_iterator=0;breaker_iterator<MAX_BREAKER;breaker_iterator++)
       {
           csv_end[TOTAL_BREAKER]&=csv_end[breaker_iterator];
       }
   }while (!csv_end[TOTAL_BREAKER]);
   /*********************/
   /* End of  writing   */
   /*********************/

   return flux;

}

//=====================================================================================================================================


IO::Input_parameters  *IO::read_argtable(int argc, char *argv[])
{

    char progname[] = "laser.exe";
    Input_parameters *input_parameters = nullptr;

    int nerrors;

    /* arg_xxx structs */
    struct arg_lit *verbose, *help_flag, *g2_flag, *histo_flag, *sd_flag, *event_flag;
    struct arg_int *number_of_modes, *number_of_emitters, *pump_phases, *SD_point_numbers, *energies_levels;
    struct arg_int *lasing_levels, *initial_photons_number, *initial_CBelectrons_number, *seed;
    struct arg_dbl *max_time, *stationnary_time, *electrical_coupling, *beta;
    struct arg_dbl *pump, *temerature, *cavity_escape_rate, *cavity_coupling;
    struct arg_file*output_file;
    struct arg_end *end;
    void *argtable[] = {

        verbose     = arg_litn("v", "verbose", 0, 1, "verbose output"),
        help_flag   = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
        g2_flag     = arg_litn("g", "g2", 0, 1, "calculate the second order correlation"),
        histo_flag  = arg_litn("h", "histo", 0, 1, "calculate and display in ouput the histogramms of photons for each modes"),
        sd_flag     = arg_litn("s", "sd", 0, 1, "calculate and display in ouput the spectral density of photons for each modes"),
        event_flag  = arg_litn("e", "event", 0, 1, "show all the trajectory"),

        number_of_modes   = arg_int0("M", "modes", "<n>", "number of modes (default value is 1)"),
        number_of_emitters   = arg_int0("N", "emitters", "<n>",  "number of emitters (default value is 1)"),
        pump_phases   = arg_int0(NULL, "pump_phases", "<n>", "number of pump phases (default value poissonnian pump so is 1)"),
        SD_point_numbers   = arg_int0(NULL, "sd_points", "<n>",  "number of emitters (default value is 2048)"),
        energies_levels   = arg_int0("B", "energy_levels", "<n>", "number of energy levels (default value is 800)"),

        lasing_levels   = arg_intn("l", "lasing_lvl", "<n>", 0, MAX_MODES,
                "position of lasing energy levels for each modes(default values is in the middle of bands)"),
        initial_photons_number = arg_intn("m", "photons", "<n>", 0, MAX_MODES,
                "initial number of photon for each modes(default values is 0)"),
        initial_CBelectrons_number = arg_intn("n", "electrons", "<n>", 0, MAX_EMITTERS,
                "initial number of electron in conduction band for emitters(default values is halfs of band filled)"),
        seed = arg_intn("r", "seed", "<n>", 0, MAX_SEED, "seed for random generator (default is set to CPU time)"),

        max_time   = arg_dbl0("t", "time", "<dbl>", "time of the simulation (default value is 1000)"),
        stationnary_time   = arg_dbl0("s", "stationnary", "<dbl>",
            "time time without analyse the trajectory to find a stationnary state (default value is the same value as time)"),
        electrical_coupling   = arg_dbl0("k", "electrical_coupling", "<dbl>",
            "electrical coupling between all emitters, 1 mean all electron are transfert in all unit of time in average (default is 0)"),
        beta   = arg_dbl0("b", "beta", "<dbl>",
            "proportion of the photon emit in the coherent mode (default is 1, all spontaneous photon are in the mode)"),

        pump   = arg_dbln("Jj", "pump", "<dbl>", 0, MAX_EMITTERS, "pumping value for all emitters,"
        " if only one pump is specify this will be the total pump and so each local pump"
        " will be this pump divided by the emitters numbers (default is 35)"),
        temerature   = arg_dbln("T", "temperature", "<dbl>", 0, MAX_EMITTERS,
            "temperature of the latcie in the possition of the differents emitters in Kelvin (default is 293.16)"),
        cavity_escape_rate   = arg_dbln("cC", "cavity", "<dbl>", 0, MAX_MODES,
            "cavity escape rates, the inverse of the photon life time (default is 0.6)"),
        cavity_coupling   = arg_dbln("y", "cavity_coupling", "<dbl>", 0, MAX_EMITTERS* MAX_MODES ,
            "coupling between cavity and the emitters for all emitters, for all emitters spectify the coupling for each modes."
            "So for example 3 emitter in a bimode will be --cavity_coupling={1,1} --cavity_coupling={1,1} --cavity_coupling={1,1}"
            "(default is 1 for all)"),




        output_file       = arg_filen("o", "output", "myfile", 0, 1, "output file default is the command output"),
        end     = arg_end(20),
    };

    /** set any command line default values prior to parsing except laser levels and stationnary time
     *  that requiered other parameters to be initialised
    **/

    verbose     -> count =      0;
    help_flag   -> count =      0;
    g2_flag     -> count =      0;
    histo_flag  -> count =      0;
    sd_flag     -> count =      0;
    event_flag  -> count =      0;

    number_of_modes     -> ival[0] =    1;
    number_of_emitters  -> ival[0] =    1;
    pump_phases         -> ival[0] =    1;
    SD_point_numbers    -> ival[0] = 2048;
    energies_levels     -> ival[0] =  800;

    max_time            -> dval[0] = 1000.0;
    stationnary_time    -> dval[0] =    0.0;
    electrical_coupling -> dval[0] =    0.0;
    beta                -> dval[0] =    1.0;

    seed        -> ival[0] =   time(NULL);
    for(int seed_iterator=1 ; seed_iterator < MAX_SEED ; seed_iterator++)
    {
        seed->ival[seed_iterator] = 1;
    }

    for(int mode = 0 ; mode < MAX_MODES ; mode ++)
    {

        initial_photons_number      -> ival[mode] =    0;
        cavity_escape_rate          -> dval[mode] =  0.6;
    }

    for(int emitter = 0 ; emitter < MAX_EMITTERS ; emitter ++)
    {

        pump        -> dval[emitter] =   35.0;
        temerature  -> dval[emitter] = 293.16;
        for(int mode = 0 ; mode < MAX_MODES ; mode ++)
        {
            cavity_coupling          -> dval[emitter*MAX_MODES + mode] =  1;
        }

    }

    output_file     -> filename[0]= "";



    /** Parsing **/
    nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help_flag->count > 0)
    {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout, argtable, "\n");
        printf( "This program perhaps calculates the photodetection spectral density of a two band 'semiconductor-like'"
                       " markovian monomode laser using the Monte-Carlo method for lasing mode(s).\n");
                printf("Copyright Arthur Vallet, Laurent Chusseau, Fabrice Philippe - July 2019\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        goto exit;
    }

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stderr, end, progname);
        printf("Try '%s --help' for more information.\n", progname);
        goto exit;
    }

    /****************************************/
    /* Argtable post-Parsing Initialisation */
    /****************************************/
    /** initialisation of photon, electron and lasers levels that requiered to be initialised   **/

    if(lasing_levels->count == 0)
    {


        /** for caculate lasers levels we calculate the minimum laser level value
         *  and increment by the mode_sep factor for each mode
         *  te minimu laser level is calculate to center all laser level aroud the center**/
        int level_min;
        int mode_separation=2;
        if(number_of_modes->ival[0] % 2)
        {
            //if MODE_NUMBERS is even the center laser level is in the band middle
            level_min=(energies_levels->ival[0] - 1)/ 2 -(number_of_modes->ival[0] / 2) * mode_separation;
        }else
        {
            //if MODE_NUMBERS is odd the tow centers lasers level are centerer in the band middle
            level_min=(energies_levels->ival[0] - 1)/ 2 -(number_of_modes->ival[0] / 2) * mode_separation/2;
        }
        for(int mode = 0 ; mode < number_of_modes->ival[0] ; mode++)
        {
            lasing_levels -> ival[mode]   = level_min  + mode * mode_separation ;
        }
    }
    for(int emitter = 0 ; emitter < number_of_emitters->ival[0] - initial_CBelectrons_number ->count ; emitter ++)
    {
        initial_CBelectrons_number->ival[emitter]=energies_levels->ival[0]/2;
    }
    /** special case if only one is specify the value is fo the total pump  **/
    if(pump->count == 1)
    {
        double total_pump =  pump->dval[0];
        for(int emitter = 0 ; emitter < number_of_emitters->ival[0] ; emitter ++)
        {
            pump->dval[emitter] = total_pump / number_of_emitters->ival[0];
        }
    }


    /********************************/
    /* Input parameters attribution */
    /********************************/
    /** normal case: take the command line options at face value */
    input_parameters = new Input_parameters(number_of_modes->ival[0], number_of_emitters->ival[0]);

    input_parameters->verbose     = verbose    ->count>0;
    input_parameters->g2_flag     = g2_flag ->count>0;
    input_parameters->histo_flag  = histo_flag ->count>0;
    input_parameters->sd_flag     = sd_flag    ->count>0;
    input_parameters->event_flag  = event_flag ->count>0;
    input_parameters->help_flag   = help_flag  ->count>0;

    input_parameters->pump_phases                    =  pump_phases          ->ival[0];
    input_parameters->SD_point_numbers               = SD_point_numbers      ->ival[0];
    input_parameters->energies_levels                = energies_levels       ->ival[0];

    input_parameters->time                           = max_time              ->dval[0];
    input_parameters->electrical_coupling            = electrical_coupling   ->dval[0];
    input_parameters->beta                           = beta                  ->dval[0];


    for(int seed_iterator = 0 ; seed_iterator < MAX_SEED ; seed_iterator++)
    {
        input_parameters->seed[seed_iterator] = seed         ->ival[seed_iterator];
    }

    for(unsigned int mode = 0 ; mode < input_parameters->number_of_modes ; mode ++)
    {
        input_parameters->initial_photons_number[mode]   = initial_photons_number-> ival[mode];
        input_parameters->cavity_escape_rate[mode]       = cavity_escape_rate    -> dval[mode];
        input_parameters->lasing_levels[mode]            = lasing_levels         -> ival[mode];
    }

    for(unsigned int emitter = 0 ; emitter < input_parameters->number_of_emitters ; emitter ++)
    {
        input_parameters->pump[emitter]      = pump      -> dval[emitter];
        input_parameters->temerature[emitter]= temerature-> dval[emitter];
        input_parameters->initial_CBelectrons_number[emitter]= initial_CBelectrons_number-> ival[emitter];

        for(unsigned int mode = 0 ; mode < input_parameters->number_of_modes ; mode ++)
        {
            input_parameters->cavity_coupling[emitter*input_parameters->number_of_modes + mode] =
                    cavity_coupling-> dval[emitter*input_parameters->number_of_modes + mode];
        }

    }

    input_parameters->output_file             = output_file ->filename[0];

    /** special case of stationnary that should be initialised with time for the defaut value **/
    if(stationnary_time->count==1)
    {
        input_parameters->stationnary_time               = stationnary_time      ->dval[0];
    }
    else
    {
        input_parameters->stationnary_time  = input_parameters->time;
    }

    /********************************/
    /* End of Argtable treatement   */
    /********************************/

    /** @todo check parameters**/

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return input_parameters;


    exit:
        /* deallocate each non-null entry in argtable[] */
        int help = help_flag->count;
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        throw help;


}//IO::input_parameters IO::read_argtable(int argc, char *argv[])

//=====================================================================================================================================
/**
 * @brief intialise all random generator pass in input
 * @param input_seeds seeds passed by user
 * @return seed a seed that it seeding by all input seeds
 */
long unsigned  IO::init_seed(unsigned int *input_seeds)
{
    long unsigned seed =1;

    /** All seed were multiply each other for obtain intial random htat we will use for all others Randoms generators
        for prevent the error of outpass int max value we module them par MAX_INT value **/
    for(int iterator=MAX_SEED-1; iterator>=0 ;iterator--)
    {
        seed=(seed % INT_MAX ) * ( input_seeds[iterator] % INT_MAX);//for not surpass max long value
    }

return seed;
}//long unsigned  IO::init_seed(int *input_seeds)

//=====================================================================================================================================

/*************************/
/*Pure virtual functions */
/*************************/

/********************/
/*Privates function */
/********************/
template <typename T>
ostream & IO::write_table(ostream &flux, T table[], unsigned int size)
{
    if(size ==1)
    {
        flux << table[0];
    }else
    {
        flux << '{' << table[0] ;
        for(unsigned int i=1 ; i< size ; i++)
        {
            flux << ',' << table[i] ;
        }
        flux << '}';
    }
    return flux;
}




//============================================================================================================================

// end of class IO
//============================================================================================================================

IO::Input_parameters::Input_parameters() : Input_parameters(1,1)
{

}
//============================================================================================================================
IO::Input_parameters::Input_parameters(const unsigned int number_of_modes,const unsigned int number_of_emitters)
{
    this->number_of_modes = number_of_modes;
    this->number_of_emitters = number_of_emitters;


    this->lasing_levels             = new unsigned int[number_of_modes];
    this->initial_photons_number    = new unsigned int[number_of_modes];
    this->initial_CBelectrons_number= new unsigned int[number_of_emitters];
    this->initial_VBelectrons_number= new unsigned int[number_of_emitters];
    this->seed                      = new unsigned int[MAX_SEED];

    this->pump                      = new double[number_of_emitters];
    this->temerature                = new double[number_of_emitters];
    this->cavity_escape_rate        = new double[number_of_modes];
    this->cavity_coupling           = new double[number_of_emitters * number_of_modes];

}

IO::Input_parameters::~Input_parameters()
{
    delete[] this->lasing_levels;
    delete[] this->initial_photons_number;
    delete[] this->initial_CBelectrons_number;
    delete[] this->initial_VBelectrons_number;
    delete[] this->seed;
    delete[] this->pump;
    delete[] this->temerature;
    delete[] this->cavity_escape_rate;
    delete[] this->cavity_coupling;


}
