### Bugs  
Jitter bug: When the mobile entities in the arena collide with the wall, there is a chance that the entity
gets stuck in a constant state of collisions. This causes the heading_angle_ of the entity to continuously
reverse. When this happens, the entity will be seen jittering at the point of collision, unable to break out.


Speed bug: When the speed of the robot is set to 1, its position is no longer updated correctly. For example,
if the heading_angle_ is 30, if speed is 1, then the robot moves as if the angle were 0.


Collision bug: Recently this was fixed, but there is a possible chance it occurs again. When the robot
collides with an obstacle, its position doesn't get updated and after one more call of UpdateEntitiesTimestep(), the robot
is still in the collision area, this causes the robot to not bounce back at the angle of incidence, and instead get stuck
in the obstacle with its heading_angle_ changing repeatedly, after a few more calls of UpdateEntitiesTimestep(), the robot
breaks out of the obstacle.
