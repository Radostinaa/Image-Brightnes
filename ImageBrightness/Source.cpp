#include <SDL/SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <algorithm>

float calcBrightness(std::string path)
{
	SDL_Surface *image;
	image = IMG_Load(path.c_str());
	if (!image)
	{
		printf("not loaded %s -> %s\n", path.c_str(), IMG_GetError());
	}

	unsigned char* pixels = (unsigned char*)image->pixels;
	int count = 0;
	Uint32 pixel, green, red, blue, temp;
	float oRed, oGreen, oBlue;
	float brightness = 0.0f;

	SDL_PixelFormat *fmt;
	fmt = image->format;

	for (int x = image->w / 3; x < image->w * 2 / 3; x++)
	{
		for (int y = image->h / 3; y < image->h * 2 / 3; y++)
		{
			pixel = *(Uint32*)(pixels + y * image->pitch + x);


			/* Get Red component */
			temp = pixel & fmt->Rmask;  /* Isolate red component */
			temp = temp >> fmt->Rshift; /* Shift it down to 8-bit */
			temp = temp << fmt->Rloss;  /* Expand to a full 8-bit number */
			red = (Uint8)temp;

			/* Get Green component */
			temp = pixel & fmt->Gmask;  /* Isolate green component */
			temp = temp >> fmt->Gshift; /* Shift it down to 8-bit */
			temp = temp << fmt->Gloss;  /* Expand to a full 8-bit number */
			green = (Uint8)temp;

			/* Get Blue component */
			temp = pixel & fmt->Bmask;  /* Isolate blue component */
			temp = temp >> fmt->Bshift; /* Shift it down to 8-bit */
			temp = temp << fmt->Bloss;  /* Expand to a full 8-bit number */
			blue = (Uint8)temp;

			/*gamma correnction */
			oRed = pow(float(red) / 255.f, 2.2f);
			oGreen = pow(float(green) / 255.f, 2.2f);
			oBlue = pow(float(blue) / 255.f, 2.2f);

			brightness += (oRed + oGreen + oBlue) / 3;
			count++;
		}
	}
	SDL_FreeSurface(image);

	return brightness / count;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " PATH" << std::endl;
		return 1;
	}

	for (int i = 1; i < argc; ++i)
	{
		printf("%s - %f \n", argv[i], calcBrightness(argv[i]));
	}

	return 0;
}