# include "tools.h"

/* Classic tools */
static inline Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y)
{
 int bpp = surf -> format -> BytesPerPixel;
 return (Uint8*)surf -> pixels + y * surf -> pitch + x * bpp;
}

Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y)
{
 Uint8 *p = pixelref(surface, x, y);
 switch(surface -> format -> BytesPerPixel) {
 case 1:
   return *p;
 case 2:
   return *(Uint16 *)p;
 case 3:
   if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
     return p[0] << 16 | p[1] << 8 | p[2];
   else
     return p[0] | p[1] << 8 | p[2] << 16;
 case 4:
   return *(Uint32 *)p;
 }
 return 0;
}

void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
 Uint8 *p = pixelref(surface, x, y);
 switch(surface -> format -> BytesPerPixel) {
 case 1:
   *p = pixel;
   break;
 case 2:
   *(Uint16 *)p = pixel;
   break;
 case 3:
   if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
     p[0] = (pixel >> 16) & 0xff;
     p[1] = (pixel >> 8) & 0xff;
     p[2] = pixel & 0xff;
   } else {
     p[0] = pixel & 0xff;
     p[1] = (pixel >> 8) & 0xff;
     p[2] = (pixel >> 16) & 0xff;
   }
   break;
 case 4:
   *(Uint32 *)p = pixel;
   break;
 }
}
void wait_for_keypressed(void)
{
    SDL_Event             event;
    for (;;) {
      SDL_PollEvent(&event);
      switch (event.type)
      {
        case SDL_KEYDOWN: return;
        default: break;
      }
    }
}
SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface          *screen;
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL) {
      errx(1, "Couldn't set %dx%d video mode: %s\n",
           img -> w, img -> h, SDL_GetError());
    }
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
      warnx("BlitSurface error: %s\n", SDL_GetError());
    SDL_UpdateRect(screen, 0, 0, img -> w, img -> h);
    wait_for_keypressed();
    return screen;
}

/* Copy a SDL_Surface */
SDL_Surface* copy_image(SDL_Surface *img)
{
  Uint32 pixel;
  SDL_Surface* copy;
  copy = SDL_CreateRGBSurface(SDL_HWSURFACE,
                              img -> w,
                              img -> h,
                              img -> format -> BitsPerPixel,0,0,0,0);
  for(int i = 0; i < img -> w; i++)
  {
    for(int j = 0;j < img -> h; j++)
    {
      pixel = getpixel(img, i, j);
      putpixel(copy, i, j, pixel);
    }
  }
  return(copy);
}

/* Array tools for mediumPixel */
void array_swap(int array[], size_t i, size_t j)
{
  int element1 = array[i];
  int element2 = array[j];
  array[i] = element2;
  array[j] = element1;
}
void array_select_sort(int array[], size_t len)
{
  size_t i = 0;
  size_t j;
  int min_index;
  while(i<len)
  {
    /* Find Min Index */
    j= i;
    min_index = j;
    while(j<len)
    {
        if(array[j]<array[min_index])
        {
            min_index = j;
        }
        j+=1;
    }
    array_swap(array,i,min_index);
    i++;
  }
}

//Create the .txt file with the values of pixels
void create_matrix_file(SDL_Surface *img, char *filename)
{
    //Variables
    Uint8 r;
    Uint8 g;
    Uint8 b;

    strtok(filename,".");
    strcat(filename,".txt");
    FILE *file = fopen(filename,"w");
    printf("%s \n",filename);

    for(int i = 0; i < img -> h; i++)
    {
        for(int j = 0;j < img -> w; j++)
        {
            Uint32 pixel = getpixel(img, j, i);
            SDL_GetRGB(pixel, img -> format, &r, &g, &b);
            if(r == 0 && g == 0 && b == 0)
                fputs("1",file);
            else
                fputs("0",file);
        }
        fputs("\n",file);
    }
    fclose(file);
}
void print_matrix(double mat[], size_t lines, size_t cols)
{
    for(size_t i = 0; i < lines; i++)
    {
        for(size_t j = 0; j < cols; j++)
            printf("%d ", (int)mat[j + i * cols]);
        printf("\n");
    }
}

//Create & return the double* pixels values from filename
double *matrixFromFile(char *filename)
{
  double *matrix = malloc(sizeof(double) * 28 * 28);
  FILE *file = fopen(filename,"r");

  if(file == NULL)
    printf("File is NULL \n");
  for(int i = 0; i <= 28; i++)
  {
    for(int j = 0; j <= 28; j++)
    {
      int c = fgetc(file);
      if(c == 49)
        matrix[j+i*28] = 1;
      if(c == 48)
        matrix[j+i*28] = 0;
    }
  }
  fclose(file);
  return matrix;
}

//Create & return the double* goals pixels values from filename
double *goalArray(char letter)
{
  double *goal = calloc(52, sizeof(double));

  if(letter >= 'A' && letter <= 'Z')
    goal[(int)(letter) - 65] = 1;

  else if(letter >= 'a' && letter <= 'z')
    goal[((int)(letter) - 97) + 26] = 1;

  return goal;
}

//Create & return all the goals matrixes (for all letters)
double **goalMatrix()
{
  double **goalMatrix = malloc(sizeof(double*) * 52);
  char maj = 'A';
  char min = 'a';
  for(int i = 0; i < 52; i++)
  {
    if(i < 26)
    {
      goalMatrix[i] = goalArray(maj);
      maj++;
    }
    else
    {
      goalMatrix[i] = goalArray(min);
      min++;
    }
  }
  return goalMatrix;
}

//Create & return all the letters matrixes (for all letters)
double **lettersMatrix()
{
  //Variables
  char majs_path[14] = "majs/0/00.txt\0";
  char mins_path[14] = "mins/0/00.txt\0";
  double **lettersMatrix = malloc(sizeof(double *) * 52);
  char maj = 'A';
  char min = 'a';
  char count = '4';

  for(int i = 0; i < 52; i++)
  {

    if(i < 26 * 1)
    {
      majs_path[5] = maj;
      majs_path[7] = maj;
      majs_path[8] = count;
      //printf("%s\n",majs_path);
      lettersMatrix[i] = matrixFromFile(majs_path);
      maj++;

    }
    else if(i >= 26*1)
    {
      count = '3';
      mins_path[5] = min;
      mins_path[7] = min;
      mins_path[8] = count;
      //printf("%s\n",mins_path);
      lettersMatrix[i] = matrixFromFile(mins_path);
      min++;

    }
  }
  return lettersMatrix;
}

double *create_matrix(SDL_Surface *img)
{
  //Variables
  double *letterMatrix = malloc(sizeof(double) * 28 * 28);
  Uint8 r;
  Uint8 g;
  Uint8 b;

  for(int i = 0; i < img -> h; i++)
  {
      for(int j = 0; j < img -> w; j++)
      {
          Uint32 pixel = getpixel(img, j, i);
          SDL_GetRGB(pixel, img -> format, &r, &g, &b);
          if(r == 0 && g == 0 && b == 0)
              letterMatrix[j + i * img -> w] = 1;
          else
              letterMatrix[j + i * img -> w] = 0;
      }
  }
  return letterMatrix;
}

void append(char* s, char c)
{
  int len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
}

char* concat(const char *s1, const char *s2)
{
    //+1 for the null-terminator
    char *result = malloc(strlen(s1) + strlen(s2) + 1);

    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
