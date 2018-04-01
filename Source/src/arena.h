/**
 * @file robot_arena.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ARENA_H_
#define SRC_ARENA_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <cmath>
#include <iostream>
#include <vector>
#include "src/event_keypress.h"
#include "src/event_collision.h"
#include "src/robot.h"
#include "src/home_base.h"
#include "src/recharge_station.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
struct arena_params;

/**
 * @brief The main class for the simulation of a 2D world with five
 * stationary objects, a moving HomeBase, and a player controlled Robot.
 *
 * It is the container from which GraphicsArenaViewer draws
 * its objects. The Arena class is also responsible for collision
 * detection between its entities, and therefore ultimately responsible
 * for deciding if the player wins or loses.
 *
 * If the Robot runs out of battery, the player loses. If it collides
 * with the HomeBase, the player wins.
 */
class Arena {
 public:
  explicit Arena(const struct arena_params * const params);
  ~Arena(void);

  /**
   * @brief Advance the simulation by a single step.
   */
  void AdvanceTime(void);

  /**
  * @brief Handle the key press passed along by the viewer.
  *
  * @param[in] e An event holding the key press.
  *
  */
  void Accept(EventKeypress * e);

  // Reset all in the arena
  void Reset(void);

  /**
   * @brief Get the # of robots in the arena.
   */
  unsigned int n_robots(void) { return n_robots_; }

  /**
   * @brief Get # of obstacles in the arena.
   */
  unsigned int n_obstacles(void) { return n_obstacles_; }

  /**
   * @brief Get a list of all obstacles (i.e. non-mobile entities in the arena).
   */
  std::vector<class Obstacle*> obstacles(void);

  /**
   * @brief Get the list of all mobile entities in the arena.
   */
  std::vector<class ArenaMobileEntity*> mobile_entities(void)
    { return mobile_entities_; }

  /**
  * @brief Returns a pointer to a Robot object.
  */
  Robot* robot(void) const { return robot_; }

  /**
  * @brief Returns a pointer to a HomeBase object.
  */
  HomeBase* home_base(void) const { return home_base_; }

  /**
  * @brief Returns a pointer to a RechargeStation object.
  */
  RechargeStation* recharge_station(void) const { return recharge_station_; }

  /**
  * @brief Returns a bool object GameOver, which represents the state
  * of the game.
  */
  bool getGameStatus() {
    return GameOver;
  }

 private:
  /**
   * @brief Determine if two entities have collided in the arena. Collision is
   * defined as the difference between the extents of the two entities being less
   * than a run-time parameter.
   *
   * @param ent1 Entity #1.
   * @param ent2 Entity #2.
   * @param pointer to a collision event
   *
   * Collision Event is populated appropriately.
   */
  void CheckForEntityCollision(const class ArenaEntity* const ent1,
    const class ArenaEntity* const ent2,
    EventCollision * ec,
    double collision_delta);

  /**
   * @brief Determine if a particular entity is gone out of the boundaries of
   * the simulation.
   *
   * @param ent The entity to check.
   * @param pointer to a collision event.
   *
   * Collision event is populated appropriately.
   */
  void CheckForEntityOutOfBounds(const class ArenaMobileEntity* const ent,
    EventCollision * ec);

  /**
   * @brief Update all entities for a single timestep
   */
  void UpdateEntitiesTimestep(void);

  // Under certain circumstance, the compiler requires that the copy
  // constructor is not defined. This is deleting the default copy const.
  Arena& operator=(const Arena& other) = delete;
  Arena(const Arena& other) = delete;

  // Dimensions of graphics window inside which robots must operate
  double x_dim_;
  double y_dim_;
  unsigned int n_robots_;
  unsigned int n_obstacles_;

  // Entities populating the arena
  Robot* robot_;
  RechargeStation * recharge_station_;
  HomeBase * home_base_;
  std::vector<class ArenaEntity*> entities_;
  std::vector<class ArenaMobileEntity*> mobile_entities_;

  /* Variable used to determine the status of game, set to true when
  * the robot collides with the Home base, causing Arena::AdvanceTime()
  * to stop.
  */
  bool GameOver = false;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_H_
