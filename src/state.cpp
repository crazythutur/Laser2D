/**
 * @class State
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 02/07/2019$
 *
 *
 * @brief Description of a state that caracterise the Markov Chain
 *
 * the state is caracterised by a number of variable that describe enterely the state.
 * Each state is caracterised by the number of photon for each mode stocked in @a photons,
 * the number of elctron in each band (Conduction and Valence bands) stocked in @a $
 * CB_electrons and @a VB_electrons.
 */

#include "../inc/state.h"


/************************/
/*Constructor/Destructor*/
/************************/
unsigned int State::number_of_modes;
unsigned int State::number_of_emiters;

State::State()
{
    init();
}


State::State(const unsigned int number_of_modes, const unsigned int number_of_emiters)
{
    State::number_of_modes=number_of_modes;
    State::number_of_emiters=number_of_emiters;

    init();

}//State::State(const unsigned int number_of_modes, const unsigned int number_of_emiters)


State::State(const unsigned int number_of_modes, const unsigned int number_of_emiters,
             unsigned int *photons, unsigned int *CB_electrons, unsigned int *VB_electrons)
    :State(number_of_modes,  number_of_emiters)
{
    std::copy(photons,
              photons + State::number_of_modes, this->photons);

    std::copy(CB_electrons,
              CB_electrons + State::number_of_emiters,
              this->CB_electrons);

    std::copy(VB_electrons,
              VB_electrons + State::number_of_emiters,
              this->VB_electrons);

}//State::State(const unsigned int number_of_modes, const unsigned int number_of_emiters,
//unsigned int *photons, unsigned int *CB_electrons, unsigned int *VB_electrons)


//============================================================================================================================


//============================================================================================================================

State::State(State const &state_to_cpy):State()
{



    std::copy(state_to_cpy.photons,
              state_to_cpy.photons + State::number_of_modes, this->photons);

    std::copy(state_to_cpy.CB_electrons,
              state_to_cpy.CB_electrons + State::number_of_emiters,
              this->CB_electrons);

    std::copy(state_to_cpy.VB_electrons,
              state_to_cpy.VB_electrons + State::number_of_emiters,
              this->VB_electrons);

}//State::State(const State &state_to_cpy) : List(state_to_cpy)

//============================================================================================================================

State::~State()
{
    delete[](this->photons);
    delete[](this->CB_electrons);
    delete[](this->VB_electrons);
}//State::~State()


//============================================================================================================================

State& State::operator=(State const& state_to_cpy)
{
    if(this != &state_to_cpy)        //On vérifie que l'objet n'est pas le même que celui reçu en argument

        {
            delete[](this->photons);
            delete[](this->CB_electrons);
            delete[](this->VB_electrons);
            init();

            std::copy(state_to_cpy.photons,
                      state_to_cpy.photons + State::number_of_modes, this->photons);

            std::copy(state_to_cpy.CB_electrons,
                      state_to_cpy.CB_electrons + State::number_of_emiters,
                      this->CB_electrons);

            std::copy(state_to_cpy.VB_electrons,
                      state_to_cpy.VB_electrons + State::number_of_emiters,
                      this->VB_electrons);

        }

        return *this; //On renvoie l'objet lui-même

}//State& State::operator=(State const& state_to_copy)


//============================================================================================================================

/******************/
/*Public function */
/******************/

unsigned int State::add_photon(unsigned int mode_number)
{
    return this->photons[mode_number]++ ;
}//unsigned int State::add_photon(unsigned int mode_number)

//============================================================================================================================

unsigned int State::add_CB_electron(unsigned int emitter_number)
{
    return this->CB_electrons[emitter_number]++;
}//unsigned int State::add_CB_electron(unsigned int emitter_number)

//============================================================================================================================


unsigned int State::add_VB_electron(unsigned int emitter_number)
{
    return this->VB_electrons[emitter_number]++;

}//unsigned int State::add_VB_electron(unsigned int emitter_number)

//============================================================================================================================



unsigned int State::drop_photon(unsigned int mode_number)
{
    return this->photons[mode_number]-- ;

}//unsigned int State::drop_photon(unsigned int mode_number)

//============================================================================================================================


unsigned int State::drop_CB_electron(unsigned int emitter_number)
{
    return this->CB_electrons[emitter_number]--;

}//unsigned int State::drop_CB_electron(unsigned int emitter_number)

//============================================================================================================================


unsigned int State::drop_VB_electron(unsigned int emitter_number)
{
    return this->VB_electrons[emitter_number]--;

}//unsigned int State::drop_VB_electron(unsigned int emitter_number)

//============================================================================================================================

/*******************/
/*Private function */
/*******************/

void State::init()
{
    this->photons       = new unsigned int[State::number_of_modes];
    this->CB_electrons  = new unsigned int[State::number_of_emiters];
    this->VB_electrons  = new unsigned int[State::number_of_emiters];
}

//============================================================================================================================
//End of Class State
