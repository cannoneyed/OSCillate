/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic startup code for a Juce application.
 
 ==============================================================================
 */

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"
#include "OSCManager.h"

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
    bool hasEditor() const override;
    
    //==============================================================================
    const String getName() const override;
    
    int getNumParameters() override;
    
    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;
    float getParameterDefaultValue (int index) override;
    
    const String getParameterName (int index) override;
    const String getParameterText (int index) override;
    
    const String getParameterTextByValue (int parameterIndex, float parameterValue);
    String getParameterTextByValue (int parameterIndex, float parameterValue, int maximumStringLength);
    
    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    void sendOSCMessage (float value);
    void setMessage();
    
    ParameterArray parameters;
    
    enum parametersIndex
    {
        OSCParameterIndex = 0,
        
        totalNumParams
    };
    
    float parameterValues [totalNumParams];
    
    OSCSender oscSender;
    OSCManager oscManager;
    
    String ip = "127.0.0.1";
    int port = 7001;
    
    String message = "/osc/address";
    
    float minimumValue = 0.0;
    float maximumValue = 127.0;
    float stepValue = 1;
    
    int bufferSize = 1024;
    
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCToolAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
