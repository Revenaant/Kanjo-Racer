#include "Script.h"
#include <string.h>
#include "loadLevelBehaviour.h"

Script & Script::instance()
{
	static Script* instance = new Script();
	return *instance;
}

void Script::setDebugging(bool pDebug)
{
	_isDebugging = pDebug;
}

void Script::setHud(Hud* pHud)
{
	_hud = pHud;
}

Hud* Script::getHud()
{
	if(_hud != nullptr)
	return _hud;
}

bool Script::getDebugging()
{
	return _isDebugging;
}

void Script::setIsResScreen(bool pResScreen)
{
	_isResolutionScreenOn = pResScreen;
}

bool Script::getIsResScreen()
{
	return _isResolutionScreenOn;
}

Script::Script()
{
}

void Script::initialize()
{
	_state = luaL_newstate();
	luaL_openlibs(_state);

	if (_isDebugging) {
		lua_pushboolean(_state, true);
		lua_setglobal(_state, "debugging");
	}

	lua_newtable(_state);
	lua_setglobal(_state, "_states"); // empty

	if (luaL_dofile(_state, "main.lua") != 0) {
		printf("Lua Error %s\n", lua_tostring(_state, -1));
	}

	registerFunctions();
}

void Script::setStartGame(bool pStart)
{
	_startGame = pStart;
}

bool Script::getStartGame()
{
	return _startGame;
}

void Script::setWinningPlayer(int pWinner)
{
	Script::instance()._pNW = pWinner;
}

int Script::getWinningPlayer()
{
	return Script::instance()._pNW;
}

void Script::setTileSet(ResourceSet pTiles)
{
	_tileSet = pTiles;
}

ResourceSet Script::getTileSet()
{
	return _tileSet;
}

Script::~Script()
{
	lua_close(_state);
}

void Script::newComponent(GameObject * object)
{
	// type.init( object )
	const char * type = object->type().c_str();

	lua_getglobal(_state, type);
	if (lua_istable(_state, -1)) {
		lua_getfield(_state, -1, "init");
		if (lua_isfunction(_state, -1)) {
			lua_pushlightuserdata(_state, object);
			if (lua_pcall(_state, 1, 1, 0) == 0) {
				lua_getglobal(_state, "_states");
				lua_pushlightuserdata(_state, object);
				lua_pushvalue(_state, -3); // object state to top
				lua_settable(_state, -3); // set key (-2 ) of table ( -3 ), to value ( -1 )
										  //stackDump(_state, "");
			}
			else {
				printf("Lua error: %s\n", lua_tostring(_state, -1));
				exit(-1);
			}
		}
	}
	_objects.push_back(object);
}

void Script::init() {
	// global init
	lua_getglobal(_state, "init");
	if (lua_isfunction(_state, -1)) {
		if (lua_pcall(_state, 0, 0, 0) != 0) {
			printf("Lua error: %s\n", lua_tostring(_state, -1));
		}
	}
}

void Script::update(float dt) {
	// global update
	lua_getglobal(_state, "update");
	if (lua_isfunction(_state, -1)) {
		if (lua_pcall(_state, 0, 0, 0) != 0) {
			printf("Lua error: %s\n", lua_tostring(_state, -1));
		}
	}

	// type update
	for (int i = _objects.size() - 1; i >= 0; --i) {
		GameObject * object = _objects[i];
		lua_getglobal(_state, "_states"); // table of all objects with their state
		lua_pushlightuserdata(_state, object); // key
											   //stackDump(_state, "");
		lua_gettable(_state, -2); // get state from states (-2) for object (-1)
		lua_getfield(_state, -1, "update"); // update from state
		if (lua_isfunction(_state, -1)) {
			lua_pushvalue(_state, -2); // state on top
			lua_pushnumber(_state, dt);
			//stackDump(_state, "" );
			if (lua_pcall(_state, 2, 0, 0) != 0) {
				printf("Lua error: %s\n", lua_tostring(_state, -1));
			}
		}
		lua_settop(_state, 0);
	}

	// remove all marked for deletion
}

/*void Script::handle(CollisionEvent * event)
{
	printf("Script handles %s\n", event->name.c_str());
	std::string name = "on" + event->name;
	printf("Event %s\n", name.c_str());
	lua_getglobal(_state, "_states"); // table of all objects with their state

	lua_getglobal(_state, name.c_str());
	if (lua_isfunction(_state, -1)) {
		lua_pushlightuserdata(_state, event->one);
		lua_gettable(_state, -3); // get state from states (-3) for object (-1), pops object
		lua_pushlightuserdata(_state, event->other);
		lua_gettable(_state, -4); // get state from states (-4) for object (-1), pops object
		if (lua_pcall(_state, 2, 0, 0) != 0) {
			printf("Error %s", lua_tostring(_state, -1));
		}
	}
}*/

void Script::registerFunctions() {
	lua_newtable(_state); // Game
	lua_pushcfunction(_state, Script::getDistance);
	lua_setfield(_state, -2, "getDistance");
	lua_pushcfunction(_state, Script::winnerN);
	lua_setfield(_state, -2, "winnerN");
	lua_setglobal(_state, "Game");

	lua_newtable(_state); // MainMenu
	lua_pushcfunction(_state, Script::menuAccept);
	lua_setfield(_state, -2, "menuAccept");
	lua_pushcfunction(_state, Script::menuBack);
	lua_setfield(_state, -2, "menuBack");
	lua_pushcfunction(_state, Script::menuUp);
	lua_setfield(_state, -2, "menuUp");
	lua_pushcfunction(_state, Script::menuDown);
	lua_setfield(_state, -2, "menuDown");
	lua_pushcfunction(_state, Script::menuLeft);
	lua_setfield(_state, -2, "menuLeft");
	lua_pushcfunction(_state, Script::menuRight);
	lua_setfield(_state, -2, "menuRight");
	lua_pushcfunction(_state, Script::changeCar);
	lua_setfield(_state, -2, "changeCar");
	lua_pushcfunction(_state, Script::loadLevel);
	lua_setfield(_state, -2, "loadLevel");
	lua_pushcfunction(_state, Script::setTileSet);
	lua_setfield(_state, -2, "setTileSet");
	lua_pushcfunction(_state, Script::switchMusic);
	lua_setfield(_state, -2, "switchMusic");
	lua_pushcfunction(_state, Script::setSound);
	lua_setfield(_state, -2, "setSound");
	lua_pushcfunction(_state, Script::drawHUD);
	lua_setfield(_state, -2, "drawHUD");
	lua_pushcfunction(_state, Script::menuPickCDTimer);
	lua_setfield(_state, -2, "menuPickCDTimer");
	lua_pushcfunction(_state, Script::exitGame);
	lua_setfield(_state, -2, "exitGame");
	lua_pushcfunction(_state, Script::startGame);
	lua_setfield(_state, -2, "startGame");
	lua_pushcfunction(_state, Script::loadNormal);
	lua_setfield(_state, -2, "loadNormal");
	lua_pushcfunction(_state, Script::loadMountain);
	lua_setfield(_state, -2, "loadMountain");
	lua_pushcfunction(_state, Script::loadBeach);
	lua_setfield(_state, -2, "loadBeach");
	lua_pushcfunction(_state, Script::loadForest);
	lua_setfield(_state, -2, "loadForest");
	lua_pushcfunction(_state, Script::loadVillage);
	lua_setfield(_state, -2, "loadVillage");
	lua_pushcfunction(_state, Script::loadCity);
	lua_setfield(_state, -2, "loadCity");
	lua_pushcfunction(_state, Script::loadKanjoEnvironment);
	lua_setfield(_state, -2, "loadKanjoEnvironment");
	lua_pushcfunction(_state, Script::loadCars);
	lua_setfield(_state, -2, "loadCars");
	lua_pushcfunction(_state, Script::loadResolutionScreen);
	lua_setfield(_state, -2, "loadResolutionScreen");
	lua_pushcfunction(_state, Script::randomNumber);
	lua_setfield(_state, -2, "randomNumber");
	lua_pushcfunction(_state, Script::getResolutionScreen);
	lua_setfield(_state, -2, "getResolutionScreen");
	lua_pushcfunction(_state, Script::drawResolutionScreen);
	lua_setfield(_state, -2, "drawResolutionScreen");
	lua_setglobal(_state, "MainMenu");
}

int Script::getDistance(lua_State * state)
{
	return 0;
}

int Script::menuAccept(lua_State * state)
{
	//if (lua_gettop(state) == 1 && lua_istable(state, 1)) {
		bool joystickA = sf::Joystick::isButtonPressed(0, 0);

		lua_pushboolean(state, joystickA);
		return 1;
	//}
	//return luaL_error(state, "You somehow managed to fuck this up, no idea how");
}

int Script::menuBack(lua_State * state)
{
	//if (lua_gettop(state) == 1 && lua_istable(state, 1)) {
		bool joystickB = sf::Joystick::isButtonPressed(0, 1);

		lua_pushboolean(state, joystickB);
		return 1;
	//}
	//return luaL_error(state, "You somehow managed to fuck this up, no idea how");
}

const float DEADZONE_X = 20;
const float DEADZONE_Y = 20;

int Script::menuUp(lua_State * state)
{
	//if (lua_gettop(state) == 1 && lua_istable(state, 1)) {

		float joystickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		bool activate = false;

		if (joystickY < -DEADZONE_Y) {
			activate = true;
		}
		lua_pushboolean(state, activate);
		return 1;
	//}
	//return luaL_error(state, "You somehow managed to fuck this up, no idea how");
}

int Script::menuDown(lua_State * state)
{
	//if (lua_gettop(state) == 1 && lua_istable(state, 1)) {
		float joystickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		bool activate = false;

		if (joystickY > DEADZONE_Y) {
			activate = true;
		}
		lua_pushboolean(state, activate);
		return 1;
	/*}
	return luaL_error(state, "You somehow managed to fuck this up, no idea how");*/
}

int Script::menuLeft(lua_State * state) 
{
	//if (lua_gettop(state) == 1 && lua_istable(state, 1)) {
		float joystickX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		bool activate = false;

		if (joystickX < -DEADZONE_X) {
			activate = true;
		}
		lua_pushboolean(state, activate);
		return 1;
	/*}
	return luaL_error(state, "You somehow managed to fuck this up, no idea how");*/
}

int Script::menuRight(lua_State * state) 
{
	//if (lua_gettop(state) == 1 && lua_istable(state, 1)) {
		float joystickX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		bool activate = false;

		if (joystickX > DEADZONE_X) {
			activate = true;
		}
		lua_pushboolean(state, activate);
		return 1;
	//}
	//return luaL_error(state, "You somehow managed to fuck this up, no idea how");
}

int Script::changeCar(lua_State * state)
{
	
	return 0;
}

int Script::loadLevel(lua_State * state)
{
	return 0;
}

int Script::setTileSet(lua_State * state)
{
	/*const char* filename = lua_tostring(state, -1);
	if (filename == NULL) return 0;
	Script::instance().setTileSet(filename);
	std::cout << Script::instance().getTileSet();*/
	return 0;
}

int Script::switchMusic(lua_State * state)
{
	return 0;
}

int Script::setSound(lua_State * state)
{
	return 0;
}

int Script::drawHUD(lua_State * state)
{
	if (Script::instance().getHud() == nullptr) return 0;

	const char* filename = lua_tostring(state, -1);
	std::cout << filename << std::endl;
	if (filename == NULL) return 0;
	Script::instance().getHud()->addSprite(filename);
	return 0;
}

int Script::menuPickCDTimer(lua_State * state)
{
	float dt = 1.0f / 60.0f;
	lua_pushnumber(state, dt);
	return 1;
}

int Script::exitGame(lua_State * state)
{
	return 1;
}

int Script::startGame(lua_State * state)
{
	if (Script::instance().canvas == nullptr) return 0;
	bool shouldStart = lua_toboolean(state, -1);
	Script::instance().setStartGame(shouldStart);
	return 0;
}

int Script::loadNormal(lua_State * state)
{
	Script::instance().setTileSet(ResourceSet::Normal);
	return 0;
}

int Script::loadMountain(lua_State * state)
{
	Script::instance().setTileSet(ResourceSet::Mountain);
	return 0;
}

int Script::loadBeach(lua_State * state)
{
	Script::instance().setTileSet(ResourceSet::Beach);
	return 0;
}

int Script::loadForest(lua_State * state)
{
	Script::instance().setTileSet(ResourceSet::Forest);
	return 0;
}

int Script::loadVillage(lua_State * state)
{
	Script::instance().setTileSet(ResourceSet::Village);
	return 0;
}

int Script::loadCity(lua_State * state)
{
	Script::instance().setTileSet(ResourceSet::City);
	return 0;
}

int Script::loadKanjoEnvironment(lua_State * state)
{
	Script::instance().setTileSet(ResourceSet::Kanjo);
	return 0;
}

int Script::loadCars(lua_State * state)
{
	Script::instance().setTileSet(ResourceSet::Car);
	return 0;
}

int Script::loadResolutionScreen(lua_State * state)
{
	if (Script::instance()._hud == nullptr) return 0;
	return 1;
}

int Script::randomNumber(lua_State * state)
{
	Script::instance().litMetter = rand() % 4 + 3;
	lua_pushinteger(state, Script::instance().litMetter);
	return 1;
}

int Script::getResolutionScreen(lua_State * state)
{
	lua_pushboolean(state, Script::instance().getIsResScreen());
	return 1;
}

int Script::drawResolutionScreen(lua_State * state)
{
	float litness = lua_tointeger(state, -1);
	Script::instance()._flameTime += 0.016f;

	if (Script::instance()._litsAndShit < litness && Script::instance()._flameTime > 0.1f
		&& Script::instance().getHud() != nullptr)
	{
		const char* filename = lua_tostring(state, -2);
		Script::instance().getHud()->addSprite(filename + std::to_string(Script::instance()._litsAndShit) + ".png");
		Script::instance()._flameTime = 0;
		Script::instance()._litsAndShit += 1;

	}
	
	return 0;
}

int Script::winnerN(lua_State * state)
{
	lua_pushinteger(state, Script::instance().getWinningPlayer());
	return 1;
}
