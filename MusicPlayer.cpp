#include "MusicPlayer.h"

Sounds::Sounds()
{
	// nothing needs to go here
}
Sounds::~Sounds()
{
	Mix_FreeChunk(sound);
	sound = NULL;
	Mix_CloseAudio();
}
void Sounds::background(int loops)
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
	sound = NULL;
	sound = Mix_LoadWAV("assets/OverworldDay.wav");
	Mix_PlayChannel(-1, sound, loops);
}
void Sounds::powerUp()
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
	sound = NULL;
	sound = Mix_LoadWAV("assets/Powerup.wav");
	Mix_PlayChannel(-1, sound, 0);
}
void Sounds::objectHit()
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
	sound = NULL;
	sound = Mix_LoadWAV("assets/fail-buzzer-03.wav");
	Mix_PlayChannel(-1, sound, 0);
}