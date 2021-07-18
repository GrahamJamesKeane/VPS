/*
 * File: vps_basic.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Vector Phaseshaping 
 */
 
#pragma once

#include "userosc.h" 

# define ZEROF 0.f

struct VPS {
	
	enum {
		flags_none = 0,
		flag_reset = 1<<0
	};
  
	struct Params {
		float   shape;
		float   shiftshape;
    
		Params(void) :
			shape(ZEROF),
			shiftshape(ZEROF) 
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

	VPS(void) {
		state = State();
		params = Params();
	}
  
	inline float PhaseShaper(float phi) {
		float d = params.shape;
		float v = params.shiftshape;
		
		float pd;
		if(phi < d) {
			float fraction = phi / d;
			pd = v * fraction;
		} else {
			float numerator = phi - d;
			float denominator = 1.f - d;
			float fraction = numerator / denominator;
			pd = (1.f - v) * fraction + v;
		}
		
		return -osc_cosf(pd);
	}
	
	State 	state;
	Params 	params;
	
};
