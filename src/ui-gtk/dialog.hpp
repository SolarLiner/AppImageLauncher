#pragma once
#include <gtkmm.h>

using namespace Gio;
using namespace Glib;
using namespace Gtk;

enum INTEGRATION_STATE { ISTATE_INTEGRATE, ISTATE_UPDATE, ISTATE_REMOVE, ISTATE__SIZE };

class IntegrationDialog : public ApplicationWindow {
public:
    IntegrationDialog(BaseObjectType *dialog, RefPtr<Builder> &builder, INTEGRATION_STATE state);
    // virtual ~IntegrationDialog();
    static RefPtr<IntegrationDialog> create(INTEGRATION_STATE state);

    void ask_integration(RefPtr<File> file);

protected:
    void on_startup();
    void on_cancel();
    void on_run();
    void on_integrate();
    void on_autointegrate_changed(bool value);

    RefPtr<SimpleAction> action_run;
    RefPtr<SimpleAction> action_cancel;
    RefPtr<SimpleAction> action_integrate;
    RefPtr<SimpleAction> action_autointegrate;

private:
    INTEGRATION_STATE m_state;
};
