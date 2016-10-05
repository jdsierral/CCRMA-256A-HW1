/*
  ==============================================================================

    Oscillator.cpp
    Created: 3 Oct 2016 12:34:30am
    Author:  Juan David Sierra

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"

double Oscillator::sampleRate = 0.0;

Oscillator::Oscillator(float newGain, float newFreq)
: gain (newGain), freq(newFreq), angle(0.0), angleDelta(0.0)
{
}

void Oscillator::setSampleRate(double newSampleRate) {
	sampleRate = newSampleRate;
	setFreq(freq);
}

void Oscillator::setFreq(float newFreq) {
	const double cyclesPerSample = newFreq / sampleRate;
	angleDelta = cyclesPerSample * PIx2;
}

void Oscillator::setGain(float gainInDecibels) {
	gain = Decibels::decibelsToGain<float>(gainInDecibels, -60);
}

float Oscillator::tick(){
	float tempAng = angle;
	angle = (tempAng + angleDelta > PIx2 ? tempAng + angleDelta - PIx2 : tempAng + angleDelta);
	return std::sin(angle) * gain;
}

void Oscillator::clearAngle() {
	angle = 0;
}
