/**
 * @header Laser_transition_structure
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
 * - @a CB_electrons  :reprensent the electron number in conduction band for all @a Elementary_laser
 *
 * @a MarmoteBox is a @a MarmoteSet with the state space rectangular
 * @a MarmoteSet permit to reprensent a system with only a one dimention with the attribuation of a number for each state
 *
 * then for simulate we need only one @a MarmoteSet so we use a carthesian product of the 2 previous @a MarmoteBox
 * this value in stocked on @a set
 *
 * then for being simulated By MarmoteCore the function TransDistrib is overwrited all the events are inside
 * For using this object it should be used in the object @a MarkovChain then initialisated by the function initial_state
 * Before being simulated.
 */

#include "../inc/LaserTransitionStructure.h"

using namespace std;

//=====================================================================================================================================

/************************/
/*Constructor/Destructor*/
/************************/

Laser_transition_structure::Laser_transition_structure (Laser_2D *laser)
{
   /** Parameters for TransitionStructure **/
   this->type_  = CONTINUOUS;
   this->uniformization_rate_ = 0.0;

   this->laser = laser;
   this->n_dims = laser->getMode_number() + laser->getElementary_laser_number();

   this->electron_max = Electron_presence::getEnergy_level_number() + 1;
    // the number of electron at maximum is the band full hence +1

   /** Initialisation of the state space  with the two @a MarmoteBox photon and CB_electrons **/
   int *photon_dim_size   = new int[laser->getMode_number()];
   int *electron_dim_size = new int[laser->getElementary_laser_number()];

   for(unsigned int dimention = 0 ; dimention < laser->getMode_number() ; dimention ++)
   {
       photon_dim_size[dimention] = Laser_transition_structure::photon_max;
       //the limit photon_max is just convenance the Marmotecore permit infinit space, but forbid staionnary distribution
   }
   this->photon =       new MarmoteBox(this-> laser->getMode_number(), photon_dim_size);

   /** @todo The case for QW and QD should be develop **/
   if(laser->getType()==Laser_2D::QW)
   {
       for(unsigned int dimention = 0 ; dimention < laser->getElementary_laser_number() ; dimention ++)
       {
           electron_dim_size[dimention] = this->electron_max;

       }
   }
   this->CB_electrons = new MarmoteBox(laser->getElementary_laser_number(), electron_dim_size);

   /** creation of the carthesian product of the two @a MarmoteBox photon and CB_electrons  stocked in Set **/
   MarmoteSet *liste[2];
   liste[0] = this->photon;
   liste[1] = this->CB_electrons;

   this->set = new MarmoteSet(liste, 2, MarmoteSet::PRODUCT);

   this->orig_size_ = set->Cardinal();
   this->dest_size_ = set->Cardinal();

   /** free memory used for the state space **/
   delete[] photon_dim_size;
   delete[] electron_dim_size;

}//Laser_transition_structure::Laser_transition_structure (Laser_2D laser)

//=====================================================================================================================================

Laser_transition_structure::~Laser_transition_structure ()
{
    delete this->photon;
    delete this->CB_electrons;
    delete this->set;
}//Laser_transition_structure::~Laser_transition_structure ()

//=====================================================================================================================================

/******************/
/*Public function */
/******************/

int Laser_transition_structure::myGetIndex(int *photons, int *electrons)
{
    int result;

    /** create a temporaly state space for cumulate the 2 state spaces **/
    int *state = new int[this->laser->getMode_number() + this->laser->getElementary_laser_number()];

    /** copy all values in the temporaly state **/
    std::copy(photons, photons + this->laser->getMode_number(), state);
    std::copy(electrons, electrons + this->laser->getElementary_laser_number(), state + this->laser->getMode_number());

    /** get the index of the state space before deleting it **/
    result =  this->set->Index(state);
    delete[]  state;

    return result;
}//int Laser_transition_structure::myGetIndex( int *photonBox, int *ElectronBox)

//=====================================================================================================================================

DiscreteDistribution *Laser_transition_structure::initial_state(int *photon_distr, int *CB_electrons_distr)
{
   int index = myGetIndex(photon_distr, CB_electrons_distr);
   DiscreteDistribution *Distr = new DiracDistribution(index);
   return Distr;
}//DiscreteDistribution *Laser_transition_structure::initial_state(int *photon_distr, int *CB_electrons_distr)

//=====================================================================================================================================

void Laser_transition_structure::decode_laser_state(int *photon, int *CB_electrons , int state_num)
{
    int* prev_state         = new int[this->n_dims];

    /** DecodeState permit with a int in index to obtain a array of int that represent the state **/
    this->set->DecodeState(state_num , prev_state);

    /** Then the total state is divisate the first values are for the photons the other part are the other **/
//    photon  = prev_state;//It's a tricky part the first part of the table is for photon
//    CB_electrons = &prev_state[this->laser->getMode_number()];//The second part begin by the next index so the laser number
    int* limit = prev_state + this->laser->getMode_number();

    std::copy(prev_state, limit , photon);
    std::copy(limit, limit + this->laser->getElementary_laser_number() , CB_electrons);

    delete[](prev_state);
}//void Laser_transition_structure::decode_laser_state(int *photon, int *CB_electrons , int state_num)

//=====================================================================================================================================

ostream &Laser_transition_structure::print_trajectory(ostream &flux, SimulationResult *result)
{
    int size = result->trajectorySize();


    this->print_header(flux);

    for(int state =0; state < size ; state ++)
    {
     this->print_event(flux,result, state);
    }
    return cout;
}

//=====================================================================================================================================

std::ostream& Laser_transition_structure::print_event(std::ostream& flux, SimulationResult *result, int event_number)
{


    int size = result->trajectorySize();
    double date;
    int* photons_number  = new int[this->laser->getMode_number()];
    int* CB_electrons_number = new int[this->laser->getElementary_laser_number()];



    if(event_number < size){

        decode_laser_state(photons_number, CB_electrons_number, result->states()[event_number]);

        date = result->dates()[event_number];

        flux << '[' << event_number << "],\t" << result->states()[event_number] << ",\t" << date << ",\t" << 0;

        for(unsigned int mode = 0 ; mode < this->laser->getMode_number() ; mode ++)
        {
            flux << ",\t" << photons_number[mode] ;

        }
        for(unsigned int laser_num = 0 ; laser_num < this->laser->getElementary_laser_number() ; laser_num ++)
        {
            flux << ",\t" << CB_electrons_number[laser_num] ;

        }
        flux << endl;
    }

    delete[](photons_number);
    delete[](CB_electrons_number);

    return flux;

}//std::ostream& Laser_transition_structure::print_event(std::ostream& flux, SimulationResult *result, int event_number)

//=====================================================================================================================================

std::ostream& Laser_transition_structure::print_header(std::ostream& flux)
{
    flux << "event_num, state, time, duration";
    for(unsigned int mode = 0 ; mode < this->laser->getMode_number() ; mode ++)
    {
        flux << ", photon";
        if(this->laser->getMode_number() > 1)
        {
           flux << "_" << mode + 1;
        }
    }
    for(unsigned int laser_num = 0 ; laser_num < this->laser->getElementary_laser_number() ; laser_num ++)
    {
        flux << ", electron";
        if(this->laser->getElementary_laser_number() > 1)
        {
           flux << "_" << laser_num + 1;
        }
    }

    flux << endl;
    return flux;
}//std::ostream& Laser_transition_structure::print_header(std::ostream& flux)

//=====================================================================================================================================

/*************************/
/*Pure Virtuals function */
/*************************/

DiscreteDistribution *Laser_transition_structure::TransDistrib(int i)
{
    DiscreteDistribution * result;    

    Rate_array *trans_probas = transition_probabilities(i);

    for(int index = 0 ; index < trans_probas->getSize() ; index ++)
    {
        trans_probas->setRate(index, trans_probas->getRate(index) / trans_probas->rateSum());
    }
    result = new DiscreteDistribution(trans_probas->getSize(), trans_probas->getValues(), trans_probas->getRates());

    delete trans_probas;

    return result;

}// DiscreteDistribution *Laser_transition_structure::TransDistrib(int i)

//=====================================================================================================================================

double Laser_transition_structure::getEntry(int i, int j)
{
    double result;
    Rate_array* trans_probas = transition_probabilities(i);

    if(i==j)//by convention the rate associated for the same state is the minus sum of all others rates
    {
        result = - trans_probas->rateSum();
    }
    else
    {
        result = trans_probas->rate(j);
    }

    delete trans_probas;
    return result;
}//double Laser_transition_structure::getEntry(int i, int j)

//=====================================================================================================================================

/**
 * All these pure virtual Functions Are Not Implemented
 * @todo To Implement
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool Laser_transition_structure::setEntry(int i, int j, double val) {cerr << "Warning Not Implemented setEntry" << endl; return false;}
int Laser_transition_structure::getNbElts(int i)  {cerr << "Warning Not Implemented getNbElts" << endl; return 0;}
int Laser_transition_structure::getCol(int i, int k)  {cerr << "Warning Not Implemented getCol" << endl; return 0;}
double Laser_transition_structure::getEntryByCol(int i, int k)  {cerr << "Warning Not Implemented getEntryByCol" << endl;return 0;}
double Laser_transition_structure::RowSum(int i)  {cerr << "Warning Not Implemented RowSum" << endl;return 0; }
TransitionStructure* Laser_transition_structure::Copy()  {cerr << "Warning Not Implemented Copy" << endl; return nullptr;}
TransitionStructure* Laser_transition_structure::Uniformize()  {cerr << "Warning Not Implemented Uniformize" << endl; return nullptr;}
TransitionStructure* Laser_transition_structure::Embed()  {cerr << "Warning Not Implemented Embed" << endl; return nullptr;}
void Laser_transition_structure::EvaluateValue(double* v, double* res)  {cerr << "Warning Not Implemented EvaluateValue" << endl; }
#pragma GCC diagnostic pop

//=====================================================================================================================================

/********************/
/*Privates function */
/********************/

void Laser_transition_structure::init_state(int *photon_prev_state, int *photon_next_state,
                                            int *elec_prev_state,int *elec_next_state )
{
    /** The 2 previous states are copied in the 2 next states **/
    std::copy(photon_prev_state, photon_prev_state + this->laser->getMode_number(),photon_next_state );
    std::copy(elec_prev_state, elec_prev_state + this->laser->getElementary_laser_number(),elec_next_state );
}//void Laser_transition_structure::init_state(int *, int * int *,int * )

//=====================================================================================================================================

Rate_array *Laser_transition_structure::transition_probabilities(int i)
{
    //states manipulation
    int* photon_prev_state= new int[this->laser->getMode_number()] ;
    int* CB_elec_prev_state = new int[this->laser->getElementary_laser_number()];


    int* photon_next_state  = new int[this->laser->getMode_number()];
    int* CB_elec_next_state = new int[this->laser->getElementary_laser_number()];


    //return values
    double *rate=new double[this->n_dims * 9];//For all dimentions the number of transition is at max 9
    double *next_index=new double[this->n_dims* 9];
    int trans_indice = 0;//number of the transition to add the 2 tables upper

    /** decoding the state and add it in photon_prev_state and CB_elec_prev_state **/
    this->decode_laser_state(photon_prev_state, CB_elec_prev_state, i);

    //------------------------//
    // Event : Out of cavity  //
    //------------------------//
    for( unsigned int mode = 0 ; mode < this->laser->getMode_number() ; mode ++ )
    {
        if(photon_prev_state[mode] > 0){

            init_state(photon_prev_state, photon_next_state, CB_elec_prev_state, CB_elec_next_state);//previous state copy

            /** The rates associated with the transition m_i -> m_i - 1 is α * m **/
            rate[trans_indice] = this->laser->getCavity_escape_rate(mode) * photon_prev_state[mode];
            photon_next_state[mode]--;//next state is the previous state with a photon lower

            /** update the next index       **/
            next_index[trans_indice++] = myGetIndex(photon_next_state, CB_elec_next_state);
        }
    }

    for( unsigned int laser_num = 0 ; laser_num < this->laser->getMode_number() ; laser_num ++ )
    {

        unsigned int electron_number = CB_elec_prev_state[laser_num];
        QW_elementary_laser *elem_laser = static_cast<QW_elementary_laser*>(this->laser->getElementary_laser(laser_num));
        double pump_electron_presence = elem_laser->getElectron_presence()->getPumping_lvl_prob(electron_number);
        double lasing_lvl_electron_pres;
        double temperature_coefficient=1.0;


        //---------------//
        // Event : Pump  //
        //---------------//
        if(electron_number < this->electron_max)
        {
            init_state(photon_prev_state, photon_next_state, CB_elec_prev_state, CB_elec_next_state);//previous state copy

            /** The rates associated with the pump transition n_i -> n_i + 1 is J * pump_lvl_occ(n)**/
            rate[trans_indice] = elem_laser->getPumping_local_rate() * pump_electron_presence;
            CB_elec_next_state[laser_num]++;//next state is the previous state with a electron more in the elementary laser

            /** update the next index       **/
            next_index[trans_indice++] = myGetIndex(photon_next_state, CB_elec_next_state);

        }

        //-----------------------------//
        // Event : Electron Transfert  //
        //-----------------------------//
        if(electron_number < this->electron_max)
        {
            init_state(photon_prev_state, photon_next_state, CB_elec_prev_state, CB_elec_next_state);//previous state copy

            for (int direction = 0 ; direction < Elementary_laser::DIRECTION_NUMBER ; direction ++)
            {
                /** If T **/
                if(elem_laser->getNeighboring_laser(direction) &&
                        CB_elec_prev_state[elem_laser->getNeighboring_laser(direction)->getLaser_num()])
                {
                    /** The rates associated with the electron tranferhe t is n * T * K**/
                    rate[trans_indice] = elem_laser->getTemperature() * electron_number * temperature_coefficient;

                    CB_elec_next_state[laser_num]--;//next state is the previous state with a electron less in the elementary laser
                    CB_elec_next_state[elem_laser->getNeighboring_laser(direction)->getLaser_num()]++;//and a photon more in the other elem. laser

                    /** update the next index       **/
                    next_index[trans_indice++] = myGetIndex(photon_next_state, CB_elec_next_state);
                }
            }
        }

        for(unsigned int mode=0 ; mode < this->laser->getMode_number() ; mode++ )
        {
            lasing_lvl_electron_pres = elem_laser->getElectron_presence()->getLasing_lvl_prob(electron_number, mode);

            //----------------------------//
            // Event : Coherent emission  //
            //----------------------------//
            if(photon_prev_state[mode] < this->photon_max && electron_number > 0)
            {
                init_state(photon_prev_state, photon_next_state, CB_elec_prev_state, CB_elec_next_state);//previous state copy

                /** The rates associated with the coherent emission in n_L² * (m + β) **/
                rate[trans_indice] = lasing_lvl_electron_pres * lasing_lvl_electron_pres *
                        ( photon_prev_state[mode] + this->laser->getBeta() );
                CB_elec_next_state[laser_num]--;//next state is the previous state with a electron lower in the elementary laser
                photon_next_state[mode]++;      //and a photon more

                /** update the next index       **/
                next_index[trans_indice++] = myGetIndex(photon_next_state, CB_elec_next_state);
            }

            //------------------------------//
            // Event : Coherent absorbtion  //
            //------------------------------//
            if(photon_prev_state[mode] > 0 && electron_number <  this->electron_max)
            {
                init_state(photon_prev_state, photon_next_state, CB_elec_prev_state, CB_elec_next_state);//previous state copy

                /** The rates associated with the coherent absorbtion is (1- n_L)² * m **/
                rate[trans_indice] = (1 - lasing_lvl_electron_pres) * (1 - lasing_lvl_electron_pres) * photon_prev_state[mode];
                CB_elec_next_state[laser_num]++;//next state is the previous state with a electron more in the elementary laser
                photon_next_state[mode]--;      //and a photon lower

                /** update the next index       **/
                next_index[trans_indice++] = myGetIndex(photon_next_state, CB_elec_next_state);
            }

            //------------------------------//
            // Event : Uncoherent emission  //
            //------------------------------//
            if( electron_number >  0)
            {
                init_state(photon_prev_state, photon_next_state, CB_elec_prev_state, CB_elec_next_state);//previous state copy

                /** The rates associated with the uncoherent emission is n_L² * (1 - β) **/
                rate[trans_indice] =  lasing_lvl_electron_pres * lasing_lvl_electron_pres * (1 - this->laser->getBeta() );
                CB_elec_next_state[laser_num]--;//next state is the previous state with a electron lower in the elementary laser


                /** update the next index       **/
                next_index[trans_indice++] = myGetIndex(photon_next_state, CB_elec_next_state);
            }
        }
    }

    Rate_array *result = new Rate_array(trans_indice, next_index, rate);

    delete[](next_index);
    delete[](rate);
    delete[](photon_prev_state);
    delete[](CB_elec_prev_state);
    delete[](photon_next_state);
    delete[](CB_elec_next_state);

    return result;
}//Rate_array Laser_transition_structure::transition_probabilities(int i)


// end of class Laser_transition_structure
//============================================================================================================================



