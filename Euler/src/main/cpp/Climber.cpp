#include "Euler.h"

Climber::Climber(frc::XboxController *xbox, frc::DoubleSolenoid *climb_solenoid)
{
    init(xbox, climb_solenoid);
}

void
Climber:: init(frc::XboxController *xbox, DoubleSolenoid *climb_solenoid)
{
    m_xbox = xbox;
	m_trolley = new WPI_TalonSRX(TROLLEY);
    if(m_trolley == NULL)
        std::bad_alloc();
    m_trolley->ConfigFactoryDefault();
    m_lift = new WPI_TalonSRX(LIFT);
    if(m_lift == NULL)
        std::bad_alloc();
    m_lift->ConfigFactoryDefault();
	m_ratchet_solenoid = new frc::Solenoid(PCM, RATCHET_LOCK);
    if(m_ratchet_solenoid == NULL)
        std::bad_alloc();
    m_ratchet_solenoid->Set(false);
	m_climb_solenoid = climb_solenoid;
    if(m_climb_solenoid == NULL)
        std::bad_alloc();
    m_climb_solenoid->Set(frc::DoubleSolenoid::kReverse);
}

Climber::~Climber()
{
    free(m_trolley);
    free(m_lift);
    free(m_ratchet_solenoid);
    free(m_climb_solenoid);
}

void
Climber::Reinit()
{
    m_climb_solenoid->Set(frc::DoubleSolenoid::kReverse);
    m_ratchet_solenoid->Set(true); 
}

void 
Climber::DisabledInit()
{
    m_ratchet_solenoid->Set(false); 
}

void
Climber::Tick()
{
    frc::SmartDashboard::PutBoolean("ratchet solenoid:", m_ratchet_solenoid->Get());
    if (m_xbox->GetBackButtonPressed()) {
        if (m_climb_solenoid->Get() == frc::DoubleSolenoid::kReverse) {
            m_climb_solenoid->Set(frc::DoubleSolenoid::kForward);
            m_ratchet_solenoid->Set(true);
        } else if (m_climb_solenoid->Get() == frc::DoubleSolenoid::kForward)
            m_climb_solenoid->Set(frc::DoubleSolenoid::kOff); //do not set back to kReverse or arm crashes down
    }

    if (m_xbox->GetBButtonPressed()) {
        m_ratchet_solenoid->Set(!m_ratchet_solenoid->Get());
    }

    if (m_climb_solenoid->Get() == frc::DoubleSolenoid::kForward) {
        double motorSpeed = m_xbox->GetY(frc::GenericHID::kLeftHand) * -1;
        m_ratchet_solenoid->Set(motorSpeed > 0); 
        m_lift->Set(motorSpeed);
    } else {
        m_lift->Set(0.0);
    }

    if (m_climb_solenoid->Get() == frc::DoubleSolenoid::kForward) {
        m_trolley->Set(m_xbox->GetX(frc::GenericHID::kRightHand) * -1);
    } else {
        m_trolley->Set(0.0);
    }
}

void
Climber::DisengageRatchet()
{
    m_ratchet_solenoid->Set(true);
}

void
Climber::EngageRatchet()
{
    m_ratchet_solenoid->Set(true);
}