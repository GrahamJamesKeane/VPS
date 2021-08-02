/*
 * File: mvps1D.cpp
 * 
 * 2021 Graham Keane - Maynooth University
 *
 * Multiple inflections points with 1D LFO modulation (d-values)
 */

#include "userosc.h"
#include "mvps1D.hpp"

static MVPS1D mvps1D;

void OSC_INIT(uint32_t platform, uint32_t api) {
	(void)platform;
	(void)api;
}

void OSC_CYCLE(const user_osc_param_t *const params, int32_t *yn, const uint32_t frames){
	
	// Local copy of the state object.
	MVPS1D::State &s = mvps1D.state;
	
	// Current flag
	const uint32_t flags = s.flags; 
	
	// Reset s.flags
    s.flags = MVPS1D::flags_none; 
	
	// Reset state if note-on trigger
	if(flags & MVPS1D::flag_reset) {
		s.reset();
	}
	
	// Current note being played.
	uint8_t note = params->pitch>>8;
	
	// Floating point phase increment for given note and fine modulation
	float w0 = s.w0 = osc_w0f_for_note(note, params->pitch & 0xFF);
	
	// Set pole for filters
	mvps1D.setFilters(w0);
	
	// Local copy of the phase and its pointer.
	float phi = s.phi;
	float *Phi = &phi;
	
	// Get LFO value
	float lfo = s.lfo = q31_to_f32(params->shape_lfo);
	
	// Pointer to start of buffer
	q31_t *__restrict y = (q31_t*)yn; 
	
	// Pointer to end of buffer
	const q31_t *y_e = y + frames; 

	// Fill buffer with samples
	for (; y != y_e; ) { 

		// Compute waveform
		float sig = mvps1D.MultiPhaseShaper(phi, lfo);
		
		// Update local osc phase of primary osc
		mvps1D.updatePhase(Phi, w0);
		
		// Softclip signal before sending to buffer
		sig = osc_softclipf(0.05f, sig);
		
		// Format in Q31 binary fixed-point representation and add to buffer
		*(y++) = f32_to_q31(sig); 
	}
	
	// Update global osc phase
	s.phi = phi;
	
}

void OSC_NOTEON(const user_osc_param_t *const params) {
	// Reset oscillator phase for new note
	MVPS1D::State &s = mvps1D.state;
	s.flags |= MVPS1D::flag_reset;
}

void OSC_NOTEOFF(const user_osc_param_t *const params) {
	// Not used	
	(void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value) { 

	MVPS1D::Params &p = mvps1D.params;
	
	switch (index) {
		case k_user_osc_param_id1: // User parameter 1
			// Set Values for Inflection Point 1: d0
			p.d0 = clip01f(value * 0.01f); 					
			break;
			
		case k_user_osc_param_id2:	// User parameter 2
			// Set Values for Inflection Point 1: v0
			p.v0 = clip01f(value * 0.01f); 
			break;
			
		case k_user_osc_param_id3:  // User parameter 3
			// Set Values for Inflection Point 2: d1
			p.d1 = clip01f(value * 0.01f); 		
			break;
			
		case k_user_osc_param_id4:	// User parameter 4
			// Set Values for Inflection Point 2: v1
			p.v1 = clip01f(value * 0.01f); 
			break;
			
		case k_user_osc_param_id5:  // User parameter 5
			// Set Values for Inflection Point 3: x2
			p.d2 = clip01f(value * 0.01f); 			
			break;
			
		case k_user_osc_param_id6:	// User parameter 6
			// Set Values for Inflection Point 3: v2
			p.v2 = clip01f(value * 0.01f); 
			break;
			
		case k_user_osc_param_shape: // A knob
			// Set the frequency ratio for filters
			p.f_ratio = 1.f + param_val_to_f32(value) * 100.f;
			break;
			
		case k_user_osc_param_shiftshape: // B knob	
			break;
			
		default: break;
	}
}

