#include <gtkmm.h>

class IntegrationDialog : public Gtk::ApplicationWindow {
public:
  IntegrationDialog();
  virtual ~IntegrationDialog();

protected:
  void action_cancel();
  void action_run();
  void action_integrate();

private:
  Glib::RefPtr<Gtk::Builder> m_builder;
}
