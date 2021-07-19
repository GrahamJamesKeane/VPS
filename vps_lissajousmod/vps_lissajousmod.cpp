/*
 * File: vps_lissajousmod.cpp
 *
 * 
 * 2021 Graham Keane - Maynooth University
 *
 * Lissajous 2D LFO Modulation
 */

# include "userosc.h"
# include "vps_lissajousmod.hpp"

static VPSLJ2D vpslj2d;

void OSC_INIT(uint32_t platform, uint32_t api) {
	(void)platform;
	(void)api;
}

void OSC_CYCLE(const user_osc_param_t *const params, int32_t *yn, const uint32_t frames){
	// Local copies of the state and params objects.
	VPSLJ2D::State &s = vpslj2d.state;
	const VPSLJ2D::Params &p = vpslj2d.params;
	
	// Current flag
	const uint32_t flags = s.flags; 
	// Reset s.flags
    s.flags = VPSLJ2D::flags_none; 
	
	// Current note being played.
	uint8_t note = params->pitch>>8;
	
	// Floating point phase increment for given note and fine modulation
	float w0 = s.w0 = osc_w0f_for_note(note, params->pitch & 0xFF);
	
	// Update LFO frequency
	if(flags & VPSLJ2D::flag_LFO) {
		s.w0LFO1 = p.w0LFO * k_samplerate_recipf;
		s.w0LFO2 = p.w0LFO * k_samplerate_recipf;
	}
	// Update LFO1 frequency ratio
	if(flags & VPSLJ2D::flag_LFO1) {
		s.w0LFO2 = p.w0LFO * k_samplerate_recipf;
		s.w0LFO1 = p.freq_shift_1 * s.w0LFO2;
	}
	// Update LFO2 frequency ratio
	if(flags & VPSLJ2D::flag_LFO2) {
		s.w0LFO1 = p.w0LFO * k_samplerate_recipf;
		s.w0LFO2 = p.freq_shift_2 * s.w0LFO1;
	}
	
	// Local copies of the state object fields.
	float phi 		= s.phi;
	float *Phi 		= &phi;
	float phiLFO1 	= s.phiLFO1;
	float *ptrLFO1 	= &phiLFO1;
	float phiLFO2 	= s.phiLFO2;
	float *ptrLFO2 	= &phiLFO2;
	float w0LFO1 	= s.w0LFO1;
	float w0LFO2 	= s.w0LFO2;
  
	// Prepare to fill buffer
	q31_t *__restrict y = (q31_t*)yn; // Pointer to start of buffer
	const q31_t *y_e = y + frames; // Pointer to end of buffer

	// Fill buffer with samples
	for (; y != y_e; ) { 
		float sig = 0.f;
		
		sig = vpslj2d.PhaseShaper(phi, phiLFO1, phiLFO2);
		
		// Update phases:
		vpslj2d.updatePhase(Phi, w0);
		vpslj2d.updatePhase(ptrLFO1, w0LFO1);
		vpslj2d.updatePhase(ptrLFO2, w0LFO2);
		
		// Softclip signal before sending to buffer
		sig = osc_softclipf(0.05f, sig);
		
		// Format in Q31 binary fixed-point representation and add to buffer
		*(y++) = f32_to_q31(sig); 
	
	}
	
	// Update global phases:
	s.phi 		= phi;
	s.phiLFO1 	= phiLFO1;
	s.phiLFO2 	= phiLFO2;
	
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
	
	// Create local copies of the state and params objects.
	VPSLJ2D::Params &p = vpslj2d.params;
	VPSLJ2D::State &s = vpslj2d.state;
	
	switch (index) {
		case k_user_osc_param_id1:  		// User parameter 1
			// Select Wave type
			p.type = value;
			break;
			
		case k_user_osc_param_id2:			// User parameter 2
			// Set frequency scaling of LFO 1
			{
				p.freq_shift_1 = 1.f + (value * 0.05f);
				s.flags |= VPSLJ2D::flag_LFO1;
			} break;
			
		case k_user_osc_param_id3:  		// User parameter 3
			// Set frequency scaling of LFO 2
			{
				p.freq_shift_2 = 1.f + (value * 0.05f);
				s.flags |= VPSLJ2D::flag_LFO2;
			} break;
			
		case k_user_osc_param_id4:			// User parameter 4
			// Set values for d amplitude
			p.Amp_d = value * 0.01f;
			break;
			
		case k_user_osc_param_id5:  		// User parameter 5
			// Set values for v amplitude
			p.Amp_v = value * 0.05f;
			break;
			
		case k_user_osc_param_id6:			// User parameter 6
			// Set offset value
			p.offset = value * 0.01f;
			break;
			
		case k_user_osc_param_shape: 		// A knob
			// Set base frequency of LFOs
			p.w0LFO = value * 0.3f; 
			s.flags |= VPSLJ2D::flag_LFO;
			break;
		
		case k_user_osc_param_shiftshape:	// B knob	
			break;
			
		default: break;
	}
}

