/**
 * @file robot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/robot.h"
#include "src/robot_motion_behavior.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Static Variables
 ******************************************************************************/
uint Robot::next_id_ = 0;

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Robot::Robot(const struct robot_params* const params) :
  ArenaMobileEntity(params->radius, params->collision_delta,
    params->pos, params->color),
  battery_(params->battery_max_charge),
  heading_angle_(0),
  angle_delta_(params->angle_delta),
  motion_handler_(),
  motion_behavior_(),
  sensor_touch_(),
  id_(-1) {
  motion_handler_.heading_angle(270);
  motion_handler_.speed(5);
  id_ = next_id_++;
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
 /**
 * @brief Updates the position, heading angle, speed, and
 * depletes the battery as a function of time (dt).
 * @param[in] dt Change in time used to update position, and
 * deplete the battery
 */
void Robot::TimestepUpdate(uint dt) {
  Position old_pos = get_pos();
  // Update heading and speed as indicated by touch sensor
  motion_handler_.UpdateVelocity(sensor_touch_);
  // Use velocity and position to update position
  motion_behavior_.UpdatePosition(this, dt);
  // Deplete battery as appropriate given distance and speed of movement
  battery_.Deplete(old_pos, get_pos(), dt);
  // Reset the status of hit_recharge_station_ so that the
  // robot loses battery when it hits obstacles
  hit_recharge_station_ = false;
} /* TimestepUpdate() */

/**
* @brief Charges the battery to full
*
* Called when the Robot collides with the RechargeStation.
*
* @param e A pointer to an EventRecharge object
*/
void Robot::Accept(EventRecharge * e) {
  battery_.EventRecharge();
}

// Pass along a collision event (from arena) to the touch sensor.
// This method provides a framework in which sensors can get different
// types of information from different sources.
/**
* @brief This is the handler for Robot to accept different sources of information.
* In the event that the robot collides into an obstacle, the speed and battery
* both get reduced.
*
* @param e Pointer to an EventCollision object
*/
void Robot::Accept(EventCollision * e) {
  sensor_touch_.Accept(e);
  if (e->collided()) {
    /* If the robot has collided with a recharge station
     * Don't reduce the battery level, since it should recharge.
     */
    if (hit_recharge_station() == false && e->collided_with_wall() == false) {
      battery_.Accept(e);
    }
  // Bug: When speed == 1, the robot stops moving
  // To address this, I prevent speed_ from going below 2.
    if (motion_handler_.speed() > 2 && e->collided_with_wall() == false) {
        motion_handler_.speed(motion_handler_.speed() - 1);
    }
  }
}
/**
* @brief User input commands to change heading or speed
*
* @param cmd An event_commands enum
*/
void Robot::EventCmd(enum event_commands cmd) {
  motion_handler_.AcceptCommand(cmd);
} /* event_cmd() */
/**
* @brief Resets the battery, motion_handler, and sensor_touch to
* their newly constructed states.
*/
void Robot::Reset(void) {
  battery_.Reset();
  motion_handler_.Reset();
  sensor_touch_.Reset();
  next_id_ = 0;
} /* Reset() */
/**
* @brief Resets the battery to its newly constructed state.
*
*/
void Robot::ResetBattery(void) {
  battery_.Reset();
}

NAMESPACE_END(csci3081);
