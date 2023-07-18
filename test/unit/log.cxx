#include <gtest/gtest.h>

#ifndef ENABLE_LOGGING
#define ENABLE_LOGGING 1
#endif
#include <log.hxx>

using namespace project;

TEST(Log, error)
{
	log::set_level(log::Level::Error);
	ASSERT_EQ(log::Level::Error, log::get_level());

	::testing::internal::CaptureStdout();
	log::error("Error: {}\n", "message");
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Error: message\n") != std::string::npos);
}

TEST(Log, error_one_arg)
{
	log::set_level(log::Level::Error);

	::testing::internal::CaptureStdout();
	log::error("message\n");
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("message\n") != std::string::npos);
}

TEST(Log, error_varargs)
{
	log::set_level(log::Level::Error);

	::testing::internal::CaptureStdout();
	log::error("Error: {} {}\n", "message", 1);
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Error: message 1\n") != std::string::npos);
}

TEST(Log, warn)
{
	log::set_level(log::Level::Warn);
	ASSERT_EQ(log::Level::Warn, log::get_level());

	::testing::internal::CaptureStdout();
	log::warn("Warning: {}\n", "message");
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Warning: message\n") != std::string::npos);
}

TEST(Log, warn_one_arg)
{
	log::set_level(log::Level::Warn);

	::testing::internal::CaptureStdout();
	log::warn("message\n");
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("message\n") != std::string::npos);
}

TEST(Log, warn_varargs)
{
	log::set_level(log::Level::Warn);

	::testing::internal::CaptureStdout();
	log::warn("Warning: {} {}\n", "message", 1);
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Warning: message 1\n") != std::string::npos);
}

TEST(Log, info)
{
	log::set_level(log::Level::Info);
	ASSERT_EQ(log::Level::Info, log::get_level());

	::testing::internal::CaptureStdout();
	log::info("Info: {}\n", "message");
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Info: message\n") != std::string::npos);
}

TEST(Log, info_one_arg)
{
	log::set_level(log::Level::Info);

	::testing::internal::CaptureStdout();
	log::info("message\n");
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("message\n") != std::string::npos);
}

TEST(Log, info_varargs)
{
	log::set_level(log::Level::Info);

	::testing::internal::CaptureStdout();
	log::info("Info: {} {}\n", "message", 1);
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Info: message 1\n") != std::string::npos);
}

TEST(Log, debug)
{
	log::set_level(log::Level::Debug);
	ASSERT_EQ(log::Level::Debug, log::get_level());

	::testing::internal::CaptureStdout();
	log::debug("Debug: {}\n", "message");
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Debug: message\n") != std::string::npos);
}

TEST(Log, debug_one_arg)
{
	log::set_level(log::Level::Debug);

	::testing::internal::CaptureStdout();
	log::debug("message\n");
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("message\n") != std::string::npos);
}

TEST(Log, debug_varargs)
{
	log::set_level(log::Level::Debug);

	::testing::internal::CaptureStdout();
	log::debug("Debug: {} {}\n", "message", 1);
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Debug: message 1\n") != std::string::npos);
}

TEST(Log, trace)
{
	log::set_level(log::Level::Trace);
	ASSERT_EQ(log::Level::Trace, log::get_level());

	::testing::internal::CaptureStdout();
	log::trace("Trace: {}\n", "message");
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Trace: message\n") != std::string::npos);
}

TEST(Log, trace_one_arg)
{
	log::set_level(log::Level::Trace);

	::testing::internal::CaptureStdout();
	log::trace("message\n");
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("message\n") != std::string::npos);
}

TEST(Log, trace_varargs)
{
	log::set_level(log::Level::Trace);

	::testing::internal::CaptureStdout();
	log::trace("Trace: {} {}\n", "message", 1);
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Trace: message 1\n") != std::string::npos);
}

TEST(Log, none)
{
	log::set_level(log::Level::Trace);

	::testing::internal::CaptureStdout();
	log::trace("Before none\n");
	std::string result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("Before none\n") != std::string::npos);

	log::set_level(log::Level::None);
	ASSERT_EQ(log::Level::None, log::get_level());

	::testing::internal::CaptureStdout();
	log::trace("During none\n");
	result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.empty());

	log::set_level(log::Level::Trace);

	::testing::internal::CaptureStdout();
	log::trace("After none\n");
	result = ::testing::internal::GetCapturedStdout();
	ASSERT_TRUE(result.find("After none\n") != std::string::npos);
}

TEST(Log, level_from)
{
	EXPECT_EQ(log::Level::Error, log::level_from("error"));
	EXPECT_EQ(log::Level::Warn, log::level_from("warn"));
	EXPECT_EQ(log::Level::Info, log::level_from("info"));
	EXPECT_EQ(log::Level::Debug, log::level_from("debug"));
	EXPECT_EQ(log::Level::Trace, log::level_from("trace"));
	EXPECT_EQ(log::Level::None, log::level_from("unknown-to-default"));
}

TEST(Log, level_from_case_insensitive)
{
	EXPECT_EQ(log::Level::Error, log::level_from("ERROR"));
	EXPECT_EQ(log::Level::Warn, log::level_from("WARN"));
	EXPECT_EQ(log::Level::Info, log::level_from("INFO"));
	EXPECT_EQ(log::Level::Debug, log::level_from("DEBUG"));
	EXPECT_EQ(log::Level::Trace, log::level_from("TRACE"));
	EXPECT_EQ(log::Level::None, log::level_from("UNKNOWN-TO-DEFAULT"));
}

TEST(Log, level_default_none)
{
	log::Level level {};
	EXPECT_EQ(log::Level::None, level);
}

TEST(Log, level_from_empty)
{
	EXPECT_EQ(log::Level::None, log::level_from(""));
}
