#include <gtest/gtest.h>

#include <project.hxx>

TEST(Project, version)
{
	ASSERT_EQ(PROJECT_VERSION, project::version());
}
