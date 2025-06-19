/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StkLite-4.6.2/BiQuad.h"
#include "Mu45FilterCalc/Mu45FilterCalc.h"

//==============================================================================
/**
*/
class DISTORTICONAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DISTORTICONAudioProcessor();
    ~DISTORTICONAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_postferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;



private:
    //==============================================================================

    // user params
    juce::AudioParameterFloat* driveParam;
    juce::AudioParameterFloat* bitDepthParam;
    juce::AudioParameterFloat* downsamplingParam;
    juce::AudioParameterFloat* preLowFreqParam;
    juce::AudioParameterFloat* preLowGainParam;
    juce::AudioParameterFloat* prePeakNotchFreqParam;
    juce::AudioParameterFloat* prePeakNotchGainParam;
    juce::AudioParameterFloat* preHighFreqParam;
    juce::AudioParameterFloat* preHighGainParam;
    juce::AudioParameterFloat* postLowFreqParam;
    juce::AudioParameterFloat* postLowGainParam;
    juce::AudioParameterFloat* postPeakNotchFreqParam;
    juce::AudioParameterFloat* postPeakNotchGainParam;
    juce::AudioParameterFloat* postHighFreqParam;
    juce::AudioParameterFloat* postHighGainParam;
    juce::AudioParameterFloat* dryGainParam;
    juce::AudioParameterFloat* wetGainParam;

    // algorithm parameters
    float sr;
    float dryGain;
    float wetGain;
    float drive;
    int bitDepth;
    int downsamplingFactor;
    int downsampleCounter;

    // methods
    void calcAlgorithmParams();
    void calcPreFilterParams();
    void calcPostFilterParams();

    // filters
    stk::BiQuad preLowShelfL, preLowShelfR, prePeakNotchL, prePeakNotchR, preHighShelfL, preHighShelfR;
    stk::BiQuad postLowShelfL, postLowShelfR, postPeakNotchL, postPeakNotchR, postHighShelfL, postHighShelfR;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DISTORTICONAudioProcessor)
};
