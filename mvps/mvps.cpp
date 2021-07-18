/*
 * File: mvps.cpp
 * 
 * 2021 Graham Keane - Maynooth University
 *
 * Multiple inflections points
 */

#include "userosc.h"
#include "mvps.hpp"

static MVPS mvps;

void OSC_INIT(uint32_t platform, uint32_t api) {
	(void)platform;
	(void)api;
}

void OSC_CYCLE(const user_osc_param_t *const params, int32_t *yn, const uint32_t frames){
	
	// Local copy of the state object.
	MVPS::State &s = mvps.state;
	
	// Current note being played.
	uint8_t note = params->pitch>>8;
	
	// Floating point phase increment for given note and fine modulation
	float w0 = s.w0 = osc_w0f_for_note(note, params->pitch & 0xFF);
	
	// Local copies of the state object fields.
	float phi = s.phi;
	float *Phi = &phi;
	
	// Prepare to fill buffer
	q31_t *__restrict y = (q31_t*)yn; // Pointer to start of buffer
	const q31_t *y_e = y + frames; // Pointer to end of buffer

	// Fill buffer with samples
	for (; y != y_e; ) { 
		float sig = 0.f;
		
		sig = mvps.MultiPhaseShaper(phi);
		
		// Update local osc phase of primary osc
		mvps.updatePhase(Phi, w0);
		
		// Softclip signal before sending to buffer
		sig = osc_softclipf(0.05f, sig);
		
		// Format in Q31 binary fixed-point representation and add to buffer
		*(y++) = f32_to_q31(sig); 
	
	}
	
	// Update global osc phase
	s.phi = phi;
	
}

void OSC_NOTEON(const user_osc_param_t *const params) {
	// Not used
	(void)params;
}

void OSC_NOTEOFF(const user_osc_param_t *const params) {
	// Not used	
	(void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value) { 

	MVPS::Params &p = mvps.params;
	
	const float valf = param_val_to_f32(value);
	switch (index) {
		case k_user_osc_param_id1:  				// User parameter 1
			// Set Values for Inflection Point 1: d0
			p.d0 = value * 0.003f; 					// scale in [0-0.3]
			break;
			
		case k_user_osc_param_id2:					// User parameter 2
			// Set Values for Inflection Point 1: v0
			p.v0 = value * 0.01f;
			break;
			
		case k_user_osc_param_id3:  				// User parameter 3
			// Set Values for Inflection Point 2: d1
			p.d1 = 0.3f + (value * 0.003f); 		// scale in [0.3-0.6]
			break;
			
		case k_user_osc_param_id4:					// User parameter 4
			// Set Values for Inflection Point 2: v1
			p.v1 = value * 0.01f;
			break;
			
		case k_user_osc_param_id5:  				// User parameter 5
			// Set Values for Inflection Point 3: d2
			p.d2 = 0.6f + (value * 0.004f);			// scale in [0.6-1.0]
			break;
			
		case k_user_osc_param_id6:					// User parameter 6
			// Set Values for Inflection Point 3: v2
			p.v2 = value * 0.01f;
			break;
			
		case k_user_osc_param_shape: 				// A knob
			// Scaling for d values
			p.shape = valf; 
			break;
			
		case k_user_osc_param_shiftshape:			// B knob	
			// Scaling for v values
			p.shiftshape = valf; 
			break;
			
		default: break;
	}
}

