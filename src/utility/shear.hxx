#ifndef SHEAR_HXX
#define SHEAR_HXX

#include <point_t.hxx>
#include <project-api.h>

namespace project {

DLL void shear(point_t<float>& point, float x_magnitude, float y_magnitude);

}  // namespace project

#endif  // SHEAR_HXX
