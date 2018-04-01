/**
 * @file robot_motion_behavior.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ROBOT_MOTION_BEHAVIOR_H_
#define SRC_ROBOT_MOTION_BEHAVIOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <Eigen/Dense>
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
 /**
 * @brief Determines where the Robot moves depending on heading angle,
 * speed, and time.
 */
class RobotMotionBehavior {
 public:
  RobotMotionBehavior(void) {}

  /**
   * @brief Update the position for an ArenaEntity, based on its current
   * position and velocity.
   *
   * RobotMotionHandler manages the modification to the velocity based on
   * user input and collisions.
   * RobotMotionBehavior translates velocity and position to a new position.
   * Both of these are straightforward, but the framework allows for more
   * sophisticated models of motion in which each wheel has distinct speed.
   *
   * @param[in] ent The entitity to update.
   * @param[in] dt Change in time
   */
  void UpdatePosition(class ArenaMobileEntity * const ent, uint dt);
};

NAMESPACE_END(csci3081);

#endif /* SRC_ROBOT_MOTION_BEHAVIOR_H_ */
