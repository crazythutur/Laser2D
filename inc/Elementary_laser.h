/**
 * @class Elementary_laser
 *
 * @author Arthur Vallet
 *
 * @version 1.0
 *
 * @date $Date: 2018/10/26$
 *
 *
 * @brief Reprensent the elementary laser that will put in the lactice of the @a 2D_laser
 *
 * This object is virtual and can be deffined as a Quantum Dot (DQ) laser or a Quantum Well (QW) laser.
 * Each  @a Elementary_laser is composed by  :
 *
 * - the @a pumping_local_rate that represent the portion of the total pump
 * that occur in this @a Elementary_laser (for exemple by a gaussian distribution).
 *
 * - the @a exponential_temperature represent the temperature at exponential form q=exp(-E/(KbT)), E is the energy lvl between two lvl
 *
 * - the @a temperature is used for the electron tranferts rates
 *
 * - the @a neighboring_lasers regroup all neigbooring laser in direction @a UP, @a RIGHT, @a DOWN and @a LEFT
 */

#ifndef ELEMENTARY_LASER_H
#define ELEMENTARY_LASER_H



/**
  * class Elementary_laser
  * 
  */

class Elementary_laser
{
public:
   /************************/
   /*Constructor/Destructor*/
   /************************/

  /**
   * @brief Elementary_laser Constructor to initialise the private parameters
   *
   * @param pumping_local_rate represent the portion of the total pump
   * that occur in this @a Elementary_laser (for exemple by a gaussian distribution).
   * @param temperaturecis used for the electron tranferts rates
   * @param q represent the temperature at exponential form q=exp(-E/(KbT)), E is the energy lvl between two lvl
   */
   Elementary_laser (double const pumping_local_rate, const double temperature, double const q);

   virtual ~Elementary_laser ();

   /********************/
   /*Public enumeration*/
   /********************/

   /**
    * @brief The direction enum represent the 4 directions for the @a neighboring_lasers
    * it can be use for exemple with neighboring_lasers[UP]
    */
   enum direction{UP, RIGHT, DOWN, LEFT, DIRECTION_NUMBER};

   /*****************/
   /*Getters/Setters*/
   /*****************/

   double getPumping_local_rate() const
   {
       return pumping_local_rate;
   }

   double getExponential_temperature() const
   {
      return exponential_temperature;
   }

   double getTemperature() const
   {
       return temperature;
   }

   Elementary_laser getNeighboring_laser (direction const value)   const{
      return *neighboring_lasers[value];

   }


   void setPumping_local_rate(double value)
   {
       pumping_local_rate = value;
   }

   void setExponential_temperature(double const value)
   {
      exponential_temperature = value;
   }

   void setTemperature(double value)
   {
       temperature = value;
   }

   void setNeighboring_laser(Elementary_laser new_elementary_laser, direction laser_direction)
   {
       neighboring_lasers[laser_direction] = &new_elementary_laser;
   }

//============================================================================================================================

private :
   /*********************/
   /*Privates attributes*/
   /*********************/

    /**
     * @brief pumping_local_rate : Local pumping rate deffined by a portion of the total pump, for exemple by gaussian distribution.
     */
    double pumping_local_rate;

    /**
     * @brief exponential_temperature represent the temperature at exponential form q=exp(-E/(KbT)), E is the energy lvl between two lvl
     */
    double exponential_temperature;

    /**
     * @brief temperature is used for the electron tranferts rates contrary to the @a exponential_temperature it's not deffined exponentialy
     */
    double temperature;

    /**
     * @brief neighboring_lasers regroup all neigbooring laser in direction @a UP, @a RIGHT, @a DOWN and @a LEFT
     */
    Elementary_laser *neighboring_lasers [DIRECTION_NUMBER] ;

};// end of class Elementary_laser

#endif // ELEMENTARY_LASER_H
