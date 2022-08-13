#ifndef SHEAR_HXX
#define SHEAR_HXX

#include <Eigen/Dense>

#include <project-export.h>

namespace project {

typedef Eigen::Vector2f point_t;

PROJECT_API void shear(point_t& point, float x_magnitude, float y_magnitude);

}  // namespace project

#endif  // SHEAR_HXX
