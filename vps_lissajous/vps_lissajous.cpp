/*
 * File: vps_lissajous.cpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * VPS with Lissajous modulation using native LFO
 */

#include "userosc.h"
#include "vps_lissajous.hpp"

static VPSLJ vpsLJ;

void OSC_INIT(uint32_t platform, uint32_t api) {
	(void)platform;
	(void)api;
}

void OSC_CYCLE(const user_osc_param_t *const params, int32_t *yn, const uint32_t frames){
	// Create local copies of the state and params objects.
	VPSLJ::State &s = vpsLJ.state;
	const VPSLJ::Params &p = vpsLJ.params;
	
	// Current note being played.
	uint8_t note = params->pitch>>8;
	
	// Floating point phase increment for given note and fine modulation
	float w0 = s.w0 = osc_w0f_for_note(note, params->pitch & 0xFF);
	
	// Local copies of the state object fields.
	float phi = s.phi;
	float *Phi = &phi;
	
	// Local copies of LFO states
	float lfo1 = s.lfo1 = q31_to_f32(params->shape_lfo);
	float lfo2 = s.lfo2 = lfo1 * p.freq_shift;
	float lfo1_last = s.lfo1_last;
	float lfo2_last = s.lfo2_last;
	const float lfo_inc_1 = (lfo1 - lfo1_last) / frames;
	const float lfo_inc_2 = (lfo2 - lfo2_last) / frames;
	
	
	// Prepare to fill buffer
	q31_t *__restrict y = (q31_t*)yn; // Pointer to start of buffer
	const q31_t *y_e = y + frames; // Pointer to end of buffer

	// Fill buffer with samples
	for (; y != y_e; ) { 
		float sig = 0.f;
		
		sig = vpsLJ.PhaseShaper(phi, lfo1_last, lfo2_last);
		
		// Update local LFO phases
		lfo1_last += lfo_inc_1;
		lfo2_last += lfo_inc_2;
		
		// Update local osc phase of primary osc
		vpsLJ.updatePhase(Phi, w0);
		
		// Softclip signal before sending to buffer
		sig = osc_softclipf(0.05f, sig);
		
		// Format in Q31 binary fixed-point representation and add to buffer
		*(y++) = f32_to_q31(sig); 
	}
	
	// Update global osc phase
	s.phi = phi;
	
	// Update local LFO phases
	s.lfo1_last = lfo1_last;
	s.lfo2_last = lfo1_last;
	
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

	VPSLJ::Params &p = vpsLJ.params;

	const float valf = param_val_to_f32(value);
	switch (index) {
		case k_user_osc_param_id1:  		// User parameter 1
			// Set values for d amplitude
			p.Amp_d = value * 0.01f;
			break;
			
		case k_user_osc_param_id2:			// User parameter 2
			// Set values for v amplitude
			p.Amp_v = value * 0.05f;
			break;
			
		case k_user_osc_param_id3:  		// User parameter 3
			// Set frequency scaling for LFO 2
			p.freq_shift = value;
			break;
			
		case k_user_osc_param_id4:			// User parameter 4
			break;
			
		case k_user_osc_param_id5:  		// User parameter 5
			break;
			
		case k_user_osc_param_id6:			// User parameter 6
			break;
			
		case k_user_osc_param_shape: 		// A knob
			break;
			
		case k_user_osc_param_shiftshape:	// B knob	
			break;
			
		default: break;
	}
}

