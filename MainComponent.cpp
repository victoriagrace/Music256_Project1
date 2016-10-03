// Music 256a / CS 476a | fall 2016
// CCRMA, Stanford University
//
// Author: Romain Michon (rmichonATccrmaDOTstanfordDOTedu)
// Description: Simple JUCE sine wave synthesizer

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sine.h"

class SineComponents
{
public:
    SineComponents(): gain (0.0), onOff (0) {
        
    }
    Slider frequencySlider;
    Slider gainSlider;
    
    ToggleButton onOffButton;
    
    Label frequencyLabel, gainLabel, onOffLabel;
    
    Sine sine; // the sine wave oscillator
    
    // Global Variables
    float gain;
    int onOff, samplingRate, nChans;

};

class MainContentComponent :
    public AudioAppComponent,
    private Slider::Listener,
    private ToggleButton::Listener
{
public:
    MainContentComponent() : samplingRate(0.0)
    {
        // configuring frequency slider and adding it to the main window
        addAndMakeVisible (sc[0].frequencySlider);
        sc[0].frequencySlider.setRange (50.0, 5000.0);
        sc[0].frequencySlider.setSkewFactorFromMidPoint (500.0);
        sc[0].frequencySlider.setValue(1000); // will also set the default frequency of the sine osc
        sc[0].frequencySlider.addListener (this);
        
        // configuring frequency label box and adding it to the main window
        addAndMakeVisible(sc[0].frequencyLabel);
        sc[0].frequencyLabel.setText ("Frequency", dontSendNotification);
        sc[0].frequencyLabel.attachToComponent (&sc[0].frequencySlider, true);
        
        
        // configuring gain slider and adding it to the main window
        addAndMakeVisible (sc[0].gainSlider);
        sc[0].gainSlider.setRange (0.0, 1.0);
        sc[0].gainSlider.setValue(0.5); // will alsi set the default gain of the sine osc
        sc[0].gainSlider.addListener (this);
        
        
        // configuring gain label and adding it to the main window
        addAndMakeVisible(sc[0].gainLabel);
        sc[0].gainLabel.setText ("Gain", dontSendNotification);
        sc[0].gainLabel.attachToComponent (&sc[0].gainSlider, true);
        
        
        // configuring on/off button and adding it to the main window
        addAndMakeVisible(sc[0].onOffButton);
        sc[0].onOffButton.addListener(this);
        
        
        // configuring on/off label and adding it to the main window
        addAndMakeVisible(sc[0].onOffLabel);
        sc[0].onOffLabel.setText ("On/Off", dontSendNotification);
        sc[0].onOffLabel.attachToComponent (&sc[0].onOffButton, true);
        
        setSize (600, 100);
        nChans = 1;
        setAudioChannels (0, nChans); // no inputs, one output
    }
    
    ~MainContentComponent()
    {
        shutdownAudio();
    }
    
    void resized() override
    {
        // placing the UI elements in the main window
        // getWidth has to be used in case the window is resized by the user
        const int sliderLeft = 80;
        sc[0].frequencySlider.setBounds (sliderLeft, 10, getWidth() - sliderLeft - 20, 20);
        sc[0].gainSlider.setBounds (sliderLeft, 40, getWidth() - sliderLeft - 20, 20);
        sc[0].onOffButton.setBounds (sliderLeft, 70, getWidth() - sliderLeft - 20, 20);
    }
    
    void sliderValueChanged (Slider* slider) override
    {
        if (samplingRate > 0.0){
            if (slider == &sc[0].frequencySlider){
                sc[0].sine.setFrequency(sc[0].frequencySlider.getValue());
            }
            else if (slider == &sc[0].gainSlider){
                sc[0].gain = sc[0].gainSlider.getValue();
            }
        }
    }
    
    void buttonClicked (Button* button) override
    {
        // turns audio on or off
        if(button == &sc[0].onOffButton && sc[0].onOffButton.getToggleState()){
            sc[0].onOff = 1;
        }
        else{
            sc[0].onOff = 0;
        }
    }
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        samplingRate = sampleRate;
        sc[0].sine.setSamplingRate(sampleRate);
    }
    
    void releaseResources() override
    {
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // getting the audio output buffer to be filled
        float* const buffer = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        
        // computing one block
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            if(sc[0].onOff == 1) buffer[sample] = sc[0].sine.tick() * sc[0].gain;
            
           // if(onOff == 1) buffer[sample] = (float) rand()/RAND_MAX * gain;

            else buffer[sample] = 0.0;
        }
    }
    
    
private:

    SineComponents sc[1];

    int samplingRate, nChans;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
