// Music 256a / CS 476a | fall 2016
// CCRMA, Stanford University
//
// Author: Romain Michon (rmichonATccrmaDOTstanfordDOTedu)
// Description: Simple JUCE sine wave synthesizer

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sine.h"
#define NUMSLIDERS 4

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
        for (int n=0; n < NUMSLIDERS; n++) {
            
        
        // configuring frequency slider and adding it to the main window
        addAndMakeVisible (sc[n].frequencySlider);
        sc[n].frequencySlider.setRange (50.0, 5000.0);
        sc[n].frequencySlider.setSkewFactorFromMidPoint (500.0);
        sc[n].frequencySlider.setValue(1000); // will also set the default frequency of the sine osc
        sc[n].frequencySlider.addListener (this);
        
        // configuring frequency label box and adding it to the main window
        addAndMakeVisible(sc[n].frequencyLabel);
        sc[n].frequencyLabel.setText ("Frequency", dontSendNotification);
        sc[n].frequencyLabel.attachToComponent (&sc[n].frequencySlider, true);
        
        
        // configuring gain slider and adding it to the main window
        addAndMakeVisible (sc[n].gainSlider);
        sc[n].gainSlider.setRange (0.0, 1.0);
        sc[n].gainSlider.setValue(0.5); // will alsi set the default gain of the sine osc
        sc[n].gainSlider.addListener (this);
        
        
        // configuring gain label and adding it to the main window
        addAndMakeVisible(sc[n].gainLabel);
        sc[n].gainLabel.setText ("Gain", dontSendNotification);
        sc[n].gainLabel.attachToComponent (&sc[n].gainSlider, true);
        
        
        // configuring on/off button and adding it to the main window
        addAndMakeVisible(sc[n].onOffButton);
        sc[n].onOffButton.addListener(this);
        
        
        // configuring on/off label and adding it to the main window
        addAndMakeVisible(sc[n].onOffLabel);
        sc[n].onOffLabel.setText ("On/Off", dontSendNotification);
        sc[n].onOffLabel.attachToComponent (&sc[n].onOffButton, true);
        }
        setSize (600, 100 * NUMSLIDERS);
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
        for(int n = 0; n < NUMSLIDERS; n++) {
            sc[n].frequencySlider.setBounds (sliderLeft, 10 + 90 * n, getWidth() - sliderLeft - 20, 20);
            sc[n].gainSlider.setBounds (sliderLeft, 40 + 90 * n, getWidth() - sliderLeft - 20, 20);
            sc[n].onOffButton.setBounds (sliderLeft, 70 + 90 * n, getWidth() - sliderLeft - 20, 20);
        }
    }
    
    void sliderValueChanged (Slider* slider) override
    {
        for(int n = 0; n < NUMSLIDERS; n++) {

            if (samplingRate > 0.0){
                if (slider == &sc[n].frequencySlider){
                    sc[n].sine.setFrequency(sc[n].frequencySlider.getValue());
                    break;
                }
                else if (slider == &sc[n].gainSlider){
                    sc[n].gain = sc[n].gainSlider.getValue();
                    break;
                }
            }
        }
    }
    
    void buttonClicked (Button* button) override
    {
        for(int n = 0; n < NUMSLIDERS; n++) {

        // turns audio on or off
            if(button == &sc[n].onOffButton && sc[n].onOffButton.getToggleState()){
                sc[n].onOff = 1;
                break;
            }
            else if(button == &sc[n].onOffButton) {
                sc[n].onOff = 0;
                break;
            }
        }
    }
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        for(int n = 0; n < NUMSLIDERS; n++) {

        samplingRate = sampleRate;
        sc[n].sine.setSamplingRate(sampleRate);
        }
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
            for(int n = 0; n < NUMSLIDERS; n++) {
                if(sc[n].onOff == 1) buffer[sample] += sc[n].sine.tick() * sc[n].gain;
                else buffer[sample] += 0.0;
            }
        }
    }
    
    
private:

    SineComponents sc[NUMSLIDERS];

    int samplingRate, nChans;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
