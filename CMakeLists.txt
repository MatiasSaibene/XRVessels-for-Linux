cmake_minimum_required(VERSION 3.16)
project(XR2Ravenstar)

set(ORBITER_ROOT $ENV{HOME}/orbiter_test/Orbiter)
set(ORBITER_BINARY_SDK_DIR ${ORBITER_ROOT}/Orbitersdk)
set(LIBRARY_DIRECTORY ${ORBITER_ROOT}Orbitersdk/lib)
file(GLOB FRAMEWORK_FILES ${CMAKE_CURRENT_SOURCE_DIR}/XRVessels/framework/framework/*.cpp)
file(GLOB XR2_FILES ${CMAKE_CURRENT_SOURCE_DIR}/XRVessels/XR2Ravenstar/XR2Ravenstar/*.cpp)
file(GLOB LIBRARIES ${LIBRARY_DIRECTORY}/*.a)
file(GLOB XR1_LIB_FILES ${CMAKE_CURRENT_SOURCE_DIR}/XRVessels/DeltaGliderXR1/XR1Lib/*.cpp)


add_library(XR2Ravenstar SHARED
    ${FRAMEWORK_FILES}
    ${XR1_LIB_FILES}
    ${XR2_FILES}
)

target_include_directories(XR2Ravenstar PUBLIC
	${ORBITER_BINARY_SDK_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/XRVessels/framework/framework
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${CMAKE_CURRENT_SOURCE_DIR}/XRVessels/XR2Ravenstar/XR2Ravenstar
    ${CMAKE_CURRENT_SOURCE_DIR}/XRVessels/DeltaGliderXR1/XR1Lib
	PUBLIC ${ORBITER_BINARY_SDK_DIR}/include/ScnEditor      # for ScnEditorAPI.h
)

target_link_libraries(XR2Ravenstar
    ${LIBRARIES}
)

add_compile_options(-fPICg)