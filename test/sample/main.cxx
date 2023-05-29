#include <application.hxx>
#include <log.hxx>

using namespace project;

int main(int argc, char* args[])
{
	auto& app = Application::instance();

	try {
		app.init();
		app.app_main(argc, args);
	} catch (std::exception& e) {
		log::error("Application failed: %s\n", e.what());
	}

	return 0;
}
