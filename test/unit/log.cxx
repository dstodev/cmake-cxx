#include <gtest/gtest.h>

#define ENABLE_LOGGING 1
#include <log.hxx>

using namespace project;

TEST(Log, error)
{
	log::set_level(log::Level::Error);
	log::error("Error: {}\n", "message");
}

TEST(Log, error_one_arg)
{
	log::set_level(log::Level::Error);
	log::error("message\n");
}

TEST(Log, error_varargs)
{
	log::set_level(log::Level::Error);
	log::error("Error: {} {}\n", "message", 1);
}

TEST(Log, warn)
{
	log::set_level(log::Level::Warn);
	log::warn("Warning: {}\n", "message");
}

TEST(Log, warn_one_arg)
{
	log::set_level(log::Level::Warn);
	log::warn("message\n");
}

TEST(Log, warn_varargs)
{
	log::set_level(log::Level::Warn);
	log::warn("Warning: {} {}\n", "message", 1);
}

TEST(Log, info)
{
	log::set_level(log::Level::Info);
	log::info("Info: {}\n", "message");
}

TEST(Log, info_one_arg)
{
	log::set_level(log::Level::Info);
	log::info("message\n");
}

TEST(Log, info_varargs)
{
	log::set_level(log::Level::Info);
	log::info("Info: {} {}\n", "message", 1);
}

TEST(Log, debug)
{
	log::set_level(log::Level::Debug);
	log::debug("Debug: {}\n", "message");
}

TEST(Log, debug_one_arg)
{
	log::set_level(log::Level::Debug);
	log::debug("message\n");
}

TEST(Log, debug_varargs)
{
	log::set_level(log::Level::Debug);
	log::debug("Debug: {} {}\n", "message", 1);
}

TEST(Log, trace)
{
	log::set_level(log::Level::Trace);
	log::trace("Trace: {}\n", "message");
}

TEST(Log, trace_one_arg)
{
	log::set_level(log::Level::Trace);
	log::trace("message\n");
}

TEST(Log, trace_varargs)
{
	log::set_level(log::Level::Trace);
	log::trace("Trace: {} {}\n", "message", 1);
}

TEST(Log, none)
{
	log::set_level(log::Level::Trace);
	log::trace("Before none\n");
	log::set_level(log::Level::None);
	log::trace("During none\n");
	log::set_level(log::Level::Trace);
	log::trace("After none\n");
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
