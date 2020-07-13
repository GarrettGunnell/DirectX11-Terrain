#include "Position.h"

Position::Position() {
	positionX = 0.0f;
	positionY = 0.0f;
	positionZ = 0.0f;

	rotationX = 0.0f;
	rotationY = 0.0f;
	rotationZ = 0.0f;

	frameTime = 0.0f;

	forwardSpeed = 0.0f;
	backwardSpeed = 0.0f;
	upwardSpeed = 0.0f;
	downwardSpeed = 0.0f;
	leftTurnSpeed = 0.0f;
	rightTurnSpeed = 0.0f;
	lookUpSpeed = 0.0f;
	lookDownSpeed = 0.0f;
}

void Position::SetPosition(float x, float y, float z) {
	positionX = x;
	positionY = y;
	positionZ = z;
}

void Position::SetRotation(float x, float y, float z) {
	rotationX = x;
	rotationY = y;
	rotationZ = z;
}

void Position::GetPosition(float& x, float& y, float& z) {
	x = positionX;
	y = positionY;
	z = positionZ;
	return;
}


void Position::GetRotation(float& x, float& y, float& z) {
	x = rotationX;
	y = rotationY;
	z = rotationZ;
	return;
}

void Position::SetFrameTime(float time) {
	frameTime = time;
}

void Position::MoveForward(bool keydown) {
	float radians;

	if (keydown) {
		forwardSpeed += frameTime * 1.0f;
		if (forwardSpeed > (frameTime * 50.0f)) {
			forwardSpeed = frameTime * 50.0f;
		}
	}
	else {
		forwardSpeed -= frameTime * 0.5f;

		if (forwardSpeed < 0.0f) {
			forwardSpeed = 0.0f;
		}
	}

	radians = rotationY * 0.0174532925f;

	positionX += sinf(radians) * forwardSpeed;
	positionZ += cosf(radians) * forwardSpeed;
}

void Position::MoveBackward(bool keydown) {
	float radians;

	if (keydown) {
		backwardSpeed += frameTime * 1.0f;

		if (backwardSpeed > (frameTime * 50.0f)) {
			backwardSpeed = frameTime * 50.0f;
		}
	}
	else {
		backwardSpeed -= frameTime * 0.5f;

		if (backwardSpeed < 0.0f) {
			backwardSpeed = 0.0f;
		}
	}
	
	radians = rotationY * 0.0174532925f;

	positionX -= sinf(radians) * backwardSpeed;
	positionZ -= cosf(radians) * backwardSpeed;
}

void Position::MoveUpward(bool keydown) {
	if (keydown) {
		upwardSpeed += frameTime * 1.5f;

		if (upwardSpeed > (frameTime * 15.0f)) {
			upwardSpeed = frameTime * 15.0f;
		}
	}
	else {
		upwardSpeed -= frameTime * 0.5f;

		if (upwardSpeed < 0.0f) {
			upwardSpeed = 0.0f;
		}
	}
	
	positionY += upwardSpeed;
}

void Position::MoveDownward(bool keydown) {
	if (keydown) {
		downwardSpeed += frameTime * 1.5f;

		if (downwardSpeed > (frameTime * 15.0f)) {
			downwardSpeed = frameTime * 15.0f;
		}
	}
	else {
		downwardSpeed -= frameTime * 0.5f;

		if (downwardSpeed < 0.0f) {
			downwardSpeed = 0.0f;
		}
	}
	
	positionY -= downwardSpeed;
}

void Position::StrafeLeft(bool keydown) {
	float radians;
	if (keydown) {
		leftSpeed += frameTime * 1.5f;

		if (leftSpeed > (frameTime * 15.0f)) {
			leftSpeed = frameTime * 15.0f;
		}
	}
	else {
		leftSpeed -= frameTime * 0.5f;

		if (leftSpeed < 0.0f) {
			leftSpeed = 0.0f;
		}
	}


	radians = rotationY * 0.0174532925f;

	positionX -= cosf(radians) * leftSpeed;
	positionZ += sinf(radians) * leftSpeed;
}

void Position::StrafeRight(bool keydown) {
	float radians;
	
	if (keydown) {
		rightSpeed += frameTime * 1.5f;

		if (rightSpeed > (frameTime * 15.0f)) {
			rightSpeed = frameTime * 15.0f;
		}
	}
	else {
		rightSpeed -= frameTime * 0.5f;

		if (rightSpeed < 0.0f) {
			rightSpeed = 0.0f;
		}
	}
	
	radians = rotationY * 0.0174532925f;

	positionX += cosf(radians) * rightSpeed;
	positionZ -= sinf(radians) * rightSpeed;
}

void Position::TurnLeft(bool keydown) {
	if (keydown) {
		leftTurnSpeed += frameTime * 5.0f;

		if (leftTurnSpeed > (frameTime * 150.0f)) {
			leftTurnSpeed = frameTime * 150.0f;
		}
	}
	else {
		leftTurnSpeed -= frameTime * 3.5f;

		if (leftTurnSpeed < 0.0f) {
			leftTurnSpeed = 0.0f;
		}
	}

	rotationY -= leftTurnSpeed;

	if (rotationY < 0.0f) {
		rotationY += 360.0f;
	}
}

void Position::TurnRight(bool keydown) {
	if (keydown) {
		rightTurnSpeed += frameTime * 5.0f;

		if (rightTurnSpeed > (frameTime * 150.0f)) {
			rightTurnSpeed = frameTime * 150.0f;
		}
	}
	else {
		rightTurnSpeed -= frameTime * 3.5f;

		if (rightTurnSpeed < 0.0f) {
			rightTurnSpeed = 0.0f;
		}
	}

	rotationY += rightTurnSpeed;

	if (rotationY > 360.0f) {
		rotationY -= 360.0f;
	}
}

void Position::LookUpward(bool keydown) {
	if (keydown) {
		lookUpSpeed += frameTime * 7.5f;

		if (lookUpSpeed > (frameTime * 75.0f)) {
			lookUpSpeed = frameTime * 75.0f;
		}
	}
	else {
		lookUpSpeed -= frameTime * 2.0f;

		if (lookUpSpeed < 0.0f) {
			lookUpSpeed = 0.0f;
		}
	}

	rotationX -= lookUpSpeed;

	if (rotationX > 90.0f) {
		rotationX = 90.0f;
	}
}

void Position::LookDownward(bool keydown) {
	if (keydown) {
		lookDownSpeed += frameTime * 7.5f;

		if (lookDownSpeed > (frameTime * 75.0f)) {
			lookDownSpeed = frameTime * 75.0f;
		}
	}
	else {
		lookDownSpeed -= frameTime * 2.0f;

		if (lookDownSpeed < 0.0f) {
			lookDownSpeed = 0.0f;
		}
	}

	rotationX += lookDownSpeed;

	if (rotationX < -90.0f) {
		rotationX = -90.0f;
	}
}