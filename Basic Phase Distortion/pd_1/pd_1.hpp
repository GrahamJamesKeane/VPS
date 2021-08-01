/*
 * File: pd_1.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Basic Phase Distortion
 */
 
#pragma once

#include "userosc.h"

#define ZEROF 	0.f
#define DEFAULT 0.5f

struct PD {
	
	enum {
		flags_none = 0,
		flag_reset = 1<<0
	};
	
	struct Params {
		float   d;
    
		Params(void) :
			d(DEFAULT)
		{ }
	};
  
	struct State {
		float    phi; 		// Main oscillator phase
		float    w0;		// Main oscillator pitch
		uint32_t flags: 1;	// Bit field
		State(void) :
			phi(ZEROF),
			w0(ZEROF),
			flags(flags_none)
		{
			reset();
		}
		
		inline void reset(void) {
			phi = ZEROF;
		}
	};

	PD(void) {
		state = State();
		params = Params();
	}
	
	inline float PhaseDistortion(float phi) {
		float d = params.d;
		
		float pd;
		if(phi < d) {
			float fraction = phi / d;
			pd = 0.5f * fraction;
		} else {
			float numerator = phi - d;
			float denominator = 1.f - d;
			float fraction = numerator / denominator;
			pd = 0.5f * (1 + fraction);
		}
		
		return -osc_cosf(pd);
	}
	
	State 	state;
	Params 	params;
	
};
