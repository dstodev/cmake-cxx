#ifndef APPLICATION_STATE_HXX
#define APPLICATION_STATE_HXX

namespace project {

enum class ApplicationState
{
	NOT_INITIALIZED,
	INITIALIZED,
	RUNNING,
	QUITTING,
	DONE,
};

}  // namespace project

#endif  // APPLICATION_STATE_HXX
