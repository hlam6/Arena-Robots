#ifndef SRC_POSITION_H_
#define SRC_POSITION_H_


NAMESPACE_BEGIN(csci3081);

struct Position {
Position(void) : x(0), y(0) { }
Position(double in_x, double in_y) : x(in_x), y(in_y) { }
  double x;
  double y;
};


NAMESPACE_END(csci3081);

#endif /* SRC_POSITION_H_ */
