#ifndef SHEAR_H
#define SHEAR_H

#include <Eigen/Dense>

namespace my {

typedef Eigen::Vector2f point_t;

void shear(point_t& point, float x_magnitude, float y_magnitude);

}  // namespace my

#endif
