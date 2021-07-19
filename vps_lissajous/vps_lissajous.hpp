/*
 * File: vps_lissajous.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * VPS with Lissajous modulation using native LFO
 */
 
#pragma once 

#include "userosc.h"

#define ZEROF 0.f
#define DEFAULT 0.5f
#define ONEF 1.f

struct VPSLJ {

	struct Params {
		float 		Amp_d;		// d amplitude
		float 		Amp_v;		// v amplitude
		uint32_t 	freq_shift;	// Frequency shift of LFO 2
		
		Params(void) :
			Amp_d(ONEF),
			Amp_v(ONEF),
			freq_shift(ONEF)
		{ }
	};
  
	struct State {
		float   	phi; 		// Oscillator phase
		float   	w0;			// Oscillator pitch
		float		lfo1;		// LFO1 signal
		float		lfo2;		// LFO2 signal
		float 		lfo1_last;	// LFO1 last signal
		float 		lfo2_last;	// LFO2 last signal
		float 		sigD_last;	// Interpolation variable for d
		float		sigV_last;	// Interpolation variable for v
		
		State(void) :
			phi(ZEROF),
			w0(ZEROF),
			lfo1(ZEROF),
			lfo2(ZEROF),
			lfo1_last(ZEROF),
			lfo2_last(ZEROF),
			sigD_last(ZEROF),
			sigV_last(ZEROF)
		{}
	};

	VPSLJ(void) {
		state = State();
		params = Params();
	}
	
	inline float PhaseShaper(float phi, float lfo1_last, float lfo2_last) {
		
		float w0_D = lfo1_last + 0.25f; // 0.25f to represent pi/2 as 1/4 cycle
		float Osc_d = 0.5f + 0.5f * osc_cosf(w0_D);
		float d = params.Amp_d * linintf(phi, state.sigD_last, Osc_d);
		state.sigD_last = Osc_d;
		
		float Osc_v = 0.5f + 0.5f * osc_cosf(lfo2_last);
		float v = params.Amp_v * linintf(phi, state.sigV_last, Osc_v);
		state.sigV_last = Osc_v;
		
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
	
	inline float updatePhase(float *phi, float w0) {
		*phi += w0;
		*phi -= (uint32_t)*phi;
	}
	
	State 	state;
	Params 	params;
	
};
