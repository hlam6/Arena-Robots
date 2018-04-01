/**
 * @file graphics_arena_viewer.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/graphics_arena_viewer.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "src/robot.h"
#include "src/home_base.h"
#include "src/obstacle.h"
#include "src/arena_params.h"
#include "src/event_keypress.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
GraphicsArenaViewer::GraphicsArenaViewer(
  const struct arena_params* const params)
    : csci3081::GraphicsApp(params->x_dim, params->y_dim, "Robot Simulation"),
      arena_(new Arena(params)),
      paused_(false),
      pause_btn_(nullptr),
      params_(params) {
  nanogui::FormHelper *gui = new nanogui::FormHelper(this);
  nanogui::ref<nanogui::Window> window = gui->addWindow(Eigen::Vector2i(10, 10),
                                                    "Simulation Controls");
/* added restart_btn_ to .cc and .h file so that the name can
 * be changed, in a similar fashion to pause_btn_. */
  restart_btn_ = gui->addButton("Restart",
    std::bind(&GraphicsArenaViewer::OnRestartBtnPressed, this));
  pause_btn_ = gui->addButton("Pause",
    std::bind(&GraphicsArenaViewer::OnPauseBtnPressed, this));

  performLayout();
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

// This is the primary driver for state change in the arena.
// It will be called at each iteration of nanogui::mainloop()
void GraphicsArenaViewer::UpdateSimulation(double dt) {
  if (!paused_) {
    last_dt += dt;
    while (last_dt > 0.05) {
      arena_->AdvanceTime();
      last_dt -= 0.05;
    }
    if (arena_->getGameStatus()) {
      restart_btn_->setCaption("Play again");
    } else {
      restart_btn_->setCaption("Restart");
    }
  }
}

/*******************************************************************************
 * Handlers for User Keyboard and Mouse Events
 ******************************************************************************/
/* For the restart function, first I called Reset() to
* set objects in the arena to their default values. Afterwards,
* arena_ gets reinitialized as a new Arena with the same params
* as inside the constructor for GraphicsArenaViewer.
*/
void GraphicsArenaViewer::OnRestartBtnPressed() {
  arena_->Reset();
  arena_ = new Arena(params_);
}

void GraphicsArenaViewer::OnPauseBtnPressed() {
  paused_ = !paused_;
  if (paused_) {
    pause_btn_->setCaption("Play");
  } else {
    pause_btn_->setCaption("Pause");
  }
}

void GraphicsArenaViewer::OnMouseMove(int x, int y) {
  std::cout << "Mouse moved to (" << x << ", " << y << ")" << std::endl;
}

void GraphicsArenaViewer::OnLeftMouseDown(int x, int y) {
  std::cout << "Left mouse button DOWN (" << x << ", " << y << ")" << std::endl;
}

void GraphicsArenaViewer::OnLeftMouseUp(int x, int y) {
  std::cout << "Left mouse button UP (" << x << ", " << y << ")" << std::endl;
}

void GraphicsArenaViewer::OnRightMouseDown(int x, int y) {
  std::cout << "Right mouse button DOWN (" << x << ", " << y << ")\n";
}

void GraphicsArenaViewer::OnRightMouseUp(int x, int y) {
  std::cout << "Right mouse button UP (" << x << ", " << y << ")" << std::endl;
}

void GraphicsArenaViewer::OnKeyDown(const char *c, int modifiers) {
  std::cout << "Key DOWN (" << c << ") modifiers=" << modifiers << std::endl;
}

void GraphicsArenaViewer::OnKeyUp(const char *c, int modifiers) {
  std::cout << "Key UP (" << c << ") modifiers=" << modifiers << std::endl;
}
/**
* @brief Framework used to pass keypresses into the arena so that
* they may be in scope of the arena and handled appropriately.
*/
void GraphicsArenaViewer::OnSpecialKeyDown(int key, int scancode,
  int modifiers) {
  EventKeypress e(key);
  arena_->Accept(&e);
//  arena_->Robot()->
  std::cout << "Special Key DOWN key=" << key << " scancode=" << scancode
            << " modifiers=" << modifiers << std::endl;
}

void GraphicsArenaViewer::OnSpecialKeyUp(int key, int scancode, int modifiers) {
  std::cout << "Special Key UP key=" << key << " scancode=" << scancode
            << " modifiers=" << modifiers << std::endl;
}

/*******************************************************************************
 * Drawing of Entities in Arena
 ******************************************************************************/
void GraphicsArenaViewer::DrawRobot(NVGcontext *ctx, const Robot* const robot) {
  // translate and rotate all graphics calls that follow so that they are
  // centered, at the position and heading for this robot
  // casting to integer since the arena is represented as a coordinate plane
  // of integers
  nvgSave(ctx);
  nvgTranslate(ctx, (int) robot->get_pos().x, (int) robot->get_pos().y);
  // nvgRotate(ctx, robot->heading_angle()); -> from original source code
  /* Commented out the line above because it caused the text to rotate with
  *  the robot. Since the battery level is in the name of the robot,
  * it's disorienting trying to check battery levels as it rotates.
  */
  nvgRotate(ctx, -90*M_PI/180);
  // robot's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0, 0.0, robot->radius());
  nvgFillColor(ctx, nvgRGBA(robot->color().r,
                            robot->color().g,
                            robot->color().b,
                            255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);
  // string stream to convert double to string to display battery level
  std::stringstream stream;
  stream << " Battery level: ";
  double battery = robot->battery_level();
  stream << battery;
  stream << "%";
  std::string formatted_name = robot->name() + stream.str();
  // robot id text label
  nvgSave(ctx);
  nvgRotate(ctx, M_PI / 2.0);
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, 0.0, 10.0, formatted_name.c_str(), NULL);
  nvgRestore(ctx);
  nvgRestore(ctx);
}

void GraphicsArenaViewer::DrawObstacle(NVGcontext *ctx,
                                       const Obstacle* const obstacle) {
  nvgBeginPath(ctx);
  nvgCircle(ctx, (int) obstacle->get_pos().x, (int) obstacle->get_pos().y,
    obstacle->radius());
  nvgFillColor(ctx, nvgRGBA(obstacle->color().r,
                            obstacle->color().g,
                            obstacle->color().b,
                            255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, (int) obstacle->get_pos().x, (int) obstacle->get_pos().y,
          obstacle->name().c_str(), NULL);
}

void GraphicsArenaViewer::DrawHomeBase(NVGcontext *ctx,
                               const HomeBase* const home) {
  nvgSave(ctx);
  // nvgTranslate(ctx, home->get_pos().x, home->get_pos().y);
  // nvgRotate(ctx, home->heading_angle());
  nvgBeginPath(ctx);
  nvgCircle(ctx, (int) home->get_pos().x, (int) home->get_pos().y, home->radius());
  nvgFillColor(ctx, nvgRGBA(home->color().r,
                            home->color().g,
                            home->color().b,
                            255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, (int) home->get_pos().x, (int) home->get_pos().y, home->name().c_str(),
    NULL);
}

// This is the primary driver for drawing all entities in the arena.
// It is called at each iteration of nanogui::mainloop()
void GraphicsArenaViewer::DrawUsingNanoVG(NVGcontext *ctx) {
  // initialize text rendering settings
  nvgFontSize(ctx, 18.0f);
  nvgFontFace(ctx, "sans-bold");
  nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

  std::vector<Obstacle*> obstacles = arena_->obstacles();
  for (size_t i = 0; i < obstacles.size(); i++) {
    DrawObstacle(ctx, obstacles[i]);
  } /* for(i..) */

  DrawRobot(ctx, arena_->robot());
  DrawHomeBase(ctx, arena_->home_base());
}

NAMESPACE_END(csci3081);
