cmake_minimum_required(VERSION 3.30)
project(Mod3DPybind VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(opencascade REQUIRED COMPONENTS
    FoundationClasses
    ModelingData
    ModelingAlgorithms
)

message("Found OpenCASCADE version: ${OpenCASCADE_MAJOR_VERSION}.${OpenCASCADE_MINOR_VERSION}.${OpenCASCADE_MAINTENANCE_VERSION}")
message("OpenCASCADE :${OpenCASCADE_INCLUDE_DIR}")

find_package(pybind11 REQUIRED)


# Build the Python extension module with OCCT linkage
pybind11_add_module(mod3d 
    src/mod3d.cpp
    src/bindGp.cpp
    src/bindGeomFill.cpp
    src/bindGeomCurves.cpp
    src/bindGeom.cpp
    src/bindTopoDS.cpp
    src/bindShapes.cpp
    src/bindBRepPrim.cpp
    src/bindBRepBuilderAPI.cpp
)
target_compile_definitions(mod3d PRIVATE VERSION_INFO=\"${PROJECT_VERSION}\")
target_link_libraries(mod3d PRIVATE
    pybind11::module
    ${OpenCASCADE_FoundationClasses_LIBRARIES}
    ${OpenCASCADE_ModelingData_LIBRARIES}
    ${OpenCASCADE_ModelingAlgorithms_LIBRARIES}
)

target_include_directories(mod3d PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src 
    ${OpenCASCADE_INCLUDE_DIR}
)

option(MOD3D_BUILD_TESTING "Add convenience targets for development" ON)
if (MOD3D_BUILD_TESTING AND BUILD_TESTING)
    add_custom_target(check COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target mod3d)
endif()

# add installation target
cmake_policy(SET CMP0177 NEW)
install(TARGETS mod3d
    LIBRARY DESTINATION ${Python3_SITEARCH}/mod3d
    ARCHIVE DESTINATION ${Python3_SITEARCH}/mod3d
    RUNTIME DESTINATION ${Python3_SITEARCH}/mod3d
)

# Install the package shim so the namespace package can resolve from Python.
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/python/mod3d/__init__.py
    DESTINATION ${Python3_SITEARCH}/mod3d)
