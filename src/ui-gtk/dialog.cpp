#include "dialog.hpp"

IntegrationDialog::IntegrationDialog(BaseObjectType *dialog, RefPtr<Builder> &builder, INTEGRATION_STATE state)
    : ApplicationWindow(dialog), m_state(state) {
    on_startup();
    build_ui(builder);
}

void IntegrationDialog::build_ui(RefPtr<Builder> &builder) {
    builder->get_widget("dialog-headerbar", m_header);
    builder->get_widget("img-appicon", m_appicon);
    builder->get_widget("label-appname", m_appname);
    builder->get_widget("label-appversion", m_appversion);
}

void IntegrationDialog::on_startup() {
    std::cout << "IntegrationDialog constructor" << std::endl;
    action_run = add_action("run", sigc::mem_fun(*this, &IntegrationDialog::on_run));
    action_cancel = add_action("cancel", sigc::mem_fun(*this, &IntegrationDialog::on_cancel));
    action_integrate = add_action("integrate", sigc::mem_fun(*this, &IntegrationDialog::on_integrate));
    action_autointegrate = add_action_bool("autointegrate");
    action_autointegrate->signal_change_state().connect(
        sigc::mem_fun(*this, &IntegrationDialog::on_autointegrate_changed));
}

IntegrationDialog *IntegrationDialog::create(INTEGRATION_STATE state) {
    auto builder = Builder::create_from_resource("/com/github/TheAssassin/AppImageLauncher/dialog.ui");
    IntegrationDialog *dialog = nullptr;
    builder->get_widget_derived("appwindow-dialog", dialog, state);

    return dialog;
}

void IntegrationDialog::on_cancel() {
    std::cout << "Action: cancel" << std::endl;
    hide();
}
void IntegrationDialog::on_run() { std::cout << "Action: run" << std::endl; }
void IntegrationDialog::on_integrate() { std::cout << "Action: integrate" << std::endl; }
void IntegrationDialog::on_autointegrate_changed(bool value) {
    std::cout << "Action: Auto-integrate: " << value << std::endl;
}

int IntegrationDialog::ask_integration(RefPtr<File> file, int argc, char** argv) {
    std::string appimage_path = file->get_path();
    std::cout << "Doing an integration on " << appimage_path << std::endl;
    if(getenv("APPIMAGELAUNCHER_DISABLE") != nullptr) {
        return run_appimage(file, argc, argv);
    }

    const auto type = appimage_get_type(appimage_path.c_str(), false);
    if(type<=0 || type > 2) {
        std::ostringstream msg;
        msg << "Not an AppImage: " << appimage_path;
        auto diag = new MessageDialog(msg.str(), false, MessageType::MESSAGE_ERROR);
        diag->run();
        return 1;
    }

    if(type==2) {
        for(int i = 0;i<argc; i++) {
            std::string arg(argv[i]);
            std::string prefix("--appimage-");
            if(arg.find(prefix) == 0) {
                if(arg == prefix+"mount" || arg == prefix + "extract" || arg==prefix + "updateinformation") {
                    return run_appimage(file, argc, argv);
                }
            }
        }
    }
}
