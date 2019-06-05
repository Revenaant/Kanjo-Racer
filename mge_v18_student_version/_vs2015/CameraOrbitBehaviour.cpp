#include "CameraOrbitBehaviour.h"
#include <iostream>
#include <cmath> 
#include <cstdint>
#include <algorithm>

CameraOrbitBehaviour::CameraOrbitBehaviour(float pDistance, float pMinTilt, float pMaxTilt, float pRotateSpeed, GameObject* pTarget, sf::Window* pWindow, AbstractGame* game) :
	AbstractBehaviour(), _distance(pDistance), _minTilt(pMinTilt), _maxTilt(pMaxTilt), _rotateSpeed(pRotateSpeed), _target(pTarget), _window(pWindow), _game(game)
{
}

CameraOrbitBehaviour::~CameraOrbitBehaviour()
{
}

float _mouseX;
float _mouseY;

float angleX;
float angleY;

void CameraOrbitBehaviour::update(float pStep)
{
	#pragma region Graveyard
	//float oldPosX = _owner->getLocalPosition().x;
	//float oldPosY = _owner->getLocalPosition().y;
	//float oldPosZ = _owner->getLocalPosition().z;


	//_owner->setLocalPosition(_target->getLocalPosition() + _distance);

	//float newPosX = _owner->getLocalPosition().x;
	//float newPosZ = _owner->getLocalPosition().z;


	//angle *= _rotateSpeed;
	//float cameraXmov = targetPosX + (oldPosX - targetPosX) * cos(angle) - (oldPosZ - targetPosZ) * sin(angle);
	//float cameraZmov = targetPosZ + (oldPosX - targetPosX) * sin(angle) + (oldPosZ - targetPosZ) * cos(angle);

	//float cameraXmov = newPosX * cos(angle) - newPosZ * sin(angle);
	//float cameraZmov = newPosZ * cos(angle) + newPosX * sin(angle);

	/*glm::mat4x4 T1 = {
	1			, 0				, 0				, 0,
	0			, 1				, 0				, 0,
	0			, 0				, 1				, 0,
	-targetPosX , -targetPosY	, -targetPosZ	, 1
	};


	glm::mat4x4 TN1 = glm::mat4x4{
	1					, 0			 , 0						, 0,
	0					, 1			 , 0						, 0,
	0					, 0			 , 1						, 0,
	targetPosX 	+_distance		, targetPosY , targetPosZ + _distance			, 1
	};

	glm::mat4x4 rotateAroundMatrix = {
	1	, 0, 0			, 0,
	0	, 1, 0			, 0,
	0	, 0, 1			, 0,
	cos(angle) - sin(angle)	, cos(angle) + sin(angle), 0, 1
	};*/

	//glm::mat4 cameraMatrix = glm::translate(_target->getLocalPosition() + _distance);// *glm::rotate(clock() / 5000.0f, glm::vec3(0, 1, 0));

	//glm::translate(-_owner->getLocalPosition());


	//glm::mat4 shitTest = glm::translate(pos -_target->getWorldPosition());

	//pos -= _target->getWorldPosition();

	//glm::mat4 rotatePos = glm::rotate(shitTest, angle, glm::vec3(0, 1, 0));//glm::rotate(pos, angle, glm::vec3(0, 1, 0));
	//glm::mat4 returnToPos = glm::translate(pos + _target->getWorldPosition());
	//glm::mat4 camMat = glm::translate(_target->getLocalPosition());
	//glm::mat4 rotateMat = glm::rotate(camMat,angle, glm::vec3(0, 1, 0));
	//glm::mat4 camMatInverse = glm::translate(-_target->getLocalPosition());

	//rotateAroundMatrix *= glm::translate(glm::vec3(_distance,_distan));
	//_owner->setTransform(camMat * rotateMat * camMatInverse);
	#pragma endregion

	DualCameraFollow(_player1, _player2, pStep);
	return;

	glm::vec3 targetPos = _target->getWorldPosition();
	sf::Vector2i sfMousePos = sf::Mouse::getPosition(*_window);
	sf::Vector2u sfWindowSize = _window->getSize();
	float _scrollWheelData = 5 - (0.1f *_game->GetScrollDelta());

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

		if (sfMousePos.x > _mouseX) angleX -= _rotateSpeed;
		else if (sfMousePos.x < _mouseX) angleX += _rotateSpeed;

		if (sfMousePos.y > _mouseY) angleY -= _rotateSpeed;
		else if (sfMousePos.y < _mouseY) angleY += _rotateSpeed;
	}

	if (sfMousePos.x >= sfWindowSize.x) sfMousePos.x = 0.1f;
	else if (sfMousePos.x <= 0) sfMousePos.x = sfWindowSize.x;

	if (sfMousePos.y >= sfWindowSize.y) sfMousePos.y = 0.1f;
	else if (sfMousePos.y <= 0) sfMousePos.y = sfWindowSize.y;


	glm::clamp(angleY, _minTilt, _maxTilt);

	_mouseX = sfMousePos.x;
	_mouseY = sfMousePos.y;

	glm::vec3 pos = glm::vec3(0, 0, _distance * _scrollWheelData * 2);

	pos = glm::rotate(pos, glm::radians(angleY), glm::vec3(1, 0, 0));
	pos = glm::rotate(pos, glm::radians(angleX), glm::vec3(0, 1, 0));

	glm::mat4 posAndTarget = glm::translate(pos + targetPos);

	glm::vec3 forward = -glm::normalize(targetPos - glm::vec3(posAndTarget[3]));
	glm::vec3 side = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	glm::vec3 up = glm::normalize(glm::cross(forward, side));


	glm::mat4x4 LookAt = {
		side.x					, side.y				, side.z				, 0,
		up.x					, up.y					, up.z					, 0,
		forward.x				, forward.y				, forward.z				, 0,
		pos.x + targetPos.x  	, pos.y + targetPos.y   , pos.z + targetPos.z   , 1
	};

	_owner->setTransform(LookAt);

	//std::cout << _scrollWheelData << std::endl;
	//if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) std::cout << angleY << std::endl;
}

void CameraOrbitBehaviour::DualCameraFollow(GameObject* t1, GameObject* t2, float deltaTime)
{
	glm::vec3 t1pos = t1->getWorldPosition();
	glm::vec3 t2pos = t2->getWorldPosition();

	// Get the distance between objects
	glm::vec3 distanceVector = t1pos - t2pos;

	// Midpoint of the distance
	glm::vec3 midpoint = t1pos - distanceVector * 0.5f;


	//glm::vec3 targetPos = _target->getWorldPosition();

	glm::vec3 pos = glm::vec3(0, 0, 15 * _zoomFactor);

	pos = glm::rotate(pos, glm::radians(angleY), glm::vec3(1, 0, 0));
	pos = glm::rotate(pos, glm::radians(angleX), glm::vec3(0, 1, 0));

	glm::mat4 posAndTarget = glm::translate(pos + midpoint);

	glm::vec3 forward = -glm::normalize(midpoint - glm::vec3(posAndTarget[3]));

	// Center the camera on the midpoint and zoom to the startOffset distance
	glm::vec3 cameraDestination = midpoint - forward * _startOffset;

	// Start zooming the camera if distance is bigger than X
	if (distanceVector.length() > _zoomBounds.x)
	{
		_zoomDistance = glm::clamp((distanceVector.length() - _zoomBounds.x), 0.0f, _zoomBounds.y);
		cameraDestination -= forward * _zoomDistance * _zoomFactor;
	}
	// Stop updating camera position if zoomed to the max
	if (_zoomDistance < _zoomBounds.y) _newCamPos = cameraDestination;

	// Update the position of the camera
	_owner->setLocalPosition(glm::slerp(_owner->getLocalPosition(), _newCamPos, _followTimeDelta * deltaTime));

	// Snap when close enough to prevent annoying slerp behavior
	if ((_newCamPos - _owner->getLocalPosition()).length() <= 0.01f)
		_owner->setLocalPosition(_newCamPos);
}