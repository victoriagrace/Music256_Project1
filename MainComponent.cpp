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
        addAndMakeVisible (sc.frequencySlider);
        sc.frequencySlider.setRange (50.0, 5000.0);
        sc.frequencySlider.setSkewFactorFromMidPoint (500.0);
        sc.frequencySlider.setValue(1000); // will also set the default frequency of the sine osc
        sc.frequencySlider.addListener (this);
        
        // configuring frequency label box and adding it to the main window
        addAndMakeVisible(sc.frequencyLabel);
        sc.frequencyLabel.setText ("Frequency", dontSendNotification);
        sc.frequencyLabel.attachToComponent (&sc.frequencySlider, true);
        
        
        // configuring gain slider and adding it to the main window
        addAndMakeVisible (sc.gainSlider);
        sc.gainSlider.setRange (0.0, 1.0);
        sc.gainSlider.setValue(0.5); // will alsi set the default gain of the sine osc
        sc.gainSlider.addListener (this);
        
        
        // configuring gain label and adding it to the main window
        addAndMakeVisible(sc.gainLabel);
        sc.gainLabel.setText ("Gain", dontSendNotification);
        sc.gainLabel.attachToComponent (&sc.gainSlider, true);
        
        
        // configuring on/off button and adding it to the main window
        addAndMakeVisible(sc.onOffButton);
        sc.onOffButton.addListener(this);
        
        
        // configuring on/off label and adding it to the main window
        addAndMakeVisible(sc.onOffLabel);
        sc.onOffLabel.setText ("On/Off", dontSendNotification);
       sc. onOffLabel.attachToComponent (&sc.onOffButton, true);
        
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
        sc.frequencySlider.setBounds (sliderLeft, 10, getWidth() - sliderLeft - 20, 20);
        sc.gainSlider.setBounds (sliderLeft, 40, getWidth() - sliderLeft - 20, 20);
        sc.onOffButton.setBounds (sliderLeft, 70, getWidth() - sliderLeft - 20, 20);
    }
    
    void sliderValueChanged (Slider* slider) override
    {
        if (samplingRate > 0.0){
            if (slider == &sc.frequencySlider){
                sc.sine.setFrequency(sc.frequencySlider.getValue());
            }
            else if (slider == &sc.gainSlider){
                sc.gain = sc.gainSlider.getValue();
            }
        }
    }
    
    void buttonClicked (Button* button) override
    {
        // turns audio on or off
        if(button == &sc.onOffButton && sc.onOffButton.getToggleState()){
            sc.onOff = 1;
        }
        else{
            sc.onOff = 0;
        }
    }
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        samplingRate = sampleRate;
        sc.sine.setSamplingRate(sampleRate);
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
            if(sc.onOff == 1) buffer[sample] = sc.sine.tick() * sc.gain;
            
           // if(onOff == 1) buffer[sample] = (float) rand()/RAND_MAX * gain;

            else buffer[sample] = 0.0;
        }
    }
    
    
private:
    // UI Elements
   // Slider frequencySlider;
   // Slider gainSlider;
    
    //ToggleButton onOffButton;
    
   // Label frequencyLabel, gainLabel, onOffLabel;
    
    //Sine sine; // the sine wave oscillator
    SineComponents sc;
    
    // Global Variables
  //  float gain;
   // int onOff,
    int samplingRate, nChans;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
