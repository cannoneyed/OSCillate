class Appearance
{
public:
    Appearance ()
    {
        labelFont.setHeight (15.0);
        labelFont.setTypefaceName ("Apple LiGothic");
        
        UIComponentColour = Colours::cornflowerblue;
        UIBackgroundColour = Colours::black;
        UISecondaryColour = Colours::white;
        
        textColour = UIComponentColour;
        focusedOutlineColour = Colours::transparentWhite;
        highlightedTextColour = Colours::white;
        highlightColour = UIComponentColour;
        textEditorTextColour = UIComponentColour;
        caretColour = UISecondaryColour;
    }
    
    Font labelFont;
    Colour  UIComponentColour;
    Colour  UIBackgroundColour;
    Colour  UISecondaryColour;
    Colour  textColour, focusedOutlineColour, highlightedTextColour,
    highlightColour, textEditorTextColour, caretColour;
};