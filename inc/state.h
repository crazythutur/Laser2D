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
    /**
     * @brief State constructor, initialise the tables with the corects size for
     * @param number_of_modes
     * @param number_of_emiters
     */
    State(const unsigned int number_of_modes, const unsigned int number_of_emiters);

    /**
     * @brief State constructor, initialise the tables with the corects size for
     * @param number_of_modes
     * @param number_of_emiters
     *
     * initialise all value with
     * @param photons photons value for each mode
     * @param CB_electrons electron in     excited state for all emitters
     * @param VB_electrons electron in non-excited state for all emitters
     */
    State(const unsigned int number_of_modes, const unsigned int number_of_emiters,
                 unsigned int *photons, unsigned int *CB_electrons, unsigned int *VB_electrons);

    /**
     * @brief State copy constructor
     * @param state_to_cpy state to copy
     */
    State(const State& state_to_cpy);

    virtual ~State ();


    /************************/
    /*     Surcharge        */
    /************************/

    /**
     * @brief operator = Overloading of the definition operator
     * @param state_to_copy
     * @return
     */
    State& operator=(State const& state_to_copy);


    /**
     * @brief operator << overloading of the display function
     * @param out input flux
     * @param state state to print
     * @return the oupt flux updated
     */
    friend std::ostream &operator<<( std::ostream &out, State const& state );


//============================================================================================================================

    /*****************/
    /*Getters/Setters*/
    /*****************/

    /**
     * @brief getPhotons getter of the photon number
     * @return the photon number for the given  @param mode
     */
    unsigned int getPhotons(unsigned int mode) const
    {
        return this->photons[mode];

    }

    /**
     * @brief getCB_electrons getter of the excited electron number
     * @return the excited electron number for the given  @param emitter_number
     */
    unsigned int getCB_electrons(unsigned int emitter_number) const
    {
        return this->CB_electrons[emitter_number];
    }

    /**
     * @brief getVB_electrons getter of the excited electron number
     * @return the non-excited electron number for the given  @param emitter_number
     */
    unsigned int getVB_electrons(unsigned int emitter_number) const
    {
        return this->VB_electrons[emitter_number];

    }


//============================================================================================================================

    /**
     * @brief setPhotons set the photons number to the
     * @param value
     */
    void setPhotons(unsigned int *value);

    /**
     * @brief setCB_electrons set the CB electons number to the
     * @param value
     */
    void setCB_electrons(unsigned int *value);

    /**
     * @brief setVB_electrons set the VB electons number to the
     * @param value
     */
    void setVB_electrons(unsigned int *value);

//============================================================================================================================

    /******************/
    /*Public function */
    /******************/

    /**
     * @brief add_photon add a photon at the mode @param mode_number
     * @return the value updated
     */
    unsigned int add_photon(unsigned int mode_number);

    /**
     * @brief add_CB_electron add a CB electron at the emitter @param emitter_number
     * @return the value updated
     */
    unsigned int add_CB_electron(unsigned int emitter_number);

    /**
     * @brief add_VB_electron add a VB electron at the emitter @param emitter_number
     * @return the value updated
     */
    unsigned int add_VB_electron(unsigned int emitter_number);


    /**
     * @brief drop_photon remove a photon at the mode @param mode_number
     * @return the value updated
     */
    unsigned int drop_photon(unsigned int mode_number);

    /**
     * @brief drop_CB_electron remove a CB electon at the emitter @param emitter_number
     * @return the value updated
     */
    unsigned int drop_CB_electron(unsigned int emitter_number);

    /**
     * @brief drop_VB_electron remove a VB electon at the emitter @param emitter_number
     * @return the value updated
     */
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
