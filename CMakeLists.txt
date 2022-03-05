cmake_minimum_required(VERSION 3.10)
project(owd-editor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG") 

set(wxWidgets_USE_LIBS)
find_package(wxWidgets)

set(EDITOR_SOURCES
	src/app.cpp
	src/app.hpp
	src/compositionDialog.cpp
	src/compositionDialog.hpp
	src/constants.hpp
	src/edits.cpp
	src/edits.hpp
	src/history.cpp
	src/history.hpp
	src/icons.cpp
	src/icons.hpp
	src/imagePanel.cpp
	src/imagePanel.hpp
	src/includes.hpp
	src/main.cpp
	src/mainFrame.cpp
	src/mainFrame.hpp
	src/regionPanel.cpp
	src/regionPanel.hpp
	src/toolbar.cpp
	src/toolbar.hpp
	src/wxField.hpp
	src/wxField.cpp
	src/wxNumberCtrl.cpp
	src/wxNumberCtrl.hpp
)

if(wxWidgets_FOUND)
	include("${wxWidgets_USE_FILE}")

	if(WIN32)
		add_executable(editor WIN32 ${EDITOR_SOURCES})
	else(WIN32)
		add_executable(editor ${EDITOR_SOURCES})
	endif(WIN32)

	target_link_libraries(editor ${wxWidgets_LIBRARIES})

else(wxWidgets_FOUND)
	message("wxWidgets not found!")

endif(wxWidgets_FOUND)
