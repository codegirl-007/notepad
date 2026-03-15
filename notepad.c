#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data) {
  (void)user_data;
  GtkWidget *window; 
  GtkWidget *text_view;
  GtkWidget *scroll;
  GtkWidget *box;
  GtkWidget *button_box;
  GtkWidget *save_button;
  GtkWidget *open_button;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "minimal notepad");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600); 

  text_view = gtk_text_view_new();
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

  open_button = gtk_button_new_with_label("Open");
  save_button = gtk_button_new_with_label("Save");
  
  gtk_box_append(GTK_BOX(button_box), open_button);
  gtk_box_append(GTK_BOX(button_box), save_button);

  scroll = gtk_scrolled_window_new();
  gtk_widget_set_vexpand(scroll, TRUE);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), text_view);

  gtk_window_set_child(GTK_WINDOW(window), box);

  gtk_box_append(GTK_BOX(box), button_box); 
  gtk_box_append(GTK_BOX(box), scroll);

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
