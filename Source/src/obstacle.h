/**
 * @file obstacle.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_OBSTACLE_H_
#define SRC_OBSTACLE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "src/arena_immobile_entity.h"
#include "src/color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
 /**
 * @brief Class used as an obstacle around the arena.
 * ArenaMobileEntity objects around the arena can
 * collide into these obstacles.
 */
class Obstacle: public ArenaImmobileEntity {
 public:
  Obstacle(double radius, const Position& pos,
                   const Color& color);

  std::string name(void) const {
    return "Obstacle" + std::to_string(id_);
  }
  /**
  * @brief Resets the next_id_ val to 0
  * when Arena::Reset() is called. This is so that
  * when the Arena state is reset, the id_ of the obstacle
  * doesn't increase.
  */
  void Reset() {
    next_id_ = 0;
  }
 private:
  static uint next_id_;

  int id_;
};

NAMESPACE_END(csci3081);

#endif /* SRC_OBSTACLE_H_ */
