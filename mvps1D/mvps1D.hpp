/*
 * File: mvps1D.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Multiple Inflection Points with x-axis LFO modulation
 */
 
#pragma once

#include "userosc.h"

#define ZEROF 0.f
#define DEFAULT 0.5f

struct MVPS1D {
	
	struct Params {
		float   shape;			// Scale d values
		float 	shiftshape; 	// Scale v values
		float 	d0;				// Inflection point 1: d0 
		float 	d1; 			// Inflection point 2: d1 
		float 	d2; 			// Inflection point 3: d2 
		float 	v0; 			// Inflection point 1: v0
		float 	v1; 			// Inflection point 2: v1
		float 	v2; 			// Inflection point 3: v2
		
		Params(void) :
			shape(DEFAULT),
			shiftshape(DEFAULT),
			d0(DEFAULT),
			d1(DEFAULT),
			d2(DEFAULT),
			v0(DEFAULT),
			v1(DEFAULT),
			v2(DEFAULT)
		{ }
	};
  
	struct State {
		float   phi; 		// Oscillator phase
		float   w0;			// Oscillator pitch
		float 	lfo;
		State(void) :
			phi(ZEROF),
			w0(ZEROF),
			lfo(ZEROF)
		{}
	};

	MVPS1D(void) {
		state = State();
		params = Params();
	}
  
	inline float MultiPhaseShaper(float phi, float lfo) {
		float d0 = params.d0 * params.shape + lfo;
		float v0 = params.v0 * params.shiftshape;
		float d1 = params.d1 * params.shape + lfo;
		float v1 = params.v1 * params.shiftshape;
		float d2 = params.d2 * params.shape + lfo;
		float v2 = params.v2 * params.shiftshape;
		
		float ps;
		if(phi < d0) {
			float fraction = phi / d0;
			ps = v0 * fraction;
		} else if(phi < d1){
			float factor = v1 - v0;
			float numerator = phi - d0;
			float denominator = d1 - d0;
			float fraction = numerator / denominator;
			ps = factor * fraction + v0;
		} else if(phi < d2) {
			float factor = v2 - v1;
			float numerator = phi - d1;
			float denominator = d2 - d1;
			float fraction = numerator / denominator;
			ps = factor * fraction + v1;
		} else {
			float factor = 1.f - v2;
			float numerator = phi - d2;
			float denominator = 1.f - d2;
			float fraction = numerator / denominator;
			ps = factor * fraction + v2;
		}
		
		return -osc_cosf(ps);
	}
	
	inline float updatePhase(float *phi, float w0) {
		*phi += w0;
		*phi -= (uint32_t)*phi;
	}

	State 	state;
	Params 	params;
};
