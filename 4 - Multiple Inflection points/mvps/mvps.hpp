/*
 * File: mvps.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Multiple Inflection Points
 */
 
#pragma once

#include "userosc.h"
#include "biquad.hpp."

#define ZEROF 	0.f
#define DEFAULT 0.5f
#define ONEF	1.f

struct MVPS {
	
	enum {
		flags_none = 0,
		flag_reset = 1<<0
	};
	
	struct Params {
		float 	d0;				// Inflection point 1: d0 
		float 	d1; 			// Inflection point 2: d1 
		float 	d2; 			// Inflection point 3: d2 
		float 	v0; 			// Inflection point 1: v0
		float 	v1; 			// Inflection point 2: v1
		float 	v2; 			// Inflection point 3: v2
		float	f_ratio; 	// Frequency ratio for filters
		
		Params(void) :
			d0(DEFAULT),
			d1(DEFAULT),
			d2(DEFAULT),
			v0(DEFAULT),
			v1(DEFAULT),
			v2(DEFAULT),
			f_ratio(ONEF)
		{ }
	};
  
	struct State {
		float   	phi; 		// Oscillator phase
		float   	w0;			// Oscillator pitch
		float 		last;     	// Last signal (for interpolation)
		uint8_t 	flags: 1;	// Bit field for flags
		State(void) :
			phi(ZEROF),
			w0(ZEROF),
			last(ZEROF),
			flags(flags_none)
		{
			reset();
		}
		
		inline void reset(void) {
			phi = ZEROF;
		}
	};

	MVPS(void) {
		state = State();
		params = Params();
	}
	
	inline void setFilters(float w0) {
		
		/*
		* We need to ensure we don't go over the
		* nyquist frequency
		*/  
		
		float limit = params.f_ratio;
		float lpf 	= clipmaxf(limit * w0, 0.49f);
		float hpf 	= clipminf((1.f / limit) * w0, ZEROF);

		LPF.mCoeffs.setFOLP(osc_tanpif(lpf));
		HPF.mCoeffs.setFOHP(osc_tanpif(hpf));
	}
	
	inline float updatePhase(float *phi, float w0) {
		*phi += w0;
		*phi -= (uint32_t)*phi;
	}
	
	inline float MultiPhaseShaper(float phi) {
		float d0 = params.d0;
		float v0 = params.v0;
		float d1 = params.d1;
		float v1 = params.v1;
		float d2 = params.d2;
		float v2 = params.v2;
		
		float ps;
		if(phi < d0) {
			ps = phi / d0;
			ps *= v0;
		} else if(phi < d1){
			ps = getPS(phi, d0, d1, v0, v1);
		} else if(phi < d2) {
			ps = getPS(phi, d1, d2, v1, v2);
		} else {
			ps = getPS(phi, d2, 1.f, v2, 1.f);
		}
		
		float sig = -osc_cosf(ps);
		sig = LPF.process_fo(sig);
		sig = HPF.process_fo(sig);
		sig = linintf(phi, state.last, sig);
		state.last = sig;
		return sig;
	}
	
	inline float getPS(float phi, float da, float db, float va, float vb) {
		float ps = phi - da;
		ps /= (db - da);
		ps *= (vb - va);
		return ps + va;
	}
	
	State 	state;
	Params 	params;
	
	dsp::BiQuad LPF;
	dsp::BiQuad HPF;
};
