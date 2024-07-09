#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include "SFML\Audio.hpp"
#include <unordered_map>
#include <vector>
#include <array>
#include <..\_vs2015\Resource.h>

class SoundPair {
public:

	SoundPair();
	SoundPair(sf::SoundBuffer& pBuffer, sf::Sound& pSound);

	sf::SoundBuffer buffer;
	sf::Sound sound;
};


class MusicManager 
{
public:
	virtual ~MusicManager();
	sf::SoundBuffer& loadSound(const std::string& pFilename);
	sf::Sound& playSound(sf::SoundBuffer& pSound);
	sf::Sound& playSound(const std::string& pFilename);
	sf::Music& playMusic(const std::string& pFilename);
	//sf::Music& getCurrentMusic() const;
	sf::Music& playMusicSet(ResourceSet pSet);
	sf::Sound& MusicManager::playSoundRandom();

	void deleteSound(std::string& pFilename);
	void clearBufferCache();

	static MusicManager& instance();
	void populateMusic();

	MusicManager();
private:

	sf::Music _music;
	std::unordered_map<int, std::vector<std::string>> _musicSets;
	std::vector<SoundPair*> _crashes;

	sf::SoundBuffer _buffer;
	std::unordered_map<std::string, SoundPair*> _sounds;
};

#endif // !MUSIC_MANAGER_H

