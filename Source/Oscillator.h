/*
  ==============================================================================

    Oscillator.h
    Created: 3 Oct 2016 12:34:30am
    Author:  Juan David Sierra

  ==============================================================================
*/

#ifndef OSCILLATOR_H_INCLUDED
#define OSCILLATOR_H_INCLUDED
// Class to create oscilators

class Oscillator
{
public:
	
	Oscillator(float newGain, float newFreq);
	
	void setSampleRate (double newSampleRate);
	void setFreq (float newFreq);
	void setGain (float gain);
	float tick();
	
	void clearAngle ();
	
	
private:
	
	// notice static Double so that it changes in all
	// of the instances of this class during application live
	
	const double PIx2 = M_PI * 2;
	static double sampleRate;
	
	// each oscillator is also responsible for its own gain 
	
	float gain, freq;
	float angle, angleDelta;
	

};


#endif  // OSCILLATOR_H_INCLUDED
