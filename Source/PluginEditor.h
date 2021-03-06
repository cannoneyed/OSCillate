/*
 ==============================================================================
 
 This file was auto-generated by the Introjucer!
 
 It contains the basic startup code for a Juce application.
 
 ==============================================================================
 */

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Appearance.h"
#include "CustomLookAndFeel.h"
#include "CustomComponents.h"



//==============================================================================
/**
 */
class OSCToolAudioProcessorEditor  : public AudioProcessorEditor,
public Slider::Listener,
public Label::Listener,
public Timer

{
public:
    OSCToolAudioProcessorEditor (OSCToolAudioProcessor* ownerFilter);
    ~OSCToolAudioProcessorEditor();
    
    //==============================================================================
    // This is just a standard Juce paint method...
    void paint (Graphics& g) override;
    
    void timerCallback() override;
    void sliderValueChanged (Slider*) override;
    void labelTextChanged (Label* label) override;
    
    void setupLookAndFeel (LookAndFeel* laf);
    void initializeGUIComponents ();
    void positionGUIComponents ();
    void addGUIListeners ();
    void addAndMakeGUIComponentsVisible ();
    
private:
    
    Label ipLabel;
    Label portLabel;
    Label messageLabel;
    Label minLabel;
    Label maxLabel;
    Label stepLabel;
    
    
    ParameterSlider oscSlider;
    
    NumericalLabel oscInputLabel;
    IPLabel ipInputLabel;
    PortLabel portInputLabel;
    Label messageInputLabel;
    NumericalLabel minInputLabel;
    NumericalLabel maxInputLabel;
    NumericalLabel stepInputLabel;
    
    Font labelFont;
    CustomLookAndFeel customLookAndFeel;
    
    OSCAudioParameterFloat* oscParam = getProcessor()->oscParam;
    
    Appearance appearance;
    
    OSCToolAudioProcessor* getProcessor() const
    {
        return static_cast <OSCToolAudioProcessor*> (getAudioProcessor());
    }
};


#endif  // PLUGINEDITOR_H_INCLUDED
