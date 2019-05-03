#pragma once
#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <iomanip>
extern "C" {
    #include <stdlib.h>
    #include <unistd.h>
    #include <appimage/appimage.h>
}

using namespace Gio;
using namespace Glib;
using namespace Gtk;

bool make_executable(RefPtr<File> file) {
    std::ostringstream cmd;
    auto workingdir = getenv("HOME");
    cmd << "chmod +x '" << file->get_path() << "'";
    return system(cmd.str()) == 0;
}

int run_appimage(RefPtr<File> file, unsigned long argc, char **argv) {
    auto path_to_appimage = file->get_path();
    auto type = appimage_get_type(path_to_appimage.c_str(), false);
    if (type < 1 || type > 3) {
        std::ostringstream message;
        message << "AppImageLauncher does not support type " << type << "AppImages at the moment.";
        auto dialog = new MessageDialog(message.str().c_str(), false, MessageType::MESSAGE_ERROR);
        dialog->run();
        return 1;
    }
    if (!make_executable(file)) {
        auto dialog = new MessageDialog("Could not make AppImage executable.", false, MessageType::MESSAGE_ERROR);
        dialog->run();
        return 1;
    }

    int rv = system("systemctl --user enable appimagelauncherfs 2>&1 1>/dev/null");
    rv += system("systemctl --user enable appimagelauncherfs 2>&1 1>/dev/null");

    if (rv != 0) {
        auto dialog = new MessageDialog("Error integrating AppImage: could not talk with the system.");
        dialog->run();
        return 1;
    }

    setenv("DESKTOPINTEGRATION", "AppImageLauncher", true);

    std::ostringstream path_to_fs_endpoint, path_to_virtual_appimage;
    path_to_fs_endpoint << "/run/user/" << getuid() << "/appimagelauncherfs/";
    bool registration_worked(false), path_resolving_worked(false), opening_map_file_worked(false);

    for (useconds_t i = 1; i < 10; i++) {
        usleep(i * 10e3);

        if (!registration_worked) {
            std::ofstream stream(path_to_fs_endpoint.str(), std::ios_base::app);
            if(!stream) continue;
            stream << path_to_appimage << std::endl;
            stream.close();
            registration_worked = true;
        }
        std::ostringstream map_file_path;
        map_file_path << path_to_fs_endpoint.str() << "/map";
        std::ifstream map_file(map_file_path.str());

        if(!map_file) {
            opening_map_file_worked = false;
            continue;
        }
        opening_map_file_worked = true;

        std::string current_line;
        while(std::getline(map_file, current_line)) {
            if(current_line.find(path_to_appimage) != std::string::npos) {
                auto virtual_file_name = current_line.substr(0, current_line.find(" -> "));
                path_to_virtual_appimage << path_to_fs_endpoint.str() << "/" << virtual_file_name;
                break;
            }
        }

        map_file.close();

        if(!path_to_virtual_appimage.str().empty()) {
            path_resolving_worked = true;
            break;
        }
    }

    if(path_to_virtual_appimage.str().empty()) {
        std::ostringstream message;
        message << "Failed to register AppImage to the system: ";

        if(!registration_worked)
            message << "failed to register the path '" << path_to_appimage << "'";
        else if(!opening_map_file_worked)
            message << "Could not open the map file";
        else message << "Unknown failure";
        auto dialog = new MessageDialog(message.str(), false, MessageType::MESSAGE_ERROR);
        dialog->run();
        return 1;
    }

    std::vector<char> argv0buf(path_to_appimage.size()+1, '\0');
    strcpy(argv0buf.data(), path_to_appimage.c_str());

    std::vector<char*> args;
    args.push_back(argv0buf.data());

    for(int i = 1; i<argc; i++) {
        args.push_back(argv[i]);
    }
    args.push_back(nullptr);
    execv(path_to_virtual_appimage.str().c_str(), args.data());

    const auto& error = errno;
    std::cerr << "execv() failed: " << strerror(error) << std::endl;
    return 1;
}
