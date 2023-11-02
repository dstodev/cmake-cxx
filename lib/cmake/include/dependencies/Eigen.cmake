# https://eigen.tuxfamily.org/index.php?title=Main_Page
# https://gitlab.com/libeigen/eigen

CPMAddPackage(NAME eigen
	GITLAB_REPOSITORY libeigen/eigen
	GIT_TAG 3.4.0
	DOWNLOAD_ONLY TRUE
)
add_library(eigen INTERFACE)
add_library(Eigen3::Eigen ALIAS eigen)
target_include_directories(eigen INTERFACE
	$<BUILD_INTERFACE:${eigen_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)
set(eigen_SOURCE_DIR ${CPM_PACKAGE_eigen_SOURCE_DIR} CACHE INTERNAL "Eigen source directory")
