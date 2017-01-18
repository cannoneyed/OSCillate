/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic startup code for a Juce application.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "OSCAudioParameter.h"

//==============================================================================
OSCToolAudioProcessor::OSCToolAudioProcessor() : oscParam (nullptr)
{
    NormalisableRange<float> range = NormalisableRange<float> (minimumValue, maximumValue, stepValue);
    addParameter (oscParam = new OSCAudioParameterFloat("OSC",
                                                     "OSC",
                                                     range,
                                                     minimumValue));
    oscManager.setIP(ip);
    oscManager.setPort(port);
    oscManager.setAddress(message);
    
    oscSender.connect(ip, port);
    
    sendOSCMessage (oscParam->get());
}

OSCToolAudioProcessor::~OSCToolAudioProcessor()
{
}

//==============================================================================
const String OSCToolAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String OSCToolAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool OSCToolAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool OSCToolAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

//==============================================================================
void OSCToolAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void OSCToolAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void OSCToolAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
}

void OSCToolAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    //const int numSamples = buffer.getNumSamples();
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    
    
    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    
    
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    
    
}

//==============================================================================
AudioProcessorEditor* OSCToolAudioProcessor::createEditor()
{
    return new OSCToolAudioProcessorEditor (this);
    
}

//==============================================================================
void OSCToolAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    ScopedPointer<XmlElement> xml (new XmlElement ("OscillateSettings"));
    xml->setAttribute ("OSC", (double) *oscParam);
    copyXmlToBinary (*xml, destData);
}

void OSCToolAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("OSCToolSETTINGS"))
        {
            // ok, now pull out our parameters..
            *oscParam = xmlState->getDoubleAttribute ("OSC", 1.0);
        }
    }
    
}


void OSCToolAudioProcessor::sendOSCMessage (float value)
{
    oscSender.send(oscManager.getAddress(), value);
}



//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OSCToolAudioProcessor();
}
