/*
 * File: pd_2.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Basic Phase Distortion
 */
 
#pragma once

#include "userosc.h"
#include "biquad.hpp."

#define ZEROF 	0.f
#define DEFAULT 0.5f
#define ONEF	1.f

struct PD {
	
	enum {
		flags_none = 0,
		flag_reset = 1<<0
	};
	
	struct Params {
		float   d; 			// D-values
		float	f_ratio; 	// Frequency ratio for filters
    
		Params(void) :
			d(DEFAULT),
			f_ratio(ONEF)
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
	
	inline void setFilters(float w0) {
		
		/*
		* We need to ensure we don't go over the
		* nyquist frequency
		*/  
		
		float limit = params.f_ratio;
		float lpf = clipmaxf(limit * w0, 0.49f);
		float hpf = clipminf((1.f / limit) * w0, ZEROF);

		LPF.mCoeffs.setFOLP(osc_tanpif(lpf));
		HPF.mCoeffs.setFOHP(osc_tanpif(hpf));
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
		
		float sig = -osc_cosf(pd);
		sig = LPF.process_fo(sig);
		sig = HPF.process_fo(sig);
		
		return sig;
	}
	
	State 	state;
	Params 	params;
	
	dsp::BiQuad LPF;
	dsp::BiQuad HPF;
};
