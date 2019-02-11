/**
 * @header rate_array
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2019/01/15$

 *
 */

#ifndef RATE_ARRAY_H
#define RATE_ARRAY_H

#include <string>

using namespace std;

/**
 * @brief object that contain 2 arrays containing @a size items that contain a couple of rate/value
 *
 * -@a size of the array
 * -@a values array containig @a size values
 * -@a rates array containig @a size rates
 */
class Rate_array
{
public:

    /************************/
    /*Constructor/Destructor*/
    /************************/

    /**
     * @brief Rate_array Basic construc for an empty @a Rate_array of size @param size
     */
    Rate_array(int size);

    /**
     * @brief Rate_array Constructor For  @a Rate_array
     * @param size size of @a Rate_array
     * @param values
     * @param rates
     */
    Rate_array(int size, double *values, double *rates);

    virtual ~Rate_array ();

//============================================================================================================================

    /*****************/
    /*Getters/Setters*/
    /*****************/

    /**
     * @return the member number of the array
     */
    int getSize() const
    {
        return size;
    }

    /**
     * @return all values in the @a rate_array
     */
    double *getValues() const
    {
        return values;
    }

    /**
     * @return the rate at the given value @param i
     */
    double getValue(const int i) const
    {
        return values[i];
    }

    /**
     * @return all rates in the @a rate_array
     */
    double *getRates() const
    {
        return rates;
    }

    /**
     * @return the value at the given value @param i
     */
    double getRate(const int i) const
    {
        return rates[i];
    }

//============================================================================================================================

    /**
     * @brief setValues change the value to @param value
     * need to have the correct size
     */
    void setValues(double *value)
    {
        values = value;
    }

    /**
     * @brief setRates change the rate to @param value
     * need to have the correct size
     */
    void setRates(double *value)
    {
        rates = value;
    }

    /**
     * @brief setRate change the rate to @param value at index @param index
     */
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
