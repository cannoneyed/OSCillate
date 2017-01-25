//
//  OSCAudioParameter.h
//  OSCillate
//
//  Created by Andrew Coenen on 1/10/17.
//
//

#ifndef OSCAudioParameter_h
#define OSCAudioParameter_h

//==============================================================================
class OSCAudioParameterFloat : public AudioParameterFloat
{
public:
    OSCAudioParameterFloat (String parameterID,
                            String name,
                            NormalisableRange<float> normalisableRange,
                            float defaultValue
                            ) :
        AudioParameterFloat (parameterID,
                             name,
                             normalisableRange,
                             defaultValue) {};
    
    OSCAudioParameterFloat& operator= (float newValue) {
        AudioParameterFloat::operator=(newValue);
        return *this;
    };
    
    String getText (float value, int /*maximumStringLength*/) const override {
        auto denormalized = this->range.convertFrom0to1(value);
        auto returnValue = (String) this->range.snapToLegalValue(denormalized);
        return returnValue;
    };
    
    String getValueString () const {
        float value = this->get();
        return (String) value;
    }
};


#endif /* OSCAudioParameter_h */