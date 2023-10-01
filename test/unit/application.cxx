#include <gtest/gtest.h>

#include <mutex>

#include <application-impl.hxx>
#include <application.hxx>

using namespace project;

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

// If tests are multithreaded, the following tests may fail because the global
// instance is not thread-safe. Lock these tests to run one-at-a-time.
static std::mutex MUTEX;

TEST(ApplicationImpl, state)
{
	std::lock_guard<std::mutex> lock(MUTEX);

	auto& app = ApplicationImpl::instance();

	EXPECT_EQ(app.state(), ApplicationState::NOT_INITIALIZED);
	app.init();
	EXPECT_EQ(app.state(), ApplicationState::INITIALIZED);
	app.quit();
	EXPECT_EQ(app.state(), ApplicationState::DONE);
	app.reset();
	EXPECT_EQ(app.state(), ApplicationState::NOT_INITIALIZED);
}

TEST(ApplicationImpl, set_state_across_instances)
{
	std::lock_guard<std::mutex> lock(MUTEX);

	auto& app1 = ApplicationImpl::instance();
	auto& app2 = ApplicationImpl::instance();

	EXPECT_EQ(app1.state(), ApplicationState::NOT_INITIALIZED);
	EXPECT_EQ(app2.state(), ApplicationState::NOT_INITIALIZED);

	app1.state(ApplicationState::INITIALIZED);

	EXPECT_EQ(app1.state(), ApplicationState::INITIALIZED);
	EXPECT_EQ(app2.state(), ApplicationState::INITIALIZED);

	app1.reset();

	EXPECT_EQ(app1.state(), ApplicationState::NOT_INITIALIZED);
	EXPECT_EQ(app2.state(), ApplicationState::NOT_INITIALIZED);
}

TEST(ApplicationImpl, move_constructible_and_assignable)
{
	std::lock_guard<std::mutex> lock(MUTEX);

	auto& app = ApplicationImpl::instance();
	app.state(ApplicationState::INITIALIZED);
	ApplicationImpl app2(std::move(app));
	EXPECT_EQ(app2.state(), ApplicationState::INITIALIZED);

	// Assign back to global instance before leaving test
	app = std::move(app2);
	app.reset();
}
