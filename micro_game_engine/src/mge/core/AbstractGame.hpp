#ifndef ABSTRACTGAME_HPP
#define ABSTRACTGAME_HPP

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <string>

#include "mge/core/GameObject.hpp"

class World;
class Renderer;

/**
 * Defines the basic structure for a game, with defaults already filled in.
 * The only item that MUST be overridden/implemented is _initializeScene()
 * where you build up your world, the rest is optional to override (with care/caution).
 */
class AbstractGame
{
    public:

        AbstractGame();
        virtual ~AbstractGame();

        //creates a window, initializes glew, a renderer and a world instance
        virtual void initialize();
        //run the actual process of updating all objects, rendering them and processing events
        virtual void run();

		float GetScrollDelta();

    protected:

        //methods above delegate behaviour to the methods below so that you can override it in a subclass

        //initialize sfml rendering context
        virtual void _initializeWindow();
        //print info about the current driver version etc
        virtual void _printVersionInfo();
        //initialize the extension wrangler
        virtual void _initializeGlew();
        //create our own custom renderer instance
        virtual void _initializeRenderer();
        //initialize a scene root to which we can attach/add objects
        virtual void _initializeWorld();

		sf::View CalcView(const sf::Vector2u &windowSize, const sf::Vector2u &desiredSize);

        //initialize the actual scene, HAS to be done by a subclass
        virtual void _initializeScene() = 0;

        //call update on all game objects in the display root
        virtual void _update(float pStep);
		//Delete all unused gameobjects
		virtual void _collectGarbage();
        //render all game objects in the display root
        virtual void _render();
        //process any sfml window events (see SystemEventDispatcher/Listener)
        virtual void _processEvents();

		sf::RenderWindow* _window;  //sfml window to render into
		Renderer* _renderer;        //the renderer class to render the world
		World* _world;              //the root game object that represents our scene
		float _fps;                 //stores the real fps

		GameObject* _camera;
		float _mouseWheelScrollDelta = 0;

    private:
        AbstractGame(const AbstractGame&);
        AbstractGame& operator=(const AbstractGame&);


};

#endif // ABSTRACTGAME_HPP
