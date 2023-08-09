#ifndef SHEAR_HXX
#define SHEAR_HXX

#include <dll-export.h>
#include <point-t.hxx>

namespace project {

DLL void shear(point_t<float>& point, float x_magnitude, float y_magnitude);

}  // namespace project

#endif  // SHEAR_HXX
