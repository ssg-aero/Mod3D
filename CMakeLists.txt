cmake_minimum_required(VERSION 3.30)
project(Mod3DPybind VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(APPLE)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()


include(GNUInstallDirs)

message("-- CMake install prefix: " ${CMAKE_INSTALL_PREFIX})
message("  -> binaries: " ${CMAKE_INSTALL_BINDIR})
message("  -> libs: " ${CMAKE_INSTALL_LIBDIR})
message("  -> includes: " ${CMAKE_INSTALL_INCLUDEDIR})


find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(OpenCASCADE REQUIRED COMPONENTS
    FoundationClasses
    ModelingData
    ModelingAlgorithms
    Visualization
    ApplicationFramework
    DataExchange
)

message("Found OpenCASCADE version: ${OpenCASCADE_MAJOR_VERSION}.${OpenCASCADE_MINOR_VERSION}.${OpenCASCADE_MAINTENANCE_VERSION}")
message("OpenCASCADE :${OpenCASCADE_INCLUDE_DIR}")

find_package(pybind11 REQUIRED)

option(MOD3D_USE_GBS "Add convenience targets for development" ON)
if(MOD3D_USE_GBS)
    find_package(Gbs OPTIONAL)

    if(Gbs_FOUND)
        message("Found GBS: ${GBS_INCLUDE_DIR} $<BOOL:${Gbs_FOUND}>")
    else()
        message(WARNING "GBS library not found. Some features may be unavailable.")
    endif()
endif()

find_package (Eigen3 REQUIRED NO_MODULE) # ToDo prperly add dep to GBS

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(WIN32)
add_library(occt-extend SHARED
    src/extend/UnionFaces.cpp
    src/extend/GeomSurfaceCompare.cpp
    src/extend/shapes.cpp
)
# if(WIN32)
#     set_target_properties(occt-extend PROPERTIES
#         WINDOWS_EXPORT_ALL_SYMBOLS ON
#     )
# endif(WIN32)
# set_target_properties(occt-extend PROPERTIES
#     $<$<PLATFORM_ID:Windows>:WINDOWS_EXPORT_ALL_SYMBOLS> ON
# )
option(MOD3D_BUILD_OCCTEXT "Add convenience targets for development" ON)
option(MOD3D_BUILD_PYTHON "Add convenience targets for development" ON)
if(MOD3D_BUILD_OCCTEXT OR MOD3D_BUILD_PYTHON)
    target_link_libraries(occt-extend PRIVATE
        ${OpenCASCADE_FoundationClasses_LIBRARIES}
        ${OpenCASCADE_ModelingData_LIBRARIES}
        ${OpenCASCADE_ModelingAlgorithms_LIBRARIES}
        ${OpenCASCADE_DataExchange_LIBRARIES}
    )

    target_include_directories(occt-extend PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src 
        ${CMAKE_CURRENT_SOURCE_DIR}/inc 
        ${OpenCASCADE_INCLUDE_DIR}
    )
    cmake_policy(SET CMP0177 NEW)
    install(TARGETS occt-extend
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
    )
endif()

if(MOD3D_BUILD_PYTHON)
    
    # Build the Python extension module with OCCT linkage
    pybind11_add_module(mod3d 
        src/mod3d.cpp
        src/bind_gp.cpp
        src/bind_geom_fill.cpp
        src/bind_geom_abs.cpp
        src/bind_geom_curves_splines.cpp
        src/bind_geom_curves_conics.cpp
        src/bind_geom_curves.cpp
        src/bind_geom_surfaces.cpp
        src/bind_geom_surfaces_splines.cpp
        src/bind_geom_surfaces_elementary.cpp
        src/bind_geom_swept_surfaces.cpp
        src/bind_geom.cpp
        src/bind_geom_api.cpp
        src/bind_geom2d.cpp
        src/bind_geom2d_curves_conics.cpp
        src/bind_geom2d_curves_splines.cpp
        src/bind_geom2d_curves.cpp
        src/bind_topo_ds.cpp
        src/bind_top_abs.cpp
        src/bind_top_exp.cpp
        src/bind_shapes.cpp
        src/bind_adaptor.cpp
        src/bind_brep.cpp
        src/bind_brep_prim.cpp
        src/bind_brep_prim_api.cpp
        src/bind_brep_prim_api_one_axis.cpp
        src/bind_brep_prim_api_sweep.cpp
        src/bind_brep_builder_api.cpp
        src/bind_brep_offset_api.cpp
        src/bind_brep_extrema.cpp
        src/bind_brep_fillet.cpp
        src/bind_brep_boolean_op.cpp
        src/bind_brep_tools.cpp
        # src/bind_brep_fillet2d.cpp
        src/bind_brep_gprop.cpp
        src/bind_brep_lib.cpp
        src/bind_shape_analysis.cpp
        src/bind_shape_fix.cpp
        src/bind_shape_upgrade.cpp
        src/bind_step_control.cpp
        src/bind_step_data.cpp
        src/bind_render.cpp
    )
    target_compile_definitions(mod3d PRIVATE 
        VERSION_INFO=\"${PROJECT_VERSION}\" 
        _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS # For Eigen
        HAS_GBS=$<BOOL:${Gbs_FOUND}>
    )
    target_link_directories(
        mod3d PRIVATE
        ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
    )
    target_link_libraries(mod3d PRIVATE
        occt-extend 
        pybind11::module
        ${OpenCASCADE_FoundationClasses_LIBRARIES}
        ${OpenCASCADE_ModelingData_LIBRARIES}
        ${OpenCASCADE_ModelingAlgorithms_LIBRARIES}
        ${OpenCASCADE_DataExchange_LIBRARIES}
    )

    target_include_directories(mod3d PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src 
        ${CMAKE_CURRENT_SOURCE_DIR}/inc 
        ${OpenCASCADE_INCLUDE_DIR}
        ${GBS_INCLUDE_DIR}
        ${EIGEN3_INCLUDE_DIR}
    )
endif(MOD3D_BUILD_PYTHON)


option(MOD3D_BUILD_TESTING "Add convenience targets for development" ON)
if (MOD3D_BUILD_TESTING AND BUILD_TESTING)
    find_package(doctest REQUIRED)
    
    add_custom_target(check COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target mod3d)
    
    # Test executable for debugging union_faces
    add_executable(test_union_faces tests/cpp/test_union_faces.cpp)
    target_link_libraries(test_union_faces PRIVATE
        doctest::doctest
        occt-extend
        ${OpenCASCADE_FoundationClasses_LIBRARIES}
        ${OpenCASCADE_ModelingData_LIBRARIES}
        ${OpenCASCADE_ModelingAlgorithms_LIBRARIES}
        ${OpenCASCADE_DataExchange_LIBRARIES}
    )
    target_include_directories(test_union_faces PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${OpenCASCADE_INCLUDE_DIR}
    )
endif()


# add installation target
cmake_policy(SET CMP0177 NEW)
install(TARGETS mod3d
    LIBRARY DESTINATION ${Python3_SITEARCH}/mod3d
    ARCHIVE DESTINATION ${Python3_SITEARCH}/mod3d
    RUNTIME DESTINATION ${Python3_SITEARCH}/mod3d
)

# Install the package shim so the namespace package can resolve from Python.
install(
    FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/python/mod3d/__init__.py
        ${CMAKE_CURRENT_SOURCE_DIR}/python/mod3d/visualyse3js.py
    DESTINATION ${Python3_SITEARCH}/mod3d)

option(MO3D_BUILD_STUBS "Automatically build python hints" ON)
if(MO3D_BUILD_STUBS)

    find_package(Python3 REQUIRED COMPONENTS Interpreter)

    # Où se trouve ton package python "mod3d"
    set(MOD3D_PYTHON_SRC_DIR "${PROJECT_SOURCE_DIR}/python")

    # Où écrire les stubs (dans le build tree)
    set(MOD3D_STUB_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/stubs")

    # Fichier "sentinelle" attendu en sortie (ajuste selon ton module)
    set(MOD3D_STUB_SENTINEL "${MOD3D_STUB_OUT_DIR}/mod3d/__init__.pyi")

    add_custom_command(
        OUTPUT "${MOD3D_STUB_SENTINEL}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${MOD3D_STUB_OUT_DIR}"
        COMMAND "${CMAKE_COMMAND}" -E env
            # Important: permettre à Python de trouver le package + l'extension
            # PYTHONPATH=${MOD3D_PYTHON_SRC_DIR}$<SEMICOLON>$<TARGET_FILE_DIR:mod3d>
            "${Python3_EXECUTABLE}" -m pybind11_stubgen
                mod3d
                # pybind11-stubgen mod3d
                -o "${MOD3D_STUB_OUT_DIR}"
                --ignore-all-errors
        DEPENDS mod3d
        COMMENT "Generating .pyi stubs for mod3d (pybind11-stubgen)"
        VERBATIM
    )

    add_custom_target(mod3d_stubs
        DEPENDS "${MOD3D_STUB_SENTINEL}"
    )
    install(
    # DIRECTORY "${MOD3D_STUB_OUT_DIR}"
    DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/stubs/mod3d"
    DESTINATION "${Python3_SITEARCH}"
    FILES_MATCHING
        PATTERN "*.py"
        PATTERN "*.pyi"
        PATTERN "py.typed"
    )
endif()