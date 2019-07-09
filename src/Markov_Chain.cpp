/**
 * @header Markov_Chain
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2019/01/03$
 *
 *
 * @brief extention of of the Marmote @a TransitionStructure For Laser simulation
 *
 * it was composed princialyt by  two @a MarmoteBox :
 *
 * - @a photon        :represent the number of Photon for each mode
 * - @a CB_electrons  :reprensent the electron number in conduction band for all @a Emitter
 * - @a VB_electrons  :reprensent the electron number in valence band for all @a Emitter
 *
 * @a MarmoteBox is a @a MarmoteSet with the state space rectangular
 * @a MarmoteSet permit to reprensent a system with only a one dimention with the attribuation of a number for each state
 *
 * then for simulate we need only one @a MarmoteSet so we use a carthesian product of the 3 previous @a MarmoteBox
 * this value in stocked on @a set
 *
 * then for being simulated By MarmoteCore the function TransDistrib is overwrited all the events are inside
 * For using this object it should be used in the object @a MarkovChain then initialisated by the function initial_state
 * Before being simulated.
 */

#include "../inc/Markov_Chain.h"

using namespace std;

//=====================================================================================================================================

/************************/
/*Constructor/Destructor*/
/************************/

Markov_Chain::Markov_Chain (Laser_2D *laser, int seed)
{
    this->laser = laser;
    init_random(seed);
    this->analyse_tool = new Analyse_tool (laser->getMode_number(), laser->getEmitter_number());

}//Markov_Chain::Markov_Chain (Laser_2D laser)

//=====================================================================================================================================

Markov_Chain::~Markov_Chain ()
{
    for(int rng_iterator=0;rng_iterator<RANDOM_NUMBERS;rng_iterator++)
    {
        gsl_rng_free(this->rng_generator[rng_iterator]);
    }
    delete this->analyse_tool;

}//Markov_Chain::~Markov_Chain ()

//=====================================================================================================================================

/******************/
/*Public function */
/******************/

std::ostream& Markov_Chain::print_event(std::ostream& flux, const Transition *transition, const double date)
{


    flux  <<  right << std::setw(6) << date                             <<  ",";
    this->print_state(flux, &transition->state);
    flux                  <<  ",";
    flux  << std::setw(6) << transition->name                           <<  ",";
    flux  << std::setw(10)<< date ;

    return flux;

}//std::ostream& Markov_Chain::print_event(std::ostream& flux, int size, int *dates, int *states, int event_number)

//=====================================================================================================================================

std::ostream& Markov_Chain::print_header(std::ostream& flux)
{
    flux << "event_num, state,      time";
    for(unsigned int mode = 0 ; mode < this->laser->getMode_number() ; mode ++)
    {
        flux << ", photon";
        if(this->laser->getMode_number() > 1)
        {
           flux << "_" << mode + 1;
        }
    }
    for(unsigned int emitter_num = 0 ; emitter_num < this->laser->getEmitter_number() ; emitter_num ++)
    {
        flux << ", CBelectron";
        if(this->laser->getEmitter_number() > 1)
        {
           flux << "_" << emitter_num + 1;
        }
        flux << ", VBelectron";
        if(this->laser->getEmitter_number() > 1)
        {
           flux << "_" << emitter_num + 1;
        }
    }

    flux << endl;
    return flux;
}//std::ostream& Markov_Chain::print_header(std::ostream& flux)

//=====================================================================================================================================


double Markov_Chain::rates_sum( std::vector<Transition> *transition_structure)
{
    vector<Markov_Chain::Transition>::iterator iterator; //iterator overs transitions
    double sum =0.0;
    for(iterator = transition_structure->begin(); iterator!=transition_structure->end(); ++iterator)
        {

            sum += iterator->rate;
        }

    return sum;
}//double Markov_Chain::rates_sum( std::vector<Transition> *transition_structure)



//=====================================================================================================================================

State * Markov_Chain::simulate(double max_time, State *initial_state)
{

    double instant_time = 0.0;
    double waiting_time;
    double weight_sum;
    double event_selector;
    double event_mark;
    std::vector<Markov_Chain::Transition> *transitions = new std::vector<Markov_Chain::Transition>();
    std::vector<Markov_Chain::Transition>::iterator iterator;  //iterator overs transitions
    Markov_Chain::Transition next_transition;

    State current_state, next_state;

    current_state = *initial_state;

    /** loop begining   **/
    while(instant_time < max_time)
    {

        /** Transition structure  from the state **/
        transition_structure(transitions, &current_state);


        /** Sum of the weights **/
        weight_sum = rates_sum(transitions) ;


        /** Calul of the next event time with an exponential distribution**/
        waiting_time = -log(gsl_rng_uniform_pos( rng_generator[0] ) ) / weight_sum;

        if((instant_time + waiting_time) < max_time)
        {

            /** Selection of the next event with a random selection in a set of all possible events **/
            event_selector = weight_sum * gsl_rng_uniform( rng_generator[1]);
            //Selection before application offer possibility to export exact trajectory off process wih all events.


            /** Selection of the next event at the position given by event_selector **/
            event_mark =0.0;
            for(iterator = transitions->begin();
                (event_mark<event_selector) && ( iterator != transitions->end() ) ;
                iterator++)
            {
                next_transition = *iterator;
                event_mark += next_transition.rate;
            }
            next_state = next_transition.state;


            /** analyse **/
            trajectory_analysis(&current_state, &next_transition, waiting_time);

            /** event application **/
            current_state = next_state;

        }
        transitions->clear();
        instant_time += waiting_time;
    }

    /** Correct the waiting as the last event not occur **/
    waiting_time = max_time - instant_time + waiting_time;


    // At this point the time mark in correlation differ from instant time so the reatribuaton of waiting time permit to not segmentation faults
    update_stat(&current_state, this->getAnalyse_tool()->correlation_interval_duration -this->getAnalyse_tool()->correlation_time_mark );


    post_calcul_analyse( max_time );

    *initial_state = current_state;

    delete transitions;
    return initial_state;

}//State * Markov_Chain::simulate(double max_time, State *initial_state)d

//=====================================================================================================================================

/*************************/
/*Pure virtual functions */
/*************************/

/********************/
/*Privates function */
/********************/

void Markov_Chain::init_state(int *photon_prev_state, int *photon_next_state,
                                            int *CBelec_prev_state, int *CBelec_next_state , int *VBelec_prev_state, int *VBelec_next_state)
{
    /** The 2 previous states are copied in the 2 next states **/
    std::copy(photon_prev_state, photon_prev_state + this->laser->getMode_number(),photon_next_state );
    std::copy(CBelec_prev_state, CBelec_prev_state + this->laser->getEmitter_number(),CBelec_next_state );
    std::copy(VBelec_prev_state, VBelec_prev_state + this->laser->getEmitter_number(),VBelec_next_state );
}//void Markov_Chain::init_state(int *, int * int *,int *, int*, int* )

//=====================================================================================================================================

std::ostream& Markov_Chain::print_state(std::ostream& flux, const State *state)
{

    for(unsigned int mode = 0 ; mode < this->laser->getMode_number() ; mode ++)
    {
        flux << "," << std::setw(9) << state->getPhotons(mode) ;

    }
    for(unsigned int emitter_num = 0 ; emitter_num < this->laser->getEmitter_number() ; emitter_num ++)
    {
        flux << "," << std::setw(9) << state->getCB_electrons(emitter_num) ;
        flux << "," << std::setw(9) << state->getVB_electrons(emitter_num) ;
    }
    flux << endl;

    return flux;
}//std::ostream& Markov_Chain::print_state(std::ostream& flux, int state)

//=====================================================================================================================================

std::vector<Markov_Chain::Transition> *Markov_Chain::transition_structure(
        std::vector<Markov_Chain::Transition> * transitions, State  * const current_state)
{
    unsigned int electron_max = Electron_presence::getEnergy_level_number();// the number of electron at maximum is the band full hence
    ///@todo change electron max

    //return values


    Electron_presence *electron_presence;
    double CB_lasing_lvl_electron_pres;
    double VB_lasing_lvl_hole_pres;
    double rate;




    //------------------------//
    // Event : Out of cavity  //
    //------------------------//
    for( unsigned int mode = 0 ; mode < this->laser->getMode_number() ; mode ++ )
    {
        if(current_state->getPhotons(mode) > 0){

            State next_state = *current_state;//previous state copy


            /** The rates associated with the transition m_i -> m_i - 1 is α * m **/
            rate=this->laser->getCavity_escape_rate(mode) * current_state->getPhotons(mode);


            /** give the transition name    **/
            std::ostringstream name;
            name << "O" << mode;


            /** rates application           **/
            next_state.drop_photon(mode);//next state is the previous state with a photon lower


            /** update the next index       **/
            transitions->push_back({name.str(), rate, next_state});
        }
    }

    for( unsigned int emitter_num = 0 ; emitter_num < this->laser->getEmitter_number() ; emitter_num ++ )
    {

        QW_emitter *emitter = static_cast<QW_emitter*>(this->laser->getEmitter(emitter_num));
        unsigned int CB_electron_number = current_state->getCB_electrons(emitter_num);
        unsigned int VB_holes_number = Electron_presence::getEnergy_level_number() - current_state->getVB_electrons(emitter_num);
        electron_presence = emitter->getElectron_presence();

        //---------------//
        // Event : Pump  //
        //---------------//
        if((CB_electron_number < electron_max) && (VB_holes_number < electron_max))
        {

            State next_state = *current_state;//previous state copy


            /** The rates associated with the pump transition n_i -> n_i + 1 is J * pump_lvl_occ(n)**/
            rate = emitter->getPumping_local_rate()
                    * (1 - electron_presence->getPumping_lvl_prob(CB_electron_number))
                    * (1 - electron_presence->getPumping_lvl_prob(VB_holes_number));


            /** give the transition name    **/
            std::ostringstream name;
            name << "J" << emitter_num;


            /** rates application           **/
            next_state.add_CB_electron(emitter_num);//next state is the previous state with a electron more in the emitter
            next_state.drop_VB_electron(emitter_num);


            /** update the next index       **/
            transitions->push_back({name.str(), rate, next_state});

        }

        //--------------------------------//
        // Event : CB Electron Transfert  //
        //--------------------------------//
        if(CB_electron_number > 0)
        {

            for (int direction = 0 ; direction < Emitter::DIRECTION_NUMBER ; direction ++)
            {
                State next_state = *current_state;//previous state copy
                Emitter *neighbour_emitter = emitter->getNeighboring_emitter(direction);

                /** If the transfert is possible **/
                if(
                        neighbour_emitter &&
                        current_state->getCB_electrons(neighbour_emitter->getEmitter_num())
                        < electron_max
                ){

                    /** The rates associated with the electron tranferhe t is n * T * K**/
                    rate = emitter->getMetropolis_criteria(direction)
                            * CB_electron_number * this->laser->getElectical_coupling();


                    /** give the transition name    **/
                    std::ostringstream name;
                    name << "C" << emitter_num << "_" << neighbour_emitter->getEmitter_num();


                    /** rates application           **/
                    next_state.drop_CB_electron(emitter_num);//next state is the previous state with a electron less in the emitter

                    next_state.add_CB_electron(neighbour_emitter->getEmitter_num());
                    //and a photon more in the other emitter


                    /** update the next index       **/
                    transitions->push_back({name.str(), rate, next_state});
                }
            }
        }

        //--------------------------------//
        // Event : VB Electron Transfert  //
        //--------------------------------//
        if(current_state->getVB_electrons(emitter_num) > 0)
        {

            for (int direction = 0 ; direction < Emitter::DIRECTION_NUMBER ; direction ++)
            {
                State next_state = *current_state;//previous state copy
                Emitter *neighbour_emitter = emitter->getNeighboring_emitter(direction);

                /** If the transfert is possible **/
                if(
                        neighbour_emitter &&
                        current_state->getVB_electrons(neighbour_emitter->getEmitter_num())
                        < electron_max
                ){
                    /** The rates associated with the electron tranferhe t is n * T * K**/
                    rate = emitter->getMetropolis_criteria(direction)
                            * current_state->getVB_electrons(emitter_num) * this->laser->getElectical_coupling();


                    /** give the transition name    **/
                    std::ostringstream name;
                    name << "V" << emitter_num << "_" << neighbour_emitter->getEmitter_num();


                    /** rates application           **/
                    next_state.drop_VB_electron(emitter_num);//next state is the previous state with a electron less in the emitter

                    next_state.add_VB_electron(neighbour_emitter->getEmitter_num());
                    //and a photon more in the other emitter


                    /** update the next index       **/
                    transitions->push_back({name.str(), rate, next_state});
                }
            }
        }

        for(unsigned int mode=0 ; mode < this->laser->getMode_number() ; mode++ )
        {

            CB_lasing_lvl_electron_pres = emitter->getElectron_presence()->getLasing_lvl_prob(CB_electron_number, mode);
            VB_lasing_lvl_hole_pres     = emitter->getElectron_presence()->getLasing_lvl_prob(VB_holes_number, mode);

            //----------------------------//
            // Event : Coherent emission  //
            //----------------------------//
            if( CB_electron_number > 0 && VB_holes_number > 0)
            {
                State next_state = *current_state;//previous state copy


                /** The rates associated with the coherent emission in n_L² * (m + β) **/
                rate = CB_lasing_lvl_electron_pres * VB_lasing_lvl_hole_pres
                        *  ( current_state->getPhotons(mode) + this->laser->getBeta() );


                /** give the transition name    **/
                std::ostringstream name;
                name << "E" << mode << "_" << emitter_num;


                /** rates application           **/
                next_state.drop_CB_electron(emitter_num);//next state is the previous state with a electron lower in the CB of the emitter

                next_state.add_VB_electron(emitter_num);

                next_state.add_photon(mode);      //and a photon more


                /** update the next index       **/
                transitions->push_back({name.str(), rate, next_state});
            }

            //------------------------------//
            // Event : Coherent absorbtion  //
            //------------------------------//
            if(current_state->getPhotons(mode) > 0 && CB_electron_number <  electron_max && VB_holes_number <  electron_max)
            {
                State next_state = *current_state;//previous state copy


                /** The rates associated with the coherent absorbtion is (1- n_L)² * m **/
                rate =
                        (1 - CB_lasing_lvl_electron_pres) * (1 - VB_lasing_lvl_hole_pres) * current_state->getPhotons(mode);


                /** give the transition name    **/
                std::ostringstream name;
                name << "A" << mode << "_" << emitter_num;


                /** rates application           **/
                next_state.add_CB_electron(emitter_num);//next state is the previous state with a electron more in the emitter

                next_state.drop_VB_electron(emitter_num);

                next_state.drop_photon(mode);      //and a photon lower


                /** update the next index       **/
                transitions->push_back({name.str(), rate, next_state});
            }

            //------------------------------//
            // Event : Uncoherent emission  //
            //------------------------------//
            if( CB_electron_number >  0 && VB_holes_number > 0 )
            {
                State next_state = *current_state;//previous state copy


                /** The rates associated with the uncoherent emission is n_L² * (1 - β) **/
                rate =  CB_lasing_lvl_electron_pres * VB_lasing_lvl_hole_pres * (1 - this->laser->getBeta() );


                /** give the transition name    **/
                std::ostringstream name;
                name << "U" << mode << "_" << emitter_num;


                /** rates application           **/
                next_state.drop_CB_electron(emitter_num);//next state is the previous state with a electron lower in the emitter

                next_state.add_VB_electron(emitter_num);


                /** update the next index       **/
                transitions->push_back({name.str(), rate, next_state});
            }
        }
    }

    return transitions;
}//Rate_array Markov_Chain::transition_probabilities(int i)


//============================================================================================================================

void Markov_Chain::init_random(long unsigned int seed)
{

    /** Initialisation of initial Random generator with the seed  **/
    gsl_rng * initial_random;
    initial_random = gsl_rng_alloc( gsl_rng_rand);
    gsl_rng_set( initial_random, seed);

    /** allocation for our random generator that we will used in the program       **/
    for(int iterator=0; iterator < RANDOM_NUMBERS ; iterator++)
    {
        this->rng_generator[iterator] = gsl_rng_alloc( gsl_rng_mt19937);
        gsl_rng_set( this->rng_generator[iterator], gsl_rng_get( initial_random));
    }

    /** Initial Random is Free      **/
    gsl_rng_free( initial_random);

}//void init_random(int seed)

//============================================================================================================================

void Markov_Chain::trajectory_analysis(const State *current_state, const Transition *transition, const double waiting_time)
{
    if(this->analyse_tool->analyse_flag)
    {

        /** update the event count **/
        this->getAnalyse_tool()->event_counter[transition->name]++;


        /**  update the photons and electrons statistic            **/
        this->update_stat(current_state, waiting_time);


        /** print event event **/
        if(this->analyse_tool->event_flag)
        {
            if(event_file)
            {
                this->print_event(event_file, transition, waiting_time);
            }else
            {
                       cerr << "ERREUR: Unable to open the evnt file : " << endl;
            }

        }

        /** calculate shutdowns     **/
        for(unsigned int mode=0; mode < this->laser->getMode_number() ; mode++)
        {
            if((transition->state.getPhotons(mode) == 0) && transition->name.at(0)=='E' )
            {
                this->getAnalyse_tool()->shutdown[mode]++;
            }
        }

    }

}//void Markov_Chain::trajectory_analysis(Markov_Chain::Transition transition, double waiting_time)


//============================================================================================================================

void Markov_Chain::update_stat(const State *state, const double waiting_time)
{

    if(this->analyse_tool->analyse_flag)
    {
        int photons_total=0;// photon total for variance update


        /** for each mode update variance and averages values       **/
        for(unsigned int mode=0 ; mode < this->getLaser()->getMode_number() ; mode++)
        {
            this->getAnalyse_tool()->ave_photon[mode] += waiting_time * state->getPhotons(mode);

            this->getAnalyse_tool()->var_photon[mode] += waiting_time * state->getPhotons(mode) * state->getPhotons(mode);


            photons_total += state->getPhotons(mode);


        }

        /** total variance update   **/
        this->getAnalyse_tool()->var_total_photon += waiting_time   *    photons_total * photons_total;

        /** aerage electron update  **/
        for(unsigned int laser_num=0 ; laser_num < this->getLaser()->getEmitter_number() ; laser_num++)
        {
            this->getAnalyse_tool()->ave_CB_electron[laser_num] += waiting_time * state->getCB_electrons(laser_num);

            this->getAnalyse_tool()->var_CB_electron[laser_num] +=
                    waiting_time * state->getCB_electrons(laser_num) * state->getCB_electrons(laser_num);

            this->getAnalyse_tool()->ave_VB_electron[laser_num] += waiting_time * state->getVB_electrons(laser_num);

            this->getAnalyse_tool()->var_VB_electron[laser_num] +=
                    waiting_time * state->getVB_electrons(laser_num) * state->getVB_electrons(laser_num);
        }


        /** histogramm of photon distribution updatate  **/
        if(this->analyse_tool->histo_flag)
        {
            calcul_histogramme(state, waiting_time);
        }

        /** correlation update                          **/
        if(this->analyse_tool->g2_flag){
            correlation_update( state, waiting_time);
        }
    }

}//void Markov_Chain::update_stat(Markov_Chain::Transition transition, double waiting_time)



//============================================================================================================================


/**
 * @brief post_calcul_analyse finalyse the calcul of variance and average values
 *
 * 2nd order correlation is calculated by shift the partial averages and calcul with the formula
 * g2(t)=<m(T) * m(T+t)>/<m(T)> * < m(T+t)>
 * with the stationnary asubption <m(T)> = < m(T+t)>
 *
 * for optimisation we use the division at the end of the loop, this occur here
 * @param tools pointer of @a Analyse_tools to update
 * @param end_time of the simulation
 * @param analyse_flag flag for knowing f second order correlation should be calculated
 */
void Markov_Chain::post_calcul_analyse(const double end_time)
{
    if(this->analyse_tool->analyse_flag)
    {
        int unsigned correlation_iterator, correlation_time_iterator;
        int total_photons=0;

        Analyse_tool *tool = this->getAnalyse_tool();

        unsigned const int number_of_mode = this->getLaser()->getMode_number();
        unsigned const int number_of_emitter = this->getLaser()->getEmitter_number();


        double *square_average_photon = new double[number_of_mode];
        int unsigned correlation_max_value = floor(
                     tool->correlation_max_value / tool->correlation_interval_duration ) +1;


        // number of correlation values wanted
        /** Time normalisation for average and variances values  **/
        for(unsigned int mode = 0 ; mode < number_of_mode ; mode++)
        {
            /** photons partial stat upadate **/
            tool->ave_photon[mode] /= end_time;

            tool->var_photon[mode] = tool->var_photon[mode] / end_time -
                    tool->ave_photon[mode] * tool->ave_photon[mode];

            total_photons += tool->ave_photon[mode];

            square_average_photon[mode] = tool->ave_photon[mode] * tool->ave_photon[mode];

        }


        tool->var_total_photon = tool->var_total_photon / end_time - total_photons * total_photons;



        for(unsigned int laser_num = 0 ; laser_num < number_of_emitter ; laser_num++)
        {
             /** CB electorns partial stat upadate **/
            tool->ave_CB_electron[laser_num] /= end_time;

            tool->var_CB_electron[laser_num] = tool->var_CB_electron[laser_num] / end_time -
                    tool->ave_CB_electron[laser_num] * tool->ave_CB_electron[laser_num];


            /** VB electorns partial stat upadate **/
            tool->ave_VB_electron[laser_num] /= end_time;

            tool->var_VB_electron[laser_num] = tool->var_VB_electron[laser_num] / end_time -
                    tool->ave_VB_electron[laser_num] * tool->ave_VB_electron[laser_num];




        }


        /** second order correlation MAJ    **/
        if(this->analyse_tool->g2_flag){

            //at the end of simulation tool->correlation_interval_number + 1 is the number of partials means intervals
            for (correlation_iterator = 0 ;
                 correlation_iterator < tool->correlation_interval_number  ;
                 correlation_iterator++)
            {
                for(unsigned int mode = 0 ; mode < number_of_mode ; mode++)
                {
                    tool->partial_average_photons[correlation_iterator][mode] /=
                            tool->correlation_interval_duration;
                    // The last group is wrongly calculated it most be a modulo value for have a real value
                    // but the lasts values are ignored in g2 calculs so it doesn't matters
                }
            }

            for (correlation_time_iterator = 0 ; correlation_time_iterator < correlation_max_value ; correlation_time_iterator++)
            {
                for(unsigned int mode = 0 ; mode < number_of_mode ; mode++)
                {
                    for (correlation_iterator = 0 ; correlation_iterator < tool->correlation_interval_number - correlation_time_iterator ; correlation_iterator++)
                    {

                        tool->second_order_correlation[correlation_time_iterator][mode] +=
                                tool->partial_average_photons[correlation_iterator][mode] *
                                tool->partial_average_photons[correlation_iterator
                                + correlation_time_iterator][mode];

                    }

                    tool->second_order_correlation[correlation_time_iterator][mode] /=
                            (square_average_photon[mode] * correlation_iterator );
                }

            }
        }

        delete[] square_average_photon;
    }

}


void Markov_Chain::calcul_histogramme(const State *state, const double waiting_time)
{
    cerr << "non implemented" << state->getPhotons(0) << waiting_time ;
}

//============================================================================================================================

void Markov_Chain::correlation_update(const State *state, const double waiting_time)
{
    cerr << "non implemented" << state->getPhotons(0) << waiting_time ;
}



//============================================================================================================================

// end of class Markov_Chain
//============================================================================================================================





