cmake_minimum_required(VERSION 2.8)

project(Kroniax)

# Set options
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
set(SFML_STATIC_LIBS FALSE CACHE BOOL "Choose whether SFML is linked statically or not.")
set(KRONIAX_STATIC_STD_LIBS FALSE CACHE BOOL "Use statically linked standard/runtime libraries? This option must match the one used for SFML.")
    
# Make sure that the runtime library gets link statically
if(KRONIAX_STATIC_STD_LIBS)
	if(NOT SFML_STATIC_LIBS)
		message("\n-> If you check KRONIAX_STATIC_STD_LIBS, you also need to check SFML_STATIC_LIBRARIES.")
		message("-> It would lead to multiple runtime environments which result in undefined behavior.\n")
	elseif(WIN32 AND MSVC)
		# Change all MSVC compiler flags to /MT
		foreach(flag CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
			if(${flag} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
		endforeach()
	elseif(CMAKE_COMPILER_IS_GNUCXX)
		# Note: Doesn't work for TDM compiler, since it's compiling the runtime libs statically by default
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
	endif()
endif()

# Kroniax uses C++11 features
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# Add directory containing FindSFML.cmake to module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}")

# Make sure that FindSFML.cmake searches for the static libraries
if(SFML_STATIC_LIBS)
	set(SFML_STATIC_LIBRARIES TRUE)
endif()

# Find SFML
find_package(SFML 2 COMPONENTS audio graphics window system network)

# Output an error if SFML wasn't found
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
else()
	set(SFML_ROOT "" CACHE PATH "SFML top-level directory")
	message("\n-> SFML directory not found. Set SFML_ROOT to SFML's top-level path (containing \"include\" and \"lib\" directories).")
	message("-> Make sure the SFML libraries with the same configuration (Release/Debug, Static/Dynamic) exist.\n")
endif()

# Add the source files
set(SRC_DIR "src")
set(INC_DIR "include")

set(
	SOURCES
	${INC_DIR}/aw/utilities/Converter.hpp
	${INC_DIR}/aw/utilities/StopWatch.hpp
	${INC_DIR}/game/DialogScreen.hpp
	${INC_DIR}/game/Game.hpp
	${INC_DIR}/game/HighscoreBoard.hpp
	${INC_DIR}/game/HighscoreUploader.hpp
	${INC_DIR}/game/Level.hpp
	${INC_DIR}/game/Menu.hpp
	${INC_DIR}/game/MenuBackground.hpp
	${INC_DIR}/game/Settings.hpp
	${INC_DIR}/gui/GuiBaseClass.hpp
	${INC_DIR}/gui/GuiController.hpp
	${INC_DIR}/gui/GuiLayer.hpp
	${SRC_DIR}/aw/utilities/Converter.cpp
	${SRC_DIR}/aw/utilities/StopWatch.cpp
	${SRC_DIR}/game/DialogScreen.cpp
	${SRC_DIR}/game/Game.cpp
	${SRC_DIR}/game/HighscoreBoard.cpp
	${SRC_DIR}/game/HighscoreUploader.cpp
	${SRC_DIR}/game/Level.cpp
	${SRC_DIR}/game/Menu.cpp
	${SRC_DIR}/game/MenuBackground.cpp
	${SRC_DIR}/game/Settings.cpp
	${SRC_DIR}/game/Tracer.cpp
	${SRC_DIR}/gui/GuiBaseClass.cpp
	${SRC_DIR}/gui/GuiController.cpp
	${SRC_DIR}/gui/GuiLayer.cpp
	${SRC_DIR}/Main.cpp
)

# Tell CMake to build a executable 
add_executable(Kroniax ${SOURCES})

# Link SFML
target_link_libraries(Kroniax ${SFML_LIBRARIES})

# Install executable
install(TARGETS Kroniax
		RUNTIME DESTINATION .)

# Install game data
install(DIRECTORY config levels data
		DESTINATION .)

# On Windows install required audio libraries
if(WIN32)
	install(FILES ${SFML_ROOT}/bin/openal32.dll ${SFML_ROOT}/bin/libsndfile-1.dll
			DESTINATION .)
endif()
