find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})

add_executable(appimagelauncher-gtk main.cpp window.cpp)
target_link_libraries(appimagelauncher-gtk shared PkgConfig::glib ${GTKMM_LIBRARIES} libappimage shared)
target_include_directories(appimagelauncher-gtk PUBLIC ${GTKMM_INCLUDE_DIRS})
set_target_properties(appimagelauncher-gtk PROPERTIES INSTALL_PATH ${_rpath})

install(TARGETS appimagelauncher-gtk
    RUNTIME DESTINATION ${_bindir} COMPONENT APPIMAGELAUNCHER
    LIBRARY DESTINATION ${_libdir} COMPONENT APPIMAGELAUNCHER
)

# TODO: Do removal tool as well
# TODO: Do update tool as well
