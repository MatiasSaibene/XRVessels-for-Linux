/**
  XR Vessel add-ons for OpenOrbiter Space Flight Simulator
  Copyright (C) 2006-2025 Douglas Beachy

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

  Email: mailto:doug.beachy@outlook.com
  Web: https://www.alteaaerospace.com
**/

// ==============================================================
// XR5Vanguard implementation class
//
// XR5Payload.cpp
// Class defining the XR5's payload bay
// ==============================================================

#pragma once

#include "XR5Vanguard.h"
#include "XR1PayloadBay.h"  // cannot use forward reference here!  Otherwise inline code using XR5Vanguard object is generated incorrectly.

class XR5PayloadBay : public XR1PayloadBay 
{
public:
    XR5PayloadBay(VESSEL &parentVessel);

    VECTOR3 GetLandedDeployToCoords(const int slotNumber);
    XR5Vanguard &GetXR5() const { return static_cast<XR5Vanguard &>(GetParentVessel()); }
};
