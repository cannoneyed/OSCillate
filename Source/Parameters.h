class Parameter
{
    
public:
    
    Parameter (double minValue, double maxValue, double initValue) :
    minimumValue (minValue),
    maximumValue (maxValue),
    defaultValue (initValue),
    currentValue (initValue) { initialize(); }
    
    Parameter() { initialize(); }
    
    void initialize () {
        setSkewFactor(1.0);
        setInterval(0.1);
        setUnit (String::empty);
        setName (String::empty);
    }
    
    virtual ~Parameter() {}
    
    double constrainInterval (double value)
    {
        if (interval > 0)
            value = minimumValue + interval * std::floor ((value - minimumValue) / interval + 0.5);
        return value;
    }
    
    double constrainedValue (double value)
    {
        value = constrainInterval (value);
        
        if (value <= minimumValue || maximumValue <= minimumValue)
            value = minimumValue;
        else if (value >= maximumValue)
            value = maximumValue;
        
        return value;
    }
    
    double constrainedRange (double value)
    {
        if (value > maximumValue)
            value = maximumValue;
        if (value < minimumValue)
            value = minimumValue;
        
        return value;
    }
    
    
    //===============================================================================
    // Basic SET Functions
    //
    
    void setValue (double newValue) { currentValue = constrainedValue (newValue); }
    
    void setMaximumValue (double newValue)
    {
        maximumValue = constrainedValue (newValue);
        if (currentValue > maximumValue)
            currentValue = maximumValue;
    }
    
    void resetMaximumValue (double newValue)
    {
        maximumValue = constrainInterval (newValue);
        if (currentValue > maximumValue)
            currentValue = maximumValue;
    }
    
    
    void setMinimumValue (double newValue)
    {
        minimumValue = constrainedValue (newValue);
        if (currentValue < minimumValue)
            currentValue = minimumValue;
    }
    
    void resetMinimumValue (double newValue)
    {
        minimumValue = constrainInterval (newValue);
        if (currentValue < minimumValue)
            currentValue = minimumValue;
    }
    
    
    void setDefaultValue (double newValue) { defaultValue = constrainedValue (newValue); }
    void setSkewFactor (double newValue) { skewFactor = newValue; }
    
    
    void setInterval (double newValue)
    {
        interval = newValue;
        minimumValue = constrainedValue(minimumValue);
        maximumValue = constrainedValue(maximumValue);
        currentValue = constrainedValue(currentValue);
    }
    
    void setIndex (int index) { indexValue = index; }
    void setShowPositive (bool toShow) { showPositive = toShow; }
    void setUnit (String toSet) { unit = toSet; }
    void setName (String toSet) { name = toSet; }
    
    //===============================================================================
    // Basic GET Functions
    //
    
    double getValue () { return currentValue; }
    double getMaximumValue () { return maximumValue; }
    double getMinimumValue () { return minimumValue; }
    double getDefaultValue () { return defaultValue; }
    double getRange () { return maximumValue - minimumValue; }
    int getIndex () { return indexValue; }
    String getUnit () { return unit; }
    
    double getSkewFactor () { return skewFactor; }
    const double getInterval () { return interval; }
    String getName () { return name; }
    
    
    //===============================================================================
    // SET Functions for conversion between scaled, normalized, and linear representation
    // of the parameter
    //
    
    void setValueFromNormalized (double normalizedValue)
    {
        setValue ( getRange() * normalizedValue + minimumValue );
    }
    
    void setValueFromLinear (double linearValue)
    {
        double normalizedValue = exp ( log (linearValue) / getSkewFactor() );
        setValueFromNormalized (normalizedValue);
    }
    
    
    //===============================================================================
    // GET Functions for conversion between scaled, normalized, and linear representation
    // of the parameter
    //
    
    double getNormalizedValue (double scaledValue)
    {
        scaledValue = constrainedRange(scaledValue);
        
        double proportion = (scaledValue - getMinimumValue()) / getRange();
        return proportion;
    }
    
    double getNormalizedValue () { return getNormalizedValue (currentValue); }
    
    double getLinearValue (double scaledValue)
    {
        double proportion = exp (log (getNormalizedValue(scaledValue) ) * getSkewFactor() );
        return proportion;
    }
    
    double getLinearValue () { return getLinearValue (currentValue); }
    
    
    
    double getScaledValueFromNormalized (double normalizedValue)
    {
        double scaledValue = getRange() * normalizedValue + minimumValue;
        return scaledValue;
    }
    
    double getScaledValueFromLinear (double linearValue)
    {
        double normalizedValue = exp ( log (linearValue) / getSkewFactor() );
        return getScaledValueFromNormalized (normalizedValue);
    }
    
    virtual void specialFormatting (String &toFormat) {}
    
    String getFormattedText (double value)
    {
        
        int nDecimals = 0;
        double testInterval = interval;
        
        while (testInterval < 1) {
            nDecimals++;
            testInterval = testInterval * 10;
        }
        
        String formattedText = String (constrainedValue(value), nDecimals);
        
        if (showPositive && value > 0)
            formattedText = "+" + formattedText;
        
        specialFormatting (formattedText);
        
        return formattedText;
    }
    
    String getFormattedText () {
        return getFormattedText (currentValue);
    }
    
    String getFormattedTextFromLinear (double value)
    {
        return getFormattedText(getScaledValueFromLinear(value));
    }
    
    
private:
    double minimumValue, maximumValue, defaultValue, currentValue;
    double skewFactor, interval;
    
    int indexValue;
    
    String name, unit;
    
    bool showPositive = true;
    
    
};


class GainParameter : public Parameter
{
public:
    
    GainParameter (double minValue, double maxValue, double initValue)
    : Parameter (minValue, maxValue, initValue) { initialize(); }
    
    ~GainParameter () {}
    
    void specialFormatting (String &toFormat)
    {
        if (toFormat.getFloatValue() == getMinimumValue())
            toFormat = "-" + String(CharPointer_UTF8 ("\xe2\x88\x9e"));
    }
    
};


class ParameterArray
{
    
public:
    
    int count () { return parameterArray.size(); }
    
    void addParameter (Parameter* parameterToAdd, String name)
    {
        parameterArray.add(parameterToAdd);
        parameterToAdd->setIndex (parameterArray.size() - 1);
        parameterToAdd->setName (name);
    }
    
    
    Parameter* getParameterByIndex (const int index) {
        
        // Parameter index outside of range
        jassert(index >= 0 && index < parameterArray.size());
        
        return parameterArray [index];
    }
    
    Parameter* getParameterByName (const String name)
    {
        Parameter* found = nullptr;
        
        for (int i=0; i < parameterArray.size(); i++)
        {
            if (parameterArray[i]->getName() == name)
                found = parameterArray[i];
        }
        
        // Parameter not found by name
        jassert(found != nullptr);
        
        return found;
    }
    
    Parameter* last() { return parameterArray.getLast(); }
    
    Parameter* operator () (const String name) { return getParameterByName (name); }
    Parameter* operator () (const int index) { return getParameterByIndex (index); }
    Parameter* operator [] (const int index) { return getParameterByIndex (index); }
    
    
private:
    
    OwnedArray <Parameter> parameterArray;
    String parameterName;
    
};