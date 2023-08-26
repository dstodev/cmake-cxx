#ifndef SHEAR_HXX
#define SHEAR_HXX

#include <dll-export.h>
#include <point.hxx>

namespace project {

DLL void shear(Point<float>& point, float x_magnitude, float y_magnitude);

}  // namespace project

#endif  // SHEAR_HXX
