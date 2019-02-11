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
 * - the @a laser_num represent the number of the laser in the @a Laser_2D
 *
 * - the @a pumping_local_rate that represent the portion of the total pump
 * that occur in this @a Elementary_laser (for exemple by a gaussian distribution).
 *
 * - the @a temperature represent the temperature in the laser
 *
 * - the @a neighboring_lasers regroup all neigbooring laser in direction @a UP, @a RIGHT, @a DOWN and @a LEFT
 */

#ifndef ELEMENTARY_LASER_H
#define ELEMENTARY_LASER_H



class Elementary_laser
{
public:

    /********************/
    /*Public enumeration*/
    /********************/

    /**
     * @brief The direction enum represent the 4 directions for the @a neighboring_lasers
     * it can be use for exemple with neighboring_lasers[UP]
     */
    enum direction{UP, RIGHT, DOWN, LEFT, DIRECTION_NUMBER};

   /************************/
   /*Constructor/Destructor*/
   /************************/

  Elementary_laser ();

  /**
   * @brief Elementary_laser Constructor to initialise the private parameters
   *
   * @param pumping_local_rate represent the portion of the total pump
   * that occur in this @a Elementary_laser (for exemple by a gaussian distribution).
   * @param temperature represent the temperature at non exponential form
   */
   Elementary_laser (double const pumping_local_rate, double const temperature);

   virtual ~Elementary_laser ();

   /*****************/
   /*Getters/Setters*/
   /*****************/

   /**
    * @return the number of the laser in the @a Laser2D
    */
   unsigned int getLaser_num() const
   {
       return laser_num;
   }

   /**
    * @return the pumping_local_rate
    */
   double getPumping_local_rate() const
   {
       return pumping_local_rate;
   }

   /**
    * @return temperature non exponential for electrons tranfers
    */
   double getTemperature() const
   {
      return temperature;
   }


   /**
    * @param value direction of the neigboring laser using @a direction enum
    * @return the pointer of the neigbooring laser
    */
   Elementary_laser getNeighboring_laser (direction const value)   const{
      return *neighboring_lasers[value];
   }

   /**
    * @param value direction of the neigboring laser without using if @a direction enum
    * @return the pointer of the neigbooring laser
    */
   Elementary_laser *getNeighboring_laser (int const value)   const{
      return neighboring_lasers[value];
   }

//=====================================================================================================================================

   /**
    * @brief setLaser_num set the number of the laser at given value
    * @param value new number of the laser
    */
   void setLaser_num(unsigned int const value)
   {
      laser_num = value;
   }

   /**
    * @brief setNeighboring_laser set the new_elementary_laser at laser_direction,
    * the reverse association is done too
    * @param new_elementary_laser the neighbooring laser to add
    * @param laser_direction the @a direction to add
    */
   void setNeighboring_laser(Elementary_laser *new_elementary_laser, direction laser_direction)
   {
       neighboring_lasers[laser_direction] = new_elementary_laser;
       new_elementary_laser->neighboring_lasers[(laser_direction + 2) % DIRECTION_NUMBER] = this;//set the other elementary neigbboring laser in the oposite direction
   }

//============================================================================================================================

private :
   /*********************/
   /*Privates attributes*/
   /*********************/


   /**
     * @brief laser_num represent the number of the laser in the @a Laser_2D
     */
    unsigned int laser_num;

    /**
     * @brief pumping_local_rate : Local pumping rate deffined by a portion of the total pump, for exemple by gaussian distribution.
     */
    double pumping_local_rate;

    /**
     * @brief temperature represent the temperature in the @a Elementary laser
     */
    double temperature;

    /**
     * @brief neighboring_lasers regroup all neigbooring laser in direction @a UP, @a RIGHT, @a DOWN and @a LEFT
     */
    Elementary_laser *neighboring_lasers [DIRECTION_NUMBER] ;

};// end of class Elementary_laser

#endif // ELEMENTARY_LASER_H
