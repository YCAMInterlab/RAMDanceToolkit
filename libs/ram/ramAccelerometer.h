#pragma once

#include "ofMain.h"
#include "ramActor.h"


class ramAccelerometer
{
	
public:
	void setup()
	{
		acceleration.set(0, 0, 0);
		angular_velocity.set(0, 0, 0, 1);
		last_pos.set(0, 0, 0);
		last_rot.set(0, 0, 0, 1);
	}
	
	void update(ramNode &node)
	{
		velocity = last_pos - node.getPosition();
		last_pos = node.getPosition();

		acceleration = last_velocity - velocity;
		last_velocity = velocity;

		angular_velocity = last_rot.inverse() * node.getOrientationQuat();
		last_rot = node.getOrientationQuat();

		angular_acceleration = last_angular_velocity.inverse() * angular_velocity;
		last_angular_velocity = angular_velocity;
	}
	
	inline ofVec3f getVelocity() { return velocity; }
	inline ofVec3f getAcceleration() { return acceleration; }

	inline ofQuaternion getAngularVelocity() { return angular_velocity; }
	inline ofQuaternion getAngularAcceleration() { return angular_acceleration; }
	
private:
	ofVec3f velocity, last_velocity, acceleration;
	ofQuaternion angular_velocity, last_angular_velocity, angular_acceleration;
	
	ofVec3f last_pos;
	ofQuaternion last_rot;
};
