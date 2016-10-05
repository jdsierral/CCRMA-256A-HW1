/*
 ==============================================================================
 
 MainProcessor.h
 Created: 3 Oct 2016 1:22:19am
 Author:  Juan David Sierra
 
 ==============================================================================
 */

#ifndef MAINPROCESSOR_H_INCLUDED
#define MAINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"
#include "Enveloper.h"
#include "Smooth.h"

//==============================================================================
/*
 */
//Really nice trick to avoid circular reference by including MainEditor.h
//The header of the processor actually only needs to know that there is a
//valid definition of a class called mainEditor, although they are not
//actually the same class.
//On the .cpp file the editor pointer will be initialized via the other
//class therefore making the editor object a true MainEditor object

class MainEditor;

class MainProcessor   : public AudioAppComponent
{

public:
	//const avoid changing this number throughout the application
	const static int totalNumOsc = 5;
	
	//==============================================================================
	//==============================================================================
	// Constructor and Destructor//
	
	MainProcessor();
	~MainProcessor();
	
	//==============================================================================
	//==============================================================================
	// GUI related functions, not very much in here as they will be managed by the
	// MainEditor Class//
	
	void paint (Graphics& g) override;
	void resized() override;
	
	//==============================================================================
	//==============================================================================
	// Main Functions for audio processing//
	
	void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	
	//==============================================================================
	//==============================================================================
	// Setters for members of this class//
	
	void setGeneralGain (float newGainInDecibels);
	void setMainGain (float newGain);
	void setFreq (float newFreq);
	void setTone (float newTone, int oscNum);
	void setTune (float newTune, int oscNum);
	
	//==============================================================================
	//==============================================================================
	// Some public objects for other parts in the applicaton (speccially the editor
	// object can access them and change their states
	
	Oscillator* osc[totalNumOsc];
	ScopedPointer<Enveloper> envelope;
	ScopedPointer<Smooth> smoothL;
	ScopedPointer<Smooth> smoothR;
	
private:
	
	//==============================================================================
	//==============================================================================
	// Important constants for frequency calculations
	// 12th root of 2 to achieve semitone transposition called toneBase
	// 12000 root of 2 to achieve cent transposition called tuneBase
	
	
	const float toneBase = std::powf(2.0f, 1/12.0f);
	const float tuneBase = std::powf(2.0f, 1/1200.0f);
	
	//==============================================================================
	//==============================================================================
	// General Fields for this class. note that tone[] and tune[] require
	// totalNumOsc to be a static member
	
	float mainGain, generalGain;
	float tone[totalNumOsc];
	float tune[totalNumOsc];
	
	//==============================================================================
	//==============================================================================
	// Pointer to object required for holding the GUI
	// ScopedPointer is used to avoid leakage.
	
	ScopedPointer<MainEditor> editor;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainProcessor)
};

#endif  // MAINPROCESSOR_H_INCLUDED
