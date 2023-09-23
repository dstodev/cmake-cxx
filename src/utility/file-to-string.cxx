#include "file-to-string.hxx"

#include <fstream>

namespace project {

std::string file_to_string(std::filesystem::path const& path)
{
	std::ifstream file {path};
	std::string content;

	if (file.is_open()) {
		content = {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
		file.close();
	}

	return content;
}

}  // namespace project
