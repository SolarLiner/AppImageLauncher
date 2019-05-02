#include "app.hpp"
#include "dialog.hpp"
#include <gtkmm.h>

void on_hide_window(IntegrationDialog *diag) { delete diag; }

int main(int argc, char **argv) {
    auto app = IntegrationApplication::create();

    return app->run(argc, argv);
}
