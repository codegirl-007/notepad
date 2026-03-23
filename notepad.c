#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>
#include "libvim.h"
#include <stdio.h>

typedef struct {
  GtkWidget *window;
  GtkWidget *text_view;
  GtkSourceBuffer *buffer;
  char *current_path;
  int is_syncing_from_vim;
  int is_syncing_from_gtk;
  buf_T *vim_buffer;
} AppState;

static void save_clicked(GtkButton *button, gpointer user_data);
static gboolean vim_init(AppState *app);
static void vim_shutdown(AppState *app);
static void vim_send_key(AppState *app, const char *key);
static void sync_gtk_from_vim(AppState *app);
static void sync_vim_from_gtk(AppState *app);

static void activate(GtkApplication *app, gpointer user_data) {
  (void)user_data;

  AppState *app_state;

  GtkWidget *window;
  GtkWidget *text_view;
  GtkWidget *scroll;
  GtkWidget *box;
  GtkWidget *button_box;
  GtkWidget *save_button;
  GtkWidget *open_button;
  GtkSettings *settings;
  GtkSourceStyleSchemeManager *scheme_manager;
  GtkSourceStyleScheme *scheme;
  GtkSourceLanguageManager *language_manager;
  GtkSourceLanguage *language;

  app_state = g_malloc0(sizeof(AppState));

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "minimal notepad");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

  settings = gtk_settings_get_default();

  if (settings != NULL) {
    g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);
  }

  text_view = gtk_source_view_new();
  gtk_text_view_set_monospace(GTK_TEXT_VIEW(text_view), TRUE);
  gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(text_view), TRUE);
  app_state->buffer =
      GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)));
  language_manager = gtk_source_language_manager_get_default();
  language = gtk_source_language_manager_get_language(language_manager, "c");
  if (language != NULL) {
    gtk_source_buffer_set_language(app_state->buffer, language);
  }
  gtk_source_buffer_set_highlight_syntax(app_state->buffer, TRUE);
  scheme_manager = gtk_source_style_scheme_manager_get_default();
  scheme = gtk_source_style_scheme_manager_get_scheme(scheme_manager,
                                                      "Adwaita-dark");
  if (scheme != NULL) {
    gtk_source_buffer_set_style_scheme(app_state->buffer, scheme);
  }
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

  app_state->window = window;
  app_state->text_view = text_view;
  g_object_set_data_full(G_OBJECT(window), "app-state", app_state, g_free);

  open_button = gtk_button_new_with_label("Open");
  save_button = gtk_button_new_with_label("Save");

  g_signal_connect(save_button, "clicked", G_CALLBACK(save_clicked), app_state);
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

static void save_clicked(GtkButton *button, gpointer user_data) {
  (void)button;

  AppState *app = user_data;

  GtkTextBuffer *buffer;
  GtkTextIter start;
  GtkTextIter end;
  char *text;
  FILE *file;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->buffer));

  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);

  text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

  file = fopen("note.txt", "w");
  if (file == NULL) {
    g_printerr("Could not open note.txt for writing\n");
    g_free(text);
    return;
  }

  fputs(text, file);
  fclose(file);

  g_print("saved to note.txt\n");

  g_free(text);
}

static gboolean vim_init(AppState *app) {
  (void)app;

  return TRUE;
}

static void vim_shutdown(AppState *app) { (void)app; }

static void vim_send_key(AppState *app, const char *key) {
  (void)app;
  (void)key;
}

static void sync_gtk_from_vim(AppState *app) { (void)app; }

static void sync_vim_from_gtk(AppState *app) { (void)app; }
