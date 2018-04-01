/**
 * @file robot_battery.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/robot_battery.h"
#include <math.h>
#include <cmath>
/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
/**
* @brief Reduces charge based on speed, and time.
*
* @param[in] old_pos Position object used to calculate distance/speed
* @param[in] new_pos Position object used to calculate distance/speed
* @param[in] dt Double representing time; used to
* calculate how much charge is depleted
*/
double RobotBattery::Deplete(__unused Position old_pos,
  __unused Position new_pos, __unused double dt) {
  double old_pos_x = old_pos.x;
  double old_pos_y = old_pos.y;
  double new_pos_x = new_pos.x;
  double new_pos_y = new_pos.y;
  double dist = std::sqrt(std::pow(new_pos_x - old_pos_x, 2) +
                          std::pow(new_pos_y - old_pos_y, 2));
  charge_ = charge_ - dist * kLINEAR_SCALE_FACTOR * dt * 5;
  if (charge_ < 0) {
    charge_ = 0.0;
  }
  return charge_;
} /* deplete() */

void RobotBattery::Accept(__unused EventCollision * e) {
  /**
  * @brief deplete battery by some value -- arbitrary selected for bumping
  */
  charge_ -= 10;
}

NAMESPACE_END(csci3081);
