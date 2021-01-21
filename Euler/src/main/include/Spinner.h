#pragma once

using namespace frc;

#define COLOR_WHEEL_RADIUS			16.0
#define SPINNER_WHEEL_RADIUS		2.0
#define WHEEL_RATIO                 ((COLOR_WHEEL_RADIUS)/(SPINNER_WHEEL_RADIUS))
#define ENCODER_TICKS_PER_REV		4096
#define NUM_TICKS_PER_COLOR_WHEEL_REV	((WHEEL_RATIO)*(ENCODER_TICKS_PER_REV))

class Spinner {
	public:
	Spinner(frc::XboxController *xbox, frc::DoubleSolenoid *climb_solenoid);
	~Spinner();
    void Tick();
	void Reinit();

	private:
	WPI_TalonSRX *m_spinner;
	XboxController *m_xbox;
   	frc::DoubleSolenoid *m_spinner_solenoid, *m_climb_solenoid;

	void init(frc::XboxController *xbox, frc::DoubleSolenoid *climb_solenoid);
};