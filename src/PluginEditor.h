/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

// https://docs.juce.com/master/tutorial_look_and_feel_customisation.html

class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel()
    {
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::antiquewhite);
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        
        // fill
        g.setColour(juce::Colours::black);
        g.fillEllipse(rx, ry, rw, rw);
        
        juce::Path p;
        auto pointerLength = radius * 0.67f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        
        // pointer
        g.setColour(juce::Colours::white);
        g.fillPath(p);

    }
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle, juce::Slider&) override
    {
        // Background Line
        auto lineThickness = 4.0f;
        auto lineColor = juce::Colours::lightgrey;

        g.setColour(lineColor);
        g.drawLine((float)x, (float)y + height * 0.5f, (float)x + width, (float)y + height * 0.5f, lineThickness);

        // Thumb (handle)
        auto thumbWidth = 10.0f;   // Width of the rectangle
        auto thumbHeight = 24.0f; // Height of the rectangle
        auto thumbColor = juce::Colours::black;

        g.setColour(thumbColor);

        // Draw the thumb as a vertical rectangle centered around sliderPos
        g.fillRect(sliderPos - thumbWidth * 0.5f, y + height * 0.5f - thumbHeight * 0.5f, thumbWidth, thumbHeight);

    }

};


//==============================================================================
/**
*/
class DISTORTICONAudioProcessorEditor  : public juce::AudioProcessorEditor, 
    public juce::Slider::Listener 
{
public:
    DISTORTICONAudioProcessorEditor (DISTORTICONAudioProcessor&);
    ~DISTORTICONAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DISTORTICONAudioProcessor& audioProcessor;

    /* LOOK AND FEEL */
    OtherLookAndFeel lookAndFeel;

    /* DISTORTION */
    // drive
    juce::Slider driveSlider;
    juce::Label driveLabel;

    // bit depth
    juce::Slider bitDepthSlider;
    juce::Label bitDepthLabel;

    // downsampling
    juce::Slider downsamplingSlider;
    juce::Label downsamplingLabel;

    /* PRE-FILTER */
    juce::Label preFilterLabel;
    juce::Label preFilterGainLabel;

    // low shelf
    juce::Slider preLowFreqSlider;
    juce::Label preLowFreqLabel;

    juce::Slider preLowGainSlider;
    juce::Label preLowGainLabel;

    // peak notch
    juce::Slider prePeakNotchFreqSlider;
    juce::Label prePeakNotchFreqLabel;

    juce::Slider prePeakNotchGainSlider;
    juce::Label prePeakNotchGainLabel;

    // high shelf
    juce::Slider preHighFreqSlider;
    juce::Label preHighFreqLabel;

    juce::Slider preHighGainSlider;
    juce::Label preHighGainLabel;
    
    /* POST-FILTER */
    juce::Label postFilterLabel;
    juce::Label postFilterGainLabel;

    // low shelf
    juce::Slider postLowFreqSlider;
    juce::Label postLowFreqLabel;

    juce::Slider postLowGainSlider;
    juce::Label postLowGainLabel;

    // peak notch
    juce::Slider postPeakNotchFreqSlider;
    juce::Label postPeakNotchFreqLabel;

    juce::Slider postPeakNotchGainSlider;
    juce::Label postPeakNotchGainLabel;

    // high shelf
    juce::Slider postHighFreqSlider;
    juce::Label postHighFreqLabel;

    juce::Slider postHighGainSlider;
    juce::Label postHighGainLabel;

    /* GAIN */
    // dry gain
    juce::Slider dryGainSlider;
    juce::Label dryGainLabel;

    // wet gain
    juce::Slider wetGainSlider;
    juce::Label wetGainLabel;   


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DISTORTICONAudioProcessorEditor)
};
