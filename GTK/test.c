#include <gtk/gtk.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "image_operation.h"

void OnDestroy(GtkWidget *pWidget, gpointer pData);
void recup(GtkWidget *button, GtkWidget *sel);

const gchar* path ;

void  selec()
{
  GtkWidget *sel = gtk_file_selection_new("File");
  gtk_widget_show(sel);
  gtk_window_set_modal(GTK_WINDOW(sel), TRUE);
  g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(sel)->ok_button),
                          "clicked", G_CALLBACK(recup), sel);
  g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(sel)->cancel_button), 
                          "clicked", G_CALLBACK(gtk_widget_destroy), sel);
}

void recup(GtkWidget *button, GtkWidget *sel)
{
  path = gtk_file_selection_get_filename(GTK_FILE_SELECTION(sel));
  
}

int main(int argc , char **argv)
{
  GtkWidget *button_open, *button_save, *button_grey ,*button_black, *button_reset;
  GtkWidget *pVbox, *align, *pHbox, *img ,*align2;
  path = "img.bmp";
  gtk_init(&argc, &argv);
  align = gtk_alignment_new(0,0,0,0);
  align2 = align;
  GtkWidget *pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(pWindow),"destroy",G_CALLBACK(OnDestroy),NULL);
  gtk_window_set_title (GTK_WINDOW(pWindow), "Coucou");
  gtk_window_set_position(GTK_WINDOW(pWindow), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(pWindow), 1100, 600);
  pHbox = gtk_hbox_new(1, 0);  
  pVbox = gtk_vbox_new(0, 0);
  gtk_container_add(GTK_CONTAINER(pWindow), align);
  gtk_container_add(GTK_CONTAINER(align), pVbox);
  gtk_container_add(GTK_CONTAINER(pVbox), pHbox);
  img = gtk_image_new_from_file("img.bmp");
  button_open = gtk_button_new_with_mnemonic("Open");
  button_save = gtk_button_new_with_mnemonic("Save");
  button_grey = gtk_button_new_with_mnemonic("Grey level");
  button_black = gtk_button_new_with_mnemonic("Black and white");  
  button_reset = gtk_button_new_with_mnemonic("Reset");
  gtk_box_pack_start(GTK_BOX(pHbox), button_open, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), button_save, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), button_grey, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), button_black, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), button_reset, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pVbox), img , 1, 0, 0);
  g_signal_connect(G_OBJECT(button_open),"clicked",G_CALLBACK(selec),NULL);
  gtk_image_new_from_file(path);
  gtk_box_pack_start(GTK_BOX(pVbox), img , 1, 0, 0);
  gtk_widget_show_all(pWindow);
  gtk_main();
  return EXIT_SUCCESS;
}

void OnDestroy(GtkWidget *pWidget, gpointer pData)
{
  gtk_main_quit();
}



