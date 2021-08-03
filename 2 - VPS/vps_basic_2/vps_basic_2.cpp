/*
 * File: vps_basic_2.cpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Vector Phaseshaping
 */

# include "userosc.h"
# include "vps_basic_2.hpp"

static VPS vps;

void OSC_INIT(uint32_t platform, uint32_t api) {
	(void)platform;
	(void)api;
}

void OSC_CYCLE(const user_osc_param_t *const params, int32_t *yn, const uint32_t frames){
	
	// Local copy of the state object.
	VPS::State &s = vps.state;
	
	// Get current flag
	const uint32_t flags = s.flags; 
	
	// Reset s.flags
    s.flags = VPS::flags_none; 
	
	// Reset state if note-on trigger
	if(flags & VPS::flag_reset) {
		s.reset();
	}
	
	// Current note being played.
	uint8_t note = params->pitch>>8;
	
	// Floating point phase increment for given note and fine modulation
	float w0 = s.w0 = osc_w0f_for_note(note, params->pitch & 0xFF);
	
	// Set pole for filters
	vps.setFilters(w0);
	
	// Local copy of the current phase.
	float phi = s.phi;
	
	// Pointer to start of buffer
	q31_t *__restrict y = (q31_t*)yn;
	
	// Pointer to end of buffer
	const q31_t *y_e = y + frames; 

	// Fill buffer with samples
	for (; y != y_e; ) { 
		
		// Get VPS signal
		float sig = vps.PhaseShaper(phi);
		
		// Softclip signal before sending to buffer
		sig = osc_softclipf(0.0125f, sig);
		
		// Format in Q31 binary fixed-point representation and add to buffer
		*(y++) = f32_to_q31(sig); 
		
		// Update local osc phase
		phi += w0;
		phi -= (uint32_t)phi;
	}
	
	// Update global osc phase
	s.phi = phi;
	
}

void OSC_NOTEON(const user_osc_param_t *const params) {
	// Reset oscillator phase for new note
	VPS::State &s = vps.state;
	s.flags |= VPS::flag_reset;
}

void OSC_NOTEOFF(const user_osc_param_t *const params) {
	// Not used
	(void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value) {
	
	VPS::Params &p = vps.params;
	
	switch (index) {
		case k_user_osc_param_id1: // User parameter 1
			// Set d value in range [0-1] (Bipolar)
			p.d = clip01f(value * 0.005f);
			break;
			
		case k_user_osc_param_id2: // User parameter 2
			// Set v value in range [0-1] (Bipolar)
			p.v = clip01f(value * 0.005f);
			break;
			
		case k_user_osc_param_id3: // User parameter 3
			// Set the frequency ratio for filters
			p.f_ratio = 1.f + (value * 1.f);
			break;
			
		case k_user_osc_param_id4:  // User parameter 4
			break;
			
		case k_user_osc_param_id5: // User parameter 5
			break;
			
		case k_user_osc_param_id6: // User parameter 6
			break;
			
		case k_user_osc_param_shape: // A knob
			break;
			
		case k_user_osc_param_shiftshape: // B knob
			break;
			
		default: break;
	}
}

