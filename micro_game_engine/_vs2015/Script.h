#ifndef SCRIPT_H
#define SCRIPT_H

#include <LuaJIT-2.0.5\src\lua.hpp>
#include "mge\core\AbstractGame.hpp"
#include "Hud.h"
#include <string.h>
#include "Resource.h"

class Script
{
public:
	virtual ~Script();

	void newComponent(GameObject * object);
	void init();
	void update(float dt);

	//void handle(CollisionEvent * event);

	static Script& instance();

	void setDebugging(bool pDebug);
	bool getDebugging();

	void setIsResScreen(bool pResScreen);
	bool getIsResScreen();

	void setHud(Hud* pHud);
	Hud* getHud();
	
	void initialize();
	GameObject* canvas;
	bool _startGame = false;		//not sure why this isnt private

	void setStartGame(bool pStart);
	bool getStartGame();

	void setWinningPlayer(int pWinner);
	int  getWinningPlayer();

	void setTileSet(ResourceSet pTiles);
	ResourceSet getTileSet();

	std::list<std::string> resolutionScreens;

private:
	Script();


	void registerFunctions();
	static int getDistance(lua_State * state);
	static int menuAccept(lua_State * state);
	static int menuBack(lua_State * state);
	static int menuUp(lua_State * state);
	static int menuDown(lua_State * state);
	static int menuLeft(lua_State * state);
	static int menuRight(lua_State * state);
	static int changeCar(lua_State * state);
	static int loadLevel(lua_State * state);
	static int setTileSet(lua_State * state);
	static int switchMusic(lua_State * state);
	static int setSound(lua_State * state);
	static int drawHUD(lua_State * state);
	static int menuPickCDTimer(lua_State * state);
	static int exitGame(lua_State * state);
	static int startGame(lua_State * state);
	static int loadNormal(lua_State * state);
	static int loadMountain(lua_State * state);
	static int loadBeach(lua_State * state);
	static int loadForest(lua_State * state);
	static int loadVillage(lua_State * state);
	static int loadCity(lua_State * state);
	static int loadKanjoEnvironment(lua_State * state);
	static int loadCars(lua_State * state);
	static int loadResolutionScreen(lua_State * state);
	static int randomNumber(lua_State * state);
	static int getResolutionScreen(lua_State * state);
	static int drawResolutionScreen(lua_State * state);
	static int winnerN(lua_State * state);

	bool _isDebugging;
	Hud* _hud;
	ResourceSet _tileSet;
	lua_State * _state;
	std::vector<GameObject *> _objects;
	int litMetter = 0;
	int _litsAndShit = 0;
	bool _isResolutionScreenOn = false;
	bool _isFlameDone = false;
	int _pNW = 0;
	float _flameTime = 0;
};

#endif
