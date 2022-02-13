cmake_minimum_required(VERSION 3.10)
project(owd-editor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(wxWidgets_USE_LIBS)
find_package(wxWidgets)

IF(wxWidgets_FOUND)
	include("${wxWidgets_USE_FILE}")
	add_executable(editor src/app.cpp
						  src/app.hpp
						  src/compositionDialog.cpp
						  src/compositionDialog.hpp
						  src/constants.hpp
						  src/edits.cpp
						  src/edits.hpp
						  src/history.cpp
						  src/history.hpp
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
						  src/wxNumberCtrl.cpp
						  src/wxNumberCtrl.hpp
)

	target_link_libraries(editor ${wxWidgets_LIBRARIES})

ELSE(wxWidgets_FOUND)
	message("wxWidgets not found!")

ENDIF(wxWidgets_FOUND)
