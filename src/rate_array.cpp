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



