class ParameterSlider : public Slider, private Timer
{
public:
    ParameterSlider () {
        Slider::setRange (0.0, 1.0, 0.00001);
        Slider::setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
        Slider::setVelocityModeParameters(3.0, 1, 0.015, true);
        
        startTimerHz (30);
    }
    
    
    void mouseDown (const MouseEvent& e) override
    {
        
        ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
        
        // check the mod keys ..
        if (modifiers.isAltDown())
        {
            if (attachedParameter != nullptr) {
                Slider::setValue(attachedParameter->range.start, sendNotification);
            }
            else {
                Slider::setValue(1.0, sendNotification);
            }
        }
        else
        {
            Slider::mouseDown(e);   // to the usual thing .... drag the slider
        }
    }
    
    void timerCallback() override       {
        updateSliderPos();
    }
    
    void startedDragging() override     { attachedParameter->beginChangeGesture(); }
    void stoppedDragging() override     { attachedParameter->endChangeGesture();   }
    
    void attachParameter (OSCAudioParameterFloat* ParameterToAttach, const NotificationType notification)
    {
        attachedParameter = ParameterToAttach;
        Slider::setValue(attachedParameter->get(), notification);
    }
    
    AudioParameterFloat* getAttachedParameter ()
    {
        return attachedParameter;
    }
    
    void updateSliderPos() {
        const float newValue = attachedParameter->get();
        if (newValue != (float) Slider::getValue() && ! isMouseButtonDown())
            DBG(newValue);
            Slider::setValue (newValue);
    }
    
    void updateAttachedParameter () {
        attachedParameter->setValueNotifyingHost(getValue());
    }
    
    void attachParameter (OSCAudioParameterFloat* ParameterToAttach)
    {
        attachParameter(ParameterToAttach, dontSendNotification);
    }
    
    
    
private:
    OSCAudioParameterFloat* attachedParameter = nullptr;
    
};



//
////==============================================================================
//// This is a handy slider subclass that controls an AudioProcessorParameter
//// (may move this class into the library itself at some point in the future..)
//class ParameterSlider   : public Slider,
//private Timer
//{
//public:
//    ParameterSlider (OSCAudioParameterFloat& p)
//    : Slider (p.getName (256)), param (p)
//    {
//        setRange (0.0, 1.0, 0.0);
//        setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
//        setVelocityModeParameters(3.0, 1, 0.015, true);
//
//        startTimerHz (30);
//        updateSliderPos();
//    }
//    
//    void mouseDown (const MouseEvent& e)
//    {
//        
//        ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
//        
//        // check the mod keys ..
//        if (modifiers.isAltDown())
//        {
//            Slider::setValue(param->range.start, sendNotification);
//        }
//        else
//        {
//            Slider::mouseDown(e);   // to the usual thing .... drag the slider
//        }
//    }
//    
//    void valueChanged() override
//    {
//        if (isMouseButtonDown())
//            param.setValueNotifyingHost ((float) Slider::getValue());
//        else
//            param.setValue ((float) Slider::getValue());
//    }
//    
//    void timerCallback() override       { updateSliderPos(); }
//    
//    void startedDragging() override     { param.beginChangeGesture(); }
//    void stoppedDragging() override     { param.endChangeGesture();   }
//    
//    double getValueFromText (const String& text) override   { return param.getValueForText (text); }
//    String getTextFromValue (double value) override         { return param.getText ((float) value, 1024); }
//    
//    void updateSliderPos()
//    {
//        const float newValue = param.getValue();
//        
//        if (newValue != (float) Slider::getValue() && ! isMouseButtonDown())
//            Slider::setValue (newValue);
//    }
//    
//    OSCAudioParameterFloat& param;
//    
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
//};





class NumericalLabel :          public Label
{
public:
    void editorShown (TextEditor* editor)
    {
        editor->setInputRestrictions (0, "-+.0123456789");
    }
};






class IPLabel :                 public Label
{
    void editorShown (TextEditor* editor)
    {
        editor->setInputRestrictions(16, ".0123456789");
    }
    
    void mouseDrag (const MouseEvent &event)
    {
        static int textClickedIndex;
        static int addressIndex;
        
        if (isDragging == false)
        {
            textClickedIndex = getTextIndexAt (event.getMouseDownX());
            addressIndex = getAddressIndexByTextClickedIndex(textClickedIndex);
            
            if (isADigit (textClickedIndex))
                startDrag();
        }
        else if (isDragging == true)
        {
            int threshold;
            
            if (event.mods.isAltDown() || event.mods.isCommandDown() || event.mods.isCtrlDown())
                threshold = modifierMoveThreshold;
            else
                threshold = moveThreshold;
            
            newMouseY = event.getDistanceFromDragStartY();
            
            
            if (newMouseY - oldMouseY >= threshold)
            {
                addValueToAddress (-interval, addressIndex);
                oldMouseY = newMouseY;
            }
            else if (oldMouseY - newMouseY >= threshold)
            {
                addValueToAddress (interval, addressIndex);
                oldMouseY = newMouseY;
            }
        }
    }
    
    void mouseUp (const MouseEvent &event)
    {
        if (isDragging)
            isDragging = false;
        
        callChangeListeners();
    }
    
    void startDrag () { isDragging = true; }
    
    void addValueToAddress (int toAdd, int addressIndex)
    {
        IPAddress ipAddress(getText());
        int addressToChange = ipAddress.address[addressIndex];
        
        addressToChange += toAdd;
        
        if (addressToChange >255)
            addressToChange = 255;
        if (addressToChange < 0)
            addressToChange = 0;
        
        ipAddress.address[addressIndex] = addressToChange;
        
        setText (ipAddress.toString(), dontSendNotification);
    }
    
    int getAddressIndexByTextClickedIndex (int textClickedIndex)
    {
        int dots = 0;
        for (int i = 0; i < textClickedIndex; i++)
        {
            if (getText().substring(i, i+1) == ".")
                dots++;
        }
        return dots;
    }
    
    int getTextIndexAt (int clickedX)
    {
        Font thisFont = getFont();
        String ipText = getText();
        
        Array <int> glyphs;
        Array <float> xOffsets;
        int textClickedIndex = 0;
        
        // Return the corresponding x positions of the string inside the Label
        thisFont.getGlyphPositions(ipText, glyphs, xOffsets);
        
        // Find the index of the character at the x position clicked
        for (int i = 0; i < glyphs.size(); i++)
        {
            if (clickedX - getBorderSize().getLeft() > xOffsets[i] &&
                clickedX - getBorderSize().getLeft() <= xOffsets[i+1])
            {
                textClickedIndex = i;
            }
        }
        
        if (clickedX - getBorderSize().getLeft() > xOffsets[glyphs.size()])
            textClickedIndex = glyphs.size() + 1;
        
        return textClickedIndex;
    }
    
    bool isADigit (int textClickedIndex)
    {
        std::cout << getText().substring(textClickedIndex, textClickedIndex+1);
        
        return (getText().substring(textClickedIndex, textClickedIndex+1) != "." &&
                getText().substring(textClickedIndex, textClickedIndex+1) != String::empty);
    }
    
    
private:
    bool isDragging = false;
    
    int newMouseY = 0;
    int oldMouseY = 0;
    
    int threshold;
    int moveThreshold = 1;
    int modifierMoveThreshold = 10;
    int interval = 1;
};




class PortLabel :                 public Label
{
    void editorShown (TextEditor* editor)
    {
        editor->setInputRestrictions(5, "0123456789");
    }
    
    void mouseDrag (const MouseEvent &event)
    {
        int threshold;
        
        if (isDragging == false)
            startDrag();
        
        if (event.mods.isAltDown() || event.mods.isCommandDown() || event.mods.isCtrlDown())
            threshold = modifierMoveThreshold;
        else
            threshold = moveThreshold;
        
        newMouseY = event.getDistanceFromDragStartY();
        
        if (newMouseY - oldMouseY >= threshold)
        {
            oldMouseY = newMouseY;
            addValue (-interval);
        }
        if (oldMouseY - newMouseY >= threshold)
        {
            oldMouseY = newMouseY;
            addValue (interval);
        }
        
        setText ( (String) newValue, dontSendNotification);
    }
    
    void mouseUp (const MouseEvent &event)
    {
        if (isDragging)
            isDragging = false;
        
        callChangeListeners();
    }
    
    void startDrag ()
    {
        isDragging = true;
    }
    
    void addValue (int toAdd)
    {
        newValue = getText().getIntValue();
        newValue += toAdd;
        setText ( (String) newValue, dontSendNotification);
    }
    
    
private:
    bool isDragging = false;
    int newValue;
    
    int newMouseY = 0;
    int oldMouseY = 0;
    
    int moveThreshold = 2;
    int modifierMoveThreshold = 10;
    int interval = 1;
    
};


class ParameterLabel :          public NumericalLabel
{
public:
    void attachParameter (AudioParameterFloat* parameterToAttach)
    {
        attachedParameter = parameterToAttach;
    }
    
    AudioParameterFloat* getAttachedParameter()
    {
        return attachedParameter;
    }
    
private:
    AudioParameterFloat* attachedParameter;
};

class NameLabel :               public Label
{
    NameLabel ()
    {
        Label::setJustificationType (Justification::centred);
        //Label::setFont (labelFont);
    }
};