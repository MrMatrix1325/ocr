#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void OnDestroy(GtkWidget *pWidget, gpointer pData);

int main(int argc , char **argv)
{
  GtkWidget* label1;
  gtk_init(&argc, &argv);
  GtkWidget *pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(OnDestroy),NULL);
  gtk_window_set_title (pWindow, "Coucou");
  gtk_window_set_position(pWindow, GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(pWindow, 1100, 600);
  label1 = gtk_label_new("Hello World");
  gtk_container_add(GTK_CONTAINER(pWindow), label1);
  gtk_widget_show_all(pWindow);
  gtk_main();
  return EXIT_SUCCESS;
}

void OnDestroy(GtkWidget *pWidget, gpointer pData)
{
  gtk_main_quit();
}
