#include <gtkmm.h>

int main(int argc, char **argv) {
  auto app =
      Gtk::Application::create(argc, argv, "org.appimage.AppImageLauncher");
  Gtk::Window win;
  win.set_default_size(200, 200);
  Gtk::Label label;
  label.set_text("Hello!");
  label.show();
  win.add(label);

  return app->run(win);
}
