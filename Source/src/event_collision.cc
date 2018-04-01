/**
 * @file event_collision.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/event_collision.h"
#include "src/arena_mobile_entity.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructor
 ******************************************************************************/
EventCollision::EventCollision() :
  collided_(false),
  point_of_contact_(0, 0),
  angle_of_contact_(0) {
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
 /**
 * @brief Prints to the console the point_of_contact_ and
 * angle_of_contact_  of this object. Used primarily for debugging.
 *
 */
void EventCollision::EmitMessage(void) {
  printf("Collision event at point %d %d. Angle %f \n",
  point_of_contact_.x, point_of_contact_.y, angle_of_contact_);
} /* EmitMessage() */

NAMESPACE_END(csci3081);
