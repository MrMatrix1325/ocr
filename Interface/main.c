# include "image_operation.h"
# include <err.h>
# include <stdlib.h>
# include <gtk/gtk.h>
# include <assert.h>

struct param{
	GtkWidget *img;
  char * path;
  int grey;
  int black;
};


void choose(GtkWidget *button ,struct param *para)
{
  GtkWidget *win;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

  win = gtk_file_chooser_dialog_new ("Open File",NULL,action,("_Cancel"),GTK_RESPONSE_CANCEL,("_Open"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

  int res = gtk_dialog_run (GTK_DIALOG (win));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    SDL_Surface *img ;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (win);
    char *path = gtk_file_chooser_get_filename (chooser);
    para->path = path;
    img = load_image(path);
    img = SDL_redim(1100,600,img);
    SDL_SaveBMP(img,"redim");
    gtk_image_set_from_file (GTK_IMAGE(para->img) , "redim");
    free(img);
    remove("redim");
  }

  gtk_widget_destroy (win);
}

void save_img(GtkWidget *button, struct param *para)
{
  if (para->path == NULL)
  {
    return ;
  }
  else
  {
  GtkWidget *dialog;
  GtkFileChooser *chooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;

  dialog = gtk_file_chooser_dialog_new ("Save File",
                                      NULL,
                                      action,
                                      ("_Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("_Save"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);
  chooser = GTK_FILE_CHOOSER (dialog);

  gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
  gtk_dialog_run (GTK_DIALOG (dialog));
  if (GTK_RESPONSE_ACCEPT)
  {
    SDL_Surface *img = load_image(para->path); 
    char *path = gtk_file_chooser_get_filename (chooser);
    if (para->black)
    {
      img = grey_lvl(img);
			img = black_and_white(img);
    }
    else if(para->grey)
    {
      img = grey_lvl(img);
    }
    SDL_SaveBMP(img,path);
   
  }

  gtk_widget_destroy (dialog);
  }

}

void reset_img(GtkWidget *button, struct param *para)
{
  if (para->path == NULL)
  {
    return;
  }
  SDL_Surface *img;
  img = load_image(para->path);
  img = SDL_redim(1100,600,img);
  SDL_SaveBMP(img,"$-redimreset");
  gtk_image_set_from_file(GTK_IMAGE(para->img), "$-redimreset");
  para->grey = 0;
  para->black = 0;
  free(img);
  remove("$-redimreset");

}

void grey (GtkWidget *button , struct param *para)
{
  if (para->path == NULL)
  {
    return ;
  }
  SDL_Surface *img;
  img = load_image(para->path);
  img = grey_lvl(img);
  img = SDL_redim(1100,600,img);
  SDL_SaveBMP(img, "$-greyimg");
  gtk_image_set_from_file(GTK_IMAGE(para->img), "$-greyimg");
  para->grey = 1;
  remove("$-greyimg");
  free(img);
}

void black (GtkWidget *button, struct param *para)
{
  if (para->path == NULL) 
  {
    return ;
  }
  SDL_Surface *img;
  img = load_image(para->path);
  img = grey_lvl(img);
  img = black_and_white(img);
  img = SDL_redim(1100,600,img);
  SDL_SaveBMP(img, "$-blackimg");
  para->black = 1;
  gtk_image_set_from_file(GTK_IMAGE(para->img), "$-blackimg");
  remove("$-blackimg");
  free(img);

} 

void ocr(GtkWidget *button, struct param *para)
{
  if (para->path == NULL)
  {
    return ;
  }
  SDL_Surface *img;
  img = load_image(para->path);
  img = SDL_redim(1100,600,img);
  img = grey_lvl(img);
  img = black_and_white(img);
  img = superposition(img);
  // Segmentation blocs de caracteres

  // RDN

  // FENETRE + SAVE
}

void OnDestroy()
{
  gtk_main_quit();
}

int main(int argc, char* argv[])
{
  init_sdl();
  GtkWidget *image;
  GtkWidget *b_open, *b_save, *b_grey ,*b_black, *b_reset ,*b_ocr;
  GtkWidget *b_quit;
  GtkWidget *pVbox, *pHbox;
  struct param *param = malloc(sizeof(struct param*));

  gtk_init(&argc, &argv);  

  image = gtk_image_new_from_file(NULL);
  GtkWidget *pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(pWindow),"destroy",G_CALLBACK(OnDestroy),NULL);
  
  gtk_window_set_title (GTK_WINDOW(pWindow), "Tigereyes");
  gtk_window_set_position(GTK_WINDOW(pWindow), GTK_WIN_POS_CENTER);
  
  pHbox = gtk_hbox_new(0, 2);
  pVbox = gtk_vbox_new(0, 10);
 
  gtk_container_add(GTK_CONTAINER(pWindow), pVbox);
  gtk_container_add(GTK_CONTAINER(pVbox), pHbox);

  b_open = gtk_button_new_with_mnemonic("Open");
  b_save = gtk_button_new_with_mnemonic("Save");
  b_grey = gtk_button_new_with_mnemonic("Grey level");
  b_black = gtk_button_new_with_mnemonic("Black and white");
  b_reset = gtk_button_new_with_mnemonic("Reset");
  b_ocr = gtk_button_new_with_mnemonic("Start OCR");
  b_quit = gtk_button_new_with_mnemonic("Quit");
  
  param->img = image;
  param->path = NULL;
  param->grey = 0;
  param->black = 0;

  gtk_box_pack_start(GTK_BOX(pHbox), b_open, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), b_save, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), b_grey, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), b_black, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), b_reset, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), b_ocr, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pHbox), b_quit,1, 0, 0);
  gtk_box_pack_start(GTK_BOX(pVbox), image , 1, 0, 0);

  g_signal_connect(G_OBJECT(b_open),"clicked",G_CALLBACK(choose),param);
  g_signal_connect(G_OBJECT(b_grey),"clicked",G_CALLBACK(grey),param);
  g_signal_connect(G_OBJECT(b_black),"clicked",G_CALLBACK(black),param);
  g_signal_connect(G_OBJECT(b_reset),"clicked",G_CALLBACK(reset_img),param);
  g_signal_connect(G_OBJECT(b_save),"clicked",G_CALLBACK(save_img),param);
  g_signal_connect(G_OBJECT(b_ocr),"clicked",G_CALLBACK(ocr),param);
  g_signal_connect(G_OBJECT(b_quit),"clicked",G_CALLBACK(OnDestroy),NULL);
  gtk_widget_show_all(pWindow);
  gtk_main();
  return EXIT_SUCCESS;
}


