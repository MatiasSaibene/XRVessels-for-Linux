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
// XR2Ravenstar implementation class
//
// XR2Ravenstar_ED.cpp
// Contains Vessel Editor methods
// ==============================================================

#include "XR2Ravenstar.h"
#include "..//..//..//..//include//ScnEditorAPI.h"
#include "..//..//DeltaGliderXR1//XR1Lib//XR1PayloadDialog.h"
#include "..//..//..//..//include//font_awesome_5.h"
#include <array>
#include <functional>

// ==============================================================
// Scenario editor interface
// ==============================================================

static void DrawState(DoorStatus status, std::array<const char *, 4> &&desc) {
    const char *txt;
    switch(status) {
        case DoorStatus::DOOR_FAILED:   txt = "Failed"; break;
        case DoorStatus::DOOR_CLOSED:   txt = desc[0]; break;
        case DoorStatus::DOOR_CLOSING:  txt = desc[1]; break;
        case DoorStatus::DOOR_OPENING:  txt = desc[2]; break;
        case DoorStatus::DOOR_OPEN:     txt = desc[3]; break;
        default: // fallthrough
        case DoorStatus::NOT_SET:       txt = "Not set"; break;
    }

	ImGui::SetNextItemWidth(80.0f);
	ImGui::BeginDisabled(true);
	ImGui::SameLine();
	ImGui::TextUnformatted(txt);
	ImGui::SameLine();
	ImGui::EndDisabled();
}

static void DrawControl(const char *name, double ratio, DoorStatus &status, std::function<void(DoorStatus)> onChange,
                        std::array<const char *, 2> &&btn, std::array<const char *, 4> &&desc) {
    const ImVec2 button_sz(ImVec2(60, 20));
    ImGui::BeginGroupPanel(name, ImVec2(ImGui::GetContentRegionAvail().x * ratio, 0));
    ImGui::PushID(name);
    if(ImGui::Button(btn[0], button_sz)) { onChange(DoorStatus::DOOR_CLOSED); }
    DrawState(status, std::move(desc));
    if(ImGui::Button(btn[1], button_sz)) { onChange(DoorStatus::DOOR_OPEN); }
    ImGui::PopID();
    ImGui::EndGroupPanel();

}

static void DrawXR2Controls(XR2Ravenstar *pXR) {
	const ImVec2 button_sz(ImVec2(60, 20));
    DoorStatus orgAPUState = pXR->apu_status;

    // hotwire the apu to ON so we can move the doors by "cheating" here
    pXR->apu_status = DoorStatus::DOOR_OPEN;

    DrawControl("Landing gear", 0.5, pXR->gear_status,
                [pXR](DoorStatus s) {pXR->ActivateLandingGear (s);},
                {"Up","Down"},{"Raised", "Raising", "Lowering", "Lowered"});
    ImGui::SameLine();
    DrawControl("Air brake", 1.0, pXR->brake_status,
                [pXR](DoorStatus s) {pXR->ActivateAirbrake (s);},
                {"Stow","Deploy"},{"Stowed", "Stowing", "Extending", "Extended"});


    DrawControl("Outer airlock", 0.5, pXR->olock_status,
                [pXR](DoorStatus s) {pXR->ActivateOuterAirlock (s);},
                {"Close","Open"},{"Closed", "Closing", "Opening", "Opened"});
    ImGui::SameLine();
    DrawControl("Inner airlock", 1.0, pXR->ilock_status,
                [pXR](DoorStatus s) {pXR->ActivateInnerAirlock (s);},
                {"Close","Open"},{"Closed", "Closing", "Opening", "Opened"});


    DrawControl("Nose cone", 0.5, pXR->nose_status,
                [pXR](DoorStatus s) {
                    switch(s) {
                        case DoorStatus::DOOR_CLOSED:
                            pXR->ActivateOuterAirlock (s);
                            pXR->ActivateNoseCone (s);
                            break;
                        case DoorStatus::DOOR_OPEN:
                            pXR->ActivateNoseCone (s);
                            break;
                        default:
                            break;
                    }
                },
                {"Close","Open"},{"Closed", "Closing", "Opening", "Opened"});
    ImGui::SameLine();
    DrawControl("Hatch", 1.0, pXR->hatch_status,
                [pXR](DoorStatus s) {pXR->ActivateHatch (s);},
                {"Close","Open"},{"Closed", "Closing", "Opening", "Opened"});


    DrawControl("SCRAM doors", 0.5, pXR->scramdoor_status,
                [pXR](DoorStatus s) {pXR->ActivateScramDoors (s);},
                {"Close","Open"},{"Closed", "Closing", "Opening", "Opened"});
    ImGui::SameLine();
    DrawControl("Hover doors", 1.0, pXR->hoverdoor_status,
                [pXR](DoorStatus s) {pXR->ActivateHoverDoors (s);},
                {"Close","Open"},{"Closed", "Closing", "Opening", "Opened"});


    DrawControl("Radiator", 0.5, pXR->radiator_status,
                [pXR](DoorStatus s) {pXR->ActivateRadiator (s);},
                {"Close","Open"},{"Closed", "Closing", "Opening", "Opened"});
    ImGui::SameLine();
    DrawControl("Bay door", 1.0, pXR->bay_status,
                [pXR](DoorStatus s) {pXR->ActivateBayDoors (s);},
                {"Retract","Extend"},{"Retracted", "Retracting", "Extending", "Extended"});


    DrawControl("Retro doors", 0.5, pXR->rcover_status,
                [pXR](DoorStatus s) {pXR->ActivateRCover (s);},
                {"Close","Open"},{"Closed", "Closing", "Opening", "Opened"});
    ImGui::SameLine();
    DrawControl("Chamber", 1.0, pXR->chamber_status,
                [pXR](DoorStatus s) {pXR->ActivateChamber (s, true);},
                {"Close","Open"},{"Closed", "Closing", "Opening", "Opened"});

    DrawControl("Ladder", 1.0, pXR->ladder_status,
                [pXR](DoorStatus s) {pXR->ActivateLadder (s);},
                {"Close","Open"},{"Closed", "Closing", "Opening", "Opened"});

    // restore original APU state
    pXR->apu_status = orgAPUState;
}

static XR1PayloadDialog *s_plEditor = nullptr;

static void DrawScnEditorTabs(OBJHANDLE hVessel) {
	XR2Ravenstar *xr2 = (XR2Ravenstar*)oapiGetVesselInterface (hVessel);
    s_plEditor->SetVessel(xr2);
	if(ImGui::BeginTabItem(ICON_FA_DRAFTING_COMPASS" Animations")) {
		DrawXR2Controls(xr2);
		ImGui::EndTabItem();
	}
	if(ImGui::BeginTabItem(ICON_FA_BOX" Payload")) {
        s_plEditor->DrawPayloadEditor();
		ImGui::EndTabItem();
	}
}

// --------------------------------------------------------------
// Add vessel-specific pages into scenario editor
// --------------------------------------------------------------

DLLCLBK ScnDrawCustomTabs secInit ()
{
    if(!s_plEditor) {
        s_plEditor = new XR1PayloadDialog("Payload Editor");
    }
	return DrawScnEditorTabs;
}
