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
set(SRC_DIR "include")
set(INC_DIR "include")

set(
        SOURCES
        ${INC_DIR}/gui/guiBaseClass.hpp
        ${INC_DIR}/gui/guiController.hpp
        ${INC_DIR}/gui/guiLayer.hpp
        ${INC_DIR}/messageBus/messageBus.hpp
        ${INC_DIR}/messageBus/receiver.hpp
        ${INC_DIR}/modules/game/camera.hpp
        ${INC_DIR}/modules/game/collisionCollum.hpp
        ${INC_DIR}/modules/game/collisionSystem.hpp
        ${INC_DIR}/modules/game/countdown.hpp
        ${INC_DIR}/modules/game/gameTimer.hpp
        ${INC_DIR}/modules/game/mapCollum.hpp
        ${INC_DIR}/modules/game/mapRenderer.hpp
        ${INC_DIR}/modules/game/player.hpp
        ${INC_DIR}/modules/game/scriptManager.hpp
        ${INC_DIR}/modules/game/timeTable.hpp
        ${INC_DIR}/modules/states/game.hpp
        ${INC_DIR}/modules/states/intro.hpp
        ${INC_DIR}/modules/states/menu.hpp
        ${INC_DIR}/modules/states/state.hpp
        ${INC_DIR}/modules/networkHandler.hpp
	${INC_DIR}/modules/chatHandler.hpp    
        ${INC_DIR}/modules/settings.hpp
        ${INC_DIR}/modules/stateMachine.hpp
        ${INC_DIR}/modules/windowHandler.hpp
        ${INC_DIR}/utilities/anyType.hpp
        ${INC_DIR}/utilities/hash.hpp
	${INC_DIR}/sfchat/sfchat.h
        ${INC_DIR}/application.hpp
	${INC_DIR}/gui/guiBaseClass.cpp
        ${INC_DIR}/gui/guiController.cpp
        ${INC_DIR}/gui/guiLayer.cpp
        ${INC_DIR}/messageBus/messageBus.cpp
        ${INC_DIR}/modules/game/camera.cpp
        ${INC_DIR}/modules/game/collisionCollum.cpp
        ${INC_DIR}/modules/game/collisionSystem.cpp
        ${INC_DIR}/modules/game/countdown.cpp
        ${INC_DIR}/modules/game/gameTimer.cpp
        ${INC_DIR}/modules/game/mapCollum.cpp
        ${INC_DIR}/modules/game/mapRenderer.cpp
        ${INC_DIR}/modules/game/player.cpp
        ${INC_DIR}/modules/game/scriptManager.cpp
        ${INC_DIR}/modules/game/timeTable.cpp
        ${INC_DIR}/modules/states/game.cpp
        ${INC_DIR}/modules/states/intro.cpp
        ${INC_DIR}/modules/states/menu.cpp
        ${INC_DIR}/modules/states/state.cpp
        ${INC_DIR}/modules/networkHandler.cpp
	${INC_DIR}/modules/chatHandler.cpp        
	${INC_DIR}/modules/settings.cpp
        ${INC_DIR}/modules/stateMachine.cpp
        ${INC_DIR}/modules/windowHandler.cpp
        ${INC_DIR}/utilities/hash.cpp
	${INC_DIR}/sfchat/sfchat.cpp
	${INC_DIR}/application.cpp
	${INC_DIR}/main.cpp
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