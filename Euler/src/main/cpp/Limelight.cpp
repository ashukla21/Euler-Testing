#include "Euler.h"

Limelight::Limelight(DalekDrive *drive) {
    m_drive = drive;
}

void Limelight::Update() {
    // SmartDashboard::PutNumber("tv", nt::NetworkTableInstance::GetDefault().GetTable("limelight")->GetNumber("tv", 0.0));
    // SmartDashboard::PutNumber("tshort", nt::NetworkTableInstance::GetDefault().GetTable("limelight")->GetNumber("tshort", 0.0));
	// SmartDashboard::PutNumber("thor", nt::NetworkTableInstance::GetDefault().GetTable("limelight")->GetNumber("thor", 0.0));
    SmartDashboard::PutNumber("tx", nt::NetworkTableInstance::GetDefault().GetTable("limelight")->GetNumber("tx", 0.0));
	SmartDashboard::PutNumber("tvert", nt::NetworkTableInstance::GetDefault().GetTable("limelight")->GetNumber("tvert", 0.0));

	// set a variable distance using a linear regression of ax+b using tvert on desmos
}

void Limelight::LightOff() {
	nt::NetworkTableInstance::GetDefault().GetTable("limelight")->PutNumber("ledMode", 1);
}

void Limelight::LightOn() {
	nt::NetworkTableInstance::GetDefault().GetTable("limelight")->PutNumber("ledMode", 3);
}
