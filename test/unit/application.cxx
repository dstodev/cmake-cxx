#include <gtest/gtest.h>

#include <application-impl.hxx>
#include <application.hxx>

using namespace project;

TEST(Application, init)
{
	Application app;
	app.init();  // Must not throw
}

struct ApplicationHelper : public Application
{
	ApplicationHelper()
	    : Application()
	{}
	ApplicationImpl* impl()
	{
		return _impl;
	}
};

TEST(Application, same_instance)
{
	ApplicationHelper app1;
	ApplicationHelper app2;
	EXPECT_EQ(app1.impl(), app2.impl());
}

TEST(ApplicationImpl, reset)
{
	auto& app = ApplicationImpl::instance();
	app.reset();  // Global state may have changed from different test
	EXPECT_EQ(app.state(), ApplicationState::NOT_INITIALIZED);
	app.init();
	EXPECT_EQ(app.state(), ApplicationState::INITIALIZED);
	app.reset();
	EXPECT_EQ(app.state(), ApplicationState::NOT_INITIALIZED);
}

TEST(ApplicationImpl, state)
{
	auto& app = ApplicationImpl::instance();
	app.reset();  // Global state may have changed from different test
	EXPECT_EQ(app.state(), ApplicationState::NOT_INITIALIZED);
	app.init();
	EXPECT_EQ(app.state(), ApplicationState::INITIALIZED);
	app.quit();
	EXPECT_EQ(app.state(), ApplicationState::DONE);
}

TEST(ApplicationImpl, set_state_across_instances)
{
	auto& app1 = ApplicationImpl::instance();
	auto& app2 = ApplicationImpl::instance();

	app1.reset();  // Global state may have changed from different test

	EXPECT_EQ(app1.state(), ApplicationState::NOT_INITIALIZED);
	EXPECT_EQ(app2.state(), ApplicationState::NOT_INITIALIZED);

	app1.state(ApplicationState::INITIALIZED);

	EXPECT_EQ(app1.state(), ApplicationState::INITIALIZED);
	EXPECT_EQ(app2.state(), ApplicationState::INITIALIZED);
}

TEST(ApplicationImpl, move_constructible)
{
	auto& app = ApplicationImpl::instance();
	app.reset();
	app.state(ApplicationState::INITIALIZED);
	ApplicationImpl app2(std::move(app));
	EXPECT_EQ(app2.state(), ApplicationState::INITIALIZED);
}

TEST(ApplicationImpl, move_assignable)
{
	auto& app = ApplicationImpl::instance();
	app.reset();
	app.state(ApplicationState::INITIALIZED);
	app = std::move(ApplicationImpl::instance());
	EXPECT_EQ(app.state(), ApplicationState::INITIALIZED);
}
