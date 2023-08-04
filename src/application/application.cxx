#include "application.hxx"

#include <application-impl.hxx>

namespace project {

Application::Application()
    : _impl(&ApplicationImpl::instance())
{}

Application::~Application() = default;

void Application::init()
{
	_impl->init();
}

int Application::app_main(int argc, char* argv[])
{
	return _impl->app_main(argc, argv);
}

}  // namespace project
