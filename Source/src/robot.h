/**
 * @file robot.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "src/robot_motion_handler.h"
#include "src/robot_motion_behavior.h"
#include "src/sensor_touch.h"
#include "src/robot_battery.h"
#include "src/arena_mobile_entity.h"
#include "src/event_recharge.h"
#include "src/event_collision.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
 /**
 * @brief Instances of this object are controlled by the player.
 *
 * It can bounce off walls and obstacles in the arena. When it collides
 * with RechargeStation objects its RobotBattery becomes fully charged.
 * When it collides with HomeBase objects, the game is won.
 */
class Robot : public ArenaMobileEntity {
 public:
  explicit Robot(const struct robot_params* const params);

  void ResetBattery(void);
  void Reset(void);
  void HeadingAngleInc(void) { heading_angle_ += angle_delta_; }
  void HeadingAngleDec(void) { heading_angle_ -= angle_delta_; }
  void TimestepUpdate(unsigned int dt);
  void Accept(EventRecharge * e);
  void Accept(EventCollision * e);
  void EventCmd(enum event_commands cmd);

  double get_battery_level(void) const { return battery_.level(); }
  double get_heading_angle(void) const { return motion_handler_.heading_angle(); }
  void set_heading_angle(double ha) { motion_handler_.heading_angle(ha); }
  double get_speed(void) { return motion_handler_.speed(); }
  void set_speed(double sp) { motion_handler_.speed(sp); }
  int get_id(void) const { return id_; }
  std::string name(void) const {
    return "Robot" + std::to_string(id());
  }
  /**
  * @brief Returns hit_recharge_station_, a boolean object used to
  * prevent the robot from reducing it's battery every time it collides
  * with the recharge station.
  */
  bool hit_recharge_station() {
    return hit_recharge_station_;
  }
  /**
  * @brief Setter used to update the value of hit_recharge_station_.
  *
  * @param hit A bool object that hit_recharge_station_ is set to
  */
  void hit_recharge_station(bool hit) {
    hit_recharge_station_ = hit;
  }

 private:
  static unsigned int next_id_;

  int id_;
  double heading_angle_;
  double angle_delta_;
  RobotBattery battery_;
  RobotMotionHandler motion_handler_;
  RobotMotionBehavior motion_behavior_;
  SensorTouch sensor_touch_;
  bool hit_recharge_station_ = false;
};

NAMESPACE_END(csci3081);

#endif /* SRC_ROBOT_H_ */
