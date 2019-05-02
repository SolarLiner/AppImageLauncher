#pragma once
#include "dialog.hpp"
#include <gtkmm.h>
#include <iostream>

#ifndef APPLICATION_ID
#define APPLICATION_ID "com.github.TheAssassin.AppImageLauncher"
#endif

using namespace Gio;
using namespace Glib;
using namespace Gtk;

class IntegrationApplication : public Gtk::Application {
public:
    static RefPtr<IntegrationApplication> create();

protected:
    IntegrationApplication();
    void on_activate() override;
    void on_open(const Gio::Application::type_vec_files &files, const Glib::ustring &hint) override;

private:
    IntegrationDialog *create_dialog();
    void on_hide_window(Window *win);

    INTEGRATION_STATE m_state;
};
