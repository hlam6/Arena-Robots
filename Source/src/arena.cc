/**
 * @file arena.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/arena.h"

#include <math.h>
#include <algorithm>

#include "src/robot.h"
#include "src/obstacle.h"
#include "src/event_collision.h"
#include "src/arena_params.h"
#include "src/recharge_station.h"
#include "src/home_base.h"
#include "src/event_recharge.h"
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
 /**
 * @brief Constructor that initializes entities in the arena.
 *
 * Initializes the dimensions of the arena, the robot, the home base,
 * the recharge station, and five obstacles.
 *
 * @param params const pointer to a const struct arena_params object.
 */
Arena::Arena(const struct arena_params* const params) :
  x_dim_(params->x_dim), y_dim_(params->y_dim),
  robot_(new Robot(&params->robot)),
  n_robots_(1),
  n_obstacles_(params->n_obstacles),
  recharge_station_(new RechargeStation(params->recharge_station.radius,
    params->recharge_station.pos,
    params->recharge_station.color)),
  home_base_(new HomeBase(&params->home_base)),
  entities_(),
  mobile_entities_() {
  robot_->heading_angle(0);
  entities_.push_back(robot_);
  entities_.push_back(home_base_);
  mobile_entities_.push_back(robot_);
  mobile_entities_.push_back(home_base_);
  entities_.push_back(recharge_station_);

  for (size_t i = 0; i < n_obstacles_; ++i) {
     entities_.push_back(new Obstacle(
         params->obstacles[i].radius,
         params->obstacles[i].pos,
         params->obstacles[i].color));
       } /* for(i..) */
}

 /**
 * @brief Destructor that frees memory consumed by entities
 * in the arena.
 */
Arena::~Arena(void) {
  for (auto ent : entities_) {
    delete ent;
  } /* for(ent..) */
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/
/**
* @brief Resets all entities in the arena to their newly constructed
* states.
*/

void Arena::Reset(void) {
  for (auto ent : entities_) {
    ent->Reset();
  } /* for(ent..) */
} /* reset() */

std::vector<Obstacle*> Arena::obstacles(void) {
  std::vector<Obstacle*> res;
  for (auto ent : entities_) {
    Obstacle* o = dynamic_cast<Obstacle*>(ent);
    if (o) {
      res.push_back(o);
    }
  } /* for(ent..) */
  return res;
} /* robots() */
/**
* @brief Advances the state of the arena while the game is still
* going. Calls UpdateEntitiesTimestep() to accomplish this.
*/
void Arena::AdvanceTime(void) {
  if (!GameOver) {
  std::cout << "Advancing simulation time by 1 timestep\n";
  UpdateEntitiesTimestep();
  }
} /* AdvanceTime() */
/**
* @brief Updates the state of all entities in the arena.
*
* Uses the fact that all ArenaEntity objects have a TimestepUpdate()
* function to accomplish this. Also checks if the robot
* has run out of battery, and if it has, then sets GameOver = true,
* ending the game.
*/
void Arena::UpdateEntitiesTimestep(void) {
  double old_robot_heading_angle = robot_->heading_angle();
  /*
   * First, update the position of all entities, according to their current
   * velocities.
   */
  for (auto ent : entities_) {
    ent->TimestepUpdate(1);
  } /* for(ent..) */

  /*
   * Next, check if the robot has run out of battery
   */
  if (robot_->battery_level() <=0) {
    std::cout << "You lose!" << std::endl;
    std::cout << std::endl;
    GameOver = true;
  }

  /*
   * Next, check if the robot has collided with the recharge station or the home
   * base. These need to be before the general collisions, which can move the
   * robot away from these "obstacles" before the "collisions" have been
   * properly processed.
   */

  EventCollision ec;

 /*  If the robot collides with the HomeBase, output "You win!"
  *  to the terminal. Also resets the values of entities in the
  *  arena, along with setting GameOver to true, causing Arena::AdvanceTime()
  *  to stop.
  */
  CheckForEntityCollision(robot_, home_base_, &ec, robot_->collision_delta());
  if (ec.collided()) {
    std::cout << "You win!" << std::endl;
    std::cout << std::endl;
    this->Reset();
    GameOver = true;
  }

  /* If the robot collides with the recharge station, generate
   * an EventRecharge object and pass it to robot_'s Accept() function.
   * This causes the battery to recharge fully. Also sets the value of
   * robot->hit_recharge_station_ to true. This prevents the Accept(ec)
   * function from draining the battery when a collision with the
   * recharge station occurs.
   */
  CheckForEntityCollision(robot_, recharge_station_,
    &ec, robot_->collision_delta());
  if (ec.collided()) {
    EventRecharge er;
    robot_->Accept(&er);
    robot_->hit_recharge_station(true);
    er.EmitMessage();
  }

  /*
   * Finally, some pairs of entities may now be close enough to be considered
   * colliding, send collision events as necessary.
   *
   * When something collides with an immobile entity, the immobile entity does
   * not move (duh), so no need to send it a collision event.
   */
  for (auto ent : mobile_entities_) {
    // Check if it is out of bounds. If so, use that as point of contact.
    assert(ent->is_mobile());
    CheckForEntityOutOfBounds(ent, &ec);

    // If not at wall, check if colliding with any other entities (not itself)
    if (!ec.collided()) {
      for (size_t i = 0; i < entities_.size(); ++i) {
        if (entities_[i] == ent) {
          continue;
        }
        CheckForEntityCollision(ent, entities_[i], &ec, ent->collision_delta());
        if (ec.collided()) {
          ec.EmitMessage();

          break;
        }
      } /* for(i..) */
    } /* else */
    ent->Accept(&ec);
  } /* for(ent..) */
} /* UpdateEntities() */

/**
* @brief Checks if ent has collided with a wall.
* If it has, reflect it by calculating the angle of incidence.
*
* @param ent Pointer to an ArenaMobileEntity object
* @param event Pointer to a EventCollision object
*/

void Arena::CheckForEntityOutOfBounds(const ArenaMobileEntity * const ent,
  EventCollision * event) {
  if (ent->get_pos().x+ ent->radius() >= x_dim_) {
    // Right Wall
    event->collided(true);
    event->collided_with_wall(true);
    event->point_of_contact(Position(x_dim_, ent->get_pos().y));
    event->angle_of_contact(ent->heading_angle() - 180);
  } else if (ent->get_pos().x- ent->radius() <= 0) {
    // Left Wall
    event->collided(true);
    event->collided_with_wall(true);
    event->point_of_contact(Position(0, ent->get_pos().y));
    if (ent->get_pos().x <= x_dim_) {
    event->angle_of_contact(ent->heading_angle() + 180);
  }
  } else if (ent->get_pos().y+ ent->radius() >= y_dim_) {
    // Bottom Wall
    event->collided(true);
    event->collided_with_wall(true);
    event->point_of_contact(Position(ent->get_pos().x, y_dim_));
    event->angle_of_contact(ent->heading_angle());
  } else if (ent->get_pos().y - ent->radius() <= 0) {
    // Top Wall
    event->collided(true);
    event->collided_with_wall(true);
    event->point_of_contact(Position(0, y_dim_));
    event->angle_of_contact(ent->heading_angle());
  } else {
    event->collided(false);
  }
} /* entity_out_of_bounds() */
/**
* @brief Checks if ent1 has collided with ent2. If it has, then
* bounce ent1 by calculating the angle of incidence.
*
* @param ent1 Pointer to ArenaEntity object
* @param ent2 Pointer to ArenaEntity object
* @param event Pointer to EventCollision object
* @param collision_delta Double used as a collision buffer
*/
void Arena::CheckForEntityCollision(const ArenaEntity* const ent1,
  const ArenaEntity* const ent2,
  EventCollision * event,
  double collision_delta) {
  /* Note: this assumes circular entities */
  double ent1_x = ent1->get_pos().x;
  double ent1_y = ent1->get_pos().y;
  double ent2_x = ent2->get_pos().x;
  double ent2_y = ent2->get_pos().y;
  double dist = std::sqrt(
    std::pow(ent2_x - ent1_x, 2) + std::pow(ent2_y - ent1_y, 2));
  if (dist > ent1->radius() + ent2->radius() + collision_delta) {
    event->collided(false);
    event->point_of_contact(ent1->get_pos());
  } else {
    // Populate the collision event.
    // Collided is true
    // Point of contact is point along perimeter of ent1
    // Angle of contact is angle to that point of contact
    event->collided(true);
    double angle_of_contact = 0.0;

   /* Case 1: If the two entities are have same x-coordinate,
    * this means that it collided  at a 90/270 degree angle; top/bottom.
    * To determine angle_of_contact, PI is added to the change
    * in y-coordinates * PI/2, essentially reflecting the angle.

    * Case 2: If ent1_x > ent2_x, then this means the the collision
    * occured on the right side of ent2, therefore the angle of contact
    * is going to be equal to the tangent(dy, dx) where dy is the
    * difference in y-coordinates and dx is the difference in
    * x-coordinates.
    *
    * Case 3: Entities collide on left side of ent2, therefore the
    * angle of contact is the tangent(dy, dx) and PI is added to
    * the angle to reflect it 180 degrees.
    */

    if (ent2_x - ent1_x == 0) {
      angle_of_contact = M_PI + (ent1_y - ent2_y) * M_PI/2;
    } else {
      if (ent1_x > ent2_x) {
        angle_of_contact = atan((ent1_y - ent2_y) / (ent2_x - ent1_x));
      } else {
        angle_of_contact = atan((ent1_y - ent2_y) / (ent2_x - ent1_x)) + M_PI;
      }
    }
    // Taihui helped me with this part, not fully knowledgeable of the logic.
    Position point_of_contact;
    double total = ent1->radius() + ent2->radius();
    point_of_contact.y = (ent2_y * (ent1->radius()) +
    (ent1_y*ent2->radius())/(total));
    point_of_contact.y = (ent1_y*ent2->radius() +
     ent2_y*ent1->radius())/(ent1->radius() + ent2->radius());
    point_of_contact.x = (ent1_x*ent2->radius() +
    ent2_x*ent1->radius())/(ent1->radius() + ent2->radius());
    event->angle_of_contact(angle_of_contact*180/M_PI);  // radians to degrees
    event->point_of_contact(point_of_contact);



    /// >>>>>>> FILL THIS IN
  }
} /* entities_have_collided() */
/**
* @brief This function takes an EventKeypress and passes
* it to robot_->EventCmd(). This allows the robot to be controlled
* by keypresses.
*
* @param e Pointer to EventKeypress object used to send keypresses
*/
void Arena::Accept(EventKeypress * e) {
  robot_->EventCmd(e->get_key_cmd());
}

NAMESPACE_END(csci3081);
