/**
 * @file home_base_params.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_HOME_BASE_PARAMS_H_
#define SRC_HOME_BASE_PARAMS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/arena_entity_params.h"
#include "src/arena_mobile_entity_params.h"
/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/
 /* Note:
 * This struct used to call arena_entity_params super constructor,
 * I switched it to call arena_mobile_entity_params since HomeBase
 * had its inheritance changed from ArenaImmobileEntity to ArenaMobileEntity.
 */

 /**
 * @brief Struct containing parameters used to initialize
 * HomeBase object variables, such as heading_angle_.
 */
struct home_base_params : public arena_mobile_entity_params {
  home_base_params(void) :
      arena_mobile_entity_params() {}
};

NAMESPACE_END(csci3081);

#endif /* SRC_HOME_BASE_PARAMS_H_ */
