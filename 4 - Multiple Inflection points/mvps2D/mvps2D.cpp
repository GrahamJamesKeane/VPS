/*
 * File: mvps2D.cpp
 * 
 * 2021 Graham Keane - Maynooth University
 *
 * Multiple inflection points with 2D LFO modulation (d and v values)
 */

#include "userosc.h"
#include "mvps2D.hpp"

static MVPS2D mvps2D;

void OSC_INIT(uint32_t platform, uint32_t api) {
	(void)platform;
	(void)api;
}

void OSC_CYCLE(const user_osc_param_t *const params, int32_t *yn, const uint32_t frames){
	
	// Local copy of the state object.
	MVPS2D::State &s = mvps2D.state;
	
	// Current flag
	const uint32_t flags = s.flags; 
	
	// Reset s.flags
    s.flags = MVPS2D::flags_none; 
	
	// Reset state if note-on trigger
	if(flags & MVPS2D::flag_reset) {
		s.reset();
	}
	
	// Current note being played.
	uint8_t note = params->pitch>>8;
	
	// Floating point phase increment for given note and fine modulation
	float w0 = s.w0 = osc_w0f_for_note(note, params->pitch & 0xFF);
	
	// Set pole for filters
	mvps2D.setFilters(w0);
	
	// Local copy of the phase and its pointer.
	float phi 	= s.phi;
	float *Phi 	= &phi;
	
	// LFO1
	float lfo1 		= s.lfo1 = q31_to_f32(params->shape_lfo);
	float lfo1_last = s.lfo1_last;
	float lfo1_inc 	= (lfo1 - lfo1_last) / frames;
	
	// LFO2
	float w0_lfo2 	= s.w0_lfo2;
	float phi_lfo2 	= s.phi_lfo2;
	float *ptrLFO2 	= &phi_lfo2;
	
	// Start buffer
	q31_t *__restrict y = (q31_t*)yn; 	
	
	// End buffer
	const q31_t *y_e = y + frames; 		

	// Fill buffer with samples
	for (; y != y_e; ) { 
		
		float lfo2 = osc_cosf(phi_lfo2);
		
		// Compute waveform
		float sig = mvps2D.MultiPhaseShaper(phi, lfo1_last, lfo2);
		
		// Update local phases
		mvps2D.updatePhase(Phi, w0);
		mvps2D.updatePhase(ptrLFO2, w0_lfo2);
		
		// Update LFO1
		lfo1_last += lfo1_inc;
		
		// Softclip signal before sending to buffer
		sig = osc_softclipf(0.05f, sig);
		
		// Format in Q31 binary fixed-point representation and add to buffer
		*(y++) = f32_to_q31(sig); 
	
	}
	
	// Update global values
	s.phi 		= phi;
	s.phi_lfo2 	= phi_lfo2;
	s.lfo1_last = lfo1_last;
}

void OSC_NOTEON(const user_osc_param_t *const params) {
	// Reset oscillator phase for new note
	MVPS2D::State &s = mvps2D.state;
	s.flags |= MVPS2D::flag_reset;
}

void OSC_NOTEOFF(const user_osc_param_t *const params) {
	// Not used	
	(void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value) { 

	MVPS2D::Params &p = mvps2D.params;
	MVPS2D::State &s = mvps2D.state;
	
	const float valf = param_val_to_f32(value);
	
	switch (index) {
		case k_user_osc_param_id1: // User parameter 1
			// Set Values for Inflection Point 1: d0
			p.d0 = clip01f(value * 0.01f); 		
			break;
			
		case k_user_osc_param_id2: // User parameter 2
			// Set Values for Inflection Point 1: v0
			p.v0 = clip01f(value * 0.01f); 
			break;
			
		case k_user_osc_param_id3: // User parameter 3
			// Set Values for Inflection Point 2: d1
			p.d1 = clip01f(value * 0.01f); 
			break;
			
		case k_user_osc_param_id4: // User parameter 4
			// Set Values for Inflection Point 2: v1
			p.v1 = clip01f(value * 0.01f); 
			break;
			
		case k_user_osc_param_id5: // User parameter 5
			// Set Values for Inflection Point 3: d2
			p.d2 = clip01f(value * 0.01f);			
			break;
			
		case k_user_osc_param_id6: // User parameter 6
			// Set Values for Inflection Point 3: v2
			p.v2 = clip01f(value * 0.01f); 
			break;
			
		case k_user_osc_param_shape: // A knob
			// Set the frequency ratio for filters
			p.f_ratio = 1.f + (valf * 100.f);
			break;
			
		case k_user_osc_param_shiftshape: // B knob	
			// Frequency of LFO2
			s.w0_lfo2 = (valf * 30.f) * k_samplerate_recipf;
			break;
			
		default: break;
	}
}

