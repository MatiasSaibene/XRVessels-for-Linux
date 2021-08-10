// ==============================================================
// XR3Phoenix implementation class
//
// Copyright 2007-2016 Douglas E. Beachy
// All rights reserved.
//
// This software is FREEWARE and may not be sold!
//
// XR3Areas.cpp
// Contains new areas for the XR3.
// ==============================================================

#include "XR3Phoenix.h"
#include "AreaIDs.h"
#include "XR3Areas.h"

//----------------------------------------------------------------------------------

RCSDockingModeButtonArea::RCSDockingModeButtonArea(InstrumentPanel &parentPanel, const COORD2 panelCoordinates, const int areaID) :
    XR1Area(parentPanel, panelCoordinates, areaID)
{
}

void RCSDockingModeButtonArea::Activate()
{
    Area::Activate();  // invoke superclass method
    oapiRegisterPanelArea(GetAreaID(), GetRectForSize(18, 15), PANEL_REDRAW_MOUSE | PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN);
    m_mainSurface = CreateSurface(IDB_GREEN_LED_TINY);
}

bool RCSDockingModeButtonArea::Redraw2D(const int event, const SURFHANDLE surf)
{
    // always redraw this since it is only drawn on request
    oapiBlt(surf, m_mainSurface, 0, 0, GetXR3().m_rcsDockingMode ? 18 : 0, 0, 18, 15);
    return true;
}

bool RCSDockingModeButtonArea::ProcessMouseEvent(const int event, const int mx, const int my)
{
    // if crew is incapacitated, nothing to do here
    if (GetXR3().IsCrewIncapacitatedOrNoPilotOnBoard())
        return false;

    GetXR3().SetRCSDockingMode(!GetXR3().m_rcsDockingMode);   // toggle
    
    return true;
}

//----------------------------------------------------------------------------------

ElevatorToggleSwitchArea::ElevatorToggleSwitchArea(InstrumentPanel &parentPanel, const COORD2 panelCoordinates, const int areaID, const int indicatorAreaID) :
    ToggleSwitchArea(parentPanel, panelCoordinates, areaID, indicatorAreaID)
{
}

bool ElevatorToggleSwitchArea::isOn()
{
    // if switch is down ("on"), door is either opening or opened
    return ((GetXR3().crewElevator_status == DOOR_OPENING) || (GetXR3().crewElevator_status == DOOR_OPEN));
}


// only invoked when switch is CHANGING state
bool ElevatorToggleSwitchArea::ProcessSwitchEvent(bool switchIsOn)
{
    GetXR3().ActivateElevator(switchIsOn ? DOOR_OPENING : DOOR_CLOSING);
    
    return true;
}

//----------------------------------------------------------------------------------

// Constructor
XR3ReentryCheckMultiDisplayMode::XR3ReentryCheckMultiDisplayMode(int modeNumber) :
    ReentryCheckMultiDisplayMode(modeNumber)
{
}

// Invoked by our parent's AddDisplayMode method immediately after we are attached to our parent MDA.
// This is useful if an MDA needs to perform some one-time initialization.
void XR3ReentryCheckMultiDisplayMode::OnParentAttach()
{
    // allocate the door array and add all the standard doors
    ReentryCheckMultiDisplayMode::OnParentAttach();

    // now add our custom doors beginning @ door index 6
    const int cx = GetCloseButtonXCoord();
    int cy = GetStartingCloseButtonYCoord() + (6 * GetLinePitch());
    m_pDoorInfo[6] = new DoorInfo("OPEN",     "CLOSED", GetXR3().bay_status,          _COORD2(cx, cy),                   (void (DeltaGliderXR1::*)(DoorStatus))(&XR3Phoenix::ActivateBayDoors));
    m_pDoorInfo[7] = new DoorInfo("DEPLYD",   "STOWED", GetXR3().crewElevator_status, _COORD2(cx, cy += GetLinePitch()), (void (DeltaGliderXR1::*)(DoorStatus))(&XR3Phoenix::ActivateElevator));
}

//----------------------------------------------------------------------------------

// Override XR1 crew display panel showing crew members; also handles EVA requests
XR3CrewDisplayArea::XR3CrewDisplayArea(InstrumentPanel &parentPanel, const COORD2 panelCoordinates, const int areaID) :
    CrewDisplayArea(parentPanel, panelCoordinates, areaID)
{
    // override X coordinate for the "next" arrow to allow for 2-digit-wide crew indexes
    m_nextArrowCoord.x = 181;
}

