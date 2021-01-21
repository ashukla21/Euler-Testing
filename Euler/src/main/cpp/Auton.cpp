#include "Euler.h"

Auton::Auton(DalekDrive *drive, AHRS * ahrs, RaspberryPi *pi, BallIntake *ballIntake)
{
	m_drive			= drive;
	m_ahrs			= ahrs;
	m_pi            = pi; 
	m_ballIntake    = ballIntake;

	auton_phase = 0;
	pickupBallStart = frc::SmartDashboard::GetData("Pickup Ball Start");
	pickupBallEnd   = frc::SmartDashboard::GetData("Pickup Ball End");

	p_temp = 0; i_temp = 0; d_temp = 0;
}

bool Auton::AutonOrginXY (double x, double y) {
	//Orgin is the (in the adiences persective) in the bottom left corner (which is blocked by an angled wall)
	// 
	//1/(1+e^atan((x-currentX)/(y-currentY)))
	
	m_drive->tankDrive( , ,false);
	return false
}

void Auton::AutonCase(int begin, int end)
{
	switch (begin) {
		case 1: //right sensor
		enter_target_x = -2; //need sensor
		break;
		case 2: //straight on
		enter_target_x = 0;
		break;
		case 3: //left sensor
		enter_target_x = 5; //need sensor
		break;
	}

	enter_target_y = lineToWall;
	
	switch (end) {
		case 1: //Our trench
			exit_target_x = -2.009775;
			exit_target_y = 3.048;
			break;
		case 2: //Shield Generator
			exit_target_x = -3.17931851816;
			exit_target_y = -5.248275;
			break;
		case 3: //Enemy trench
			exit_target_x = 3.851275;
			exit_target_y = 5.248275;
			break;
	}

	enter_target_ang = atan2(enter_target_y, enter_target_x);
	exit_target_ang  = atan2(exit_target_y, exit_target_x);
	enter_target_dist = sqrt(enter_target_x * enter_target_x + enter_target_y * enter_target_y);
	exit_target_dist = sqrt(exit_target_x * exit_target_x + exit_target_y * exit_target_y);
	//here set up what happens at the end (after delivery)
}

void Auton::AutonDrive(double period)
{
	// if (!m_ahrs->IsCalibrating()) {
		switch (auton_phase) {
			case 0: // turn to target
			if (turnToFace(enter_target_ang)) {
				auton_phase++;
				travelled_dist = 0;
			}
			break;
			case 1: // drive to target
			if (SmartDashboard::GetBoolean("Limelight Found", false) || driveToCoordinates(enter_target_x, enter_target_y, enter_target_ang, period)) { // dashboard not in yet
				auton_phase++;
			}
			break;
			case 2: // turn straight
			// THIS IS LIMELIGHT-CONTROLLED MOVEMENT 
			break;
			case 3: // drive to wall
			if (driveToCoordinates(0, .5, PI / 2, period)) { // THIS SHOULD BE DRIVING AFTER LIMELIGHT DOESN'T SENSE ANYMORE
				auton_phase++;
			}
			break;
			case 4: //delivers balls
			m_ballIntake->Yeet();
			if (m_ballIntake->GetBallCount() == 0) {
				auton_phase++;
				travelled_dist = 0;
			}
			break;
			case 5: //give us a little space to turn around (can be lowered)
			travelled_dist += m_drive->GetVelocity() * period;
			m_drive->TankDrive(-.5, -.5, false);
			if (travelled_dist < -.5) { // guess
				auton_phase++;
			}
			break;
			case 6: //turn around
			if (turnToFace(exit_target_ang)) {
				auton_phase++;
				travelled_dist = 0;
			}
			break;		
			case 7: //drive towards exit
			if (driveToCoordinates(exit_target_x, exit_target_y, exit_target_ang, period)) {
				auton_phase++;
			}
			break;
			case 8: //collect balls if warrented
			//I don't know if the followBall will work the way I put it
			if (m_pi->FollowBall()) {
				//Currently not in code
				m_ballIntake->StartIntake();
				if (m_ballIntake->GetBallCount() == 3)
					auton_phase++;
			}
			break;
			// default: //end
			// 	//return true;
			// 	break;
			// add more once we get there
		}
	// }
	
	frc::SmartDashboard::PutNumber ("Auton Phase", auton_phase);
	//frc::SmartDashboard::PutNumber("enterance angle offset", angleOffset(enter_target_ang) * 180 / PI);
	//frc::SmartDashboard::PutNumber("exit angle offset", angleOffset(exit_target_ang) * 180 / PI);
	frc::SmartDashboard::PutNumber("ahrs_ang", m_ahrs->GetAngle() + 90);
	frc::SmartDashboard::PutNumber("enter_target_ang", enter_target_ang);
	frc::SmartDashboard::PutNumber("exit_target_ang", exit_target_ang);
	frc::SmartDashboard::PutNumber("enter_target_x", enter_target_x);
	frc::SmartDashboard::PutNumber("exit_target_x", exit_target_x);
	frc::SmartDashboard::PutNumber("enter_target_y", enter_target_y);
	frc::SmartDashboard::PutNumber("exit_target_y", exit_target_y);
	frc::SmartDashboard::PutNumber("displ_x", m_ahrs->GetDisplacementX());
	frc::SmartDashboard::PutNumber("displ_y", m_ahrs->GetDisplacementY());
	frc::SmartDashboard::PutNumber("enter dist offset", sqrt(pow(enter_target_x - m_ahrs->GetDisplacementX(), 2) + pow(enter_target_y - m_ahrs->GetDisplacementY(), 2)));
	frc::SmartDashboard::PutNumber("exit dist offset", sqrt(pow(exit_target_x - m_ahrs->GetDisplacementX(), 2) + pow(exit_target_y - m_ahrs->GetDisplacementY(), 2)));
}

bool Auton::driveToCoordinates(double x, double y, double angle, double period)
{
	travelled_dist += m_drive->GetVelocity() * period;
	SmartDashboard::PutNumber("velocity", m_drive->GetVelocity());
	SmartDashboard::PutNumber("period", period);
	SmartDashboard::PutNumber("sensesd travel", travelled_dist);
	SmartDashboard::PutNumber("angle offset", angleOffset(angle) * 180 / PI); // i think angle offset is wack
	SmartDashboard::PutNumber("dist offset", sqrt(x*x + y*y) - travelled_dist); // this is not correct currently
	return m_pi->driveAdjusted(angleOffset(angle), sqrt(x*x + y*y) - travelled_dist, angleOffsetCoefficient);
}


bool Auton::turnToFace(double angle)
{
	double prev_error = p_temp;
	p_temp = angleOffset(angle);
	if (abs(p_temp) < turningErrorThreshold) {
		return true;
	}
	i_temp += p_temp;
	d_temp = p_temp - prev_error;
	double pid_result = pTurn * p_temp + iTurn * i_temp + dTurn * d_temp;
	if (pid_result > 0) {
		m_drive->TankDrive(min(pid_result, maxTurnSpeed), -min(pid_result, maxTurnSpeed), false);
	} else {
		m_drive->TankDrive(max(pid_result, -maxTurnSpeed), -max(pid_result, -maxTurnSpeed), false);
	}
	return false;
}

<<<<<<< HEAD
bool Auton::driveToCoordinates(double x, double y)
{
	SmartDashboard::PutNumber("angle offset", m_pi->angleOffset(atan(y/x)) * 180 / PI);
	SmartDashboard::PutNumber("dist offset", sqrt(pow(x - m_ahrs->GetDisplacementX(), 2) + pow(y - m_ahrs->GetDisplacementY(), 2))); // this is not correct currently
	return m_pi->driveAdjusted(m_pi->angleOffset(angle), sqrt(pow(x - m_ahrs->GetDisplacementX(), 2) + pow(y - m_ahrs->GetDisplacementY(), 2)), angleOffsetCoefficient);
}

double Auton::angleOffset(double angle)
{
	SmartDashboard::PutNumber("ahrs ang", m_ahrs->GetAngle());
	double offset = fmod(angle - (m_ahrs->GetAngle()) * PI / 180, 2 * PI);
	//Josh's function = fmod(angle - (m_ahrs->GetAngle()) * PI / 180, 360)
	if (offset > PI) {
		offset -= PI * 2;
	}
	return offset;
} 
>>>>>>> 7634aa5155ce80c241f9802c9c7e81d39ff474b9
