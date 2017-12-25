#include "main.h"



void help()
{
    printf("Welcome to Epiread V0.2 \nHere is some commands : \n");
    printf("   --help  To picture commands\n");
    printf("   --XOR  To show the neural network for the XOR function\n");
    printf("   --OCR  To show the neural network for OCR\n");
    printf("   (No args)  Launch GUI\n");
}

int main(int argc, char *argv[])
{
    //Argument gesture
    if(argc>1)
    {
        // for (int i = 1; i < argc; i++)
        // {
        //     SDL_Surface *load = IMG_Load(argv[i]);
        //     SDL_Surface *img = increaseChar(load);
        //     create_matrix_file(img, argv[i]);
        // }
        if(strcmp(argv[1], "--help") == 0)
        {
            help();
        }
        if(strcmp(argv[1], "--XOR") == 0)
        {
            XOR();
        }
        if(strcmp(argv[1], "--OCR") == 0)
        {
            trainNN();
        }
    }
    else
    {
      printf("#-----------------------#\n");
      printf("|Welcome to Epiread V0.2|\n");
      printf("#-----------------------#\n");
      //Create GTK window
      create_window(argc,argv);
    }
    return(EXIT_SUCCESS);
}
