/*
 ==============================================================================
 
 MainProcessor.cpp
 Created: 3 Oct 2016 1:22:19am
 Author:  Juan David Sierra
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainProcessor.h"
#include "MainEditor.h"



//==============================================================================
//==============================================================================

MainProcessor::MainProcessor()
: mainGain(0.8), generalGain(1)
{
	//Initialization of Processor
	//MainGain is relative to mousePosition and controlled by user with it
	//GeneralGain is the volume of the application's output;
	
	//Initialize múltiple Oscilators
	for (int i = 0; i < totalNumOsc; i++) {
		osc[i] = new Oscillator(1, 200);
	}
	
	//Initialize Envelope
	envelope = new Enveloper(1.0f, 1.0f, 0.5f, 1.0f);
	
	//A couple of smoothers ofr the general gain on Left and Right channel
	smoothL = new Smooth();
	smoothR = new Smooth();
	
	//Creates another component that will contain all the GUI called editor
	//A pointer of this instance (MainProcessor) is passed to the editor
	//so that it can access all of its public methods and members
	editor = new MainEditor(this);
	addAndMakeVisible(editor);
	
	//Fixed Size application
	setSize (600, 600);
	
	//No inputs, two ouputs
	setAudioChannels (0, 2);
}

MainProcessor::~MainProcessor()
{
	//Delete stuff to avoid leakage
	
	editor = nullptr;
	for (int i = 0; i < totalNumOsc; i++) {
		delete osc[i];
	}
	
	shutdownAudio();
}

//==============================================================================
//==============================================================================

void MainProcessor::paint (Graphics& g)
{
}

void MainProcessor::resized()
{
	//The MainContentComponent will only have one GUI object called editor
	//all of the sliders and GUI will be inside this editor;
	editor->setBounds(0, 0, getWidth(), getHeight());
}


//==============================================================================
//==============================================================================

void MainProcessor::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	//Initialize sampleRate for oscilators, and evelope
	//setFixed Smoothing factor for smoothers
	for (int i = 0; i < totalNumOsc; i++) {
		osc[i]->setSampleRate(sampleRate);
	}
	envelope->setSampleRate(sampleRate);
	smoothL->setSmooth(0.99);
	smoothR->setSmooth(0.99);
}

void MainProcessor::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	//create two buffers for stereo sound though it isnt stereo yet
	float* const outL = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
	float* const outR = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
	
	
	//clear buffer so recurrent sum doesnt take into account possible garbage in buffer
	bufferToFill.clearActiveBufferRegion();
	
	for (int smpl = 0; smpl < bufferToFill.numSamples; smpl++) {
		//recurrent addition of different sinusoids, their own level is implemented insde
		//their own class as well as their freq managment.
		for (int i = 0; i < totalNumOsc; i++) {
			outL[smpl] += osc[i]->tick();
			outR[smpl] += osc[i]->tick();
		}
		
		//The sum of all the sinusoids is multipied by the mainGain (mousePosition)
		//by the generalGain (applications general Volume) and envelope
		//all of this is divided by the total number of oscilators to avoid distortion
		//although user must be careful as higher levels can be setup for generalGain
		//meaning overdriving the output is actually possible.
		//all of this is smoothed to avid ticks due to very fast envelope or gain manipulation
		//through mousePosition
		
		outL[smpl] *= smoothL->tick(envelope->get() * generalGain * mainGain/totalNumOsc);
		outR[smpl] *= smoothR->tick(envelope->get() * generalGain * mainGain/totalNumOsc);
	}
	
}

void MainProcessor::releaseResources()
{
}

//==============================================================================

void MainProcessor::setGeneralGain (float newGainInDecibels) {
	//all of the user input for gain is controlled in decibles
	
	//function to change generalGain
	generalGain = Decibels::decibelsToGain<float>(newGainInDecibels);
}

void MainProcessor::setMainGain (float newGainInDecibels) {
	//function to change mainGain
	mainGain = Decibels::decibelsToGain<float>(newGainInDecibels);
}

void MainProcessor::setFreq(float newFreq) {
	//setting the base Frequency will change all of the other frequencies preserving
	//musical relations to base frequency.
	
	//this is achieved by multiplying the base freq by the 12th root of 2 to the power
	//of the number of semitones desired relative to base freq.
	
	//higher precission tunning in cents is also possible achieved y mutiplying
	//the latter by the 1200th root of 2 to the power of the number of desired cents.
	
	//This is done for every sinusoid every time the base freq is changed via the mouse
	//position
	
	for (int i = 0; i < totalNumOsc; i++) {
		float freq = newFreq * std::powf(toneBase, tone[i]) * std::powf(tuneBase, tune[i]);
		osc[i]->setFreq(freq);
	}
}

void MainProcessor::setTone(float newTone, int oscNum) {
	//function to determine semitone distance of every partial to the base
	//sinusoid
	
	tone[oscNum] = newTone;
}

void MainProcessor::setTune(float newTune, int oscNum) {
	//function to determine the cents distance of every partial to the
	//semitone distance of every partial to the base freq
	tune[oscNum] = newTune;
}


//==============================================================================


Component* createMainProcessor()     { return new MainProcessor(); }
