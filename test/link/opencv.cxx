#include <gtest/gtest.h>

#include <vector>

#include <opencv2/core.hpp>

using namespace cv;

TEST(Link, opencv)
{
	Mat o;
}

// Include "sanity check" tests in the Link suite

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

/* Interpret as flat (1-dimensional) array of RGB values where every
 * 3 values (R (0-255), G (0-255), B (0-255)) represent one pixel.
 * This variable represents 6 pixels, each with color:
 *
 *   R G B   R red   (255, 0,   0)    C cyan    (0,   255, 255)
 *   C M Y   G green (0,   255, 0)    M magenta (255, 0,   255)
 *           B blue  (0,   0,   255)  Y yellow  (255, 255, 0)
 */
// clang-format off
static constexpr unsigned char RGBCMY[] {
//  R    G    B
    255, 0,   0,    // R
    0,   255, 0,    // G
    0,   0,   255,  // B
    0,   255, 255,  // C
    255, 0,   255,  // M
    255, 255, 0     // Y
};
// clang-format on

TEST(Sanity, opencv_load_rgb)
{
	Mat pixels(2, 3, CV_8UC3, (void*) RGBCMY);
	Mat converted(pixels.size(), pixels.type());

	// Assert pixels were loaded in exact RGB order before converting
	// color space
	ASSERT_EQ(Vec3b({255, 0, 0}), pixels.at<Vec3b>(0, 0));  // R
	ASSERT_EQ(Vec3b({0, 255, 0}), pixels.at<Vec3b>(0, 1));  // G
	ASSERT_EQ(Vec3b({0, 0, 255}), pixels.at<Vec3b>(0, 2));  // B
	ASSERT_EQ(Vec3b({0, 255, 255}), pixels.at<Vec3b>(1, 0));  // C
	ASSERT_EQ(Vec3b({255, 0, 255}), pixels.at<Vec3b>(1, 1));  // M
	ASSERT_EQ(Vec3b({255, 255, 0}), pixels.at<Vec3b>(1, 2));  // Y

	// OpenCV expects BGR color space
	cvtColor(pixels, converted, COLOR_RGB2BGR);

	// Save visual-inspection test image
	imwrite("opencv-sanity-check-RGBCMY.png", converted);
}
