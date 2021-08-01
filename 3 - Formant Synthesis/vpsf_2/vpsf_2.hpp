/*
 * File: vpsf_2.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Formants with interpolation
 */
 
#pragma once

#include "userosc.h"
#include "biquad.hpp."

#define ZEROF 	0.f
#define DEFAULT 0.5f
#define ONEF	1.f

struct VPSFI {
	
	enum {
		flags_none = 0,
		flag_reset = 1<<0
	};
	
	struct Params {
		float   d;
		float   v;
		float 	v_range;
		float	f_ratio; 	// Frequency ratio for filters
		
		Params(void) :
			d(DEFAULT),
			v(DEFAULT),
			v_range(ONEF),
			f_ratio(ONEF)	
		{ }
	};
  
	struct State {
		float    	phi; 		// Oscillator phase
		float    	w0;			// Oscillator pitch
		float 		last_1;     // Last VPS-1 signal (for interpolation gain)
		float 		last_2;     // Last VPS-2 signal (for interpolation gain)
		uint8_t 	flags: 1;	// Bit field for flags
		
		State(void) :
			phi(ZEROF),
			w0(ZEROF),
			last_1(ZEROF),
			last_2(ZEROF),
			flags(flags_none)
		{
			reset();
		}
		
		inline void reset(void) {
			phi = ZEROF;
		}
		
	};
	
	
	VPSFI(void) {
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
	
	inline float updatePhase(float *phi, float w0) {
		*phi += w0;
		*phi -= (uint32_t)*phi;
	}
	
	inline float PhaseShaper(float phi, float d, float v, float *last_sig) {
		
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
		sig = linintf(phi, *last_sig, sig);

		return sig;
	}

	inline float Formant(float phi) {
		
		float d = params.d;
		float v = params.v;
		
		float *last_1 = &state.last_1;
		float *last_2 = &state.last_2;
		
		// Equation 7 in article
		float formant_freq = si_fabsf((2.f * v) - 1.f);
		float interpolation_gain = formant_freq - (uint32_t)formant_freq;
		
		// Equation 8
		float v2 = v + 0.5f;
		
		float sig_1 = PhaseShaper(phi, d, v, last_1);
		sig_1 *= (1 - interpolation_gain);
		state.last_1 = sig_1;
		
		float sig_2 = PhaseShaper(phi, d, v2, last_2);
		sig_2 *= interpolation_gain;
		state.last_2 = sig_2;
		
		float sig = sig_1 + sig_2;
		
		return sig;
	}
	
	State 	state;
	Params 	params;
	
	dsp::BiQuad LPF;
	dsp::BiQuad HPF;
	
};
