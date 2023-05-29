#include <gtest/gtest.h>

#include <application-impl.hxx>
#include <application.hxx>

using namespace project;

TEST(Application, init)
{
	auto& app = Application::instance();
	app.init();  // Must not throw
}

TEST(Application, same_instance)
{
	auto& app1 = Application::instance();
	auto& app2 = Application::instance();
	EXPECT_EQ(&app1, &app2);
}

TEST(ApplicationImpl, state)
{
	ApplicationImpl app;
	EXPECT_EQ(app.state(), ApplicationState::NOT_INITIALIZED);
	app.init();
	EXPECT_EQ(app.state(), ApplicationState::INITIALIZED);
	app.quit();
	EXPECT_EQ(app.state(), ApplicationState::DONE);
}
