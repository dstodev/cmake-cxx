#include <gtest/gtest.h>

#include <log.hxx>

using namespace project;

TEST(Log, error)
{
	log::error("Error: %s\n", "message");
}

TEST(Log, error_one_arg)
{
	log::error("message\n");
}

TEST(Log, error_varargs)
{
	log::error("Error: %s %d\n", "message", 1);
}

TEST(Log, warn)
{
	log::warn("Warning: %s\n", "message");
}

TEST(Log, warn_one_arg)
{
	log::warn("message\n");
}

TEST(Log, warn_varargs)
{
	log::warn("Warning: %s %d\n", "message", 1);
}

TEST(Log, info)
{
	log::info("Info: %s\n", "message");
}

TEST(Log, info_one_arg)
{
	log::info("message\n");
}

TEST(Log, info_varargs)
{
	log::info("Info: %s %d\n", "message", 1);
}

TEST(Log, debug)
{
	log::debug("Debug: %s\n", "message");
}

TEST(Log, debug_one_arg)
{
	log::debug("message\n");
}

TEST(Log, debug_varargs)
{
	log::debug("Debug: %s %d\n", "message", 1);
}

TEST(Log, trace)
{
	log::trace("Trace: %s\n", "message");
}

TEST(Log, trace_one_arg)
{
	log::trace("message\n");
}

TEST(Log, trace_varargs)
{
	log::trace("Trace: %s %d\n", "message", 1);
}
