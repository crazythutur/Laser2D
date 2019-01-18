/**
 * @h rate_array
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

#ifndef RATE_ARRAY_H
#define RATE_ARRAY_H

#include <string>

using namespace std;


class Rate_array
{
public:

    /************************/
    /*Constructor/Destructor*/
    /************************/

    Rate_array(int size);

    Rate_array(int size, double *values, double *rates);

    virtual ~Rate_array ();

//============================================================================================================================

    /*****************/
    /*Getters/Setters*/
    /*****************/

    int getSize() const
    {
        return size;
    }

    double *getValues() const
    {
        return values;
    }
    double getValue(const int i) const
    {
        return values[i];
    }

    double *getRates() const
    {
        return rates;
    }
    double getRate(const int i) const
    {
        return rates[i];
    }

//============================================================================================================================

    void setValues(double *value)
    {
        values = value;
    }

    void setRates(double *value)
    {
        rates = value;
    }
    void setRate(const int index, const double value)
    {
        rates[index] = value;
    }
//============================================================================================================================

    /******************/
    /*Public function */
    /******************/

    /**
     * @brief rate_from_value give the rate from a corresponding value
     * @param value input
     * @return the rate correspoonding to the @param value input
     */
    double rate_from_value(const double value);

    /**
     * @brief rateSum make the sum of all rates avaible in the current state
     * @return the sum of all other rates in @param trans_probas
     */
    double rateSum();

//============================================================================================================================

private:

    /*********************/
    /*Privates attributes*/
    /*********************/

    /**
     * @brief size of the array
     */
    int size;

    /**
     * @brief values array containig @a size values
     */
    double *values;

    /**
     * @brief rates array containig @a size rates
     */
    double *rates;

};// end of class Rate_array

//============================================================================================================================

#endif // RATE_ARRAY_H
