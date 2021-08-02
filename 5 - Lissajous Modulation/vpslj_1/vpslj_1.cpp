/*
 * File: vpslj_1.cpp
 *
 * 
 * 2021 Graham Keane - Maynooth University
 *
 * Lissajous 2D LFO Modulation
 */

# include "userosc.h"
# include "vpslj_1.hpp"

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
	
	// Reset state if note-on trigger
	if(flags & VPSLJ2D::flag_reset) {
		s.reset();
	}
	
	// Current note being played.
	uint8_t note = params->pitch>>8;
	
	// Floating point phase increment for given note and fine modulation
	float w0 = s.w0 = osc_w0f_for_note(note, params->pitch & 0xFF);
	
	// Set pole for filters
	vpslj2d.setFilters(w0);
	
	// Update LFO frequency
	if(flags & VPSLJ2D::flag_LFO) {
		s.w0LFOD = p.w0LFO * k_samplerate_recipf;
		s.w0LFOV = p.w0LFO * k_samplerate_recipf;
	}
	
	// Update LFO1 frequency ratio
	if(flags & VPSLJ2D::flag_LFOD) {
		s.w0LFOV = p.w0LFO * k_samplerate_recipf;
		s.w0LFOD = p.freq_shift_D * s.w0LFOV;
	}
	
	// Update LFO2 frequency ratio
	if(flags & VPSLJ2D::flag_LFOV) {
		s.w0LFOD = p.w0LFO * k_samplerate_recipf;
		s.w0LFOV = p.freq_shift_V * s.w0LFOD;
	}
	
	// Local copies of the state object fields.
	float phi 		= s.phi;
	float *Phi 		= &phi;
	float phiLFOD 	= s.phiLFOD;
	float *ptrLFOD 	= &phiLFOD;
	float phiLFOV 	= s.phiLFOV;
	float *ptrLFOV 	= &phiLFOV;
	float w0LFOD 	= s.w0LFOD;
	float w0LFOV 	= s.w0LFOV;
  
	// Pointer to start of buffer
	q31_t *__restrict y = (q31_t*)yn; 
	
	// Pointer to end of buffer
	const q31_t *y_e = y + frames; 

	// Fill buffer with samples
	for (; y != y_e; ) { 
	
		// Compute waveform
		float sig = vpslj2d.PhaseShaper(phi, phiLFOD, phiLFOV);
		
		// Update phases:
		vpslj2d.updatePhase(Phi, w0);
		vpslj2d.updatePhase(ptrLFOD, w0LFOD);
		vpslj2d.updatePhase(ptrLFOV, w0LFOV);
		
		// Softclip signal before sending to buffer
		sig = osc_softclipf(0.0125f, sig);
		
		// Format in Q31 binary fixed-point representation and add to buffer
		*(y++) = f32_to_q31(sig); 
	
	}
	
	// Update global phases:
	s.phi 		= phi;
	s.phiLFOD 	= phiLFOD;
	s.phiLFOV 	= phiLFOV;
	
}

void OSC_NOTEON(const user_osc_param_t *const params) {
	// Reset oscillator phase for new note
	VPSLJ2D::State &s = vpslj2d.state;
	s.flags |= VPSLJ2D::flag_reset;
}

void OSC_NOTEOFF(const user_osc_param_t *const params) {
	// Not used	
	(void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value) { 
	
	// Create local copies of the state and params objects.
	VPSLJ2D::Params &p = vpslj2d.params;
	VPSLJ2D::State &s = vpslj2d.state;
	
	const float valf = param_val_to_f32(value);
	
	switch (index) {
		case k_user_osc_param_id1:  		// User parameter 1
			// Set base frequency of LFOs
			p.w0LFO = value * 0.3f; 
			s.flags |= VPSLJ2D::flag_LFO;
			break;
			
		case k_user_osc_param_id2:			// User parameter 2
			// Set frequency scaling of LFO-D
			p.freq_shift_D = 1.f + (value * 0.05f);
			s.flags |= VPSLJ2D::flag_LFOD;
			break;
			
		case k_user_osc_param_id3:  		// User parameter 3
			// Set frequency scaling of LFO-V
			p.freq_shift_V = 1.f + (value * 0.05f);
			s.flags |= VPSLJ2D::flag_LFOV;
			break;
			
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
			// Set the frequency ratio for filters
			p.f_ratio = 1.f + (valf * 100.f);
			break;
		
		case k_user_osc_param_shiftshape:	// B knob	
			break;
			
		default: break;
	}
}

