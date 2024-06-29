#include <gtest/gtest.h>

#define ENABLE_LOGGING 1
#include <log.hxx>
#undef ENABLE_LOGGING

using namespace project;

class Log : public ::testing::Test
{
protected:
	void SetUp() override
	{
		log::set_target(stderr);
		log::set_level(log::Level::None);
	}
};

TEST_F(Log, error)
{
	log::set_level(log::Level::Error);
	ASSERT_EQ(log::Level::Error, log::get_level());
	::testing::internal::CaptureStderr();
	log::error("{}", "message");
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Error: message\n") != std::string::npos);
}

TEST_F(Log, error_one_arg)
{
	log::set_level(log::Level::Error);
	::testing::internal::CaptureStderr();
	log::error("message");
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Error: message\n") != std::string::npos);
}

TEST_F(Log, error_varargs)
{
	log::set_level(log::Level::Error);
	::testing::internal::CaptureStderr();
	log::error("{} {}", "message", 1);
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Error: message 1\n") != std::string::npos);
}

TEST_F(Log, warning)
{
	log::set_level(log::Level::Warning);
	ASSERT_EQ(log::Level::Warning, log::get_level());
	::testing::internal::CaptureStderr();
	log::warning("{}", "message");
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Warning: message\n") != std::string::npos);
}

TEST_F(Log, warning_one_arg)
{
	log::set_level(log::Level::Warning);
	::testing::internal::CaptureStderr();
	log::warning("message");
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Warning: message\n") != std::string::npos);
}

TEST_F(Log, warning_varargs)
{
	log::set_level(log::Level::Warning);
	::testing::internal::CaptureStderr();
	log::warning("{} {}", "message", 1);
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Warning: message 1\n") != std::string::npos);
}

TEST_F(Log, info)
{
	log::set_level(log::Level::Info);
	ASSERT_EQ(log::Level::Info, log::get_level());
	::testing::internal::CaptureStderr();
	log::info("{}", "message");
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Info: message\n") != std::string::npos);
}

TEST_F(Log, info_one_arg)
{
	log::set_level(log::Level::Info);
	::testing::internal::CaptureStderr();
	log::info("message");
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Info: message\n") != std::string::npos);
}

TEST_F(Log, info_varargs)
{
	log::set_level(log::Level::Info);
	::testing::internal::CaptureStderr();
	log::info("{} {}", "message", 1);
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Info: message 1\n") != std::string::npos);
}

TEST_F(Log, debug)
{
	log::set_level(log::Level::Debug);
	ASSERT_EQ(log::Level::Debug, log::get_level());
	::testing::internal::CaptureStderr();
	log::debug("{}", "message");
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Debug: message\n") != std::string::npos);
}

TEST_F(Log, debug_one_arg)
{
	log::set_level(log::Level::Debug);
	::testing::internal::CaptureStderr();
	log::debug("message");
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Debug: message\n") != std::string::npos);
}

TEST_F(Log, debug_varargs)
{
	log::set_level(log::Level::Debug);
	::testing::internal::CaptureStderr();
	log::debug("{} {}", "message", 1);
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Debug: message 1\n") != std::string::npos);
}

TEST_F(Log, trace)
{
	log::set_level(log::Level::Trace);
	ASSERT_EQ(log::Level::Trace, log::get_level());
	::testing::internal::CaptureStderr();
	log::trace("{}", "message");
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Trace: message\n") != std::string::npos);
}

TEST_F(Log, trace_one_arg)
{
	log::set_level(log::Level::Trace);
	::testing::internal::CaptureStderr();
	log::trace("message");
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Trace: message\n") != std::string::npos);
}

TEST_F(Log, trace_varargs)
{
	log::set_level(log::Level::Trace);
	::testing::internal::CaptureStderr();
	log::trace("{} {}", "message", 1);
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Trace: message 1\n") != std::string::npos);
}

TEST_F(Log, none)
{
	log::set_level(log::Level::Trace);
	::testing::internal::CaptureStderr();
	log::trace("Before none");
	std::string result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Before none\n") != std::string::npos);

	log::set_level(log::Level::None);
	ASSERT_EQ(log::Level::None, log::get_level());
	::testing::internal::CaptureStderr();
	log::trace("During none");
	result = ::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(result.empty());

	log::set_level(log::Level::Trace);
	::testing::internal::CaptureStderr();
	log::trace("After none");
	result = ::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(result.find("After none\n") != std::string::npos);
}

TEST_F(Log, level_default_none)
{
	log::Level constexpr level {};
	EXPECT_EQ(log::Level::None, level);
}

TEST_F(Log, level_from)
{
	EXPECT_EQ(log::Level::Error, log::level_from("error"));

	EXPECT_EQ(log::Level::Warning, log::level_from("warning"));
	EXPECT_EQ(log::Level::Warning, log::level_from("warn"));

	EXPECT_EQ(log::Level::Info, log::level_from("info"));
	EXPECT_EQ(log::Level::Info, log::level_from("status"));

	EXPECT_EQ(log::Level::Debug, log::level_from("debug"));
	EXPECT_EQ(log::Level::Trace, log::level_from("trace"));

	EXPECT_EQ(log::Level::None, log::level_from("none"));
	EXPECT_EQ(log::Level::None, log::level_from("unknown-to-default"));
}

TEST_F(Log, level_from_case_insensitive)
{
	EXPECT_EQ(log::Level::Error, log::level_from("ERROR"));
	EXPECT_EQ(log::Level::Warning, log::level_from("WARNING"));
	EXPECT_EQ(log::Level::Info, log::level_from("INFO"));
	EXPECT_EQ(log::Level::Debug, log::level_from("DEBUG"));
	EXPECT_EQ(log::Level::Trace, log::level_from("TRACE"));
	EXPECT_EQ(log::Level::None, log::level_from("NONE"));
	EXPECT_EQ(log::Level::None, log::level_from("UNKNOWN-TO-DEFAULT"));
}

TEST_F(Log, level_from_empty)
{
	EXPECT_EQ(log::Level::None, log::level_from(""));
}

TEST_F(Log, level_label)
{
	EXPECT_EQ("Error", log::level_label(log::Level::Error));
	EXPECT_EQ(5, log::level_label(log::Level::Error).size());

	EXPECT_EQ("Warning", log::level_label(log::Level::Warning));
	EXPECT_EQ(7, log::level_label(log::Level::Warning).size());

	EXPECT_EQ("Info", log::level_label(log::Level::Info));
	EXPECT_EQ(4, log::level_label(log::Level::Info).size());

	EXPECT_EQ("Debug", log::level_label(log::Level::Debug));
	EXPECT_EQ(5, log::level_label(log::Level::Debug).size());

	EXPECT_EQ("Trace", log::level_label(log::Level::Trace));
	EXPECT_EQ(5, log::level_label(log::Level::Trace).size());

	EXPECT_EQ("None", log::level_label(log::Level::None));
	EXPECT_EQ(4, log::level_label(log::Level::None).size());
}

TEST_F(Log, set_target)
{
	ASSERT_EQ(stderr, log::get_target());
	log::set_target(stdout);
	ASSERT_EQ(stdout, log::get_target());

	log::set_level(log::Level::Error);
	::testing::internal::CaptureStdout();
	log::error("{}", "message");
	std::string const result {::testing::internal::GetCapturedStdout()};
	ASSERT_TRUE(result.find("Error: message\n") != std::string::npos);
}

TEST_F(Log, print_enabled_levels_error)
{
	log::set_level(log::Level::Error);
	::testing::internal::CaptureStderr();
	log::print_enabled_levels();
	std::string const result {::testing::internal::GetCapturedStderr()};
	ASSERT_TRUE(result.find("Logging: Error\n") != std::string::npos);
}

TEST_F(Log, print_enabled_levels_warning)
{
	log::set_level(log::Level::Warning);
	::testing::internal::CaptureStderr();
	log::print_enabled_levels();
	std::string const result = ::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(result.find("Logging: Error, Warning\n") != std::string::npos);
}

TEST_F(Log, print_enabled_levels_info)
{
	log::set_level(log::Level::Info);
	::testing::internal::CaptureStderr();
	log::print_enabled_levels();
	std::string const result = ::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(result.find("Logging: Error, Warning, Info\n") != std::string::npos);
}

TEST_F(Log, print_enabled_levels_debug)
{
	log::set_level(log::Level::Debug);
	::testing::internal::CaptureStderr();
	log::print_enabled_levels();
	std::string const result = ::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(result.find("Logging: Error, Warning, Info, Debug\n") != std::string::npos);
}

TEST_F(Log, print_enabled_levels_trace)
{
	log::set_level(log::Level::Trace);
	::testing::internal::CaptureStderr();
	log::print_enabled_levels();
	std::string const result = ::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(result.find("Logging: Error, Warning, Info, Debug, Trace\n") != std::string::npos);
}

TEST_F(Log, print_enabled_levels_none)
{
	log::set_level(log::Level::None);
	::testing::internal::CaptureStderr();
	log::print_enabled_levels();
	std::string const result = ::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(result.find("Logging: None\n") != std::string::npos);
}

TEST_F(Log, print_enabled_levels_stdout_error)
{
	log::set_target(stdout);
	log::set_level(log::Level::Error);
	::testing::internal::CaptureStdout();
	log::print_enabled_levels();
	std::string const result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Logging: Error\n") != std::string::npos);
}

TEST_F(Log, print_enabled_levels_stdout_none)
{
	log::set_target(stdout);
	log::set_level(log::Level::None);
	::testing::internal::CaptureStdout();
	log::print_enabled_levels();
	std::string const result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Logging: None\n") != std::string::npos);
}
