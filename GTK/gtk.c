# include "gtk.h"


gchar *filename = "";
char *text = "";
GtkWidget *parent;
void save_text(GtkButton *button, GtkTextBuffer *buffer)
{
  UNUSED(button);
  UNUSED(buffer);
  GtkWidget *dialog;
  GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET(button));
  dialog = gtk_file_chooser_dialog_new ("Save Text ",
                    GTK_WINDOW (toplevel),
                    GTK_FILE_CHOOSER_ACTION_SAVE,
                    "Cancel", GTK_RESPONSE_CANCEL,
                    "Save", GTK_RESPONSE_ACCEPT,
                    NULL);
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    /* set the contents of the file to the text from the buffer */
    if (filename != NULL)
      g_file_set_contents (filename, text, strlen(text), NULL);
    else
      g_file_set_contents (filename, text, strlen(text), NULL);
  }
  gtk_widget_destroy (dialog);

}
void load_image(GtkButton *button, GtkImage *image)
{
  if(strcmp(filename,"") == 0)
    return;
  UNUSED(button);
	SDL_Surface *img = IMG_Load((char *)filename);
	if(img->w > 576 && img->h > 460)
	{

		printf("Need Resize \n");
		//SDL_Surface *new = imageResize(img,576,460);
		SDL_Surface *new = Resize(img);

		SDL_SaveBMP(new,"image_resize");
    //SDL_FreeSurface(new);
		gtk_image_set_from_file (GTK_IMAGE (image), "image_resize");
	}
	else
    	gtk_image_set_from_file (GTK_IMAGE (image), filename);
}

//Colors for print
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

int trainNN()
{
	//Variables
	int nbEpoch = 5000;
	int nbLetters = 26 * 1 + 26 * 1; //5 fonts for uppers & 4 for lowers
	int currentChar = 0;
	//int count = 0;
	srand(time(NULL));

	//Intialize network
	struct Neural_Network *net = InitializeNetwork();

	//Initialize all goals & letters
	double **goal = goalMatrix();
	double **letters = lettersMatrix();

	for (int epoch = 0; epoch < nbEpoch; epoch++)
	{
			currentChar = 0;
			for (int l = 0; l < nbLetters; l++)
			{

					Neural_Network_OCR(net, letters[l], goal[currentChar]);
          currentChar++;

					if (epoch % 100 == 0)
					{
							PrintState(net);
					}
					//count++;
			}
			//== PRINT ERROR EVERY 100 EPOCHs ==//
			if (epoch % 100 == 0)
			{
					if(net -> MaxErrorRate > 0.005)
							printf("Epoch %-5d | MaxErrorRate = %s %f \n",
                    epoch,KRED,net->MaxErrorRate);
					else
					{
							printf("Epoch %-5d | MaxErrorRate = %s %f \n",
                      epoch,KGRN,net->MaxErrorRate);
							// canSave = 1;
					}
					printf("%s",KWHT);
			}
      if(net->MaxErrorRate<0.0005 && net->MaxErrorRate != 0.0)
      {
         break;
      }
			net -> MaxErrorRate = 0.0;
	}
  printf("Save data...\n");
	SaveData(net);
  printf("Learn finish\n");
	return EXIT_SUCCESS;
}
void openFile(GtkButton *button, GtkLabel *text_label)
{
  GtkWidget *label = (GtkWidget *) text_label;
	GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET(button));
	//GtkFileFilter *filter = gtk_file_filter_new ();
	GtkWidget *dialog = gtk_file_chooser_dialog_new (("Open image"),
	                                                GTK_WINDOW (toplevel),
	                                                GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                "Open", GTK_RESPONSE_ACCEPT,
	                                                "Cancel", GTK_RESPONSE_CANCEL,
	                                                NULL);

	//gtk_file_filter_add_pixbuf_formats (filter);
	//gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),filter);

	switch (gtk_dialog_run (GTK_DIALOG (dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
		{
			filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      gtk_label_set_text(GTK_LABEL(label),filename);
			break;
		}
		default:
			break;
	}
	gtk_widget_destroy(dialog);
}

int launchOCR(GtkButton *button, GtkTextBuffer *buffer)
{
  if(strcmp(filename,"") == 0)
  {
    return 1;
  }
	UNUSED(button);
	SDL_Init(SDL_INIT_VIDEO);
	printf("%s \n ",filename);
	SDL_Surface *img = IMG_Load((char *)filename);
	//display_image(img);

	greyscale(img);
	printf("Greyscale \n");
	//display_image(img);

	blacknwhite(img);
	printf("Black and white \n");
	//display_image(img);

	// SDL_Surface *copy = copy_image(img);
	// blockDetection_horizontal(copy);
	// printf("Block Detection Horizontal\n");
	// display_image(copy);
	// blockDetection_vertical(copy);
	// printf("Block Detection Vertical\n");
	// display_image(copy);
	// SDL_FreeSurface(copy);

	SDL_Surface *image_cut = lineCut(img);
	printf("Line Cuts\n");
	//display_image(image_cut);
	printf("Character cuts\n");
  struct Neural_Network *net = ExtractData();
  printf("Extract Data Done \n");
	isolateLine(image_cut,net);
  printf("Isolate Line Done \n");
  gtk_text_buffer_set_text (buffer,net->str,strlen(net->str));
  text = net->str;
	printf("Finish Treatment\n");

	SDL_Quit();
	return EXIT_SUCCESS;
}

void create_window(int argc, char *argv[])
{
	//Init variables
  GtkWidget *main_window;
	SGlobalData data;
	//Init GTK
	gtk_init(&argc, &argv);
	//Build from .glade
  data.builder = gtk_builder_new();
  gtk_builder_add_from_file(data.builder, "main.glade", NULL);
	//Get main_window
	main_window =  GTK_WIDGET(gtk_builder_get_object(data.builder,"main_window"));
  parent = main_window;
	//Connect signals
	gtk_builder_connect_signals(data.builder, &data);

  gtk_window_set_title(GTK_WINDOW(main_window), "EpiRead");
  gtk_widget_show_all(main_window);
  gtk_main();
}
