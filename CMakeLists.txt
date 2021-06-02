##---------------------------------------------------------------------------
## Author:      Jorgen Bodde
## Copyright:   (c) Jorgen Bodde
## License:     wxWidgets License
## Update:      2008/12 by Werner Smekal
##---------------------------------------------------------------------------

# define minimum cmake version
# cmake_minimum_required(VERSION 2.6.2)
cmake_minimum_required(VERSION 3.19)

# Our project is called 'minimal' this is how it will be called in
# visual studio, and in our makefiles. 
project(GardenPhotos)

# Location where cmake first looks for cmake modules.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

# We set C++ version
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##---------------------------------------------------
## Please set your wxWidgets configuration here
##--------------------------------------------------- 
 
# Here you can define what libraries of wxWidgets you need for your
# application. You can figure out what libraries you need here;
# https://www.wxwidgets.org/manuals/2.8/wx_librarieslist.html
# We need the Find package for wxWidgets to work
# NOTE: if you're using aui, include aui in this required components list.

# It was noticed that when using MinGW gcc it is essential that 'core' is mentioned before 'base'.
find_package(wxWidgets COMPONENTS core base REQUIRED)
find_package(Poco REQUIRED Data)
find_package(Poco REQUIRED DataSQLite)
find_package(Poco REQUIRED Foundation)
find_package(exiv2 REQUIRED CONFIG NAMES exiv2)
find_package(OpenCV REQUIRED COMPONENTS core imgproc)
 
##---------------------------------------------------
## Actual config file starts here
##--------------------------------------------------- 
 
# wxWidgets include (this will do all the magic to configure everything)
include( "${wxWidgets_USE_FILE}" )

# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out
set(SRCS
    src/utils.h
    src/utils.cpp
    src/garden_photos.h
    src/garden_photos.cpp
    src/image_panel.h
    src/image_panel.cpp
    src/photos.h
    src/photos.cpp
    src/menu.h
    src/menu.cpp    
    src/sidebar.h
    src/sidebar.cpp    
    src/sidebar_item.h
    src/sidebar_item.cpp
    src/main_frame.h
    src/main_frame.cpp
    src/config.h
    src/config.cpp
)

set(RESOURCE_FILES
icons/photos-24.png
icons/photos-18.png
icons/photos-72.png  
icons/favorite-18.png
icons/favorite-24.png
icons/favorite-48.png
icons/favoritewhite-24.png
icons/favoritewhite-48.png
)

# If we build for windows systems, we also include the resource file
# containing the manifest, icon and other resources
if(WIN32)
  set(SRCS ${SRCS} minimal.rc)
endif(WIN32)

# Here we define the executable minimal.exe or minimal on other systems
# the above paths and defines will be used in this build
add_executable(GardenPhotos WIN32 ${SRCS} ${RESOURCE_FILES})

# We add to our target 'minimal' the wxWidgets libraries. These are
# set for us by the find script. If you need other libraries, you
# can add them here as well. 
target_link_libraries(GardenPhotos ${wxWidgets_LIBRARIES} exiv2 PocoDataSQLite opencv_core opencv_imgproc opencv_imgcodecs Poco::Data PocoFoundation)
  
set_target_properties(GardenPhotos PROPERTIES
  MACOSX_BUNDLE TRUE
  MACOSX_FRAMEWORK_IDENTIFIER com.andreagiardina.GardenPhotos
  RESOURCE "${RESOURCE_FILES}")
