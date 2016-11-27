#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>


void OnDestroy ();

void display(int argc, char **argv)
{
  GtkWidget* box1, *window, *button_open, *zone;
  gtk_init(&argc, &argv);
  zone = gtk_fixed_new();
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(OnDestroy),NULL);
  gtk_window_set_title(GTK_WINDOW(window),"Tigereyes");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 1100, 700);
  box1 = gtk_vbox_new(1,0);
  gtk_container_add(GTK_CONTAINER(window), box1);
  button_open = gtk_button_new_with_mnemonic("Open");
  gtk_box_pack_start(GTK_BOX(box1),button_open,1,0,0);
  //gtk_fixed_put(GTK_FIXED(zone), box1, 0, 0);
  gtk_widget_show_all(window);
  gtk_main();

}

void OnDestroy()
{
  gtk_main_quit();
}
 
int main(int argc, char**argv)
{
  display(argc, argv);
  return EXIT_SUCCESS;
}

