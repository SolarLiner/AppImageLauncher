include(${CMAKE_SOURCE_DIR}/cmake/modules/gresources.cmake)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
glib_compile_resources(AILGTK_RESOURCES SOURCE ui-gtk.gresource.xml)

link_directories(${GTKMM_LIBRARY_DIRS})

set(AILGTK_FILES main.cpp app.cpp dialog.cpp)

add_executable(ail-gtk ${AILGTK_RESOURCES} ${AILGTK_FILES})
target_link_libraries(ail-gtk shared PkgConfig::glib ${GTKMM_LIBRARIES} libappimage shared)
target_include_directories(ail-gtk PUBLIC ${GTKMM_INCLUDE_DIRS})
set_target_properties(ail-gtk PROPERTIES INSTALL_PATH ${_rpath})

install(TARGETS ail-gtk
    RUNTIME DESTINATION ${_bindir} COMPONENT APPIMAGELAUNCHER
    LIBRARY DESTINATION ${_libdir} COMPONENT APPIMAGELAUNCHER
)

# TODO: Do removal tool as well
# TODO: Do update tool as well
