#include "MusicManager.h"
#include <iostream>
#include <glm.hpp>
#include "mge\config.hpp"

MusicManager::MusicManager() 
{
	populateMusic();
}

MusicManager::~MusicManager() 
{
}

SoundPair::SoundPair() {

}

SoundPair::SoundPair(sf::SoundBuffer& pBuffer, sf::Sound& pSound) {
	buffer = pBuffer;
	sound = pSound;
}

sf::SoundBuffer& MusicManager::loadSound(const std::string& pFilename) {
	if (_buffer.loadFromFile(config::MGE_SOUND_PATH + pFilename)) {
		
		sf::SoundBuffer* buff = new sf::SoundBuffer(_buffer);
		SoundPair* soundPair = new SoundPair(*buff, sf::Sound(*buff));
		_sounds.emplace(config::MGE_SOUND_PATH + pFilename, soundPair);

		return *buff;
	}

	return _buffer;
}

sf::Sound& MusicManager::playSound(const std::string& pFilename) {

	if(_sounds[config::MGE_SOUND_PATH + pFilename]->sound.getStatus() == sf::Sound::Status::Stopped)
		_sounds[config::MGE_SOUND_PATH + pFilename]->sound.play();
		
	return _sounds[config::MGE_SOUND_PATH + pFilename]->sound;
}

sf::Sound& MusicManager::playSoundRandom() {
	int index = glm::linearRand(0, (int)_crashes.size() - 1);

	switch (index)
	{
	case 1:
		playSound("CarCrash.ogg").setVolume(150);
		break;
	case 2:
		playSound("CarCrash1.ogg").setVolume(150);
		break;
	case 3:
		playSound("CarCrash2.ogg").setVolume(150);
		break;
	case 4:
		playSound("CarCrash3.ogg").setVolume(150);
		break;
	case 5:
		playSound("CarCrash4.ogg").setVolume(150);
		break;
	default:
		playSound("CarCrash.ogg").setVolume(150);
		break;
	}

	return _crashes[index]->sound;
}

sf::Music& MusicManager::playMusic(const std::string& pFilename) {
	if (_music.openFromFile(pFilename)) {
		_music.play();
		_music.setLoop(true);
		return _music;
	}
}

void MusicManager::populateMusic() {
	std::vector<std::string> forest = { "Micro_Fire.ogg", "Mob_Battle.ogg", "Work_Week.ogg" };
	std::vector<std::string> mountain = { "Give.ogg", "Periscope.ogg", "Work_Week.ogg" };
	std::vector<std::string> beach = { "Tidal_Wave.ogg", "Whats_It_To_Ya_Punk.ogg", "Catch_a_Way.ogg", "Blue_Whale.ogg" };
	std::vector<std::string> village = { "Free_Dog.ogg", "Propeller.ogg", "Time_Piece.ogg" };
	std::vector<std::string> kanjo = { "Armadillo.ogg", "Half_Pipe.ogg", "Lucky_Shot.ogg" };
	std::vector<std::string> city = { "Cut_It.ogg", "Hitting_the_Streets.ogg", "Tactical_Confusion.ogg" };
	std::vector<std::string> menu = { "Jack_s_Garage.ogg" };

	_musicSets.emplace(Forest, forest);
	_musicSets.emplace(Mountain, mountain);
	_musicSets.emplace(Beach, beach);
	_musicSets.emplace(Village, village);
	_musicSets.emplace(Kanjo, kanjo);
	_musicSets.emplace(City, city);
	_musicSets.emplace(Menu, menu);

	loadSound("P1Idle.ogg");
	loadSound("P1Startup.ogg");
	loadSound("P1Acceleration.ogg");
	loadSound("P1Deceleration.ogg");
	loadSound("P1ConstantSpeed.ogg");

	loadSound("P2Idle.ogg");
	loadSound("P2Startup.ogg");
	loadSound("P2Acceleration.ogg");
	loadSound("P2Deceleration.ogg");
	loadSound("P2ConstantSpeed.ogg");

	loadSound("Hover.ogg");
	loadSound("Select.ogg");
	loadSound("StartSound.wav");

	sf::SoundBuffer c1 = loadSound("CarCrash.ogg");
	sf::SoundBuffer c2 = loadSound("CarCrash1.ogg");
	sf::SoundBuffer c3 = loadSound("CarCrash2.ogg");
	sf::SoundBuffer c4 = loadSound("CarCrash3.ogg");
	sf::SoundBuffer c5 = loadSound("CarCrash4.ogg");

	SoundPair* soundPair1 = new SoundPair(c1, sf::Sound(c1));
	_sounds.emplace(config::MGE_SOUND_PATH + "CarCrash.ogg", soundPair1);
	SoundPair* soundPair2 = new SoundPair(c2, sf::Sound(c2));
	_sounds.emplace(config::MGE_SOUND_PATH + "CarCrash1.ogg", soundPair2);
	SoundPair* soundPair3 = new SoundPair(c3, sf::Sound(c3));
	_sounds.emplace(config::MGE_SOUND_PATH + "CarCrash2.ogg", soundPair3);
	SoundPair* soundPair4 = new SoundPair(c4, sf::Sound(c4));
	_sounds.emplace(config::MGE_SOUND_PATH + "CarCrash3.ogg", soundPair4);
	SoundPair* soundPair5 = new SoundPair(c5, sf::Sound(c5));
	_sounds.emplace(config::MGE_SOUND_PATH + "CarCrash4.ogg", soundPair5);

	_crashes.push_back(soundPair1);
	_crashes.push_back(soundPair2);
	_crashes.push_back(soundPair3);
	_crashes.push_back(soundPair4);
	_crashes.push_back(soundPair5);



	_music.setVolume(30);
}

sf::Music& MusicManager::playMusicSet(ResourceSet pSet) {
	int index = glm::linearRand(0, (int)_musicSets[pSet].size() -1);
	if (_music.openFromFile(config::MGE_SOUND_PATH + _musicSets[pSet][index])) {
		_music.play();
		_music.setLoop(true);
		return _music;
	}
}

void MusicManager::deleteSound(std::string& pFilename) {
	_sounds[pFilename];
}

void MusicManager::clearBufferCache() {
	//std::unordered_map< std::string&, sf::SoundBuffer&>::iterator it = _sounds.begin();

	//while (it != _sounds.end())
	//{
	//	delete (*it).second;

	//	it = _sounds.erase(it);
	//}
}

MusicManager& MusicManager::instance() {
	static MusicManager* instance = new MusicManager();
	return *instance;
}