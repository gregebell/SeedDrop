#ifndef MUSIC_PLAYER
#define MUSIC_PLAYER

#include "SDL_mixer.h"


class Sounds
{
public:
	Sounds();
	~Sounds();

	void background(int loops);
	void powerUp();
	void objectHit();

private:
	Mix_Chunk *sound;
	
};
#endif