add_executable(appimagelauncher-gtk main.cpp)
target_link_libraries(appimagelauncher-gtk shared PkgConfig::glib libappimage shared)
set_target_properties(appimagelauncher-gtk PROPERTIES INSTALL_PATH ${_rpath})

install(TARGETS appimagelauncher-gtk
    RUNTIME DESTINATION ${_bindir} COMPONENT APPIMAGELAUNCHER
    LIBRARY DESTINATION ${_libdir} COMPONENT APPIMAGELAUNCHER
)

# TODO: Do removal tool as well
# TODO: Do update tool as well
