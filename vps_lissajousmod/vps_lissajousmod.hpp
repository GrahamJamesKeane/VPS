/*
 * File: vps_lissajousmod.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Lissajous 2D LFO Modulation
 */
 
#pragma once // Ensures the file is only included within a single compilation

#include "userosc.h" // Must include the userosc header to use its contents

# define ZEROF 0.f
# define DEFAULT 0.5f
# define ONEF 1.f

struct VPSLJ2D {
	enum {
		flags_none 	= 0,
		flag_LFO 	= 1<<0,
		flag_LFO1	= 1<<1,
		flag_LFO2	= 1<<2
	};
	
	struct Params {
		float 		w0LFO;			// LFO base Frequency
		float 		Amp_d;			// d amplitude
		float 		Amp_v;			// v amplitude
		float 		freq_shift_1;	// Frequency shift of LFO 1
		float 		freq_shift_2;	// Frequency shift of LFO 2
		float 		offset;			// Phase offset
		uint16_t	type;			// Base waveform type
		
		Params(void) :
			w0LFO(DEFAULT),
			Amp_d(ONEF),
			Amp_v(ONEF),
			freq_shift_1(ONEF),
			freq_shift_2(ONEF),
			offset(ZEROF),
			type(1)
		{ }
	};
  
	struct State {
		float   	phi; 		// Oscillator phase
		float   	w0;			// Oscillator pitch
		float		w0LFO1;		// LFO 1 frequency
		float		w0LFO2;		// LFO 2 frequency
		float 		phiLFO1;	// LFO 1 phase
		float 		phiLFO2;	// LFO 2 frequency
		float 		sigD;		// Last D signal
		float		sigV;		// Last V signal
		uint32_t 	flags: 3;	// Bit field for control flags
		
		State(void) :
			phi(ZEROF),
			w0(ZEROF),
			w0LFO1(ZEROF),
			w0LFO2(ZEROF),
			phiLFO1(ZEROF),
			phiLFO2(ZEROF),
			sigD(ZEROF),
			sigV(ZEROF),
			flags(flags_none)
		{}
	};

	VPSLJ2D(void) {
		state = State();
		params = Params();
	}
	
	inline float PhaseShaper(float phi, float phiLFO1, float phiLFO2) {
		
		float phase_d = phiLFO1 + params.offset;
		float Osc_d = 0.5f + 0.5f * osc_cosf(phase_d);
		float d = params.Amp_d * linintf(phi, state.sigD, Osc_d);
		state.sigD = Osc_d;
		
		float Osc_v = 0.5f + 0.5f * osc_cosf(phiLFO2);
		float v = params.Amp_v * linintf(phi, state.sigV, Osc_v);
		state.sigV = Osc_v;
		
		float ps;
		if(phi < d) {
			float fraction = phi / d;
			ps = v * fraction;
		} else {
			float numerator = phi - d;
			float denominator = 1.f - d;
			float fraction = numerator / denominator;
			ps = (1.f - v) * fraction;
			ps += v;
		}
		
		switch(params.type) {
			case 1: return -osc_parf(ps); 
			case 2: return -osc_cosf(ps);
			case 3: return -osc_sqrf(ps);
			case 4: return -osc_sawf(ps);
		}
	}
	
	inline float updatePhase(float *phi, float w0) {
		*phi += w0;
		*phi -= (uint32_t)*phi;
	}
	
	State 	state;
	Params 	params;
};
