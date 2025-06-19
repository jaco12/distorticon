/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DISTORTICONAudioProcessorEditor::DISTORTICONAudioProcessorEditor (DISTORTICONAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 600);
    auto& processorParams = audioProcessor.getParameters();

    /* LOOK AND FEEL */
    setLookAndFeel(&lookAndFeel);

    /* DISTORTION */
    // DRIVE
    driveSlider.setBounds(450, 50, 100, 100);
    driveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    juce::AudioParameterFloat* driveParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(14);
    driveSlider.setRange(driveParam->range.start, driveParam->range.end, 1);
    driveSlider.setValue(*driveParam);
    driveSlider.setDoubleClickReturnValue(true, 1);
    driveSlider.addListener(this);
    addAndMakeVisible(driveSlider);

    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.setJustificationType(juce::Justification::centredTop);
    driveLabel.attachToComponent(&driveSlider, false);
    addAndMakeVisible(driveLabel);

    // BIT DEPTH
    bitDepthSlider.setBounds(350, 50, 100, 100);
    bitDepthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    bitDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    juce::AudioParameterFloat* bitDepthParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(15);
    bitDepthSlider.setRange(bitDepthParam->range.start, bitDepthParam->range.end, 1);
    bitDepthSlider.setDoubleClickReturnValue(true, 16);
    bitDepthSlider.setValue(*bitDepthParam);
    bitDepthSlider.addListener(this);
    addAndMakeVisible(bitDepthSlider);

    bitDepthLabel.setText("Bit Depth", juce::dontSendNotification);
    bitDepthLabel.setJustificationType(juce::Justification::centredTop);
    bitDepthLabel.attachToComponent(&bitDepthSlider, false);
    addAndMakeVisible(bitDepthLabel);

    // DOWNSAMPLING
    downsamplingSlider.setBounds(550, 50, 100, 100);
    downsamplingSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    downsamplingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    juce::AudioParameterFloat* downsamplingParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(16);
    downsamplingSlider.setRange(downsamplingParam->range.start, downsamplingParam->range.end, 1);
    downsamplingSlider.setValue(*downsamplingParam);
    downsamplingSlider.setDoubleClickReturnValue(true, 1);
    downsamplingSlider.addListener(this);
    addAndMakeVisible(downsamplingSlider);

    downsamplingLabel.setText("Downsampling", juce::dontSendNotification);
    downsamplingLabel.setJustificationType(juce::Justification::centredTop);
    downsamplingLabel.attachToComponent(&downsamplingSlider, false);
    addAndMakeVisible(downsamplingLabel);

    /* PRE-FILTER */
    preFilterLabel.setText("Pre-Filter", juce::dontSendNotification);
    preFilterLabel.setBounds(185, 150, 100, 50);
    addAndMakeVisible(preFilterLabel);
    preFilterGainLabel.setText("Gain", juce::dontSendNotification);
    preFilterGainLabel.setBounds(390, 150, 75, 50);
    addAndMakeVisible(preFilterGainLabel);

    // PRE LOW
    preLowFreqSlider.setBounds(125, 213, 225, 50);
    preLowFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    preLowFreqSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 60, 25);
    juce::AudioParameterFloat* preLowFreqParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(0);
    preLowFreqSlider.setRange(preLowFreqParam->range.start, preLowFreqParam->range.end, 1);
    preLowFreqSlider.setDoubleClickReturnValue(true, 0);
    preLowFreqSlider.setValue(*preLowFreqParam);
    preLowFreqSlider.addListener(this);
    addAndMakeVisible(preLowFreqSlider);

    preLowFreqLabel.setText("Low Shelf Freq", juce::dontSendNotification);
    preLowFreqLabel.attachToComponent(&preLowFreqSlider, true);
    addAndMakeVisible(preLowFreqLabel);

    preLowGainSlider.setBounds(375, 200, 75, 75);
    preLowGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    preLowGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 60, 25);
    juce::AudioParameterFloat* preLowGainParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(6);
    preLowGainSlider.setRange(preLowGainParam->range.start, preLowGainParam->range.end, 1);
    preLowGainSlider.setDoubleClickReturnValue(true, 0);
    preLowGainSlider.setValue(*preLowGainParam);
    preLowGainSlider.addListener(this);
    addAndMakeVisible(preLowGainSlider);

    // PRE PEAK NOTCH
    prePeakNotchFreqSlider.setBounds(125, 288, 225, 50);
    prePeakNotchFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    prePeakNotchFreqSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 60, 25);
    juce::AudioParameterFloat* prePeakNotchFreqParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(1);
    prePeakNotchFreqSlider.setRange(prePeakNotchFreqParam->range.start, prePeakNotchFreqParam->range.end, 1);
    prePeakNotchFreqSlider.setDoubleClickReturnValue(true, 1000);
    prePeakNotchFreqSlider.setValue(*prePeakNotchFreqParam);
    prePeakNotchFreqSlider.addListener(this);
    addAndMakeVisible(prePeakNotchFreqSlider);

    prePeakNotchFreqLabel.setText("Peak/Notch Freq", juce::dontSendNotification);
    prePeakNotchFreqLabel.attachToComponent(&prePeakNotchFreqSlider, true);
    addAndMakeVisible(prePeakNotchFreqLabel);

    prePeakNotchGainSlider.setBounds(375, 275, 75, 75);
    prePeakNotchGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    prePeakNotchGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 60, 25);
    juce::AudioParameterFloat* prePeakNotchGainParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(7);
    prePeakNotchGainSlider.setRange(prePeakNotchGainParam->range.start, prePeakNotchGainParam->range.end, 1);
    prePeakNotchGainSlider.setDoubleClickReturnValue(true, 0);
    prePeakNotchGainSlider.setValue(*prePeakNotchGainParam);
    prePeakNotchGainSlider.addListener(this);
    addAndMakeVisible(prePeakNotchGainSlider);

    // PRE HIGH
    preHighFreqSlider.setBounds(125, 363, 225, 50);
    preHighFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    preHighFreqSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 60, 25);
    juce::AudioParameterFloat* preHighFreqParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(2);
    preHighFreqSlider.setRange(preHighFreqParam->range.start, preHighFreqParam->range.end, 1);
    preHighFreqSlider.setDoubleClickReturnValue(true, 10000);
    preHighFreqSlider.setValue(*preHighFreqParam);
    preHighFreqSlider.addListener(this);
    addAndMakeVisible(preHighFreqSlider);

    preHighFreqLabel.setText("High Shelf Freq", juce::dontSendNotification);
    preHighFreqLabel.attachToComponent(&preHighFreqSlider, true);
    addAndMakeVisible(preHighFreqLabel);

    preHighGainSlider.setBounds(375, 350, 75, 75);
    preHighGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    preHighGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 60, 25);
    juce::AudioParameterFloat* preHighGainParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(8);
    preHighGainSlider.setRange(preHighGainParam->range.start, preHighGainParam->range.end, 1);
    preHighGainSlider.setDoubleClickReturnValue(true, 0);
    preHighGainSlider.setValue(*preHighGainParam);
    preHighGainSlider.addListener(this);
    addAndMakeVisible(preHighGainSlider);

    /* POST-FILTER */
    postFilterLabel.setText("Post-Filter", juce::dontSendNotification);
    postFilterLabel.setBounds(685, 150, 100, 50);
    addAndMakeVisible(postFilterLabel);
    postFilterGainLabel.setText("Gain", juce::dontSendNotification);
    postFilterGainLabel.setBounds(915, 150, 75, 50);
    addAndMakeVisible(postFilterGainLabel);

    // POST LOW
    postLowFreqSlider.setBounds(625, 213, 225, 50);
    postLowFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    postLowFreqSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 60, 25);
    juce::AudioParameterFloat* postLowFreqParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(3);
    postLowFreqSlider.setRange(postLowFreqParam->range.start, postLowFreqParam->range.end, 1);
    postLowFreqSlider.setDoubleClickReturnValue(true, 0);
    postLowFreqSlider.setValue(*postLowFreqParam);
    postLowFreqSlider.addListener(this);
    addAndMakeVisible(postLowFreqSlider);

    postLowFreqLabel.setText("Low Shelf Freq", juce::dontSendNotification);
    postLowFreqLabel.attachToComponent(&postLowFreqSlider, true);
    addAndMakeVisible(postLowFreqLabel);

    postLowGainSlider.setBounds(900, 200, 75, 75);
    postLowGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    postLowGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 60, 25);
    juce::AudioParameterFloat* postLowGainParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(9);
    postLowGainSlider.setRange(postLowGainParam->range.start, postLowGainParam->range.end, 1);
    postLowGainSlider.setDoubleClickReturnValue(true, 0);
    postLowGainSlider.setValue(*postLowGainParam);
    postLowGainSlider.addListener(this);
    addAndMakeVisible(postLowGainSlider);

    // POST PEAK NOTCH
    postPeakNotchFreqSlider.setBounds(625, 288, 225, 50);
    postPeakNotchFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    postPeakNotchFreqSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 60, 25);
    juce::AudioParameterFloat* postPeakNotchFreqParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(4);
    postPeakNotchFreqSlider.setRange(postPeakNotchFreqParam->range.start, postPeakNotchFreqParam->range.end, 1);
    postPeakNotchFreqSlider.setDoubleClickReturnValue(true, 1000);
    postPeakNotchFreqSlider.setValue(*postPeakNotchFreqParam);
    postPeakNotchFreqSlider.addListener(this);
    addAndMakeVisible(postPeakNotchFreqSlider);

    postPeakNotchFreqLabel.setText("Peak/Notch Freq", juce::dontSendNotification);
    postPeakNotchFreqLabel.attachToComponent(&postPeakNotchFreqSlider, true);
    addAndMakeVisible(postPeakNotchFreqLabel);

    postPeakNotchGainSlider.setBounds(900, 275, 75, 75);
    postPeakNotchGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    postPeakNotchGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 60, 25);
    juce::AudioParameterFloat* postPeakNotchGainParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(10);
    postPeakNotchGainSlider.setRange(postPeakNotchGainParam->range.start, postPeakNotchGainParam->range.end, 1);
    postPeakNotchGainSlider.setDoubleClickReturnValue(true, 0);
    postPeakNotchGainSlider.setValue(*postPeakNotchGainParam);
    postPeakNotchGainSlider.addListener(this);
    addAndMakeVisible(postPeakNotchGainSlider);

    // POST HIGH
    postHighFreqSlider.setBounds(625, 363, 225, 50);
    postHighFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    postHighFreqSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 60, 25);
    juce::AudioParameterFloat* postHighFreqParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(5);
    postHighFreqSlider.setRange(postHighFreqParam->range.start, postHighFreqParam->range.end, 1);
    postHighFreqSlider.setValue(*postHighFreqParam);
    postHighFreqSlider.setDoubleClickReturnValue(true, 10000);
    postHighFreqSlider.addListener(this);
    addAndMakeVisible(postHighFreqSlider);

    postHighFreqLabel.setText("High Shelf Freq", juce::dontSendNotification);
    postHighFreqLabel.attachToComponent(&postHighFreqSlider, true);
    addAndMakeVisible(postHighFreqLabel);

    postHighGainSlider.setBounds(900, 350, 75, 75);
    postHighGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    postHighGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 60, 25);
    juce::AudioParameterFloat* postHighGainParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(11);
    postHighGainSlider.setRange(postHighGainParam->range.start, postHighGainParam->range.end, 1);
    postHighGainSlider.setValue(*postHighGainParam);
    postHighGainSlider.setDoubleClickReturnValue(true, 0);
    postHighGainSlider.addListener(this);
    addAndMakeVisible(postHighGainSlider);
    
    /* GAIN */
    // DRY GAIN
    dryGainSlider.setBounds(350, 475, 100, 100);
    dryGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    dryGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    juce::AudioParameterFloat* dryGainParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(12);
    dryGainSlider.setRange(dryGainParam->range.start, dryGainParam->range.end, 1);
    dryGainSlider.setDoubleClickReturnValue(true, 50);
    dryGainSlider.setValue(*dryGainParam);
    dryGainSlider.addListener(this);
    addAndMakeVisible(dryGainSlider);

    dryGainLabel.setText("Dry", juce::dontSendNotification);
    dryGainLabel.setJustificationType(juce::Justification::centredTop);
    dryGainLabel.attachToComponent(&dryGainSlider, false);
    addAndMakeVisible(dryGainLabel);

    // WET GAIN
    wetGainSlider.setBounds(550, 475, 100, 100);
    wetGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    wetGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    juce::AudioParameterFloat* wetGainParam = (juce::AudioParameterFloat*) processorParams.getUnchecked(13);
    wetGainSlider.setRange(wetGainParam->range.start, wetGainParam->range.end, 1);
    wetGainSlider.setDoubleClickReturnValue(true, 50);
    wetGainSlider.setValue(*wetGainParam);
    wetGainSlider.addListener(this);
    addAndMakeVisible(wetGainSlider);

    wetGainLabel.setText("Wet", juce::dontSendNotification);
    wetGainLabel.setJustificationType(juce::Justification::centredTop);
    wetGainLabel.attachToComponent(&wetGainSlider, false);
    addAndMakeVisible(wetGainLabel);

}

void DISTORTICONAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    auto& processorParams = processor.getParameters();
    // freq sliders
    if (slider == &preLowFreqSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(0);
        *audioParam = preLowFreqSlider.getValue();
    }
    else if (slider == &prePeakNotchFreqSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(1);
        *audioParam = prePeakNotchFreqSlider.getValue();
    }
    else if (slider == &preHighFreqSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(2);
        *audioParam = preHighFreqSlider.getValue();
    }
    else if (slider == &postLowFreqSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(3);
        *audioParam = postLowFreqSlider.getValue();
    }
    else if (slider == &postPeakNotchFreqSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(4);
        *audioParam = postPeakNotchFreqSlider.getValue();
    }
    else if (slider == &postHighFreqSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(5);
        *audioParam = postHighFreqSlider.getValue();
    }
    // gain sliders
    else if (slider == &preLowGainSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(6);
        *audioParam = preLowGainSlider.getValue();
    }
    else if (slider == &prePeakNotchGainSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(7);
        *audioParam = prePeakNotchGainSlider.getValue();
    }
    else if (slider == &preHighGainSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(8);
        *audioParam = preHighGainSlider.getValue();
    }
    else if (slider == &postLowGainSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(9);
        *audioParam = postLowGainSlider.getValue();
    }
    else if (slider == &postPeakNotchGainSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(10);
        *audioParam = postPeakNotchGainSlider.getValue();
    }
    else if (slider == &postHighGainSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(11);
        *audioParam = postHighGainSlider.getValue();
    }
    else if (slider == &dryGainSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(12);
        *audioParam = dryGainSlider.getValue();
    }
    else if (slider == &wetGainSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(13);
        *audioParam = wetGainSlider.getValue();
    }
    else if (slider == &driveSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(14);
        *audioParam = driveSlider.getValue();
    }
    else if (slider == &bitDepthSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(15);
        *audioParam = bitDepthSlider.getValue();
    }
    else if (slider == &downsamplingSlider) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(16);
        *audioParam = downsamplingSlider.getValue();
    }
}

DISTORTICONAudioProcessorEditor::~DISTORTICONAudioProcessorEditor()
{
}

//==============================================================================
void DISTORTICONAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::steelblue);

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("DISTORTICON", getLocalBounds(), juce::Justification::centred, 1);
}

void DISTORTICONAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
