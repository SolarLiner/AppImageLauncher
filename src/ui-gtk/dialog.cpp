#include "dialog.hpp"
#include <iostream>

IntegrationDialog::IntegrationDialog(BaseObjectType *dialog, RefPtr<Builder> &builder, INTEGRATION_STATE state)
    : ApplicationWindow(dialog), m_state(state) {
    on_startup();
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

void IntegrationDialog::ask_integration(RefPtr<File> file) {
    std::cout << "Doing an integration on " << file->get_path() << std::endl;
}
