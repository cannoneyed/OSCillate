/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic startup code for a Juce application.
 
 ==============================================================================
 */

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "OSCManager.h"
#include "OSCAudioParameter.h"

//==============================================================================
/**
 */

class OSCToolAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    OSCToolAudioProcessor();
    ~OSCToolAudioProcessor();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset() override;
    
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
    
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                                             { return true; }
    
    //==============================================================================
    const String getName() const override                                       { return JucePlugin_Name; }
    
    bool acceptsMidi() const override                                           { return true; }
    bool producesMidi() const override                                          { return true; }
    
    double getTailLengthSeconds() const override                                { return 0.0; }
    
    //==============================================================================
    int getNumPrograms() override                                               { return 0; }
    int getCurrentProgram() override                                            { return 0; }
    void setCurrentProgram (int /*index*/) override                             {}
    const String getProgramName (int /*index*/) override                        {   return String(); }
    void changeProgramName (int /*index*/, const String& /*name*/) override     {}
        
    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;
    
    bool silenceInProducesSilenceOut() const override                           { return false; }
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    void sendOSCMessage (float value);
    void setMessage();
    
    OSCAudioParameterFloat* oscParam;
    
    OSCSender oscSender;
    OSCManager oscManager;
    
    String ip = "127.0.0.1";
    int port = 7001;
    
    String message = "/osc/address";
    
    float minimumValue = 0.0f;
    float maximumValue = 127.0f;
    float stepValue = 1.0f;
    
    int bufferSize = 1024;
    
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCToolAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
