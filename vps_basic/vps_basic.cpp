/*
 * File: vps_basic.cpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Vector Phaseshaping
 */

# include "userosc.h"
# include "vps_basic.hpp"

// Create VPS object
static VPS vps;

void OSC_INIT(uint32_t platform, uint32_t api) {
	(void)platform;
	(void)api;
}

void OSC_CYCLE(const user_osc_param_t *const params, int32_t *yn, const uint32_t frames){
	
	// Local copies of the state and params objects.
	VPS::State &s = vps.state;
	const VPS::Params &p = vps.params;
	
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
	
	// Local copies of the state object fields.
	float phi = s.phi;
	
	// Prepare to fill buffer
	q31_t *__restrict y = (q31_t*)yn; // Pointer to start of buffer
	const q31_t *y_e = y + frames; // Pointer to end of buffer

	// Fill buffer with samples
	for (; y != y_e; ) { 
		
		float sig = vps.PhaseShaper(phi);
		
		// Softclip signal before sending to buffer
		sig = osc_softclipf(0.05f, sig);
		
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
	VPS::State &s = vps.state;
	s.flags |= VPS::flag_reset;
}

void OSC_NOTEOFF(const user_osc_param_t *const params) {
	// Not used
	(void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value) {
	
	VPS::Params &p = vps.params;
	
	const float valf = param_val_to_f32(value);
	switch (index) {
		case k_user_osc_param_id1: break; 	// User parameter 1
			
		case k_user_osc_param_id2: break; 	// User parameter 2
			
		case k_user_osc_param_id3: break; 	// User parameter 3
			
		case k_user_osc_param_id4: break; 	// User parameter 4
			
		case k_user_osc_param_id5: break; 	// User parameter 5
			
		case k_user_osc_param_id6: break; 	// User parameter 6
			
		case k_user_osc_param_shape: 		// A knob
			// 10bit parameter
			p.shape = valf; break;
		case k_user_osc_param_shiftshape: 	// B knob
			// 10bit parameter
			p.shiftshape = valf; break;
			
		default: break;
	}
}

