/*
 * File: vpslj_1.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Lissajous 2D LFO Modulation
 */
 
#pragma once // Ensures the file is only included within a single compilation

#include "userosc.h" // Must include the userosc header to use its contents
#include "biquad.hpp."

# define ZEROF 0.f
# define DEFAULT 0.5f
# define ONEF 1.f

struct VPSLJ2D {
	enum {
		flags_none 	= 0,
		flag_LFO 	= 1<<0,
		flag_LFOD	= 1<<1,
		flag_LFOV	= 1<<2,
		flag_reset 	= 1<<3
	};
	
	struct Params {
		float 		w0LFO;			// LFO base Frequency
		float 		Amp_d;			// d amplitude
		float 		Amp_v;			// v amplitude
		float		f_ratio; 		// Frequency ratio for filters
		float 		freq_shift_D;	// Frequency shift of LFO-D
		float 		freq_shift_V;	// Frequency shift of LFO-V
		float 		offset;			// Phase offset
		
		Params(void) :
			w0LFO(DEFAULT),
			Amp_d(ONEF),
			Amp_v(ONEF),
			f_ratio(ONEF),
			freq_shift_D(ONEF),
			freq_shift_V(ONEF),
			offset(ZEROF)
		{ }
	};
  
	struct State {
		float   	phi; 		// Oscillator phase
		float   	w0;			// Oscillator pitch
		float		w0LFOD;		// LFO-D frequency
		float		w0LFOV;		// LFO-V frequency
		float 		phiLFOD;	// LFO-D phase
		float 		phiLFOV;	// LFO-V frequency
		float 		last;     	// Last signal (for interpolation)
		uint32_t 	flags: 4;	// Bit field for control flags
		
		State(void) :
			phi(ZEROF),
			w0(ZEROF),
			w0LFOD(ZEROF),
			w0LFOV(ZEROF),
			phiLFOD(ZEROF),
			phiLFOV(ZEROF),
			last(ZEROF),
			flags(flags_none)
		{
			reset();
		}
		
		inline void reset(void) {
			phi = ZEROF;
		}
	};

	VPSLJ2D(void) {
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
	
	inline float PhaseShaper(float phi, float phiLFOD, float phiLFOV) {
		
		float phase_d = phiLFOD + params.offset;
		float Osc_d = 0.5f + 0.5f * osc_cosf(phase_d);
		float d = params.Amp_d * Osc_d;
		
		float Osc_v = 0.5f + 0.5f * osc_cosf(phiLFOV);
		float v = params.Amp_v * Osc_v;
		
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
