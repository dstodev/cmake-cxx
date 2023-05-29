#ifndef APPLICATION_HXX
#define APPLICATION_HXX

#include <memory>

#include <project-api.h>

namespace project {

class ApplicationImpl;

class PROJECT_API Application
{
public:
	Application static& instance();

	void init();
	int app_main(int argc, char* argv[]);

	virtual ~Application();

protected:
	std::unique_ptr<ApplicationImpl> _impl;

private:
	Application();
};

}  // namespace project

#endif  // APPLICATION_HXX
