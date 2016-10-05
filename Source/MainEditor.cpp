/*
 ==============================================================================
 
 MainEditor.cpp
 Created: 3 Oct 2016 12:31:03am
 Author:  Juan David Sierra
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainEditor.h"

//==============================================================================

// This class will hold the editor or gui of the application.

// Notice that the constructor requires a pointer to the processor so that it
// can access all of its public members easily

MainEditor::MainEditor(MainProcessor* p)
: processor(p)
{
	// Setting and initalizing sliders and buttons
	// changing their range, color, textBox properties.
	// AddingListeners for them which will live inside this same object
	// Finally their value is initailized and therefore the remaining
	// uninitized values on processor are initialized via this callback
	
	addAndMakeVisible (mainGainSlider = new Slider ("Main Gain Slider"));
	mainGainSlider->setRange (-60, 12, 0);
	mainGainSlider->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
	mainGainSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
	mainGainSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
	mainGainSlider->setColour (Slider::rotarySliderOutlineColourId, Colour (0x00000000));
	mainGainSlider->setColour (Slider::textBoxTextColourId, Colours::white);
	mainGainSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00ffffff));
	mainGainSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
	mainGainSlider->addListener (this);
	mainGainSlider->setValue(-6);
	mainGainSlider->setDoubleClickReturnValue(true, -6);
	
	addAndMakeVisible (octaSlider = new Slider ("Octave Slider"));
	octaSlider->setRange (-5, 5, 1);
	octaSlider->setSliderStyle (Slider::IncDecButtons);
	octaSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
	octaSlider->setColour (Slider::textBoxTextColourId, Colours::black);
	octaSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
	octaSlider->addListener (this);
	octaSlider->setValue(0);
	
	
	// loop setting for repeated sliders
	// note int = 1 as the base freq of the synth cant be changed in transposition
	// level or tunning.
	// therefore some pointers related to osc[0] are never initialized, nevertheless
	// they are just pointers so this waste of memory is justified to have a better
	// coding
	
	for (int i = 1; i < totalNumOsc; i++) {
		addAndMakeVisible (gainSlider[i] = new Slider ("Gain Slider" + String(i)));
		gainSlider[i]->setRange (-60, 12, 0.1);
		gainSlider[i]->setSliderStyle (Slider::LinearHorizontal);
		gainSlider[i]->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
		gainSlider[i]->setColour (Slider::thumbColourId, Colours::white);
		gainSlider[i]->setColour (Slider::textBoxTextColourId, Colours::white);
		gainSlider[i]->setColour (Slider::textBoxBackgroundColourId, Colour (0x00ffffff));
		gainSlider[i]->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
		gainSlider[i]->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
		gainSlider[i]->setColour (Slider::rotarySliderOutlineColourId, Colour (0x00000000));
		gainSlider[i]->addListener (this);
		gainSlider[i]->setValue(-6 * i);
		gainSlider[i]->setDoubleClickReturnValue(true, 0);
		
		addAndMakeVisible (toneSlider[i] = new Slider ("Tone Slider" + String(i)));
		toneSlider[i]->setRange (-48, 48, 1);
		toneSlider[i]->setSliderStyle (Slider::IncDecButtons);
		toneSlider[i]->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
		toneSlider[i]->setColour (Slider::thumbColourId, Colours::white);
		toneSlider[i]->setColour (Slider::textBoxTextColourId, Colours::white);
		toneSlider[i]->setColour (Slider::textBoxBackgroundColourId, Colour (0x00ffffff));
		toneSlider[i]->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
		toneSlider[i]->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
		toneSlider[i]->setColour (Slider::rotarySliderOutlineColourId, Colour (0x00000000));
		toneSlider[i]->addListener (this);
		toneSlider[i]->setValue(6 * i);
		toneSlider[i]->setDoubleClickReturnValue(true, 0);
		
		addAndMakeVisible (tuneSlider[i] = new Slider ("Tune" + String(i)));
		tuneSlider[i]->setRange (-100, 100, 1);
		tuneSlider[i]->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
		tuneSlider[i]->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
		tuneSlider[i]->setColour (Slider::thumbColourId, Colours::white);
		tuneSlider[i]->setColour (Slider::textBoxTextColourId, Colours::white);
		tuneSlider[i]->setColour (Slider::textBoxBackgroundColourId, Colour (0x00ffffff));
		tuneSlider[i]->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
		tuneSlider[i]->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
		tuneSlider[i]->setColour (Slider::rotarySliderOutlineColourId, Colour (0x00000000));
		tuneSlider[i]->addListener (this);
		tuneSlider[i]->setValue(10 * i * pow(-1, i));
		tuneSlider[i]->setDoubleClickReturnValue(true, 0);
	}
	
	// Note asdr sliders are actually in millis
	
	addAndMakeVisible (attackSlider = new Slider ("Attack Slider"));
	attackSlider->setRange (0, 500, 1);
	attackSlider->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
	attackSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
	attackSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
	attackSlider->setColour (Slider::textBoxTextColourId, Colours::white);
	attackSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00ffffff));
	attackSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x00000000));
	attackSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
	attackSlider->addListener (this);
	attackSlider->setValue(100);
	
	addAndMakeVisible (decaySlider = new Slider ("Decay Slider"));
	decaySlider->setRange (0, 500, 1);
	decaySlider->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
	decaySlider->setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
	decaySlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
	decaySlider->setColour (Slider::textBoxTextColourId, Colours::white);
	decaySlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00ffffff));
	decaySlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
	decaySlider->addListener (this);
	decaySlider->setValue(10);
	
	addAndMakeVisible (sustainSlider = new Slider ("Sustain Slider"));
	sustainSlider->setRange (-60, 0, 1);
	sustainSlider->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
	sustainSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
	sustainSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
	sustainSlider->setColour (Slider::textBoxTextColourId, Colours::white);
	sustainSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00ffffff));
	sustainSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
	sustainSlider->addListener (this);
	sustainSlider->setValue(-20);
	
	addAndMakeVisible (releaseSlider = new Slider ("Release Slider"));
	releaseSlider->setRange (0, 2000, 1);
	releaseSlider->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
	releaseSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
	releaseSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
	releaseSlider->setColour (Slider::textBoxTextColourId, Colours::white);
	releaseSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00ffffff));
	releaseSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
	releaseSlider->addListener (this);
	releaseSlider->setValue(100);
	
	
	
}

MainEditor::~MainEditor()
{
	//deallocation of resources to avoid leakage
	
	for (int i = 1; i < totalNumOsc; i++) {
		delete gainSlider[i];
		delete toneSlider[i];
		delete tuneSlider[i];
	}
	
	mainGainSlider = nullptr;
	octaSlider = nullptr;
	attackSlider = nullptr;
	decaySlider = nullptr;
	sustainSlider = nullptr;
	releaseSlider = nullptr;
}

void MainEditor::paint (Graphics& g)
{
	
	// graphic stuff as background color, some textfields and keyboard
	
	g.fillAll (Colour (0xff202020));
	
	g.setColour (Colours::white);
	g.setFont (Font ("Gurmukhi MN", 37.10f, Font::plain));
	g.drawText (TRANS("Add Synth"),
				68, 31, 129, 73,
				Justification::centred, true);
	
	g.setColour (Colours::white);
	g.setFont (Font ("Gurmukhi MN", 15.60f, Font::plain));
	g.drawText (TRANS("JuanSaudio"),
				76, 79, 105, 30,
				Justification::centred, true);
	
	Colour dark(Colour(0xff373f40));
	Colour light(Colour(0xff475153));
	
	g.setColour (Colour (light));
	g.fillRect (playSpaceX, playSpaceY, playSpaceWidth, playSpaceHeight);
	
	for (int i = 0; i <= 12; i++) {
		if (i == 1 || i == 3 || i == 6 || i == 8 || i == 10) {
			g.setColour (Colour (dark));
			g.fillRect (playSpaceX + i * playSpaceWidth/13,
						playSpaceY,
						playSpaceWidth/13,
						playSpaceHeight);
		}
	}
}

void MainEditor::resized()
{
	
	// although all the resized code is static
	// the application is not resizable so it is actually not
	// required
		mainGainSlider->setBounds (48, 128, 160, 160);
		octaSlider->setBounds (542, 304, 40, 48);
	
	// sliders positions are defined in for loop
	for (int i = 1; i < totalNumOsc; i++) {
		gainSlider[i]->setBounds(248, 38 + 64 * (i -1), 120, 56);
		toneSlider[i]->setBounds(384, 46 + 64 * (i -1), 40, 48);
		tuneSlider[i]->setBounds(448, 48 + 64 * (i -1), 40, 48);
	}
	
		attackSlider->setBounds (512, 56, 40, 48);
		decaySlider->setBounds (512, 112, 40, 48);
		sustainSlider->setBounds (512, 168, 40, 48);
		releaseSlider->setBounds (512, 224, 40, 48);
}

void MainEditor::sliderValueChanged(Slider* slider) {
	
	// This callback function call is called whenever any slider is changed
	// comparisson makes possible to define which slider was changed
	// Notice how each slider is accessing a member of mainProcessor object
	// Making a neat structure of GUI vs. Processor type
	
	if (slider == mainGainSlider) {
		processor->setGeneralGain(slider->getValue());
	}
	
	if (slider == octaSlider) {
	}
	
	for (int i = 1; i < totalNumOsc; i++) {
		if (slider == gainSlider[i]) {
			processor->osc[i]->setGain(slider->getValue());
		}
		if (slider == toneSlider[i]) {
			processor->setTone(slider->getValue(), i);
		}
		
		if (slider == tuneSlider[i]) {
			processor->setTune(slider->getValue(), i);
		}
	}
	
	if (slider == attackSlider) {
		processor->envelope->setAttackTime(slider->getValue()/1000.0f);
	}
	
	if (slider == decaySlider) {
		processor->envelope->setDecayTime(slider->getValue()/1000.0f);
	
	}
	
	if (slider == sustainSlider) {
		processor->envelope->setSustainLevel(slider->getValue());
		
	}
	
	if (slider == releaseSlider) {
		processor->envelope->setReleaseTime(slider->getValue()/1000.0f);
	}
	
}

void MainEditor::mouseDown(const MouseEvent& e) {
	
	// mouseListener function called whenever click is done
	// if mouse is inside keyboard box call envelope start and mouseDrag who will
	// take care of frequency setup and mainGain setup
 
	if (e.position.x >= playSpaceX && e.position.x <= playSpaceX + playSpaceWidth) {
		if (e.position.y >= playSpaceY && e.position.y <= playSpaceY + playSpaceHeight) {
			processor->envelope->start();
			mouseDrag(e);
		}
	}
}

void MainEditor::mouseUp(const MouseEvent& e) {
	// if mouse button is released, enveope's stop function is called
	
	processor->envelope->stop();
}

void MainEditor::mouseDrag(const MouseEvent& e) {
	
	// the keyboard area is used to simulate a notes
	// its width is scaled from 0 to 13 to have a full octave of midi notes
	// then everything is shifted by 60 midiNotes to get to C4
	// the octave slider is then used to add or substract 12 and therefore
	// change to other octaves in keyboard range.
	
	int midiNote = (int)13 * (e.position.x - playSpaceX)/playSpaceWidth + 60 + 12 * octaSlider->getValue();
	
	// notice how the setFreq method receives the midiNote transformation into Hz
	// therefore frequencies are actually discrete on the processor.
	
	processor->setFreq(MidiMessage::getMidiNoteInHertz(midiNote));
	
	// level is set in relation to the keyboard area's height directly mapping it
	// to 0 to 1;
	int level = 20 * (float)((e.position.y - playSpaceY)/playSpaceHeight) - 20;
	processor->setMainGain(level);
}


