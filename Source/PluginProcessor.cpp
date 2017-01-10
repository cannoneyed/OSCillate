/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic startup code for a Juce application.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OSCToolAudioProcessor::OSCToolAudioProcessor()
{
    
    parameters.addParameter (new Parameter(minimumValue, maximumValue, minimumValue), "OSC");
    
    parameters("OSC")->setInterval(stepValue);
    parameters("OSC")->setShowPositive(false);
    
    oscManager.setIP(ip);
    oscManager.setPort(port);
    oscManager.setAddress(message);
    
    oscSender.connect(ip, port);
    
    sendOSCMessage (parameters("OSC")->getValue());
}

OSCToolAudioProcessor::~OSCToolAudioProcessor()
{
}

//==============================================================================
int OSCToolAudioProcessor::getNumParameters()
{
    return parameters.count();
}

float OSCToolAudioProcessor::getParameter (int index)
{
    return parameters(index)->getLinearValue();
}

void OSCToolAudioProcessor::setParameter (int index, float linearValue)
{
    if (index == parameters("OSC")->getIndex())
    {
        sendOSCMessage(parameters("OSC")->getValue());
    }
    
    return parameters(index)->setValueFromLinear(linearValue);
}

float OSCToolAudioProcessor::getParameterDefaultValue (int index)
{
    return parameters(index)->getDefaultValue();
}

const String OSCToolAudioProcessor::getParameterName (int index)
{
    DBG ("GET PARAMETER NAME");
    return parameters(index)->getName();
}



const String OSCToolAudioProcessor::getParameterText (int index)
{
    return "DEPRECATE";
}

const String OSCToolAudioProcessor::getParameterTextByValue (int index, float linearValue)
{
    return parameters(index)->getFormattedTextFromLinear(linearValue) + parameters(index)->getUnit();
}

String OSCToolAudioProcessor::getParameterTextByValue (int parameterIndex, float parameterValue, int maximumStringLength)
{
    return getParameterTextByValue (parameterIndex, parameterValue).substring (0, maximumStringLength);
}

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
    
    
    // Create an outer XML element..
    XmlElement xml ("OSCToolSETTINGS");
    
    // add some attributes to it..
    xml.setAttribute ("OSC", parameters("OSC")->getLinearValue());
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
    
}

void OSCToolAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    DBG ("setStateInformation");
    
    
    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("OSCToolSETTINGS"))
        {
            // ok, now pull out our parameters..
            
            parameters("OSC")->setValueFromLinear((double) xmlState->getDoubleAttribute ("OSC", parameters("OSC")->getLinearValue()));
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
