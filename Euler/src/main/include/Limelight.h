#pragma once

#include "Euler.h"

class Limelight {
    public:
        Limelight(DalekDrive *drive);
        void Update();
		void LightOff();
		void LightOn();
    private:
        DalekDrive *m_drive;
};