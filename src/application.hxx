#ifndef APPLICATION_HXX
#define APPLICATION_HXX

#include <project-api.h>

namespace project {

class ApplicationImpl;

class PROJECT_API Application
{
public:
	Application();

	void init();
	int app_main(int argc, char* argv[]);

	virtual ~Application();

protected:
	ApplicationImpl* _impl;
};

}  // namespace project

#endif  // APPLICATION_HXX
