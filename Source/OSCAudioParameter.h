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
    
    virtual String getText (float value, int /*maximumStringLength*/) const override {
        return "STRING";
    };
    
};


#endif /* OSCAudioParameter_h */