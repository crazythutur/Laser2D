/**
 * @class rate_array
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2019/01/15$
 *
 *
 * @brief object that contain 2 arrays containing @a size items that contain a couple of rate/value
 *
 * -@a size of the array
 * -@a values array containig @a size values
 * -@a rates array containig @a size rates
 *
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


#include "../inc/rate_array.h"

//============================================================================================================================

/************************/
/*Constructor/Destructor*/
/************************/

Rate_array::Rate_array(int size)
{
    this->size = size;
    this->values = new double[size];
    this->rates = new double[size];
}//Rate_array::Rate_array(int size)

//============================================================================================================================

Rate_array::Rate_array(int size, double *values, double *rates):
     Rate_array(size)
{

    std::copy(values, values + size, this->values );
    std::copy(rates, rates + size, this->rates );
}//Rate_array::Rate_array(int size, double *values, double *rates):

//============================================================================================================================

Rate_array::~Rate_array()
{
    delete[](values);
    delete[](rates);

}//Rate_array::~Rate_array()

//============================================================================================================================

double Rate_array::rate_from_value(const double value)
{

    double result =0;

    for(int num = 0 ; num < this->size; num ++)
    {
        if(value==this->values[num])
        {
            result = this->rates[num];
        }
    }

    return result;
}//double Rate_array::rate(const double value)

//============================================================================================================================

double Rate_array::rateSum()
{
    double rate_sum=0.0;

    for(int index = 0 ; index < this->size ; index ++){
        rate_sum += this->getRate(index);
    }
    return rate_sum;
}

//============================================================================================================================// end of class Rate_array



