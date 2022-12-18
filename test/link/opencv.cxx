#include <gtest/gtest.h>

#include <vector>

#include <opencv2/core.hpp>

using namespace cv;

TEST(Link, opencv)
{
	Mat o;
	(void) o;
}

// Include "sanity check" tests in the Link suite

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

/* Interpret as flat (1-dimensional) array of RGB values where every
 * 3 values (R (0-255), G (0-255), B (0-255)) represent one pixel.
 * This variable represents 8 RGB pixels:
 *
 *   R G B W  R red   (255, 0,   0)    C cyan    (0,   255, 255)
 *   C M Y K  G green (0,   255, 0)    M magenta (255, 0,   255)
 *            B blue  (0,   0,   255)  Y yellow  (255, 255, 0)
 *            W white (255, 255, 255)  K black   (0,   0,   0)
 */
// clang-format off
static constexpr unsigned char RGBWCMYK[] {
//  R    G    B
    255, 0,   0,    // R
    0,   255, 0,    // G
    0,   0,   255,  // B
    255, 255, 255,  // W
    0,   255, 255,  // C
    255, 0,   255,  // M
    255, 255, 0,    // Y
    0,   0,   0     // K
};
// clang-format on

TEST(Sanity, opencv_load_rgb)
{
	Mat pixels(2, 4, CV_8UC3, (void*) RGBWCMYK);

	// Assert pixels were loaded in exact RGB order before converting
	// color space
	/* R */ EXPECT_EQ(Vec3b({255, 0, 0}), pixels.at<Vec3b>(0, 0));
	/* G */ EXPECT_EQ(Vec3b({0, 255, 0}), pixels.at<Vec3b>(0, 1));
	/* B */ EXPECT_EQ(Vec3b({0, 0, 255}), pixels.at<Vec3b>(0, 2));
	/* W */ EXPECT_EQ(Vec3b({255, 255, 255}), pixels.at<Vec3b>(0, 3));
	/* C */ EXPECT_EQ(Vec3b({0, 255, 255}), pixels.at<Vec3b>(1, 0));
	/* M */ EXPECT_EQ(Vec3b({255, 0, 255}), pixels.at<Vec3b>(1, 1));
	/* Y */ EXPECT_EQ(Vec3b({255, 255, 0}), pixels.at<Vec3b>(1, 2));
	/* K */ EXPECT_EQ(Vec3b({0, 0, 0}), pixels.at<Vec3b>(1, 3));

	// OpenCV expects BGR color space
	Mat converted(pixels.size(), pixels.type());
	cvtColor(pixels, converted, COLOR_RGB2BGR);

	// Save visual-inspection-test image
	imwrite("opencv-sanity-check-rgbwcmyk.png", converted);
}
