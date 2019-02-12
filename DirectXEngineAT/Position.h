#pragma once
#include <math.h>

class Position
{
public:
	Position();
	Position(const Position&);
	~Position();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);

	void SetFrameTime(float);

	void MoveForward();
	void MoveBackward();
	void MoveUpward();
	void MoveDownward();
	void TurnLeft();
	void TurnRight();
	void LookUpward();
	void LookDownward();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	float m_frameTime;

	float m_forwardSpeed, m_backwardSpeed;
	float m_upwardSpeed, m_downwardSpeed;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_lookUpSpeed, m_lookDownSpeed;
};