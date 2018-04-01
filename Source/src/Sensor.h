/**
 * @file sensor_touch.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */


#ifndef SRC_SENSOR_H_
#define SRC_SENSOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <vector>
#include <utility>

#include "src/common.h"
#include "src/event_collision.h"


 /*******************************************************************************
  * Namespaces
  ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/
/**
* @brief This class acts as an abstract interface for a family of sensors, which
* collect information.
*
* Currently the only one implemented is SensorTouch.
* Sensor contains one private member variable, activated_,
* getters and setters for that variable, and another function
* called Accept() which should take an EventCollision as
* its argument. There is also a Reset() function, which should reset the
* object to its newly constructed state.
*/
class Sensor {
 public:
    Sensor(): activated_(false) {}

    bool activated(void) { return activated_; }
    void activated(bool value) { activated_ = value; }
    /** @brief Accepts an EventCollision object and does something
    * depending on the activated() status of the EventCollision object.
    */
    virtual void Accept(EventCollision * e) = 0;

    /**
    * @brief Reset the sensor to a newly constructed state.
    */
    virtual void Reset(void) = 0;
 protected:
    bool activated_;
};

NAMESPACE_END(csci3081);

#endif  /* SRC_SENSOR_H_ */
