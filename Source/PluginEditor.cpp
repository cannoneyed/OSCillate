/*
 ==============================================================================

 This file was auto-generated by the Introjucer!

 It contains the basic startup code for a Juce application.

 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
OSCToolAudioProcessorEditor::OSCToolAudioProcessorEditor (OSCToolAudioProcessor* ownerFilter)
:   AudioProcessorEditor (ownerFilter)

{
    setSize (350, 250);

    setupLookAndFeel(&customLookAndFeel);
    LookAndFeel::setDefaultLookAndFeel(&customLookAndFeel);

    initializeGUIComponents();
    positionGUIComponents();
    addGUIListeners();
    addAndMakeGUIComponentsVisible();

    startTimer (30);
}

OSCToolAudioProcessorEditor::~OSCToolAudioProcessorEditor()
{
}

//==============================================================================
void OSCToolAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (appearance.UIBackgroundColour);
}


//==============================================================================
// This timer periodically checks whether any of the filter's parameters have changed...
void OSCToolAudioProcessorEditor::timerCallback()
{
    
}


void OSCToolAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    // It's vital to use setParameterNotifyingHost to change any parameters that are automatable
    // by the host, rather than just modifying them directly, otherwise the host won't know
    // that they've changed.

    oscParam->setValueNotifyingHost(oscSlider.getValue());
    DBG(oscParam->range.start);
    DBG(oscParam->range.end);
    DBG(oscParam->range.interval);
    DBG(oscParam->get());
    DBG("---");
    oscInputLabel.setText ((String)oscParam->get(), dontSendNotification);


}

void OSCToolAudioProcessorEditor::labelTextChanged(Label* label)
{
    AudioParameterFloat* oscParam = getProcessor()->oscParam;
    OSCManager* oscManager = &getProcessor()->oscManager;
    OSCSender* oscSender = &getProcessor()->oscSender;

    if (label == &oscInputLabel)
    {
        double newValue = oscInputLabel.getText().getDoubleValue();

        oscSlider.setValue(newValue, sendNotification);
        oscInputLabel.setText ("HEY", dontSendNotification); // MUST FIX ME!!!
    }

    if (label == &ipInputLabel)
    {
        if (oscManager->checkIP(ipInputLabel.getText()))
        {
            oscManager->setIP(ipInputLabel.getText());
            oscSender->disconnect();
            oscSender->connect(oscManager->getIP(), oscManager->getPort());
        }
        else {
            ipInputLabel.setText(oscManager->getIP(), dontSendNotification);
        }
    }

    if (label == &portInputLabel)
    {
        if (oscManager->checkPort(portInputLabel.getText().getIntValue()))
        {
            oscManager->setPort(portInputLabel.getText().getIntValue());
            oscSender->disconnect();
            oscSender->connect(oscManager->getIP(), oscManager->getPort());
        }
        else {
            portInputLabel.setText((String) oscManager->getPort(), dontSendNotification);
        }
    }

    if (label == &messageInputLabel)
    {
        oscManager->setAddress(messageInputLabel.getText());

        messageInputLabel.setText(oscManager->getAddress(), dontSendNotification);
    }

    if (label == &minInputLabel)
    {
        float newValue = minInputLabel.getText().getFloatValue();
        float oldValue = oscParam->range.start;

        if (newValue <= (oscParam->range.end - oscParam->range.interval))
        {
            oscParam->range.start = newValue;
            oscParam->range.end = maxInputLabel.getText().getFloatValue();
            oscInputLabel.setText ("HEY", dontSendNotification); // MUST FIX ME!!!
        }
        else
            minInputLabel.setText((String) oldValue, dontSendNotification);
    }

    if (label == &maxInputLabel)
    {
        float newValue = maxInputLabel.getText().getFloatValue();
        float oldValue = oscParam->range.end;

        if (newValue >= (oscParam->range.start + oscParam->range.interval))
        {
            oscParam->range.end = newValue;
            oscParam->range.start = minInputLabel.getText().getFloatValue();
            oscInputLabel.setText ("HEY", dontSendNotification); // MUST FIX ME!!!
        }
        else
            maxInputLabel.setText((String) oldValue, dontSendNotification);
    }

    if (label == &stepInputLabel)
    {
        float newValue = stepInputLabel.getText().getFloatValue();
        float oldValue = oscParam->range.end;

        if (newValue <= (oscParam->range.end - oscParam->range.start && newValue > 0))
        {
            oscParam->range.interval = newValue;

            minInputLabel.setText ( (String) oscParam->range.start, sendNotification);
            maxInputLabel.setText ( (String) oscParam->range.end, sendNotification);

            oscParam->range.end = maxInputLabel.getText().getFloatValue();
            oscParam->range.start = minInputLabel.getText().getFloatValue();
        }
        else
            stepInputLabel.setText((String) oldValue, dontSendNotification);
    }

}






void OSCToolAudioProcessorEditor::initializeGUIComponents ()
{
    oscSlider.attachParameter(oscParam, sendNotification);
    oscSlider.setSliderStyle (Slider::LinearBarVertical);

    oscInputLabel.setEditable(false, true, false);
    oscInputLabel.setJustificationType (Justification::centred);
    oscInputLabel.setFont (appearance.labelFont);
    oscInputLabel.setColour(TextEditor::textColourId, appearance.textEditorTextColour);
    oscInputLabel.setText ("HEY", dontSendNotification); // MUST FIX ME!!!!

    ipInputLabel.setText (getProcessor()->oscManager.getIP(), dontSendNotification);
    ipInputLabel.setEditable(false, true, false);
    ipInputLabel.setColour(TextEditor::textColourId, appearance.UIComponentColour);
    ipInputLabel.setFont(appearance.labelFont);
    ipInputLabel.setMinimumHorizontalScale(1.0);

    portInputLabel.setText ((String) getProcessor()->oscManager.getPort(), dontSendNotification);
    portInputLabel.setEditable(false, true, false);
    portInputLabel.setColour(TextEditor::textColourId, appearance.UIComponentColour);
    portInputLabel.setFont(appearance.labelFont);
    portInputLabel.setMinimumHorizontalScale(1.0);

    messageInputLabel.setText(getProcessor()->oscManager.getAddress(), dontSendNotification);
    messageInputLabel.setEditable(false, true, false);
    messageInputLabel.setColour(TextEditor::textColourId, appearance.UIComponentColour);
    messageInputLabel.setFont(appearance.labelFont);
    messageInputLabel.setMinimumHorizontalScale(1.0);

    minInputLabel.setText ((String) oscParam->range.start,
                           dontSendNotification);
    minInputLabel.setEditable(false, true, false);
    minInputLabel.setColour(TextEditor::textColourId, appearance.UIComponentColour);
    minInputLabel.setFont(appearance.labelFont);
    minInputLabel.setMinimumHorizontalScale(1.0);

    maxInputLabel.setText ((String) oscParam->range.end,
                           dontSendNotification);
    maxInputLabel.setEditable(false, true, false);
    maxInputLabel.setColour(TextEditor::textColourId, appearance.UIComponentColour);
    maxInputLabel.setFont(appearance.labelFont);
    maxInputLabel.setMinimumHorizontalScale(1.0);


    stepInputLabel.setText((String) oscParam->range.interval, dontSendNotification);
    stepInputLabel.setEditable(false, true, false);
    stepInputLabel.setColour(TextEditor::textColourId, appearance.UIComponentColour);
    stepInputLabel.setFont(appearance.labelFont);
    stepInputLabel.setMinimumHorizontalScale(1.0);


    ipLabel.setText("IP", dontSendNotification);
    ipLabel.setColour(Label::textColourId, appearance.UISecondaryColour);
    ipLabel.setFont(appearance.labelFont);

    portLabel.setText("PORT", dontSendNotification);
    portLabel.setColour(Label::textColourId, appearance.UISecondaryColour);
    portLabel.setFont(appearance.labelFont);

    messageLabel.setText("MSG", dontSendNotification);
    messageLabel.setColour(Label::textColourId, appearance.UISecondaryColour);
    messageLabel.setFont(appearance.labelFont);

    minLabel.setText("MIN", dontSendNotification);
    minLabel.setColour(Label::textColourId, appearance.UISecondaryColour);
    minLabel.setFont(appearance.labelFont);

    maxLabel.setText("MAX", dontSendNotification);
    maxLabel.setColour(Label::textColourId, appearance.UISecondaryColour);
    maxLabel.setFont(appearance.labelFont);

    stepLabel.setText("STEP", dontSendNotification);
    stepLabel.setColour(Label::textColourId, appearance.UISecondaryColour);
    stepLabel.setFont(appearance.labelFont);


}

void OSCToolAudioProcessorEditor::positionGUIComponents()
{
    int topRowOffset = 25;
    int rowHeight = 33;

    oscSlider.setBounds (30, 30, 20, getHeight() - 90);
    oscInputLabel.setBounds (15, 200, 50, 20);

    ipLabel.setBounds(85, topRowOffset, 20, 20);
    ipInputLabel.setBounds(120, topRowOffset, 100, 20);

    portLabel.setBounds(235, topRowOffset, 40, 20);
    portInputLabel.setBounds(275, topRowOffset, 75, 20);

    int secondRowOffset = topRowOffset + rowHeight;

    messageLabel.setBounds(85, secondRowOffset, 40, 20);
    messageInputLabel.setBounds(120, secondRowOffset, 220, 20);

    int thirdRowOffset = secondRowOffset + rowHeight;

    minLabel.setBounds(85, thirdRowOffset, 35, 20);
    minInputLabel.setBounds(120, thirdRowOffset, 45, 20);

    maxLabel.setBounds(160, thirdRowOffset, 35, 20);
    maxInputLabel.setBounds(195, thirdRowOffset, 45, 20);

    stepLabel.setBounds(235, thirdRowOffset, 35, 20);
    stepInputLabel.setBounds(270, thirdRowOffset, 75, 20);

}

void OSCToolAudioProcessorEditor::addGUIListeners()
{
    oscSlider.addListener (this);
    oscInputLabel.addListener (this);

    ipInputLabel.addListener (this);
    portInputLabel.addListener (this);
    messageInputLabel.addListener (this);
    minInputLabel.addListener (this);
    maxInputLabel.addListener (this);
    stepInputLabel.addListener (this);
}

void OSCToolAudioProcessorEditor::addAndMakeGUIComponentsVisible ()
{
    addAndMakeVisible (oscSlider);
    addAndMakeVisible (oscInputLabel);

    addAndMakeVisible(ipLabel);
    addAndMakeVisible(ipInputLabel);

    addAndMakeVisible(portLabel);
    addAndMakeVisible(portInputLabel);

    addAndMakeVisible(messageLabel);
    addAndMakeVisible(messageInputLabel);

    addAndMakeVisible(minLabel);
    addAndMakeVisible(minInputLabel);

    addAndMakeVisible(maxLabel);
    addAndMakeVisible(maxInputLabel);

    addAndMakeVisible(stepLabel);
    addAndMakeVisible(stepInputLabel);

}

void OSCToolAudioProcessorEditor::setupLookAndFeel(LookAndFeel *laf)
{
    laf->setColour (Label::textColourId, appearance.textColour);
    laf->setColour (TextEditor::focusedOutlineColourId, appearance.focusedOutlineColour);
    laf->setColour (TextEditor::highlightedTextColourId, appearance.highlightedTextColour);
    laf->setColour (TextEditor::textColourId, appearance.textEditorTextColour);
    laf->setColour (TextEditor::highlightColourId, appearance.highlightColour);
    laf->setColour (CaretComponent::caretColourId, appearance.caretColour);

    laf->setColour (Slider::rotarySliderFillColourId, appearance.UIComponentColour);
    laf->setColour (Slider::textBoxOutlineColourId, Colours::transparentWhite);
}
