/**
 * @file robot_motion_handler.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/robot_motion_handler.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
RobotMotionHandler::RobotMotionHandler() :
  heading_angle_(0),
  speed_(0),
  max_speed_(5) {
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
/**
 * @brief This function handles keypresses by changing heading angle
 * when the left and right keys are pressed down, and the speed
 * when the up and down keys are pressed down.
 *
 * @param[in] cmd An event_commands enum that contains commands to
 * turn in different directions.
 *
 */
void RobotMotionHandler::AcceptCommand(enum event_commands cmd) {
  double turn_delta = 10;  // shift 5 degrees in either direction on key turn
  switch (cmd) {
  case COM_TURN_LEFT:
  heading_angle_ = heading_angle_ - turn_delta;

  break;
  case COM_TURN_RIGHT:
  heading_angle_ = heading_angle_ + turn_delta;

  break;
  case COM_SPEED_UP:
  if (speed_ < max_speed_) {
    speed_ = speed_ + 1;
  }
  break;
  case COM_SLOW_DOWN:
  if (speed_ > 0) {
    speed_ = speed_ - 1;
  }
  break;
  default:
    std::cerr << "FATAL: bad actuator command" << std::endl;
    assert(0);
  } /* switch() */
} /* accept_command() */

/**
* @brief Changes the heading_angle to the angle of incidence if the
* SensorTouch object is activated(). This causes the ArenaMobileEntity
* to change its direction when it collides with something.
*
* @param st A SensorTouch object that can be activated()
*/
void RobotMotionHandler::UpdateVelocity(SensorTouch st) {
  if (st.activated()) {
    // In the event the angle is -0, switch to 180 since it's the same.
    if (st.angle_of_contact() == -0) {
      heading_angle_ = 180;
    } else {
    heading_angle_ = - st.angle_of_contact();
  }
  }
}


NAMESPACE_END(csci3081);
