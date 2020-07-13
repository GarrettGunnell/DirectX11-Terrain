#pragma once

#include <math.h>

class Position {
public:
	Position();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);

	void SetFrameTime(float);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void StrafeLeft(bool);
	void StrafeRight(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);

private:
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;

	float frameTime;

	float forwardSpeed, backwardSpeed;
	float upwardSpeed, downwardSpeed;
	float leftSpeed, rightSpeed;
	float leftTurnSpeed, rightTurnSpeed;
	float lookUpSpeed, lookDownSpeed;
};