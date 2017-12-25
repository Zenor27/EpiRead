# include "image_operations.h"

/* Transform the image in a greyscale image */
void greyscale(SDL_Surface *img)
{
   /* Variables */
   Uint32 pixel;
   Uint8 r;
   Uint8 g;
   Uint8 b;
   float coef_r;
   float coef_g;
   float coef_b;
   int w;
   int h;
   w = img -> w;
   h = img -> h;

  /* Iterate on each pixels and greyscale */
  for(int i = 0; i < w; i++)
  {
    for(int j = 0; j < h; j++)
    {
      /* Apply luminance on r g b */
      pixel = getpixel(img, i, j);
      SDL_GetRGB(pixel, img->format, &r, &g, &b);
      coef_r = r * 0.3;
      coef_g = g * 0.59;
      coef_b = b * 0.11;
      r = coef_r + coef_g + coef_b;
      g = coef_r + coef_g + coef_b;
      b = coef_r + coef_g + coef_b;
      pixel = SDL_MapRGB(img->format, r, g, b);
      putpixel(img,i,j,pixel);
    }
  }
}

/*
Useless for the moment, only for real pictures*/
int mediumPixel(SDL_Surface *img)
{
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int mediumPixel = 0;

  int w;
  int h;
  w = img -> w;
  h = img -> h;
  int totalPixel = w * h;
 for(int i = 0; i < w; i++)
 {
   for(int j = 0; j < h; j++)
   {
     pixel = getpixel(img, i, j);
     SDL_GetRGB(pixel, img->format, &r, &g, &b);
     mediumPixel += r;
   }
 }
 mediumPixel = mediumPixel / totalPixel;
 return(mediumPixel);
}

/* Transform image in Binary Image */
void blacknwhite(SDL_Surface *img)
{
  /* Variables */
  Uint32 pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  int w;
  int h;
  w = img -> w;
  h = img -> h;

  //int med = mediumPixel(img);
 /* Iterate on each pixels and blacknwhite */
 for(int i = 0; i < w; i++)
 {
   for(int j = 0; j < h; j++)
   {
     pixel = getpixel(img,i,j);
     SDL_GetRGB(pixel, img->format, &r, &g, &b);
     if(r >= 127 && g >= 127 && b >= 127)
     {
      r = 255;
      g = 255;
      b = 255;
     }
     else
     {
      r = 0;
      g = 0;
      b = 0;
     }
     pixel = SDL_MapRGB(img->format, r, g, b);
     putpixel(img, i, j, pixel);
   }
 }
}
/* Noise reduction X4 for the moment...*/
void noiseReduction(SDL_Surface *img)
{
  /* Variables */
  int w;
  int h;
  w = img -> w;
  h = img -> h;
  int pixelTable[4];

 for(int i = 0; i < w; i++)
 {
   for(int j = 0; j < h; j++)
   {
     for(int k = j; k <= j + 4; k++)
     {
       //Borders of picture
       if(i == 0)
       {
         if(k == 0)
         {
            pixelTable[0] = getpixel(img, i, k);
            pixelTable[1] = getpixel(img, i, k);
            pixelTable[2] = getpixel(img, i, k);
            pixelTable[3] = getpixel(img, i, k + 1);
            pixelTable[4] = getpixel(img, i + 1, k);
            break;
         }
         if(k == h)
         {
            pixelTable[0] = getpixel(img, i, k);
            pixelTable[1] = getpixel(img, i, k - 1);
            pixelTable[2] = getpixel(img, i, k);
            pixelTable[3] = getpixel(img, i, k);
            pixelTable[4] = getpixel(img, i + 1, k);
            break;
         }
         else
         {
          pixelTable[0] = getpixel(img, i, k);
          pixelTable[1] = getpixel(img, i, k - 1);
          pixelTable[2] = getpixel(img, i, k);
          pixelTable[3] = getpixel(img, i, k + 1);
          pixelTable[4] = getpixel(img, i + 1, k);
          break;
         }
       }
       if(i == w)
       {
          if(k == 0)
          {
            pixelTable[0] = getpixel(img, i, k);
            pixelTable[1] = getpixel(img, i, k);
            pixelTable[2] = getpixel(img, i - 1, k);
            pixelTable[3] = getpixel(img, i, k + 1);
            pixelTable[4] = getpixel(img, i, k);
            break;
          }
          if(k == h)
          {
            pixelTable[0] = getpixel(img, i, k);
            pixelTable[1] = getpixel(img, i, k - 1);
            pixelTable[2] = getpixel(img, i - 1, k);
            pixelTable[3] = getpixel(img, i, k);
            pixelTable[4] = getpixel(img, i, k);
            break;
          }
          else
          {
            pixelTable[0] = getpixel(img, i, k);
            pixelTable[1] = getpixel(img, i, k - 1);
            pixelTable[2] = getpixel(img, i - 1, k);
            pixelTable[3] = getpixel(img, i, k + 1);
            pixelTable[4] = getpixel(img, i, k);
            break;
          }
       }
       if(k == 0)
       {
          pixelTable[0] = getpixel(img, i, k);
          pixelTable[1] = getpixel(img, i, k);
          pixelTable[2] = getpixel(img, i - 1, k);
          pixelTable[3] = getpixel(img, i, k + 1);
          pixelTable[4] = getpixel(img, i + 1, k);
          break;
       }
       if(k == h)
       {
        pixelTable[0] = getpixel(img, i, k);
        pixelTable[1] = getpixel(img, i, k - 1);
        pixelTable[2] = getpixel(img, i - 1, k);
        pixelTable[3] = getpixel(img, i, k);
        pixelTable[4] = getpixel(img, i + 1, k);
        break;
       }
       else
       {
        pixelTable[0] = getpixel(img, i, k);
        pixelTable[1] = getpixel(img, i, k - 1);
        pixelTable[2] = getpixel(img, i - 1, k);
        pixelTable[3] = getpixel(img, i, k + 1);
        pixelTable[4] = getpixel(img, i + 1, k);
        break;
       }
     }
      array_select_sort(pixelTable, 5);
      int med = pixelTable[2];
      putpixel(img, i, j, med);
   }
 }
}

SDL_Surface* increaseChar(SDL_Surface *img)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                        28,
                        28,
                        img->format->BitsPerPixel,0,0,0,0);
  SDL_SoftStretch(img, NULL, dest, NULL);
  return dest;
}
SDL_Surface* Resize(SDL_Surface *img)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                        576,
                        460,
                        img->format->BitsPerPixel,0,0,0,0);
  SDL_SoftStretch(img, NULL, dest, NULL);
  return dest;
}
