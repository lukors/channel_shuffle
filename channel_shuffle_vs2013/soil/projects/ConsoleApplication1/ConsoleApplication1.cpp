#include "SOIL.h"

int main(int argc, char* argv[])
{
	int width, height, channels;
	unsigned char *ht_map = SOIL_load_image
		(
		"terrain.tga",
		&width, &height, &channels,
		SOIL_LOAD_L
		);

	SOIL_free_image_data(ht_map);

	return(0);
}