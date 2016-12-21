class CustomLookAndFeel    : public LookAndFeel_V3
{
    
    const float lineThickness = 1.5;
    
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                           float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
    {
        
        const float radius = jmin (width / 2, height / 2) - 2.0f;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;
        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        const float arcThickness = 0.35;
        const float arcBorder = 0.03;
        const float detentSize = lineThickness * 1.5;
        
        float bx = rx + rw * arcBorder;
        float by = ry + rw * arcBorder;
        float bw = rw - (rw * arcBorder * 2);
        
        
        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
        
        if (slider.isEnabled())
            g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.9f));
            else
                g.setColour (Colour (0x80808080));
                
            {
                Path filledArc;
                
                if (sliderPos > 0.5) {
                    filledArc.addPieSegment (bx, by, bw, bw, float_Pi * 2, angle, 1.0 - arcThickness - arcBorder);
                    g.fillPath (filledArc);
                }
                if (sliderPos <= 0.5) {
                    filledArc.addPieSegment (bx, by, bw, bw, angle, float_Pi * 2, 1.0 - arcThickness - arcBorder);
                    g.fillPath (filledArc);
                }
                if (sliderPos == 0.5) {
                    Path detent;
                    Line <float> detentLine;
                    detentLine.setStart(rx + rw / 2, by);
                    detentLine.setEnd(rx + rw / 2, by + (bw * arcThickness / 2));
                    g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.9f));
                    detent.addLineSegment(detentLine, 0);
                    g.strokePath(detent, PathStrokeType (detentSize));
                }
                
                
            }
        
        {
            /*Path outlineArc;
             
             g.setColour(slider.findColour (Slider::rotarySliderFillColourId));
             outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 1.0 - arcThickness);
             g.strokePath (outlineArc, PathStrokeType (lineThickness));
             */
            
            Path outlineArc;
            g.setColour(slider.findColour (Slider::rotarySliderFillColourId));
            outlineArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 1.0);
            g.strokePath (outlineArc, PathStrokeType (lineThickness));
            
            /*
             Path detent;
             Line <float> detentLine;
             detentLine.setStart(rx + rw / 2, by);
             detentLine.setEnd(rx + rw / 2, by + (bw * arcThickness / 2));
             g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.9f));
             detent.addLineSegment(detentLine, 0);
             g.strokePath(detent, PathStrokeType (lineThickness));
             */
        }
    }
    
    
    
    void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle style, Slider& slider) override
    
    {
        g.fillAll (slider.findColour (Slider::backgroundColourId));
        
        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
        
        if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
        {
            const float fx = (float) x, fy = (float) y, fw = (float) width, fh = (float) height;
            
            Path p;
            
            if (style == Slider::LinearBarVertical)
                p.addRectangle (fx, sliderPos, fw, 1.0f + fh - sliderPos);
                else
                    p.addRectangle (fx, fy, sliderPos - fx, fh);
                    
                    if (slider.isEnabled())
                        g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.9f));
                        else
                            g.setColour (Colour (0x80808080));
                            
                            g.fillPath (p);
                            
                            g.setColour (slider.findColour (Slider::rotarySliderFillColourId));
                            g.drawRect (slider.getLocalBounds().toFloat(), lineThickness);
                            
                            }
        else
        {
            drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
            drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        }
    }
    
    
};
