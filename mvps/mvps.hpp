/*
 * File: mvps.hpp
 *
 * 2021 Graham Keane - Maynooth University
 *
 * Multiple Inflection Points
 */
 
#pragma once

#include "userosc.h"

#define ZEROF 0.f
#define DEFAULT 0.5f

struct VPS {
	
	struct Params {
		float   shape;
		float 	x0;	// Inflection point 1: x-axis 
		float 	x1; // Inflection point 2: x-axis 
		float 	x2; // Inflection point 3: x-axis 
		float 	y0; // Inflection point 1: y-axis 
		float 	y1; // Inflection point 2: y-axis 
		float 	y2; // Inflection point 3: y-axis 
		
		Params(void) :
			shape(DEFAULT),
			x0(DEFAULT),
			x1(DEFAULT),
			x2(DEFAULT),
			y0(DEFAULT),
			y1(DEFAULT),
			y2(DEFAULT)
		{ }
	};
  
	struct State {
		float   phi; 		// Oscillator phase
		float   w0;			// Oscillator pitch
		State(void) :
			phi(ZEROF),
			w0(ZEROF)
		{}
	};

	VPS(void) {
		state = State();
		params = Params();
	}
  
	inline float MultiPhaseShaper(float phi) {
		float x0 = params.x0 * params.shape;
		float y0 = params.y0 * params.shape;
		float x1 = params.x1 * params.shape;
		float y1 = params.y1 * params.shape;
		float x2 = params.x2 * params.shape;
		float y2 = params.y2 * params.shape;
		
		float ps;
		if(phi < x0) {
			float fraction = phi / x0;
			ps = y0 * fraction;
		} else if(phi < x1){
			float factor = y1 - y0;
			float numerator = phi - x0;
			float denominator = x1 - x0;
			float fraction = numerator / denominator;
			ps = factor * fraction + y0;
		} else if(phi < x2) {
			float factor = y2 - y1;
			float numerator = phi - x1;
			float denominator = x2 - x1;
			float fraction = numerator / denominator;
			ps = factor * fraction + y1;
		} else {
			float factor = 1.f - y2;
			float numerator = phi - x2;
			float denominator = 1.f - x2;
			float fraction = numerator / denominator;
			ps = factor * fraction + y2;
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
