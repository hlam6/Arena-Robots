/**
 * @file event_collision.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_EVENT_COLLISION_H_
#define SRC_EVENT_COLLISION_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include "src/event_base_class.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
* @brief Responsible for storing collision information, such as
* the state of collision, along with point_of_contact_
* and angle_of_contact_.
*
* All three are used to calculate motion for
* ArenaMobileEntity objects.
*/

class EventCollision : public EventBaseClass {
 public:
  EventCollision();
  void EmitMessage(void);
  bool collided() { return collided_; }
  void collided(bool c) { collided_ = c; }
  Position point_of_contact() { return point_of_contact_; }
  void point_of_contact(Position p) { point_of_contact_ = p; }
  double angle_of_contact() { return angle_of_contact_; }
  void angle_of_contact(double aoc) { angle_of_contact_ = aoc; }
  void collided_with_wall(bool c) { collided_with_wall_ = c; }
  bool collided_with_wall() { return collided_with_wall_; }
 private:
  bool collided_;
  Position point_of_contact_;
  double angle_of_contact_;

  /* Variable used to determine whether or not the robot hit a wall or not.
  * Used in calculating whether or not the robot should lose battery when it hits obstacles.
  * For example, it should not lose battery when it hits the wall.
  */
  bool collided_with_wall_ = false;
};

NAMESPACE_END(csci3081);

#endif /* SRC_EVENT_COLLISION_H_ */
