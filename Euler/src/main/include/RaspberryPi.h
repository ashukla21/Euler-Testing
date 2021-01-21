  
#pragma once

#include "Euler.h"

using namespace std;
using namespace frc;

#define angleOffsetCoefficient	0.01

class RaspberryPi {
	public:
    RaspberryPi(DalekDrive *drive);

	bool FollowBall(); // returns false if ball not found [maybe change this to print to dashboard, then return if we can pick up a ball]
	bool driveAdjusted(double offset, double distance, double coefficient);
	bool driveAdjusted(double offset, double distance, double coefficient, bool inverted);

	private:
    DalekDrive *m_drive;
};