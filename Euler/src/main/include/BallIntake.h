#pragma once

using namespace frc;

class BallIntake {
	public:
	BallIntake(frc::XboxController *xbox);
    BallIntake(frc::XboxController *xbox, int ballCount);
	~BallIntake();
    void Tick();
	void Reinit();
	void StartIntake();
	void StopIntake();
	void Yeet();
	int GetBallCount();
	
	private:
	WPI_TalonSRX *m_intake, *m_conveyor;
	DigitalInput *m_pickupSensor, *m_releaseSensor;
	XboxController *m_xbox;
	frc::DoubleSolenoid *intake_solenoid;

    int ballCount, pickupPhase;
    bool triggerHeld, triggerOn, eject, seeBall;

	void init(frc::XboxController *xbox, int ballCount);
};