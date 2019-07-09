#ifndef STATE_H
#define STATE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip> // std::setbase

class State
{

    /************************/
    /*Constructor/Destructor*/
    /************************/
public:

    State();
    State(const unsigned int number_of_modes, const unsigned int number_of_emiters);
    State(const unsigned int number_of_modes, const unsigned int number_of_emiters,
                 unsigned int *photons, unsigned int *CB_electrons, unsigned int *VB_electrons);
    State(const State& state_to_cpy);

    virtual ~State ();


    /************************/
    /*     Surcharge        */
    /************************/


    State& operator=(State const& state_to_copy);

    friend std::ostream &operator<<( std::ostream &out, State const& state );


//============================================================================================================================

    /*****************/
    /*Getters/Setters*/
    /*****************/

    unsigned int getPhotons(unsigned int mode) const
    {
        return this->photons[mode];

    }


    unsigned int getCB_electrons(unsigned int emitter_number) const
    {
        return this->CB_electrons[emitter_number];
    }


    unsigned int getVB_electrons(unsigned int emitter_number) const
    {
        return this->VB_electrons[emitter_number];

    }


//============================================================================================================================


    void setPhotons(unsigned int *value);

    void setCB_electrons(unsigned int *value);

    void setVB_electrons(unsigned int *value);

//============================================================================================================================

    /******************/
    /*Public function */
    /******************/

    unsigned int add_photon(unsigned int mode_number);
    unsigned int add_CB_electron(unsigned int emitter_number);
    unsigned int add_VB_electron(unsigned int emitter_number);

    unsigned int drop_photon(unsigned int mode_number);
    unsigned int drop_CB_electron(unsigned int emitter_number);
    unsigned int drop_VB_electron(unsigned int emitter_number);





//============================================================================================================================

    /*******************/
    /*Private function */
    /*******************/
private:

    void init();///< initialisation of tables

    void print(std::ostream& out) const; ///< print the state in the flux



    /*********************/
    /*Privates attributes*/
    /*********************/

    unsigned int *photons;///< photon number for each mode

    unsigned int *CB_electrons;///< Conduction band electron number for each @a Emitter

    unsigned int *VB_electrons;///< Conduction band electron number for each @a Emitter

    /************************/
    /*Static attributes     */
    /************************/

    static unsigned int number_of_modes;///< size of photons

    static unsigned int number_of_emiters;///< size of CB_electrons and VB_electrons


};

#endif // STATE_H
