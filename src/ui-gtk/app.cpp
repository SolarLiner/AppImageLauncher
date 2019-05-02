#include "app.hpp"

IntegrationApplication::IntegrationApplication()
    : Gtk::Application("org.appimage.AppImageLauncher", Gio::APPLICATION_HANDLES_OPEN), m_state(ISTATE_INTEGRATE) {}

RefPtr<IntegrationApplication> IntegrationApplication::create() {
    return RefPtr<IntegrationApplication>(new IntegrationApplication());
}

RefPtr<IntegrationDialog> IntegrationApplication::create_dialog() {
    auto builder = Builder::create_from_file("dialog.ui");
    IntegrationDialog *dialog = nullptr;
    builder->get_widget_derived("appwindow-dialog", dialog, m_state);
    dialog->signal_hide().connect(
        sigc::bind<Window *>(sigc::mem_fun(*this, &IntegrationApplication::on_hide_window), dialog));

    add_window(*dialog);

    return RefPtr<IntegrationDialog>(dialog);
}

void IntegrationApplication::on_activate() { auto diag = create_dialog(); }

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
            diag = create_dialog().operator->();
        diag->ask_integration(file);
    }

    diag->present();
}

void IntegrationApplication::on_hide_window(Window *win) { delete win; }
