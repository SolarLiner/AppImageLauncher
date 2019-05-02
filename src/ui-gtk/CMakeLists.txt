include(${CMAKE_SOURCE_DIR}/cmake/modules/gresources.cmake)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})

add_executable(AILgtk main.cpp app.cpp dialog.cpp)
target_link_libraries(AILgtk shared PkgConfig::glib ${GTKMM_LIBRARIES} libappimage shared)
target_include_directories(AILgtk PUBLIC ${GTKMM_INCLUDE_DIRS})
set_target_properties(AILgtk PROPERTIES INSTALL_PATH ${_rpath})

glib_compile_resources(AILgtk_RESOURCES SOURCES ui-gtk.gresources.xml)

install(TARGETS AILgtk
    RUNTIME DESTINATION ${_bindir} COMPONENT APPIMAGELAUNCHER
    LIBRARY DESTINATION ${_libdir} COMPONENT APPIMAGELAUNCHER
)

# TODO: Do removal tool as well
# TODO: Do update tool as well
