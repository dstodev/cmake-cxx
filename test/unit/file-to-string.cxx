#include <gtest/gtest.h>

#include <file-to-string.hxx>
#include <this-file.hxx>

using namespace project;

TEST(FileToString, file_to_string)
{
	auto const path = THIS_DIRECTORY() / "resource/sample-main.cxx";
	auto const expected = "#include <iostream>\n"
	                      "\n"
	                      "int main()\n"
	                      "{\n"
	                      "	std::cout << \"Hello, world!\" << std::endl;\n"  // note tab characters in string
	                      "	return 0;\n"
	                      "}\n";
	auto const content = file_to_string(path);

	EXPECT_FALSE(content.empty());
	EXPECT_EQ(expected, content);
}
