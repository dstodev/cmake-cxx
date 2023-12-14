#include <gtest/gtest.h>

#include <project.hxx>
#include <version.h>

TEST(Project, version)
{
	ASSERT_EQ(PROJECT_VERSION, project::version());
}
