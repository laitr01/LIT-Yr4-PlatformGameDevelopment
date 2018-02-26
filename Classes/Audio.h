/*
	Audio.h

	Joe O'Regan
	K00203642

	Game audio class, with singleton access
*/

#ifndef __AUDIO__
#define __AUDIO__

#include "SimpleAudioEngine.h"
#include <string>
using namespace CocosDenshion;			// Audio engine

class Audio {
public:	
	static Audio* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Audio();
			return s_pInstance;
		}
		return s_pInstance;				// Return singleton instance
	}

	void init();						// Play music and preload effects

	void explodeFX();					// Play explosion sound effect
	void laserFX();						// Play laser sound effect

	void skipTrackForwards();			// Skip to the next track in the array, or first track if already at the last
	void skipTrackBackwards();			// Skip to the previous track in the array, or last track if already at the 1st
	void play();						// Play the background music
	void pause();						// Pause the background music

	// Volumes (Not working windows)
	void musicVolIncrease();			// Increase the music volume
	void musicVolDecrease();			// Decrease the music volume
	void sfxVolIncrease();				// Increase the FX volume
	void sfxVolDecrease();				// Decrease the FX volume
	void selectMenuOption();			// Play button sound

	std::string getTrackName();			// Get the name of the track
	bool isPaused();					// If music paused, show play button on music player

private:
	Audio() {};							// Constructor is private for use as a Singleton. Initializes the variables
	~Audio() {};						// Private destructor shuts down and cleans up the mixer API

	static Audio* s_pInstance;			// Single instance of Audio used as singleton so only one instance exists thoughout the game

	float currentVol;					// Current music volume
};

#endif // __AUDIO__