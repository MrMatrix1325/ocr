#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void OnDestroy(GtkWidget *pWidget, gpointer pData);
void recup(GtkWidget *button, GtkWidget *sel);
void Destroy(GtkWidget *win);
void file_selection()
{
  GtkWidget *sel = gtk_file_selection_new("File");
  gtk_widget_show(sel);
  gtk_window_set_modal(GTK_WINDOW(sel), TRUE);
  g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(sel)->ok_button), "clicked", G_CALLBACK(recup), sel);
     
  g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(sel)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), sel);
}

void Destroy(GtkWidget *win)
{
  gtk_widget_destroy(win);
}

void recup(GtkWidget *button, GtkWidget *sel)
{
  const gchar* path;
  path = gtk_file_selection_get_filename(GTK_FILE_SELECTION(sel) );
  GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(file_selection),
  GTK_DIALOG_MODAL,
  GTK_MESSAGE_INFO,
  GTK_BUTTONS_OK,
  "Vous avez choisi :\n%s", path);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(sel);
}

int main(int argc , char **argv)
{
  GtkWidget* label1 , *button_open;
  gtk_init(&argc, &argv);
  GtkWidget *pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(OnDestroy),NULL);
  gtk_window_set_title (GTK_WINDOW(pWindow), "Coucou");
  gtk_window_set_position(GTK_WINDOW(pWindow), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(pWindow), 1100, 600);
  label1 = gtk_label_new("Hello World");
  button_open = gtk_button_new_with_mnemonic("Open_img");
  g_signal_connect(G_OBJECT(button_open), "clicked", G_CALLBACK(file_selection),NULL);
  gtk_misc_set_alignment(GTK_MISC(label1), 0.5, 0);
  //gtk_container_add(GTK_CONTAINER(pWindow), label1);
  gtk_container_add(GTK_CONTAINER(pWindow), button_open);
  gtk_widget_show_all(pWindow);
  gtk_main();
  return EXIT_SUCCESS;
}

void OnDestroy(GtkWidget *pWidget, gpointer pData)
{
  gtk_main_quit();
}



