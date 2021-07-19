/*
 * File: vps_formants_intpl.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Formants with interpolation
 */
 
#pragma once

#include "userosc.h"

#define ZEROF 0.f
#define DEFAULT 0.5f

struct VPSFI {
	
	enum {
		flags_none 		= 0,
		flag_phaseShape = 1<<0,
		flag_formant 	= 1<<1
	};
	
	struct Params {
		float   shape;
		float   shiftshape;
		float 	formant;
		
		Params(void) :
			shape(DEFAULT),
			shiftshape(DEFAULT),
			formant(1.5f)			
		{ }
	};
  
	struct State {
		float    	phi; 		// Oscillator phase
		float    	w0;			// Oscillator pitch
		float 		last_sig;	// Last signal
		uint8_t 	flags: 2;	// Bit field for flags
		
		State(void) :
			phi(ZEROF),
			w0(ZEROF),
			last_sig(ZEROF),
			flags(flag_phaseShape)
		{}
	};

	VPSFI(void) {
		state = State();
		params = Params();
	}
  
	inline float PhaseShaper(float phi, float d, float v) {
		
		float ps;
		if(phi < d) {
			float fraction = phi / d;
			ps = v * fraction;
		} else {
			float numerator = phi - d;
			float denominator = 1.f - d;
			float fraction = numerator / denominator;
			ps = (1.f - v) * fraction + v;
		}
		
		return -osc_cosf(ps);
	}

	inline float Formant(float phi) {
		
		float d = params.shape;
		float v = params.formant;
		
		float interpolation_gain = (2.f * v) - 1.f;
		interpolation_gain -= (uint32_t)interpolation_gain;
		
		float vZ = v + 0.5f;
		
		float sig_A = PhaseShaper(phi, d, v);
		sig_A *= (1 - interpolation_gain);
		
		float sig_B = PhaseShaper(phi, d, vZ);
		sig_B *= interpolation_gain;
		
		float sig = sig_A + sig_B;
		state.last_sig = linintf(phi, state.last_sig, osc_cosf(sig));
		return state.last_sig;
	}
	
	inline float updatePhase(float *phi, float w0) {
		*phi += w0;
		*phi -= (uint32_t)*phi;
	}
	
	State 	state;
	Params 	params;
	
};
