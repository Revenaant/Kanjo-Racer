#ifndef CAMERAORBIT_HPP
#define CAMERAORBIT_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/AbstractGame.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Graphics.hpp>

class CameraOrbitBehaviour : public AbstractBehaviour
{
public:
	CameraOrbitBehaviour(float pDistance, float pMinTilt, float pMaxTilt, float pRotateSpeed,
		GameObject* pTarget, sf::Window* pWindow, AbstractGame* game);//, GameObject* pPlayer1, GameObject* pPlayer2);

	virtual ~CameraOrbitBehaviour();
	virtual void update(float pStep);
	void DualCameraFollow(GameObject* t1, GameObject* t2, float deltaTime);


	 GameObject* _player1;
	 GameObject* _player2;

	 float _zoomFactor = 1.5f;
	 float _followTimeDelta = 0.8f;
	 glm::vec2 _zoomBounds = { 0.5f, 10.0f };

private:
	float _distance;
	float _minTilt;
	float _maxTilt;
	float _rotateSpeed;
	GameObject* _target;
	sf::Window* _window;
	AbstractGame* _game;


	glm::vec3 _twoPlayerCam;
	glm::vec3 _newCamPos;

	float _zoomDistance = 0;
	float _startOffset = 0;
};

#endif