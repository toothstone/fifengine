#------------------------------------------------------------------------------
#                                 Fifengine                                          
#------------------------------------------------------------------------------

cmake_minimum_required (VERSION 2.6)

project (fifengine)

#------------------------------------------------------------------------------
#                                 Version                                          
#------------------------------------------------------------------------------

set (FIFE_MAJOR_VERSION 0)
set (FIFE_MINOR_VERSION 4)
set (FIFE_PATCH_VERSION 0)

set(FIFE_VERSION ${FIFE_MAJOR_VERSION}.${FIFE_MINOR_VERSION}.${FIFE_PATCH_VERSION})

configure_file (
  "${PROJECT_SOURCE_DIR}/engine/core/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
)

#------------------------------------------------------------------------------
#                                 Options                                          
#------------------------------------------------------------------------------

option(opengl           "Enable OpenGL support"                                 ON)
option(fifechan         "Enable Fifechan GUI subsystem"                         ON)
option(librocket        "Enable Librocket GUI subsystem"                        OFF)
option(cegui            "Enable Crazy Eddie's GUI subsystem"                    OFF)
option(zip              "Enable ZIP archive support"                            ON)
option(logging          "Enable logging"                                        ON)
option(rend-camzone     "Enable camera zone renderer"                           OFF)
option(rend-grid        "Enable camera grid renderer"                           OFF)
option(use-githash      "Try to determine the git hash for the current commit"  ON)

#------------------------------------------------------------------------------
#                                 Configure                                          
#------------------------------------------------------------------------------

# set path to our custom CMAKE scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Create a "Release" build by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Select build type. Options are: 'Debug', 'Release', 'RelWithDebInfo' and 'MinSizeRel'."
    FORCE
  )
  message(STATUS "Set CMAKE_BUILD_TYPE to default '${CMAKE_BUILD_TYPE}'.")
endif(NOT CMAKE_BUILD_TYPE)

# Do not allow an in-source-tree build, request an out-of-source-tree build.
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "#Please build outside of the source tree!\n                        
                       Make a separate folder for building and run CMake there:\n
                       mkdir _build; cd _build; cmake ..\n
                       Before that, cleanup:\nrm -rf CMakeCache.txt CMakeFiles")
endif()

# Disable in-source builds and modifications to the source tree.
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

#------------------------------------------------------------------------------
#                                 Compiler Options                                         
#------------------------------------------------------------------------------

if(WIN32)

  if(MSVC)

    # ensure we use minimal "windows.h" lib without the crazy min max macros    
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D \"WIN32_LEAN_AND_MEAN\"")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D \"NOMINMAX\"")

    # disable SAFESEH - to avoid "LNK2026: module unsafe" on "OpenAL32.lib"
    SET(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} /D \"SAFESEH:NO\"")
    SET(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

    # use static libs - to avoid "LNK1104: cannot open file 'libboost_filesystem-vc140-mt-1_59.lib'"
    set(Boost_USE_STATIC_LIBS ON)     
    set(Boost_USE_MULTITHREADED ON)

  endif(MSVC)

endif(WIN32)

# defines
add_definitions(-DTIXML_USE_STL)

#------------------------------------------------------------------------------
#                                 Display Status                                         
#------------------------------------------------------------------------------

message("")
message("== Building FIFE version ${FIFE_VERSION} using build type '${CMAKE_BUILD_TYPE}'.")
message(STATUS "    Source directory is '${PROJECT_SOURCE_DIR}'.")
message(STATUS "    Build  directory is '${PROJECT_BINARY_DIR}'.")
message(STATUS "    Installation path is '${CMAKE_INSTALL_PREFIX}' (set -DCMAKE_INSTALL_PREFIX=<path> to change).")
message(STATUS "    CXX Compiler Flags: '${CMAKE_CXX_FLAGS}'.")
message("")

#------------------------------------------------------------------------------
#                           Find Dependencies - Windows                                        
#------------------------------------------------------------------------------

# Tell CMake where to search for binaries (dlls), includes and libs on Windows
if(WIN32)
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "${PROJECT_SOURCE_DIR}/../fifengine-dependencies/build-tools/swig"
    "${PROJECT_SOURCE_DIR}/../fifengine-dependencies/includes/bin"
    "${PROJECT_SOURCE_DIR}/../fifengine-dependencies/includes/include"
    "${PROJECT_SOURCE_DIR}/../fifengine-dependencies/includes/include/SDL2"
    "${PROJECT_SOURCE_DIR}/../fifengine-dependencies/includes/include/OpenAL"
    "${PROJECT_SOURCE_DIR}/../fifengine-dependencies/includes/include/fifechan"
    "${PROJECT_SOURCE_DIR}/../fifengine-dependencies/includes/lib/x86"
    "${PROJECT_SOURCE_DIR}/../fifengine-dependencies/includes/lib"    
  )
endif(WIN32)

#------------------------------------------------------------------------------
#                                 Source Files                                         
#------------------------------------------------------------------------------

# tell compiler to search in the following folders for include files
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/engine/core")
include_directories("${PROJECT_SOURCE_DIR}")

# Core
set (FIFE_CORE_SRC
  ${PROJECT_SOURCE_DIR}/engine/core/audio/soundclip.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/audio/soundclipmanager.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/audio/soundemitter.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/audio/soundmanager.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/controller/engine.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/controller/enginesettings.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/eventchannel/eventmanager.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/ext/glee/GLee.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/loaders/native/audio/ogg_loader.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/loaders/native/audio/sounddecoder_ogg.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/loaders/native/map/animationloader.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/loaders/native/map/atlasloader.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/loaders/native/map/maploader.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/loaders/native/map/objectloader.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/loaders/native/map/percentdonelistener.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/loaders/native/video/imageloader.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/model.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/metamodel/action.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/metamodel/object.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/metamodel/timeprovider.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/metamodel/grids/cellgrid.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/metamodel/grids/hexgrid.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/metamodel/grids/squaregrid.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/structures/cell.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/structures/cellcache.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/structures/instance.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/structures/instancetree.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/structures/layer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/structures/location.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/structures/map.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/structures/renderernode.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/structures/trigger.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/model/structures/triggercontroller.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/pathfinder/route.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/pathfinder/routepather/multilayersearch.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/pathfinder/routepather/routepather.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/pathfinder/routepather/routepathersearch.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/pathfinder/routepather/singlelayersearch.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/savers/native/map/mapsaver.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/util/base/exception.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/util/base/fifeclass.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/util/base/stringutils.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/util/log/logger.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/util/math/angles.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/util/resource/resource.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/util/time/timeevent.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/util/time/timemanager.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/util/time/timer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/directoryprovider.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/fife_boost_filesystem.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/vfs.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/vfsdirectory.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/vfssource.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/vfssourceprovider.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/dat/dat1.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/dat/dat2.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/dat/lzssdecoder.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/dat/rawdatadat1.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/dat/rawdatadat2.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/raw/rawdata.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/raw/rawdatafile.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/raw/rawdatamemsource.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/raw/rawdatasource.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/animation.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/atlasbook.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/color.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/cursor.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/devicecaps.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/image.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/imagemanager.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/renderbackend.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/fonts/fontbase.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/fonts/imagefontbase.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/fonts/subimagefont.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/fonts/textrenderpool.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/fonts/truetypefont.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/opengl/glimage.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/opengl/renderbackendopengl.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/sdl/renderbackendsdl.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/sdl/sdlblendingfunctions.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/video/sdl/sdlimage.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/camera.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/layercache.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/rendererbase.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderitem.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/visual.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/blockinginforenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/cellrenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/cellselectionrenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/coordinaterenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/floatingtextrenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/genericrenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/gridrenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/instancerenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/lightrenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/offrenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/quadtreerenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/view/renderers/targetrenderer.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/hybrid/hybridguimanager.cpp
)

# ZIP
set (FIFE_ZIP_SRC
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/zip/zipfilesource.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/zip/zipnode.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/zip/zipprovider.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/zip/zipsource.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/vfs/zip/ziptree.cpp
)

# CEGUI v0.8.0+
set (FIFE_CEGUI_SRC
  ${PROJECT_SOURCE_DIR}/engine/core/gui/cegui/ceguimanager.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/cegui/base/ceguiinputprocessor.cpp
)

# FIFECHAN
set (FIFE_FCHAN_SRC
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/fifechanmanager.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/base/gui_font.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/base/gui_image.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/base/gui_imageloader.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/base/opengl/opengl_gui_graphics.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/base/sdl/sdl_gui_graphics.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/console/commandline.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/console/console.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/util/utf8/utf8stringeditor.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/widgets/clicklabel.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/widgets/icon2.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/widgets/percentagebar.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/widgets/togglebutton.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/fifechan/widgets/twobutton.cpp
)

# LIBROCKET
set (FIFE_LROCKET_SRC
  ${PROJECT_SOURCE_DIR}/engine/core/gui/librocket/librocketmanager.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/librocket/base/librocketinputprocessor.cpp
  ${PROJECT_SOURCE_DIR}/engine/core/gui/librocket/base/librocketrenderinterface.cpp
)

# SWIG files

# Core
set (FIFE_CORE_SWIG
  version.i
  audio/soundclip.i
  audio/soundclipmanager.i
  audio/soundemitter.i
  audio/soundmanager.i
  controller/engine.i
  eventchannel/eventchannel.i
  loaders/native/map/ianimationloader.i
  loaders/native/map/iatlasloader.i
  loaders/native/map/imaploader.i
  loaders/native/map/iobjectloader.i
  loaders/native/map/maploader.i
  loaders/native/map/percentdonelistener.i
  model/metamodel/action.i
  model/metamodel/ipather.i
  model/metamodel/ivisual.i
  model/metamodel/modelcoords.i
  model/metamodel/object.i
  model/metamodel/timeprovider.i
  model/metamodel/grids/cellgrids.i
  model/structures/cell.i
  model/structures/cellcache.i
  model/structures/instance.i
  model/structures/layer.i
  model/structures/location.i
  model/structures/map.i
  model/structures/renderernode.i
  model/structures/trigger.i
  model/model.i
  pathfinder/route.i
  pathfinder/routepather/routepather.i
  savers/native/map/ianimationsaver.i
  savers/native/map/iatlassaver.i
  savers/native/map/imapsaver.i
  savers/native/map/iobjectsaver.i
  savers/native/map/mapsaver.i
  util/base/utilbase.i
  util/log/logger.i
  util/math/math.i
  util/resource/resource.i
  util/structures/utilstructures.i
  util/time/timeevent.i
  util/time/timemanager.i
  vfs/vfs.i
  vfs/raw/rawdata.i
  video/video.i
  video/fonts/fonts.i
  view/camera.i
  view/rendererbase.i
  view/visual.i
  view/renderers/blockinginforenderer.i
  view/renderers/cellrenderer.i
  view/renderers/cellselectionrenderer.i
  view/renderers/coordinaterenderer.i
  view/renderers/floatingtextrenderer.i
  view/renderers/genericrenderer.i
  view/renderers/gridrenderer.i
  view/renderers/instancerenderer.i
  view/renderers/lightrenderer.i
  view/renderers/offrenderer.i
  view/renderers/targetrenderer.i
  gui/guimanager.i
  gui/hybrid/hybridguimanager.i
)

# CEGUI
set (FIFE_CEGUI_SWIG
  gui/cegui/ceguimanager.i
)

# FIFECHAN
set (FIFE_FCHAN_SWIG_LIB
  gui/fifechan/lib/fifechanlistener.i
  gui/fifechan/lib/widgets.i
)

set (FIFE_FCHAN_SWIG
  gui/fifechan/base/gui_font.i
  gui/fifechan/base/gui_image.i
  gui/fifechan/console/console.i
  gui/fifechan/fifechanmanager.i
  gui/fifechan/widgets/widgets.i
)

# LIBROCKET
set (FIFE_LROCKET_SWIG
  gui/librocket/librocketmanager.i
)

#------------------------------------------------------------------------------
#                                 Find Packages                                         
#------------------------------------------------------------------------------

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Boost 1.33.1 COMPONENTS system filesystem regex)
find_package(PNG REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(TinyXML REQUIRED)

if(cegui)
  find_package(CEGUI REQUIRED)

  if(NOT CEGUI_OpenGLRenderer_FOUND)
    message(FATAL_ERROR "FIFE needs CEGUI with the OpenGLRenderer enabled.")
  endif(NOT CEGUI_OpenGLRenderer_FOUND)

  if(NOT opengl)
    message(FATAL_ERROR "OpenGL cannot be disabled, when CEGUI is enabled.")
  endif(NOT opengl)

  include_directories(${CEGUI_INCLUDE_DIR})

  message("Using CEGUI v0.8+")
  set(CEGUI TRUE)
  set(FIFE_CORE_SRC ${FIFE_CORE_SRC} ${FIFE_CEGUI_SRC})

  find_package(GLEW REQUIRED)
  include_directories(${GLEW_INCLUDE_DIRS})
endif(cegui)

# Setting up SWIG files

set(CMAKE_SWIG_IMPORTS "")
set(CMAKE_SWIG_INCLUSIONS "")
set(CMAKE_SWIG_IMPORTS_FCHAN "")
set(CMAKE_SWIG_INCLUSIONS_FCHAN "")

foreach(filename ${FIFE_CORE_SWIG})
  set(CMAKE_SWIG_INCLUSIONS "${CMAKE_SWIG_INCLUSIONS}%include ${filename}\n")
endforeach(filename ${FIFE_CORE_SWIG})

if(fifechan)
  
  foreach(filename ${FIFE_FCHAN_SWIG_LIB})
    set(CMAKE_SWIG_INCLUSIONS_FCHAN "${CMAKE_SWIG_INCLUSIONS_FCHAN}%include ${filename}\n")
  endforeach(filename ${FIFE_FCHAN_SWIG_LIB})
  foreach(filename ${FIFE_FCHAN_SWIG})
    set(CMAKE_SWIG_INCLUSIONS "${CMAKE_SWIG_INCLUSIONS}%include ${filename}\n")
  endforeach(filename ${FIFE_FCHAN_SWIG})
  set(CMAKE_SWIG_IMPORTS "${CMAKE_SWIG_IMPORTS}%import ${PROJECT_BINARY_DIR}/fifechan.i")

  # TODO Change to search library once its been implemented for fifechan
  # https://github.com/fifengine/fifechan/issues/19
  set(FIFECHAN_INCLUDE ${PROJECT_SOURCE_DIR}/../fifengine-dependencies/includes/include/fifechan)
  set(FIFECHAN_LIBDIR  ${PROJECT_SOURCE_DIR}/../fifengine-dependencies/includes/lib)
  set(FIFECHAN_LIBS    fifechan fifechan_sdl)

  message(STATUS "FIFECHAN_INCLUDE = '${FIFECHAN_INCLUDE}'")
  message(STATUS "FIFECHAN_LIBDIR  = '${FIFECHAN_LIBDIR}'")
  message(STATUS "FIFECHAN_LIBS    = '${FIFECHAN_LIBS}'")

  if(opengl)
    set(FIFECHAN_LIBS ${FIFECHAN_LIBS} fifechan_opengl)
    add_definitions(-DHAVE_OPENGL)
  endif(opengl)
  if(zip)
    set(FIFE_CORE_SRC ${FIFE_CORE_SRC} ${FIFE_ZIP_SRC})
    add_definitions(-DHAVE_ZIP)
  endif(zip)
  if(logging)
    add_definitions(-DLOG_ENABLED)
  endif(logging)
  if(rend-camzone)
    add_definitions(-DRENDER_CAMZONES)
  endif(rend-camzone)
  if(rend-grid)
    add_definitions(-DRENDER_GRID)
  endif(rend-grid)
  if(use-githash)
    execute_process(
      COMMAND git rev-parse --short=8 HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_COMMIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    add_definitions(-DFIFE_GIT_HASH=${GIT_COMMIT_HASH})
  endif(use-githash)
  include_directories(${FIFECHAN_INCLUDE})
  link_directories(${FIFECHAN_LIBDIR})
  configure_file (
    "${PROJECT_SOURCE_DIR}/engine/swigwrappers/python/fifechan.i.templ.cm"
    "${PROJECT_BINARY_DIR}/fifechan.i"
    )
endif(fifechan)

if(cegui)  
  foreach(filename ${FIFE_CEGUI_SWIG})
    set(CMAKE_SWIG_INCLUSIONS "${CMAKE_SWIG_INCLUSIONS}%include ${filename}\n")
  endforeach(filename ${FIFE_CEGUI_SWIG})    
endif(cegui)

# TODO: Add the other gui libraries

configure_file (
  "${PROJECT_SOURCE_DIR}/engine/swigwrappers/python/fife.i.templ.cm"
  "${PROJECT_BINARY_DIR}/fife.i"
)

# Find SWIG and include the use swig file
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

# Find ogg dependency
find_package(PkgConfig QUIET)
pkg_check_modules(PC_OGG QUIET ogg)
find_path(OGG_INCLUDE_DIRS NAMES ogg/ogg.h HINTS ${PC_OGG_INCLUDE_DIRS} PATH_SUFFIXES ogg)
find_library(OGG_LIBRARIES NAMES ogg HINTS ${PC_OGG_LIBRARY_DIRS})

# Find vorbis dependency
find_package(PkgConfig QUIET)
pkg_check_modules(PC_VORBISFILE QUIET vorbisfile)
find_path(VORBIS_INCLUDE_DIRS NAMES vorbis/vorbisfile.h HINTS ${PC_VORBIS_INCLUDE_DIRS} PATH_SUFFIXES vorbis)
find_library(VORBIS_LIBRARY NAMES vorbis HINTS ${PC_VORBIS_LIBRARY_DIRS})

# Find vorbisfile dependency
find_package(PkgConfig QUIET)
pkg_check_modules(PC_VORBISFILE QUIET vorbisfile)
find_library(VORBISFILE_LIBRARY NAMES vorbisfile HINTS ${PC_VORBIS_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(OGG DEFAULT_MSG OGG_INCLUDE_DIRS OGG_LIBRARIES)
find_package_handle_standard_args(vorbisfile DEFAULT_MSG VORBIS_INCLUDE_DIRS VORBIS_LIBRARY)
find_package_handle_standard_args(vorbisfile DEFAULT_MSG VORBIS_INCLUDE_DIRS VORBISFILE_LIBRARY)

include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${OPENAL_INCLUDE_DIR})
include_directories(${OGG_INCLUDE_DIRS})
include_directories(${VORBIS_INCLUDE_DIRS})
include_directories(${TinyXML_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})

add_definitions(${PNG_DEFINITIONS})
add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})

# Find python library and add include path for python headers
find_package (PythonLibs)
include_directories (${PYTHON_INCLUDE_PATH})

set (CMAKE_SWIG_FLAGS -modern -modernargs -w511 -w473 -w404)

set_source_files_properties("${PROJECT_BINARY_DIR}/fife.i" PROPERTIES CPLUSPLUS ON)

swig_add_module(fife python "${PROJECT_BINARY_DIR}/fife.i"  ${FIFE_CORE_SRC} ${FIFE_FCHAN_SRC})
swig_link_libraries(fife ${PYTHON_LIBRARIES})
swig_link_libraries(fife ${SDL2_LIBRARY}
                         ${SDL2_IMAGE_LIBRARIES}
                         ${SDL2_TTF_LIBRARIES}
                         ${OPENAL_LIBRARY})
swig_link_libraries(fife ${Boost_LIBRARIES})
swig_link_libraries(fife ${PNG_LIBRARIES})
swig_link_libraries(fife ${VORBISFILE_LIBRARY})
swig_link_libraries(fife ${VORBIS_LIBRARY})
swig_link_libraries(fife ${OGG_LIBRARIES})
swig_link_libraries(fife ${TinyXML_LIBRARIES})
swig_link_libraries(fife ${OPENGL_gl_LIBRARY})

if(fifechan)
  set_source_files_properties("${PROJECT_BINARY_DIR}/fifechan.i" PROPERTIES CPLUSPLUS ON)
  swig_add_module(fifechan python "${PROJECT_BINARY_DIR}/fifechan.i")
  swig_link_libraries(fifechan ${PYTHON_LIBRARIES})
  swig_link_libraries(fifechan ${FIFECHAN_LIBS})
  swig_link_libraries(fife ${FIFECHAN_LIBS})
endif(fifechan)

if(cegui)
  swig_link_libraries(fife ${CEGUI_LIBRARIES})
  swig_link_libraries(fife ${CEGUI_OpenGLRenderer_LIBRARIES})
endif(cegui)

#
# Install Python package locally, e.g. into "C:\Python27\Lib\site-packages\fife"
#
execute_process(
  COMMAND python -c "from distutils.sysconfig import get_python_lib; print (get_python_lib())" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Slash-Fix: Cmake would create a warning, when using the variable in FILES or DIRECTORY without changing it.
if(NOT WIN32)
  STRING(REGEX REPLACE "\\\\" "/" PYTHON_SITE_PACKAGES ${PYTHON_SITE_PACKAGES} )
endif(NOT WIN32)

message(STATUS "    Python install directory is '${PYTHON_SITE_PACKAGES}/fife'.")

install(TARGETS     _fife                                               DESTINATION ${PYTHON_SITE_PACKAGES}/fife)
install(FILES       ${PROJECT_BINARY_DIR}/fife.py                       DESTINATION ${PYTHON_SITE_PACKAGES}/fife)
install(FILES       ${CMAKE_SOURCE_DIR}/engine/python/fife/__init__.py  DESTINATION ${PYTHON_SITE_PACKAGES}/fife)
install(DIRECTORY   ${CMAKE_SOURCE_DIR}/engine/python/fife/extensions   DESTINATION ${PYTHON_SITE_PACKAGES}/fife)

if(fifechan)
  install(TARGETS   _fifechan                                           DESTINATION ${PYTHON_SITE_PACKAGES}/fife)
  install(FILES     ${PROJECT_BINARY_DIR}/fifechan.py                   DESTINATION ${PYTHON_SITE_PACKAGES}/fife)
endif(fifechan)
