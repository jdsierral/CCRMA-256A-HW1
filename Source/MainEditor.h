/*
  ==============================================================================

    MainEditor.h
    Created: 3 Oct 2016 12:31:03am
    Author:  Juan David Sierra

  ==============================================================================
*/

#ifndef MAINEDITOR_H_INCLUDED
#define MAINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainProcessor.h"

//==============================================================================
/*
	This class contain all of the GUI it is attached to the
	processor through a pointer
*/
class MainEditor    :	public Component,
						public SliderListener
{
public:
	
//==============================================================================
//==============================================================================
//	Constructor and destructor
	
    MainEditor(MainProcessor* p0);
    ~MainEditor();

//==============================================================================
//==============================================================================
//	Graphics management


    void paint (Graphics&) override;
    void resized() override;
	
//==============================================================================
//==============================================================================
//	Listeners for Sliders and mouse
	
	void sliderValueChanged(Slider* slider) override;
	
	void mouseDown (const MouseEvent& e) override;
	void mouseDrag (const MouseEvent& e) override;
	void mouseUp (const MouseEvent& e) override;

private:
	
//	some constant important for the editor
	
	const static int totalNumOsc = 5;
	
	float	playSpaceX = 20;
	float	playSpaceY = 303;
	float	playSpaceWidth = 564;
	float	playSpaceHeight = 281;

//	all of the objects in the editor.
//	Notice how all of them are pointers to manage their properties
//	during initailization
	
	MainProcessor* processor;

	Slider* gainSlider[totalNumOsc];
	Slider* toneSlider[totalNumOsc];
	Slider* tuneSlider[totalNumOsc];
	ScopedPointer<Slider> octaSlider;
	ScopedPointer<Slider> mainGainSlider;
	ScopedPointer<Slider> attackSlider;
	ScopedPointer<Slider> decaySlider;
	ScopedPointer<Slider> sustainSlider;
	ScopedPointer<Slider> releaseSlider;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainEditor)
};


#endif  // MAINEDITOR_H_INCLUDED
