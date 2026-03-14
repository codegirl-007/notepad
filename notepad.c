#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data) {
  (void)user_data;
  GtkWidget *window; 
  GtkWidget *text_view;
  GtkWidget *scroll;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "minimal notepad");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600); 

  text_view = gtk_text_view_new();
  
  scroll = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), text_view);

  gtk_window_set_child(GTK_WINDOW(window), scroll);

  gtk_window_present(GTK_WINDOW(window));

}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("com.example.notepad", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
