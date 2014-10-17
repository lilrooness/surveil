#include <SDL2/SDL_image.h>

class ImageBank {
public:
	ImageBank();

	bool loadImage(const char *filename, const char *key);
};