#include "image.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

/**
 * Image
 **/
Image::Image (int width_, int height_){

    assert(width_> 0);
    assert(height_> 0);

    width           = width_;
    height          = height_;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;

    data.raw = new uint8_t[num_pixels*4];
	int b = 0; //which byte to write to
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			data.raw[b++] = 0;
			data.raw[b++] = 0;
			data.raw[b++] = 0;
			data.raw[b++] = 0;
		}
	}

    assert(data.raw != NULL);
}

Image::Image (const Image& src){

	width           = src.width;
    height          = src.height;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;

    data.raw = new uint8_t[num_pixels*4];

    //memcpy(data.raw, src.data.raw, num_pixels);
    *data.raw = *src.data.raw;
}

Image::Image (char* fname){

	int numComponents; //(e.g., Y, YA, RGB, or RGBA)
	data.raw = stbi_load(fname, &width, &height, &numComponents, 4);

	if (data.raw == NULL){
		printf("Error loading image: %s", fname);
		exit(-1);
	}


	num_pixels = width * height;
	sampling_method = IMAGE_SAMPLING_POINT;

}

Image::~Image (){
    delete data.raw;
    data.raw = NULL;
}

void Image::Write(char* fname){

	int lastc = strlen(fname);

	switch (fname[lastc-1]){
	   case 'g': //jpeg (or jpg) or png
	     if (fname[lastc-2] == 'p' || fname[lastc-2] == 'e') //jpeg or jpg
	        stbi_write_jpg(fname, width, height, 4, data.raw, 95);  //95% jpeg quality
	     else //png
	        stbi_write_png(fname, width, height, 4, data.raw, width*4);
	     break;
	   case 'a': //tga (targa)
	     stbi_write_tga(fname, width, height, 4, data.raw);
	     break;
	   case 'p': //bmp
	   default:
	     stbi_write_bmp(fname, width, height, 4, data.raw);
	}
}
// ./hw1 -input sample_images/hopkins.jpg -noise 0.3 -output outnoise.jpg
void Image::AddNoise (double factor)
{
	for (int x = 0; x < Width(); x++) {
        for (int y = 0; y < Height(); y++) {
            Pixel p = GetPixel(x, y);
            Pixel noisy_p;
            noisy_p.SetClamp(rand() % 512 - 256, rand() % 512 - 256, rand() % 512 - 256);
            GetPixel(x, y) = PixelLerp(p, noisy_p, factor);
        }
    }
	/* WORK HERE */
}

void Image::Brighten (double factor)
{
	int x,y;
	for (x = 0 ; x < Width() ; x++)
	{
		for (y = 0 ; y < Height() ; y++)
		{
			Pixel p = GetPixel(x, y);
			Pixel scaled_p = p*factor;
			GetPixel(x,y) = scaled_p;
		}
	}
}

//./hw1 -input sample_images/hopkins.jpg -contrast 5.0 -output outcontrast.jpg
void Image::ChangeContrast (double factor)
{
  int averagegrey=0;
	for (int x = 0 ; x < Width() ; x++)
	{
		for (int y= 0 ; y < Height() ; y++)
		{
      Pixel p = GetPixel(x, y);
      averagegrey+= p.Luminance();
		}
	}
  averagegrey=averagegrey/(Width()*Height());
  Pixel grey;
  grey.Set((Component)averagegrey,(Component)averagegrey,(Component)averagegrey);
  int x,y;
	for (int x = 0 ; x < Width() ; x++)
	{
		for (int y = 0 ; y < Height() ; y++)
		{
			Pixel p = GetPixel(x, y);
      GetPixel(x,y) = PixelLerp(grey,p,factor);
		}
	}



	/* WORK HERE */

}
//./hw1 -input sample_images/hopkins.jpg -saturation 5  -output outsaturation.jpg


void Image::ChangeSaturation(double factor)
{	int x,y;
	for (x = 0 ; x < Width() ; x++)
	{
		for (y = 0 ; y < Height() ; y++)
		{
			Pixel p = GetPixel(x, y);
      Pixel grey;
      grey.Set(p.Luminance(),p.Luminance(),p.Luminance());
			GetPixel(x,y) = PixelLerp(grey,p,factor);
		}
	}
	/* WORK HERE */
}


Image* Image::Crop(int x, int y, int w, int h)
{   assert(ValidCoord(x, y));
    assert(ValidCoord(x + w, y + h));
	Image * cropimage=new Image(w,h);
	for (int i = x; i < (w + x); i++){
for (int j = y; j < (h + y); j++)
 {
 Pixel pixel = GetPixel(i, j);
 cropimage->SetPixel(i - x, j - y, pixel);
		}
	}
	/* WORK HERE */
	return cropimage;
}

//-extractChannel <channel no>
//./hw1 -input sample_images/hopkins.jpg -extractChannel 0 -output outchannel.jpg
void Image::ExtractChannel(int channel)
{
  // red:0 green:1 Blue:2
  switch(channel){
    case 0:
    for (int x=0;x<Width();x++){
      for (int y=0;y<Height();y++){
        Pixel p= GetPixel(x, y);
        p.Set(p.r,0,0);
        GetPixel(x,y) = p;
      }
    }
    break;
    case 1:
    for (int x=0;x<Width();x++){
      for (int y=0;y<Height();y++){
        Pixel p= GetPixel(x, y);
        p.Set(0,p.g,0);
        GetPixel(x,y) = p;

      }
    }
    break;
    case 2:
    for (int x=0;x<Width();x++){
      for (int y=0;y<Height();y++){
        Pixel p= GetPixel(x, y);
        p.Set(0,0,p.b);
        GetPixel(x,y) = p;
      }
    }
    break;
    default:
    return;
  }

	/* WORK HERE */
}

//./hw1 -input sample_images/hopkins.jpg -quantize 2 -output quantize.jpg
void Image::Quantize (int nbits)
{
  if (nbits>7)return;
  for (int x = 0 ; x < Width() ; x++)
	{
		for (int y= 0 ; y < Height() ; y++)
		{
      Pixel p = GetPixel(x, y);
      Pixel quant=PixelQuant(p, nbits);
      GetPixel(x,y)=quant;

		}
	}
	/* WORK HERE */
}

void Image::RandomDither (int nbits)
{
  if (nbits>7)return;
  for (int x = 0 ; x < Width() ; x++){
  		for (int y= 0 ; y < Height() ; y++){
        Pixel p=GetPixel(x,y);
        double randomnoise=rand()%(2^(8-nbits))-(2^(7-nbits));
        Component r = trunc(p.r +randomnoise + 0.5);
        Component g = trunc(p.r +randomnoise + 0.5);
        Component b = trunc(p.b + randomnoise + 0.5);
        Pixel dither;
        dither.SetClamp(r,g,b);
        SetPixel(x,y,PixelQuant(p, nbits));


        	/* WORK HERE */
}
}
}


static int Bayer4[4][4] =
{
    {15,  7, 13,  5},
    { 3, 11,  1,  9},
    {12,  4, 14,  6},
    { 0,  8,  2, 10}
};

//./hw1 -input sample_images/hopkins.jpg -orderedDither 1  -output orderdither.jpg

void Image::OrderedDither(int nbits)
{    if (nbits>7)return;
  for (int x=0;x<Width();x++){
   for(int y=0;y<Height();y++){
     int i=x % 4;
     int j=y % 4;
     Pixel p=GetPixel(x,y);
     double re=double(p.r-trunc(p.r));
     double ge=double(p.g-trunc(p.g));
     double be=double(p.b-trunc(p.b));
     double r,g,b;
     if(re>Bayer4[i][j]){
       r=ceil(p.r);
     }else{
       r=floor(p.r);
     }

     if(ge>Bayer4[i][j]){
       g=ceil(p.g);
     }else{
       g=floor(p.g);
     }

     if(be>Bayer4[i][j]){
       b=ceil(p.b);
     }else{
       b=floor(p.b);
     }
     Pixel temp;
     temp.SetClamp(r,g,b);
     SetPixel(x,y,PixelQuant(temp,nbits));

   }
  }
  }




	/* WORK HERE */


/* Error-diffusion parameters */
const double
    ALPHA = 7.0 / 16.0,
    BETA  = 3.0 / 16.0,
    GAMMA = 5.0 / 16.0,
    DELTA = 1.0 / 16.0;

void Image::FloydSteinbergDither(int nbits)
{
  if (nbits>7)return;
  for (int x = 1 ; x < Width()-1 ; x++){
  		for (int y= 0; y < Height()-1; y++){
        Pixel p=GetPixel(x,y);
        Pixel alpha=GetPixel(x+1,y);
        Pixel beta=GetPixel(x-1,y+1);
        Pixel gamma=GetPixel(x,y+1);
        Pixel delta=GetPixel(x+1,y+1);
        double er=ALPHA*alpha.r+BETA*beta.r+GAMMA*gamma.r+DELTA*delta.r;
        double eg=ALPHA*alpha.g+BETA*beta.g+GAMMA*gamma.g+DELTA*delta.g;
        double eb=ALPHA*alpha.b+BETA*beta.b+GAMMA*gamma.b+DELTA*delta.b;
        Component r = trunc(p.r +er + 0.5);
        Component g = trunc(p.r +eg + 0.5);
        Component b = trunc(p.b +eb + 0.5);
        Pixel dither;
        dither.SetClamp(r,g,b);
        SetPixel(x,y,PixelQuant(p, nbits));   // for(int i=)
}
}
	/* WORK HERE */
}
// static int Gaussian[5][5] =
// {
//     {1/273,  4/273, 7/273,4/273,1/273},
//     {4/273,  16/273, 26/273,16/273,4/273},
//     {7/273,  26/273, 41/273,26/273,7/273},
//     {7/273,  26/273, 41/273,26/273,7/273},
//     {1/273,  4/273, 7/273,4/273,1/273},
//
// };
void Image::Blur(int n)
{
double sigma = 1;
double Gaussion[n][n];
double mean = n/2;
double sum = 0.0; // For accumulating the kernel values
for (int x = 0; x < n; ++x){
    for (int y = 0; y < n; ++y) {
        Gaussion[x][y] = exp( -0.5 * (pow((x-mean)/sigma, 2.0) + pow((y-mean)/sigma,2.0)) )
                         / (2 * M_PI * sigma * sigma);

        // Accumulate the kernel values
        sum += Gaussion[x][y];


    }
  }
// Normalize the kernel
for (int x = 0; x < n; ++x){
    for (int y = 0; y < n; ++y){
        Gaussion[x][y]= Gaussion[x][y]/sum;


}
}
  Image* temp=new Image(Width(),Height());
    for (int x = (n-1)/2 ; x < Width()-((n-1)/2); x++){
        for (int y= (n-1)/2; y < Height()-((n-1)/2); y++){
          double r=0,g=0,b=0;
          for(int i =0;i<n;i++){
            for(int j=0;j<n;j++){
              r+=Gaussion[i][j]*GetPixel(x-(n-1)/2+i,y-(n-1)/2+j).r;
              g+=Gaussion[i][j]*GetPixel(x-(n-1)/2+i,y-(n-1)/2+j).g;
              b+=Gaussion[i][j]*GetPixel(x-(n-1)/2+i,y-(n-1)/2+j).b;
          }
          }
          Pixel p;
          p.SetClamp(r,g,b);
          temp->SetPixel(x,y,p);
        }
  }

  for (int x = 1 ; x < Width() ; x++){
      for (int y= 1; y < Height(); y++){
        Pixel temp2= temp->GetPixel(x,y);
        SetPixel(x,y,temp2);}}
	/* WORK HERE */
}

void Image::Sharpen(int n)
{
  Image thisImage= Image(Width(),Height());
  for (int x = 0 ; x < Width() ; x++){
          for (int y= 0; y < Height(); y++){
            thisImage.SetPixel(x,y,GetPixel(x,y));}}
  thisImage.Blur(n);
  Image* temp=new Image(Width(),Height());
  // printf("tempweight%stempheight%s\n",temp.Width(),temp.Height() );
  // printf("weight%spheight%s\n",Width(),Height());
  // for (int x = 0 ; x < Width() ; x++){
  //     for (int y= 0; y < Height(); y++){
  //       Pixel temp2= GetPixel(x,y);
  //       temp.SetPixel(x,y,temp2);}}
        // printf("x%dfy%d\n",x,y);
  for (int x =0 ; x < Width() ; x++){
      for (int y= 0; y < Height(); y++){
        Pixel blur=GetPixel(x,y);
        Pixel blurblur=thisImage.GetPixel(x,y);
        temp->SetPixel(x,y,PixelLerp(blur,blurblur,-0.5);}}
    for (int x = 0 ; x < Width() ; x++){
            for (int y= 0; y < Height(); y++){
              Pixel temp2= temp->GetPixel(x,y);
              SetPixel(x,y,temp2);}}


	/* WORK HERE */
}
static int Filter[3][3] =
{
    {-1,  -1, -1},
    {-1, 8,  -1},
    {-1,  -1, -1},
};
//./hw1 -input sample_images/hopkins.jpg -edgeDetect  -output edge.jpg
void Image::EdgeDetect()
{ Image* temp=new Image(Width(),Height());
  for (int x = 1 ; x < Width()-1 ; x++){
      for (int y= 1; y < Height()-1; y++){
        int value=0;
        for(int i =0;i<3;i++){
          for(int j=0;j<3;j++){
            value+=Filter[i][j]*GetPixel(x-1+i,y-1+i).Luminance();
        }
        }
        Pixel p;
        p.SetClamp(value,value,value);
        temp->SetPixel(x,y,p);




      }


	/* WORK HERE */
}
for (int x = 1 ; x < Width()-1 ; x++){
    for (int y= 1; y < Height()-1; y++){
      Pixel temp2= temp->GetPixel(x,y);
      SetPixel(x,y,temp2);}}
}
//./hw1 -input sample_images/hopkins.jpg -scale 0.8 0.5 -output scale.jpg

Image* Image::Scale(double sx, double sy)
{ int U=Width() * sx;
  int V=Height() * sy;
	Image *scale = new Image(U, V);
    for (int y = 0; y<V; y++){
      for (int x=0;x<U; x++){
             double u = x / sx;
            double v = y / sy;
            scale->SetPixel(x, y, Sample(u, v));
        }
    }
    return scale;
}
//./hw1 -input sample_images/hopkins.jpg -rotate 45 -output rotate.jpg

Image* Image::Rotate(double angle)
{
   double theta = angle * M_PI/ 180;
   Image *newImage = new Image(Width(), Height());
   for (int y = 0; y < Height(); y++) {
       for (int x = 0; x < Width(); x++) {
           double u = (x-Width()/2) * cos(theta) - (y-Height()/2) * sin(theta)+Width()/2;
           double v = (x-Width()/2) * sin(theta) + (y-Height()/2) * cos(theta)+Height()/2;
           newImage->SetPixel(x,y,Sample(u,v));
       }
   }
   return newImage;
	/* WORK HERE */
}
//mirrow a picture
//./hw1 -input sample_images/hopkins.jpg -fun  -output fun.jpg
void Image::Fun()
{
   for (int y = 0; y<Height(); y++){
     for (int x=0;x<Width(); x++){
          double u = x+ 20*cos(x*3.14159*60/20);
           double v = y;
           SetPixel(x,y,Sample(u,v));
       }
   }
       }



/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method)
{
    assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
    sampling_method = method;
}

Pixel Image::Sample (double u, double v){
  Pixel p;
  p.Set(0,0,0);

  if (u > Width()-1 || v>Height()-1|| u<0 || v < 0)
        return p;

  switch (sampling_method) {
        case IMAGE_SAMPLING_POINT:
            return GetPixel((int) u, (int) v);
    /* WORK HERE */
       // case IMAGE_SAMPLING_GAUSSIAN:
       // return new *pixel;
 // Deal with edge values


}
}
