#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

// struct data for g_signal_connect
struct data_array
{
  GtkWindow *window;
  GtkTextBuffer *buffer;
};
typedef struct data_array Data;

// to send error if filename is wrong
GdkPixbuf *create_pixbuf(const gchar * filename) {
    
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   
   if (!pixbuf) {
       
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

// save buffer to text file
void save_text(struct data_array *data)
{
  static GtkWidget *dialog = NULL;
  if(!dialog){
    dialog = gtk_file_chooser_dialog_new("Save file", data->window,
        GTK_FILE_CHOOSER_ACTION_SAVE,
        GTK_STOCK_SAVE,
        GTK_RESPONSE_ACCEPT,
        GTK_STOCK_CANCEL,
        GTK_RESPONSE_REJECT,
        NULL);
  }
  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT){
    gchar *filename;
    gchar *text;
    g_object_get(G_OBJECT(data->buffer), "text", &text, NULL);
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    g_file_set_contents(filename, text, -1, NULL);
    g_free(filename);
    g_free(text);
  }
  gtk_widget_hide(dialog);
}

// choose_file aux function
void open_file(char *filename)
{
}

// create choose file window when load image is clicked
void choose_file(GtkWindow *window)
{
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new ("Open File",
      window,
      GTK_FILE_CHOOSER_ACTION_OPEN,
      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
      NULL);
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    open_file (filename);
    g_free (filename);
  }
  gtk_widget_destroy (dialog);
}
    

int main(int argc, char *argv[])
{
  // var declarations
  GtkWidget *window = NULL;

  GtkWidget *button[4];
  GtkWidget *table = NULL;

  GtkWidget *vbox = NULL; // to align text area with buttons
  GtkWidget *view = NULL;
  GtkWidget *scroll = NULL;

  GdkPixbuf *icon = NULL;
  Data *data;

  gtk_init(&argc, &argv);
  data = g_slice_new(Data);

  // main window setup
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "OCR");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 480);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);

  // table setup
  table = gtk_table_new(4, 1, TRUE);
  gtk_table_set_row_spacings(GTK_TABLE(table), 4);
  gtk_container_set_border_width(GTK_CONTAINER(table), 0);

  // buttons and alignment
  button[0] = gtk_button_new_with_label("Load image");
  gtk_widget_set_tooltip_text(button[0], "Load an image in a standard format");
  gtk_table_attach_defaults(GTK_TABLE(table), button[0], 0, 1, 0, 1);

  button[1] = gtk_button_new_with_label("Save text");
  gtk_widget_set_tooltip_text(button[1], "Save text in a file.");
  gtk_table_attach_defaults(GTK_TABLE(table), button[1], 1, 2, 0, 1);
  
  button[2] = gtk_button_new_with_label("Load weight");
  gtk_widget_set_tooltip_text(button[2], "Load your own weight for the neural  network");
  gtk_table_attach_defaults(GTK_TABLE(table), button[2], 2, 3, 0, 1);

  button[3] = gtk_button_new_with_label("Train");
  gtk_widget_set_tooltip_text(button[3], "Train the neural network");
  gtk_table_attach_defaults(GTK_TABLE(table), button[3], 3, 4, 0, 1);

  vbox = gtk_vbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  // text area
  scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);
  view = gtk_text_view_new();
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll), view);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), 
      GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  
  // buttons actions
  g_signal_connect(button[0], "clicked",  G_CALLBACK(choose_file), GTK_WINDOW(window));
  data->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  data->window = GTK_WINDOW(window);
  g_signal_connect(button[1], "clicked",  G_CALLBACK(save_text), data); 
  
  // icon setup
  icon = create_pixbuf("icon.png");
  gtk_window_set_icon(GTK_WINDOW(window), icon);

  gtk_widget_show_all(window); // show the toplevel window and childs

  g_signal_connect(G_OBJECT(window), "destroy", 
    G_CALLBACK(gtk_main_quit), NULL); // to link X to destroy

  g_object_unref(icon); // to free the icon when the count drops to 0


  gtk_main();
  g_slice_free(Data, data);

  return 0;
}
