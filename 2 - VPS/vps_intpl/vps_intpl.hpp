/*
 * File: vps_intpl.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Vector Phaseshaping 
 */
 
#pragma once

#include "userosc.h" 
#include "biquad.hpp."

#define ZEROF 	0.f
#define DEFAULT 0.5f
#define ONEF	1.f

struct VPS {
	
	enum {
		flags_none = 0,
		flag_reset = 1<<0
	};
  
	struct Params {
		float   d;
		float   v;
		float	f_ratio; 	// Frequency ratio for filters
		
		Params(void) :
			d(DEFAULT),
			v(DEFAULT),
			f_ratio(ONEF)
		{ }
	};
  
	struct State {
		float   phi; 	// Oscillator phase
		float   w0;		// Oscillator pitch
		float 	last;	// Last signal (for interpolation)
		uint32_t flags: 1;	// Bit field
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

	VPS(void) {
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
	
	inline float PhaseShaper(float phi) {
		float d = params.d;
		float v = params.v;
		
		float ps;
		if(phi < d) {
			ps = phi / d;
			ps *= v;
		} else {
			ps = phi - d;
			ps /= (1.f - d);
			ps *= (1.f - v);
			ps += v;
		}
		
		float sig = -osc_cosf(ps);
		sig = LPF.process_fo(sig);
		sig = HPF.process_fo(sig);
		sig = linintf(phi, state.last, sig);
		state.last = sig;
		return sig;
		
	}
	
	State 	state;
	Params 	params;
	
	dsp::BiQuad LPF;
	dsp::BiQuad HPF;
	
};
