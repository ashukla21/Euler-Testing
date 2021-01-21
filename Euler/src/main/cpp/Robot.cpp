#include "Euler.h"

using namespace frc;

void Robot::RobotInit() 
{
  cs::AxisCamera camera = CameraServer::GetInstance()->AddAxisCamera(FORWARD_CAMERA);  // Initialize Camera
  // camera.SetResolution(160, 90);    // Only use these two lines if needed
  // camera.SetFPS(15);
  try {
    m_xbox        = new frc::XboxController(XBOX);
    m_leftStick   = new frc::Joystick(LEFT_JOY);
    m_rightStick  = new frc::Joystick(RIGHT_JOY);
    m_climb_solenoid = new frc::DoubleSolenoid(PCM, CLIMB_DEPLOY, CLIMB_EXHAUST);
    m_drive       = new DalekDrive(LEFT_FRONT_DRIVE, LEFT_REAR_DRIVE, RIGHT_FRONT_DRIVE, RIGHT_REAR_DRIVE, DalekDrive::driveType::kDifferential);
    m_ahrs        = new AHRS(SPI::Port::kMXP);
    m_pi          = new RaspberryPi(m_drive);
    m_compressor  = new frc::Compressor(PCM);
	m_ballIntake  = new BallIntake(m_xbox);
    m_auton       = new Auton(m_drive, m_ahrs, m_pi, m_ballIntake);
    m_climber     = new Climber(m_xbox,m_climb_solenoid);
    m_spinner     = new Spinner(m_xbox,m_climb_solenoid);
    m_limelight   = new Limelight(m_drive);
  }
  catch (std::exception& e) {
    std::string err_string = "Error instantiating components:  ";
    err_string += e.what();
    DriverStation::ReportError(err_string.c_str());
  }
  frc::SmartDashboard::PutNumber("Start Auton", 2);
  frc::SmartDashboard::PutNumber("End Auton", 2);
  frc::SmartDashboard::PutNumber("Delay", 0);
  frc::SmartDashboard::PutNumber("Delay Phase", 0);
  frc::SmartDashboard::PutNumber("Auton Phase", 0);
  frc::SmartDashboard::PutBoolean("Pickup Ball End", false);
  frc::SmartDashboard::PutBoolean("Pickup Ball Start", false);
  //frc::SmartDashboard::PutNumber("Starting # of Balls", 3);

  m_ahrs->ZeroYaw();
  m_ahrs->Reset();
  m_ahrs->ResetDisplacement();
  m_compressor->Start();
}

void Robot::RobotPeriodic()
{

}

// I think I have some errors here, I wanna test this
void Robot::AutonomousInit()
{
  auton_start = 2;//(int)frc::SmartDashboard::GetData("Start Auton");
  auton_end =   1;//(int)frc::SmartDashboard::GetData("End Auton");
  waitSeconds = 0;//(int)frc::SmartDashboard::GetData("Delay");
  m_auton->AutonCase((int)auton_start, (int)auton_end); // the parameters change based on what auton sequence we are going to use
  m_climber->Reinit();
  m_spinner->Reinit();
  m_ballIntake->Reinit();
}

void Robot::AutonomousPeriodic() 
{
	double period = (double)this->GetPeriod();
	waitSeconds += period;
	if (waitSeconds >= 0) { // the number 0 change based on how long we want to wait in the auton sequence
		m_auton->AutonDrive(period);
	}
}

void Robot::TeleopInit()
{
   m_climber->Reinit();
   m_spinner->Reinit();
   m_ballIntake->Reinit();
}

void Robot::TeleopPeriodic()
{
    if (m_drive) {
	  if (m_rightStick->GetTrigger() || m_leftStick->GetTrigger()) { // JUST FOR TESTING
		m_pi->FollowBall();
      } else {
        m_drive->TankDrive(m_leftStick, m_rightStick, true);
        frc::SmartDashboard::PutBoolean("start button pressed", false);
      }
    }


	m_ballIntake->Tick();  
  m_spinner->Tick();
  m_climber->Tick();


}

void Robot::TestInit()
{

}

void Robot::TestPeriodic()
{

}

void Robot::DisabledInit()
{
  m_climber->DisabledInit();
}


#ifndef RUNNING_FRC_TESTS
int main() { return StartRobot<Robot>(); }
#endif
