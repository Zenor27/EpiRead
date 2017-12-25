# include "segmentation.h"


/* Cut the lines of the image */
SDL_Surface* lineCut(SDL_Surface *img)
{
    /* Variables */
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int isBlank = 1;
    int firstCut = 1;
    SDL_Surface *img_copy = copy_image(img);
    for(int i = 0; i < (img_copy -> h) ; i++)
    {
      isBlank = 1 ;
      for(int j = 0 ; j < (img_copy -> w); j++)
      {
        pixel = getpixel(img, j, i);
        SDL_GetRGB(pixel, img -> format, &r, &g, &b);
        //Check if there is a black character in this line
        if(!r && !g && !b)
        {
          isBlank = 0;
          break;
        }
      }
      //For the first cut we cut the pixel line
      //before the line with a black character
      if(!isBlank && firstCut)
      {
          for(int k = 0; k < (img_copy -> w); k++)
          {
            pixel = SDL_MapRGB(img_copy -> format, 255, 0, 0);
            putpixel(img_copy, k, i - 1, pixel);
          }
          firstCut = 0;
      }
      //For the second cut we cut the first white line
      if(isBlank && !firstCut)
      {
        for(int k = 0; k < (img_copy -> w); k++)
        {
          pixel = SDL_MapRGB(img_copy -> format, 255, 0, 0);
          putpixel(img_copy, k, i, pixel);
        }
        firstCut = 1;
      }
    }
    return(img_copy);
}
/* Isolate the lines */
void isolateLine(SDL_Surface *img, struct Neural_Network *net)
{
  /* Variables */
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int firstCut;
  int lastCut = 0;

  for(int i = 0; i < (img -> h); i++)
  {
    if(i != 0 && i <= lastCut)
    {
      continue;
    }
    pixel = getpixel(img, 0, i);
    SDL_GetRGB(pixel, img -> format, &r, &g, &b);
    //If there is a red line start cut
    if(r == 255 && b == 0 && g == 0)
    {
      firstCut = i + 1;

      for(int j = firstCut + 1; j < (img -> h); j++)
      {
        pixel = getpixel(img, 0, j);
        SDL_GetRGB(pixel, img -> format, &r, &g, &b);

        //Detect the second line
        if(r == 255 && b==0 && g == 0)
        {
          lastCut = j;
          //Isolate into a surface the linecut
          cuttedSurface(img, firstCut, lastCut, net);
          break;
        }
      }
    }
  }
}
/* Display the isolated cuts */
void cuttedSurface(SDL_Surface *img, int firstCut,
                  int lastCut, struct Neural_Network *net)
{
  /* Variables */
  Uint32 pixel;
  SDL_Surface* copy = NULL;

  //Create a surface that contain the cutting zone
  copy = SDL_CreateRGBSurface(SDL_HWSURFACE,
                              img -> w,
                              lastCut - firstCut,
                              img -> format -> BitsPerPixel, 0, 0, 0, 0);
  //Copy the cutting zone into the new surface
  for(int i = 0; i < copy -> w; i++)
  {
    for(int j = 0; j < copy -> h; j++)
    {
      pixel = getpixel(img, i, firstCut + j);
      putpixel(copy, i, j, pixel);
    }
  }
  //Cut the characters
  charCut(copy);
  isolateChar(copy, net);
  net -> str = concat(net -> str, "\n");
}

/* Cut the characters into the isolated cuts */
void charCut(SDL_Surface *img)
{
  // Variables
  Uint32 pixel;
  Uint8 r ;
  Uint8 g;
  Uint8 b;
  int thereIsChar = 0;
  int lineWasWhite = 1;
  int canCut = 0;

  for(int i = 0; i < img -> w; i++)
  {
    lineWasWhite = 1;
    for(int j = 0; j < img -> h; j++)
    {
      pixel = getpixel(img, i, j);
      SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      if(r == 0 && g == 0 && b == 0)
      {
          thereIsChar = 1;
          lineWasWhite = 0;
          break;
      }
    }
    if(lineWasWhite && !canCut)
    {
      continue;
    }
    if(thereIsChar && !canCut)
    {
      for(int k = 0; k < img -> h; k++)
      {
        pixel = SDL_MapRGB(img -> format, 255, 0, 0);
        putpixel(img, i - 1, k, pixel);
      }
      canCut = 1;
    }
    if(lineWasWhite && canCut)
    {
      for(int k = 0; k < img -> h; k++)
      {
        pixel = SDL_MapRGB(img -> format, 255, 0, 0);
        putpixel(img, i, k, pixel);
      }
      canCut = 0;
    }

  }
}
void isolateChar(SDL_Surface *img, struct Neural_Network *net)
{
  //Variables
  Uint32 pixel;
  Uint8 r ;
  Uint8 g;
  Uint8 b;
  int firstCut;
  int lastCut = -1;
  int lastRead = -1;
  SDL_Surface* copy = NULL;
  SDL_Surface *resize = NULL;

  for(int i = 0; i < img -> w; i++)
  {
    if(i < lastRead)
    {
      continue;
    }
    pixel = getpixel(img, i, 0);
    SDL_GetRGB(pixel, img -> format, &r, &g, &b);
    if(r == 255 && g == 0 && b == 0)
    {
      firstCut = i + 1;
      pixel = getpixel(img, firstCut, 0);
      SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      while(r == 255 && g == 0 && b == 0)
      {
        firstCut++;
        pixel = getpixel(img, firstCut, 0);
        SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      }

      //Search for second Cut
      for(int j = firstCut + 1; j < img -> w; j++)
      {
        pixel = getpixel(img, j, 0);
        SDL_GetRGB(pixel, img -> format, &r, &g, &b);
        if(r == 255 && g == 0 && b == 0)
        {
          lastRead = j;
          lastCut = j - 1;

          //Create a surface that contain the cutting zone
          copy = SDL_CreateRGBSurface(SDL_HWSURFACE,
                                      lastCut-firstCut + 1,
                                      img->h,
                                      img->format->BitsPerPixel,0,0,0,0);
          //Copy the cutting zone into the new surface
          for(int w = 0; w < copy -> w; w++)
          {
            for(int h = 0; h < copy -> h; h++)
            {
              pixel = getpixel(img, firstCut + w, h);
              putpixel(copy, w, h, pixel);
            }
          }
          resize = increaseChar(copy);
          //Detect the chars
          double *letter = create_matrix(resize);
          //Prevent false space
          int space = 1;
          //printf("%d\n",copy->w);
          if(copy->w <5)
          {
            for(int i = 0;i < 28*28; i++)
            {
              if((int)letter[i] == 1)
              {
                space = 0;
                break;
              }
            }
            if(space == 1)
              break;
          }

          space = 1;
          for(int i = 0;i < 28*28; i++)
          {
            if((int)letter[i] == 1)
            {
              space = 0;
              break;
            }
          }
          if(space)
          {

            net -> str = concat(net->str," ");
          }
          else
          {
            char res = DetectText(net, letter);
            char str[2] = "\0";
            str[0] = res;
            net -> str = concat(net -> str, str);
          }
          break;
        }
      }
    }
  }
}


int mediumPixelSpacingHorizontal(SDL_Surface *img)
{
  //Variables
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int whitePixel = 0;
  int blackPixel = 0;
  int res = 0;

  for(int i = 0; i < img -> h; i++)
  {
    for(int j = 0; j < img -> w; j++)
    {
      pixel = getpixel(img, j, i);
      SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      if(r == 255 && g == 255 && b == 255)
        whitePixel++;
      else
        blackPixel++;
    }

  }
  res = whitePixel / (blackPixel / 2);
  return(res);
}
int mediumPixelSpacingVertical(SDL_Surface *img)
{
  //Variables
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int whitePixel = 0;
  int blackPixel = 0;
  int res = 0;

  for(int i = 0; i < img -> w; i++)
  {
    for(int j = 0; j < img -> h; j++)
    {
      pixel = getpixel(img, i, j);
      SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      if(r == 255 && g == 255 && b == 255)
        whitePixel++;
      else
        blackPixel++;
    }
  }
  res = whitePixel / (blackPixel / 2);
  return(res);
}

/* RLSA Algorithm to detect blocks*/
void blockDetection_horizontal(SDL_Surface *img)
{
  /* Some Variables */
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int C = mediumPixelSpacingHorizontal(img)*4;
  int countWhite;

  for(int i = 0; i < img -> h; i++)
  {
    countWhite = 0;
    for(int j = 0; j < img -> w; j++)
    {
      pixel = getpixel(img, j, i);
      SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      if(r == 255 && g == 255 && b == 255)
      {
        countWhite++;
      }
      if(r == 0 && g == 0 && b == 0)
      {
        if(countWhite <= C)
        {
            int k = j - 1;
            while(countWhite > 0)
            {
              pixel = SDL_MapRGB(img -> format, 0, 0, 0);
              putpixel(img, k, i, pixel);
              countWhite--;
              k--;
            }
        }
        else
        {
          countWhite = 0;
        }
      }
    }
  }
}
void blockDetection_vertical(SDL_Surface *img)
{
  /* Some Variables */
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int C = mediumPixelSpacingVertical(img) * 4;
  int countBlack;

  for(int i = 0; i < img -> w; i++)
  {
    countBlack = 0;
    for(int j = 0; j < img -> h; j++)
    {
      pixel = getpixel(img, i, j);
      SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      if(r == 255 && g == 255 && b == 255)
      {
        countBlack++;
      }
      else
      {
        if(countBlack <= C)
        {
            int k = j - 1;
            while(countBlack > 0)
            {
              pixel = SDL_MapRGB(img -> format, 0, 0, 0);
              putpixel(img, i, k, pixel);
              countBlack--;
              k--;
            }
        }
        else
        {
          countBlack = 0;
        }
      }
    }
  }
}
