/*
 * File: vpsf_1.cpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Formant Synthesis (Scaling V parameter) 
 */

#include "userosc.h"
#include "vpsf_1.hpp"

static VPSF vpsf;

void OSC_INIT(uint32_t platform, uint32_t api) {
	(void)platform;
	(void)api;
}

void OSC_CYCLE(const user_osc_param_t *const params, int32_t *yn, const uint32_t frames){
	
	// Local copy of the state object.
	VPSF::State &s = vpsf.state;
	
	// Current flag
	const uint32_t flags = s.flags; 
	
	// Reset s.flags
    s.flags = VPSF::flags_none; 
	
	// Reset state if note-on trigger
	if(flags & VPSF::flag_reset) {
		s.reset();
	}
	
	// Current note being played.
	uint8_t note = params->pitch>>8;
	
	// Floating point phase increment for given note and fine modulation
	float w0 = s.w0 = osc_w0f_for_note(note, params->pitch & 0xFF);
	
	// Set pole for filters
	vpsf.setFilters(w0);
	
	// Local copy of the current phase and its pointer.
	float phi = s.phi;
	float *Phi = &phi;
	
	// Pointer to start of buffer
	q31_t *__restrict y = (q31_t*)yn; 
	
	// Pointer to end of buffer
	const q31_t *y_e = y + frames; 

	// Fill buffer with samples
	for (; y != y_e; ) { 
		
		// Compute waveform
		float sig = vpsf.PhaseShaper(phi);
		
		// Softclip signal before sending to buffer
		sig = osc_softclipf(0.0125f, sig);
		
		// Format in Q31 binary fixed-point representation and add to buffer
		*(y++) = f32_to_q31(sig); 
		
		// Update local osc phase
		vpsf.updatePhase(Phi, w0);
	}
	
	// Update global osc phase
	s.phi = phi;
	
}

void OSC_NOTEON(const user_osc_param_t *const params) {
	// Reset oscillator phase for new note
	VPSF::State &s = vpsf.state;
	s.flags |= VPSF::flag_reset;
}

void OSC_NOTEOFF(const user_osc_param_t *const params) {
	// Not used
	(void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value) { 

	VPSF::Params &p = vpsf.params;
	
	switch (index) {
		case k_user_osc_param_id1: // User parameter 1
			// Set d values scale in [0-1]
			p.d = clip01f(value * 0.005f);
			break;
			
		case k_user_osc_param_id2: // User parameter 2
			// Set v values
			p.v = clip01f(value * 0.005f) * p.v_range; 	
			break;
			
		case k_user_osc_param_id3: // User parameter 3
			// Set range of v [1-10]
			p.v_range = 1.f + value * 0.09f;
			break;
			
		case k_user_osc_param_id4: // User parameter 4
			// Set the frequency ratio for filters
			p.f_ratio = 1.f + (value * 1.f);
			break;
			
		case k_user_osc_param_id5: // User parameter 5
			break;
			
		case k_user_osc_param_id6: // User parameter 6
			break;
			
		case k_user_osc_param_shape: // A knob 
			break;
			
		case k_user_osc_param_shiftshape: // B knob
			break;
			
		default: 
			break;
	}
}

