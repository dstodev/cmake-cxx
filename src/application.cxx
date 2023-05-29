#include "application.hxx"

#include <application-impl.hxx>

namespace project {

Application::Application()
    : _impl(new ApplicationImpl {})
{}

Application& Application::instance()
{
	Application static instance;
	return instance;
}

void Application::init()
{
	_impl->init();
}

int Application::app_main(int argc, char* argv[])
{
	return _impl->app_main(argc, argv);
}

Application::~Application() = default;

}  // namespace project
