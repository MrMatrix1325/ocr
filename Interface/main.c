# include "image_operation.h"
# include <err.h>
# include <stdlib.h>
# include <gtk/gtk.h>
# include <assert.h>

struct param{
  GtkWidget 	*wid;
  GtkImage		*img;
  gchar 			*path;
};

void recup(struct param *param);

void selec (struct param *par)
{
  struct param *param = malloc(sizeof(struct param *));
  param->wid = gtk_file_selection_new("File Selection");
  gtk_widget_show(param->wid);
  g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(param->wid)->ok_button),"clicked", G_CALLBACK(recup), &param);
}

void recup (struct param *param)
{
  gchar *path = gtk_file_selection_get_filename(GTK_FILE_SELECTION(param->wid));
  gtk_widget_destroy(param->wid); 
}

void OnDestroy(GtkWidget *pWidget, gpointer pData)
{
  gtk_main_quit();
}

int main(int argc, char* argv[])
{
  /*if ( argc < 2 )
    errx(1, "must provide an argument");
  SDL_Surface* img = NULL;*/
  init_sdl();
  struct param *par = malloc(sizeof(struct param *));
  GtkWidget *image;
  GtkWidget *b_open, *b_save, *b_grey ,*b_black, *b_reset;
  GtkWidget *pVbox, *align, *pHbox,*align2;
  //gchar *path;
  gtk_init(&argc, &argv);
  align = gtk_alignment_new(0,0,0,0);
  align2 = align;
  GtkWidget *pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(pWindow),"destroy",G_CALLBACK(OnDestroy),NULL);
  gtk_window_set_title (GTK_WINDOW(pWindow), "Coucou");
  gtk_window_set_position(GTK_WINDOW(pWindow), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(pWindow), 1100, 600);
  pHbox = gtk_hbox_new(0, 2);
  pVbox = gtk_vbox_new(0, 10);
  gtk_container_add(GTK_CONTAINER(pWindow), align);
  gtk_container_add(GTK_CONTAINER(align), pVbox);
  gtk_container_add(GTK_CONTAINER(pVbox), pHbox);
  //image = gtk_image_new_from_file(path);
  b_open = gtk_button_new_with_mnemonic("Open");
  b_save = gtk_button_new_with_mnemonic("Save");
  b_grey = gtk_button_new_with_mnemonic("Grey level");
  b_black = gtk_button_new_with_mnemonic("Black and white");
  b_reset = gtk_button_new_with_mnemonic("Reset");
  gtk_box_pack_start(GTK_BOX(pHbox), b_open, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), b_save, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), b_grey, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), b_black, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), b_reset, 1, 0, 0);
  //gtk_box_pack_start(GTK_BOX(pVbox), image , 1, 0, 0);
  g_signal_connect(G_OBJECT(b_open),"clicked",G_CALLBACK(selec),&par);
  //gtk_image_new_from_file(path);
  //gtk_box_pack_start(GTK_BOX(pVbox), image , 1, 0, 0);
  gtk_widget_show_all(pWindow);
  gtk_main();
  return EXIT_SUCCESS;
    	/*img = load_image(argv[1]);
 	display_image(img);
	SDL_Surface* img2 = SDL_redim(1200 , 700, img);
	display_image(img2);
	SDL_Surface* img3 = grey_lvl(img2);
	display_image(img3);
	SDL_Surface* img4 = black_and_white(img3);
	display_image(img4);
	SDL_Surface* img5 = superposition(img4);
	display_image(img5);

	int *T = malloc(4 * sizeof (int));
   	*(T + 0) = 0;
   	*(T + 1) = 0;
  	*(T + 2) = 150;
 	*(T + 3) = 90;
	SDL_Surface* img6= cut_image(img, T);
	SDL_Surface* img7 = green(img6);
	img7 = green2(img7);
	SDL_SaveBMP(img7,"img7.bmp");
	display_image(img7);
	character_block(img7);
	SDL_FreeSurface(img7);
	SDL_FreeSurface(img);
	return 0;*/
}


