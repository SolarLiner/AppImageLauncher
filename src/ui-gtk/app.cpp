#include "app.hpp"

IntegrationApplication::IntegrationApplication()
    : Gtk::Application("org.appimage.AppImageLauncher", Gio::APPLICATION_HANDLES_OPEN | Gio::APPLICATION_HANDLES_COMMAND_LINE), m_state(ISTATE_INTEGRATE), m_argc(-1) {
    set_application_name("AppImage Integration dialog");
    signal_command_line().connect(sigc::mem_fun(*this, &IntegrationApplication::on_cmd));
}


RefPtr<IntegrationApplication> IntegrationApplication::create() {
    return RefPtr<IntegrationApplication>(new IntegrationApplication());
}

IntegrationDialog *IntegrationApplication::create_dialog() {
    auto dialog = IntegrationDialog::create(m_state);
    add_window(*dialog);

    return dialog;
}

void IntegrationApplication::on_activate() {
    std::cout << "IntegrationApplication activated" << std::endl;
    create_dialog();
}

void IntegrationApplication::on_cmd(RefPtr<ApplicationCommandLine> &cmd) {
    m_argv = cmd->get_arguments(m_argc);
}

void IntegrationApplication::on_open(const Gio::Application::type_vec_files &files, const Glib::ustring &hint) {
    IntegrationDialog *diag = nullptr;
    auto windows = get_windows();
    if (windows.size() > 1) {
        std::cout << "Another application is integrating. Retry when it's done." << std::endl;
        // TODO: Actual message box
    } else if (files.size() > 1) {
        std::cout << "Only open one application at a time." << std::endl;
        // TODO: Actual message box
    } else {
        auto file = files[0];
        if (windows.size() > 0)
            diag = dynamic_cast<IntegrationDialog *>(windows[0]);
        else
            diag = create_dialog();
        diag->ask_integration(file, m_argc, m_argv);
    }

    diag->present();
}

void IntegrationApplication::on_hide_window(Window *win) { delete win; }
