#include "Euler.h"

BallIntake::BallIntake(frc::XboxController *xbox, int ballCount)
{
    init(xbox, ballCount);
}

BallIntake::BallIntake(frc::XboxController *xbox)
{
    init(xbox, 0);
}

void
BallIntake:: init(frc::XboxController *xbox, int ballCount)
{
	m_conveyor = new WPI_TalonSRX(CONVEYOR_BELT);
	if(m_conveyor == NULL)
		std::bad_alloc();
	m_conveyor->ConfigFactoryDefault();
    m_pickupSensor = new frc::DigitalInput(CONVEYOR_INPUT);
	if(m_pickupSensor == NULL)
		std::bad_alloc();
    m_releaseSensor = new frc::DigitalInput(CONVEYOR_STOP);
	if(m_releaseSensor == NULL)
		std::bad_alloc();
    m_intake = new WPI_TalonSRX(ROLLER_BAR);
	if(m_intake == NULL)
		std::bad_alloc();
	m_intake->ConfigFactoryDefault();
    //ballCount = (int)frc::SmartDashboard::GetData("Starting # of Balls");
	intake_solenoid = new frc::DoubleSolenoid(PCM, INTAKE_DEPLOY,INTAKE_EXHAUST);
	if(intake_solenoid == NULL)
		std::bad_alloc();
    intake_solenoid->Set(frc::DoubleSolenoid::kReverse);
    m_xbox = xbox;
    triggerHeld = false;
	triggerOn = false;
	eject = false;
	seeBall = false;
    this->ballCount = ballCount;
    pickupPhase = 0;
}

BallIntake::~BallIntake()
{
    free(m_conveyor);
    free(m_pickupSensor);
    free(m_releaseSensor);
    free(m_intake);
}

void BallIntake::Reinit()
{
	m_conveyor->Set(0);
	m_intake->Set(0);
    intake_solenoid->Set(frc::DoubleSolenoid::kReverse);
}

void
BallIntake::StartIntake()
{
	triggerOn = true;
}

void
BallIntake::StopIntake()
{
	triggerOn = false;
}

int
BallIntake::GetBallCount()
{
	return ballCount;
}

void
BallIntake::Yeet() {
	eject = true;
}

void
BallIntake::Tick()
{
	SmartDashboard::PutBoolean("Input Sensed", m_pickupSensor->Get());
	SmartDashboard::PutBoolean("Output Sensed", m_releaseSensor->Get());
	SmartDashboard::PutNumber("ballCount", GetBallCount());
	
	if (m_xbox->GetAButtonPressed()) {
    	if (intake_solenoid->Get() == frc::DoubleSolenoid::kForward){
      		intake_solenoid->Set(frc::DoubleSolenoid::kReverse);
    	} else {
      		intake_solenoid->Set(frc::DoubleSolenoid::kForward);
    	}
 	}
	if (m_xbox->GetBumper(frc::GenericHID::kRightHand)) {
		m_conveyor->Set(-1);
		m_intake->Set(-1);
	}
	else if (m_xbox->GetBumper(frc::GenericHID::kLeftHand) || eject) {
		pickupPhase = 0;
		// if (seeBall && m_releaseSensor->Get() && ballCount > 0) {
		// 	ballCount--;
		// }
		if (eject && ballCount == 0) {
			m_conveyor->Set(0);
			m_intake->Set(0);
			eject = false;
		} else {
			m_conveyor->Set(1);
			m_intake->Set(1);
		}
	}
	else {
    	if (m_xbox->GetTriggerAxis(frc::GenericHID::kRightHand)) {
			if (!triggerHeld) {
				triggerOn = !triggerOn;
			}
			triggerHeld = true;
  		} else {
			triggerHeld = false;
		}

    	if (triggerOn && m_releaseSensor->Get()) {
        	switch (pickupPhase) {
        	    case 0:
	    	        if (!m_pickupSensor->Get()) {
        	            ballCount++;
        	            pickupPhase++;
						m_conveyor->Set(0.5 + .0 * ballCount);
						m_intake->Set(0);
		            } else {
						intake_solenoid->Set(frc::DoubleSolenoid::kForward);
						m_intake->Set(0.5);
						m_conveyor->Set(0);
					}
            	    break;
            	case 1:
            	    if (m_pickupSensor->Get()) {
     		            pickupPhase++;
						m_conveyor->Set(0);
						m_intake->Set(0);
    	            } else {
						m_conveyor->Set(0.5 + .075 * ballCount);
						m_intake->Set(0);
					}
    	            break;
				case 2:
					m_conveyor->Set(0);
					m_intake->Set(0);
					triggerOn = false;
					break;
    	    }
    	} else {
			pickupPhase = 0;
			m_conveyor->Set(0);
			if (triggerOn) {
				m_intake->Set(0.5);
			} else {
				m_intake->Set(0);
			}
    	}
	}
	if (seeBall && m_releaseSensor->Get() && ballCount > 0) {
		ballCount--;
	}
	seeBall = !m_releaseSensor->Get();
}