/**
 * @file arena_mobile_entity.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/arena_mobile_entity.h"
#include "src/robot_motion_behavior.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
/**
* @brief Update position of this class by instantiating a
* RobotMotionBehavior object and calling UpdatePosition() on that
* object, passing in a pointer to this class and dt to UpdatePosition().
*
* @param[in] dt uint representing change in time
*/
void ArenaMobileEntity::TimestepUpdate(uint dt) {
  RobotMotionBehavior h;
  h.UpdatePosition(this, dt);
} /* TimestepUpdate() */

NAMESPACE_END(csci3081);
