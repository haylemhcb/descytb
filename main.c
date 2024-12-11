/*

   Programa para descargar un video de youtube
   creado por Haylem Candelario Bauza.

   Software Libre GPLv2. Cuba 2024.

   requiere: yt-dlp

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

struct WIN
{
  GtkWidget *w;
  GtkWidget *vbox;
  GtkWidget *label1;
  GtkWidget *input;
  GtkWidget *cmb;
  GtkWidget *btn1;
  GtkWidget *btn2;
  GtkWidget *spn;
} win = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

struct VARS
{
  char fmt[5000];
  char url[5000];
} vars = {"'bv*[height<=360]+ba'", '\0'};

static void start_dsc(GtkWidget *w, gpointer data);
gpointer hilo_desc (gpointer user_data);
static void on_fmt(GtkWidget *w, gpointer data);
void stop(GtkWidget *w, gpointer data);



int main(int argc, char *argv[])
{
  setenv("GDK_DPI_SCALE", "0.8", 1);
  gtk_init(&argc, &argv);

  win.w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  win.vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  win.label1 = gtk_label_new("URL del video de youtube");
  win.input = gtk_entry_new();
  win.cmb = gtk_combo_box_text_new();
  win.btn1 = gtk_button_new_with_label("Descargar");
  win.btn2 = gtk_button_new_with_label("Detener!!!");
  win.spn = gtk_spinner_new();

  gtk_window_set_position(GTK_WINDOW(win.w), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable(GTK_WINDOW(win.w), FALSE);
  gtk_window_set_icon_from_file(GTK_WINDOW(win.w), "ico.png", NULL);
  gtk_window_set_default_size(GTK_WINDOW(win.w), 352, 288);
  gtk_window_set_title(GTK_WINDOW(win.w), "descytb de Haylem Candelario Bauza");
  gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(win.cmb), "baja", "Calidad baja");
  gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(win.cmb), "alta", "Calidad alta");
  gtk_combo_box_set_active (GTK_COMBO_BOX(win.cmb), 0);
  gtk_spinner_stop(GTK_SPINNER(win.spn));

  g_signal_connect(G_OBJECT(win.w), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(win.btn1), "clicked", G_CALLBACK(start_dsc), NULL);
  g_signal_connect(G_OBJECT(win.btn2), "clicked", G_CALLBACK(stop), NULL);
  g_signal_connect(G_OBJECT(win.cmb), "changed", G_CALLBACK(on_fmt), NULL);

  gtk_container_add(GTK_CONTAINER(win.w), win.vbox);
  gtk_container_add(GTK_CONTAINER(win.vbox), win.label1);
  gtk_container_add(GTK_CONTAINER(win.vbox), win.input);
  gtk_container_add(GTK_CONTAINER(win.vbox), win.cmb);
  gtk_container_add(GTK_CONTAINER(win.vbox), win.btn1);
  gtk_container_add(GTK_CONTAINER(win.vbox), win.btn2);
  gtk_container_add(GTK_CONTAINER(win.vbox), win.spn);
  gtk_widget_show_all(win.w);

  gtk_main();
  system("pkill yt-dlp");
  return 0;
}

static void start_dsc(GtkWidget *w, gpointer data)
{
 GThread *hilo = NULL;

 g_thread_new ("hilo1", hilo_desc, NULL);

}

gpointer hilo_desc (gpointer user_data)
{
  char cmd[5000] = {'\0'};
  const gchar *url = NULL;

  url = gtk_entry_get_text(GTK_ENTRY(win.input));
  if(url == NULL) return NULL;

  strcpy(vars.url, url);

  if(vars.fmt[0] == '\0') return NULL;
  if(vars.url[0] == '\0') return NULL;

  if(strcmp(vars.fmt, "Calidad baja") == 0)
    strcpy(vars.fmt, "'bv*[height<=360]+ba'");

  if(strcmp(vars.fmt, "Calidad alta") == 0)
    strcpy(vars.fmt, "'bv*[height>=720]+ba'");

  gtk_spinner_start(GTK_SPINNER(win.spn));

  sprintf(cmd, "yt-dlp -f %s '%s'", vars.fmt, vars.url);
  system(cmd);
  gtk_spinner_stop(GTK_SPINNER(win.spn));

}

static void on_fmt(GtkWidget *w, gpointer data)
{
  gchar *fmt = NULL;

  fmt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(win.cmb));
  if(fmt[0] == '\0') return;

  strcpy(vars.fmt, fmt);
}

void stop(GtkWidget *w, gpointer data)
{
  system("pkill yt-dlp");
}
