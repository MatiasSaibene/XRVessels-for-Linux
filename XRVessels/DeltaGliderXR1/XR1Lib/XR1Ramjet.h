// ==============================================================
// XR1 Base Class Library
// These classes extend and use the XR Framework classes
//
// Copyright 2006-2016 Douglas E. Beachy
// All rights reserved.
//
// This software is FREEWARE and may not be sold!
//
// XR1Ramjet.h
// Interface for the delta glider XR1Ramjet engine
//
// Notes:
// This class only requires a generic VESSEL reference, so it can
// be ported to other vessel classes.
// It is designed to manage all XR1Ramjet/scramjet engines of a
// vessel, so only a single instance should be created. Individual
// engines can then be defined by the AddThrusterDefinition method.
// ==============================================================

#pragma once

#include "Orbitersdk.h"

#include "XR1Globals.h"

class DeltaGliderXR1;

class XR1Ramjet 
{
public:
    // NOTE: must use 'DeltaGliderXR1 *' here, NOT VESSEL * or VESSEL2 *!  Otherwise,
    // our member pointers will be 4 bytes off because MSVC++ expects the incoming
    // pointer to be the actual (i.e., "most-derived") object type.  
    // i.e., we can't receive a 'VESSEL2 *' and downcast it to 'DeltaGliderXR1 *'.
	XR1Ramjet (DeltaGliderXR1 *_vessel);
	// constructor

	virtual ~XR1Ramjet ();
	// destructor

	void AddThrusterDefinition (THRUSTER_HANDLE th,
		double Qr, double Ai, double Tb_max, double dmf_max);
	// Add a new thruster definition to the list of engines managed by
	// the XR1Ramjet object. Engine design parameters:
	// Qr: Fuel heating value (FHV) [J/kg]. Value for typical jet fuel
	//     is about 4.5e7. Qr influences the rate at which fuel must be
	//     burned to obtain a given amount of thrust.
    //
    // {DEB} Hydrogen FHV = 1.42e8 J/kg.
    //
	// Ai: Inlet cross section [m^2]. Ai basically scales the size of
	//     the engine, i.e. determines how much thrust it generates and
	//     how much fuel it burns.
	// Tb_max: Maximum burner temperature [K] (due to material limits).
	//     Determines how much thrust can be generated without melting
	//     the engine, and up to what velocity the engine can be
	//     operated. A typical value may be around 2400K.

	void Thrust (double *F) const;
	// calculates the thrust generated by each thruster
	// and returns the force value in the supplied list F
	// On input, F must point to an array of at least the same
	// length as the number of thruster definitions (nthdef)

	inline double DMF (UINT idx) const { return thdef[idx]->dmf; }
	// returns current fuel mass flow of thruster idx

	double Temp (UINT idx, UINT which) const; 
	// returns diffuser, combustion or exhaust temperature [K] of thruster idx
    // {DEB} these are ADJUSTED temperatures for the XR1

	double TSFC (UINT idx) const;
	// returns thrust-specific fuel consumption of thruster idx
	// based on last thrust calculation

    // new for XR1: retrieve most recent frame's thrust for the given thruster index (0-n)
    // this is useful for thrust gauges, where we don't want to re-compute the thrust again in the same frame
    // Also, we are getting some weirdness here where F is sometimes -0.0000...  Therefore we, check for that here.
    double GetMostRecentThrust(int index) const 
    { 
        double retVal = thdef[index]->F;
        if (retVal <= 0.0) 
            retVal = 0;

        return retVal;
    }

    // made public for the XR1
    struct THDEF        // list of XR1Ramjet thrusters
    {             
		THRUSTER_HANDLE th;    //   thruster handle                -+
		double Qr;             //   fuel heating parameter [J/kg]   | static
		double Ai;             //   air intake cross section [m^2]  | parameters
		double Tb_max;         //   max. burner temperature [K]     |
		double dmf_max;        //   max. fuel flow rate [kg/s]     -+

		double dmf;            //   current fuel mass rate [kg/s]  -+ dynamic
		double F;              //   current thrust [N]              | parameters
		double T[3];           //   temperatures                   -+ 0=diffuser, 1=burner, 2=exhaust
        double pd;             //   diffuser pressure
	} **thdef;
	UINT nthdef;               // number of XR1Ramjet thrusters

    // disable/enable an engine
    void SetEngineIntegrity(int engine, double integ) { m_integrity[engine] = integ; }
    double GetEngineIntegrity(int engine) const { return m_integrity[engine]; }

protected:
    DeltaGliderXR1 &GetXR1() const { return *vessel; }

private:
	DeltaGliderXR1 *vessel;        // vessel pointer
    double m_integrity[2];         // 0...1
};
