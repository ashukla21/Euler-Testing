#include "Euler.h"

RaspberryPi::RaspberryPi(DalekDrive *drive) {
    m_drive			= drive;
}

bool RaspberryPi::FollowBall()
{
	int offset = frc::SmartDashboard::GetNumber("X Offset", 100000), distance = frc::SmartDashboard::GetNumber("Distance", -1);
	if (offset == 100000 || distance == -1) {
		m_drive->TankDrive(0.0, 0.0, false);
		SmartDashboard::PutBoolean("camera sees ball?", false);
		return false;
	}
	SmartDashboard::PutBoolean("camera sees ball?", true);
    return driveAdjusted(offset, distance, pixelOffsetCoefficient);
}


bool RaspberryPi::driveAdjusted(double offset, double distance, double coefficient)
{
<<<<<<< HEAD
	double offset = fmod(angle - (m_ahrs->GetAngle()) * PI / 180, 2 * PI);
	//Josh's function = fmod(angle - (m_ahrs->GetAngle()) * PI / 180, 360)
	if (offset > PI) {
		offset -= PI * 2;
	}
	if (math.abs(offset)<angleError){
		return offset;
	}else if (offset>0){
		return offset-2*(angle - (m_ahrs->GetAngle()))
	}
	
=======
	return driveAdjusted(offset, distance, coefficient, false);
>>>>>>> 7634aa5155ce80c241f9802c9c7e81d39ff474b9
}

bool RaspberryPi::driveAdjusted(double offset, double distance, double coefficient, bool inverted)
{
	if (distance < distanceErrorThreshold) {
		m_drive->TankDrive(0.0, 0.0, false);
		return true;
	}
	double ratio = exp(coefficient * offset / distance), magnatude = (distance + .3) / 8.0; // this needs to be changed
	// seans formula ->  = -.4 / (1 + 4 * exp(-distanceCoefficient * distance));
	if (magnatude > .5) {
		magnatude = .5;
	}
	if (!inverted) {
		magnatude *= -1;
	}
	if (ratio < 1) {
		m_drive->TankDrive(magnatude, magnatude * ratio, false);
	} else {
		m_drive->TankDrive(magnatude / ratio, magnatude, false);
	}
	return false;
}