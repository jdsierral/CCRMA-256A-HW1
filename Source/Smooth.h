/*
  ==============================================================================

    smooth.h
    Created: 3 Oct 2016 9:21:47pm
    Author:  Juan David Sierra

  ==============================================================================
*/

#ifndef SMOOTH_H_INCLUDED
#define SMOOTH_H_INCLUDED

// Just regular smooth class used in class

class Smooth{
private:
	double delay,s;
	
public:
	Smooth():delay(0.0),s(0.0){}
	
	~Smooth(){}
	
	// set the smoothing (pole)
	void setSmooth(double smooth){
		s = smooth;
	}
	
	// compute one sample
	double tick(double input){
		double currentSample = input * (1.0-s);
		currentSample = currentSample + delay;
		delay = currentSample * s;
		return currentSample;
	}
};

#endif  // SMOOTH_H_INCLUDED
