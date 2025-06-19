/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DISTORTICONAudioProcessor::DISTORTICONAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(preLowFreqParam = new juce::AudioParameterFloat(
        "preLowFreq", 
        "Pre Low Freq",
        0.0f,
        500.0f,
        100.0f));
    addParameter(prePeakNotchFreqParam = new juce::AudioParameterFloat(
        "prePeakNotchFreq",
        "Pre Peak/Notch Freq",
        200.0f,
        5000.0f,
        1000.0f));
    addParameter(preHighFreqParam = new juce::AudioParameterFloat(
        "preHighFreq",
        "Pre High Freq",
        1000.0f,
        20000.0f,
        10000.0f));
    addParameter(postLowFreqParam = new juce::AudioParameterFloat(
        "postLowFreq",
        "Post Low Freq",
        0.0f,
        500.0f,
        100.0f));
    addParameter(postPeakNotchFreqParam = new juce::AudioParameterFloat(
        "postPeakNotchFreq",
        "Post Peak/Notch Freq",
        200.0f,
        5000.0f,
        1000.0f));
    addParameter(postHighFreqParam = new juce::AudioParameterFloat(
        "postHighFreq",
        "Post High Freq",
        1000.0f,
        20000.0f,
        10000.0f));
    addParameter(preLowGainParam = new juce::AudioParameterFloat(
        "preLowGain",
        "Pre Low Gain",
        -40.0f,
        20.0f,
        0.0f));
    addParameter(prePeakNotchGainParam = new juce::AudioParameterFloat(
        "prePeakNotchGain",
        "Pre Peak/Notch Gain",
        -40.0f,
        20.0f,
        0.0f));
    addParameter(preHighGainParam = new juce::AudioParameterFloat(
        "preHighGain",
        "Pre High Gain",
        -40.0f,
        20.0f,
        0.0f));
    addParameter(postLowGainParam = new juce::AudioParameterFloat(
        "postLowGain",
        "Post Low Gain",
        -40.0f,
        20.0f,
        0.0f));
    addParameter(postPeakNotchGainParam = new juce::AudioParameterFloat(
        "postPeakNotchGain",
        "Post Peak/Notch Gain",
        -40.0f,
        20.0f,
        0.0f));
    addParameter(postHighGainParam = new juce::AudioParameterFloat(
        "postHighGain",
        "Post High Gain",
        -40.0f,
        20.0f,
        0.0f));
    addParameter(dryGainParam = new juce::AudioParameterFloat(
        "dryGain", 
        "Dry Gain", 
        0.0f,
        100.0f,
        0.0f));
    addParameter(wetGainParam = new juce::AudioParameterFloat(
        "wetGain", 
        "Wet Gain", 
        0.0f,
        100.0f,
        100.0f));
    addParameter(driveParam = new juce::AudioParameterFloat(
        "drive",
        "Drive",
        1.0f,
        100.0f,
        1.0f));
    addParameter(bitDepthParam = new juce::AudioParameterFloat(
        "bitDepth",
        "Bit Depth",
        1.0f,
        16.0f,
        16.0f));
    addParameter(downsamplingParam = new juce::AudioParameterFloat(
        "downsampling", 
        "Downsampling", 
        1.0f, 
        16.0f, 
        1.0f));
}

DISTORTICONAudioProcessor::~DISTORTICONAudioProcessor()
{
}

//==============================================================================
const juce::String DISTORTICONAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DISTORTICONAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DISTORTICONAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DISTORTICONAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DISTORTICONAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DISTORTICONAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DISTORTICONAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DISTORTICONAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DISTORTICONAudioProcessor::getProgramName (int index)
{
    return {};
}

void DISTORTICONAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DISTORTICONAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sr = sampleRate;
    dryGain = dryGainParam->get();
    wetGain = wetGainParam->get();
    drive = driveParam->get();
    bitDepth = bitDepthParam->get();
    downsamplingFactor = downsamplingParam->get();
}

void DISTORTICONAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DISTORTICONAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DISTORTICONAudioProcessor::calcPreFilterParams()
{
    // pre low shelf
    float preLowCoeffs[5];
    float preLowFreq = preLowFreqParam->get();
    float preLowGain = preLowGainParam->get();
    Mu45FilterCalc::calcCoeffsLowShelf(preLowCoeffs, preLowFreq, preLowGain, sr);
    preLowShelfL.setCoefficients(preLowCoeffs[0], preLowCoeffs[1], preLowCoeffs[2], preLowCoeffs[3], preLowCoeffs[4]);
    preLowShelfR.setCoefficients(preLowCoeffs[0], preLowCoeffs[1], preLowCoeffs[2], preLowCoeffs[3], preLowCoeffs[4]);

    // pre peak notch
    float prePeakNotchCoeffs[5];
    float prePeakNotchFreq = prePeakNotchFreqParam->get();
    float prePeakNotchGain = prePeakNotchGainParam->get();
    float prePeakNotchQ = 15.0f;
    Mu45FilterCalc::calcCoeffsPeak(prePeakNotchCoeffs, prePeakNotchFreq, prePeakNotchGain, prePeakNotchQ, sr);
    prePeakNotchL.setCoefficients(prePeakNotchCoeffs[0], prePeakNotchCoeffs[1], prePeakNotchCoeffs[2], prePeakNotchCoeffs[3], prePeakNotchCoeffs[4]);
    prePeakNotchR.setCoefficients(prePeakNotchCoeffs[0], prePeakNotchCoeffs[1], prePeakNotchCoeffs[2], prePeakNotchCoeffs[3], prePeakNotchCoeffs[4]);

    // pre high shelf
    float preHighCoeffs[5];
    float preHighFreq = preHighFreqParam->get();
    float preHighGain = preHighGainParam->get();
    Mu45FilterCalc::calcCoeffsHighShelf(preHighCoeffs, preHighFreq, preHighGain, sr);
    preHighShelfL.setCoefficients(preHighCoeffs[0], preHighCoeffs[1], preHighCoeffs[2], preHighCoeffs[3], preHighCoeffs[4]);
    preHighShelfR.setCoefficients(preHighCoeffs[0], preHighCoeffs[1], preHighCoeffs[2], preHighCoeffs[3], preHighCoeffs[4]);
}

void DISTORTICONAudioProcessor::calcPostFilterParams()
{
    // post low shelf
    float postLowCoeffs[5];
    float postLowFreq = postLowFreqParam->get();
    float postLowGain = postLowGainParam->get();
    Mu45FilterCalc::calcCoeffsLowShelf(postLowCoeffs, postLowFreq, postLowGain, sr);
    postLowShelfL.setCoefficients(postLowCoeffs[0], postLowCoeffs[1], postLowCoeffs[2], postLowCoeffs[3], postLowCoeffs[4]);
    postLowShelfR.setCoefficients(postLowCoeffs[0], postLowCoeffs[1], postLowCoeffs[2], postLowCoeffs[3], postLowCoeffs[4]);

    // post peak notch
    float postPeakNotchCoeffs[5];
    float postPeakNotchFreq = postPeakNotchFreqParam->get();
    float postPeakNotchGain = postPeakNotchGainParam->get();
    float postPeakNotchQ = 15.0f;
    Mu45FilterCalc::calcCoeffsPeak(postPeakNotchCoeffs, postPeakNotchFreq, postPeakNotchGain, postPeakNotchQ, sr);
    postPeakNotchL.setCoefficients(postPeakNotchCoeffs[0], postPeakNotchCoeffs[1], postPeakNotchCoeffs[2], postPeakNotchCoeffs[3], postPeakNotchCoeffs[4]);
    postPeakNotchR.setCoefficients(postPeakNotchCoeffs[0], postPeakNotchCoeffs[1], postPeakNotchCoeffs[2], postPeakNotchCoeffs[3], postPeakNotchCoeffs[4]);

    // post high shelf
    float postHighCoeffs[5];
    float postHighFreq = postHighFreqParam->get();
    float postHighGain = postHighGainParam->get();
    Mu45FilterCalc::calcCoeffsHighShelf(postHighCoeffs, postHighFreq, postHighGain, sr);
    postHighShelfL.setCoefficients(postHighCoeffs[0], postHighCoeffs[1], postHighCoeffs[2], postHighCoeffs[3], postHighCoeffs[4]);
    postHighShelfR.setCoefficients(postHighCoeffs[0], postHighCoeffs[1], postHighCoeffs[2], postHighCoeffs[3], postHighCoeffs[4]);
}

void DISTORTICONAudioProcessor::calcAlgorithmParams()
{
    dryGain = dryGainParam->get();
    wetGain = wetGainParam->get();
    drive = driveParam->get();
    bitDepth = bitDepthParam->get();
    downsamplingFactor = downsamplingParam->get();
}

void applyDownsampling(float* channelData, int numSamples, int downsamplingFactor)
{
    int writeIndex = 0;

    for (int readIndex = 0; readIndex < numSamples; readIndex += downsamplingFactor)
    {
        channelData[writeIndex++] = channelData[readIndex];
    }

    // Zero out the rest of the buffer to avoid unexpected artifacts
    std::fill(channelData + writeIndex, channelData + numSamples, 0.0f);
}

    void DISTORTICONAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
    {
        juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels  = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();

        // In case we have more outputs than inputs, this code clears any output
        // channels that didn't contain input data, (because these aren't
        // guaranteed to be empty - they may contain garbage).
        // This is here to avoid people getting screaming feedback
        // when they first compile a plugin, but obviously you don't need to keep
        // this code if your algorithm always overwrites all the output channels.
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());

        calcPreFilterParams();
        calcPostFilterParams();
        calcAlgorithmParams();

        auto* leftChannel = buffer.getWritePointer(0);
        auto* rightChannel = buffer.getWritePointer(1);

        // This is the place where you'd normally do the guts of your plugin's
        // audio processing...
        // Make sure to reset the state if your inner loop is processing
        // the samples and the outer loop is handling the channels.
        // Alternatively, you can process the samples with the channels
        // interleaved by keeping the same state.
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float dryLeft = leftChannel[sample];
            float dryRight = rightChannel[sample];
            float wetLeft = leftChannel[sample];
            float wetRight = rightChannel[sample];

            if (sample % downsamplingFactor == 0)
            {

                // pre filter
                float preFilterSampleLeft = preLowShelfL.tick(dryLeft);
                preFilterSampleLeft = prePeakNotchL.tick(preFilterSampleLeft);
                preFilterSampleLeft = preHighShelfL.tick(preFilterSampleLeft);

                float preFilterSampleRight = preLowShelfR.tick(dryRight);
                preFilterSampleRight = prePeakNotchR.tick(preFilterSampleRight);
                preFilterSampleRight = preHighShelfR.tick(preFilterSampleRight);

                // distortion -- normalize 1-100 of user param into 1-10 for drive algo param
                float distortedSampleLeft = std::tanh((((drive - 1) / 99.0f) * 9.0f + 1.0f) * preFilterSampleLeft);
                float distortedSampleRight = std::tanh((((drive - 1) / 99.0f) * 9.0f + 1.0f) * preFilterSampleRight);

                // post filter
                wetLeft = postLowShelfL.tick(distortedSampleLeft);
                wetLeft = postPeakNotchL.tick(distortedSampleLeft);
                wetLeft = postHighShelfL.tick(distortedSampleLeft);

                wetRight = postLowShelfR.tick(distortedSampleRight);
                wetRight = postPeakNotchR.tick(distortedSampleRight);
                wetRight = postHighShelfR.tick(distortedSampleRight);

                
            }
            else
            {
                leftChannel[sample] = 0;
                rightChannel[sample] = 0;
            }

            // bit depth
            float stepSize = 2.0f / (1 << bitDepth);
            float bitDepthSampleLeft = std::floor(wetLeft / stepSize + 0.5f) * stepSize;
            float bitDepthSampleRight = std::floor(wetRight / stepSize + 0.5f) * stepSize;

            // dry wet
            leftChannel[sample] = (dryGain / 100.0f) * dryLeft + (wetGain / 100.0f) * bitDepthSampleLeft;
            rightChannel[sample] = (dryGain / 100.0f) * dryRight + (wetGain / 100.0f) * bitDepthSampleRight;
            
        }
    }


//==============================================================================
bool DISTORTICONAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DISTORTICONAudioProcessor::createEditor()
{
    return new DISTORTICONAudioProcessorEditor (*this);
}

//==============================================================================
void DISTORTICONAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DISTORTICONAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DISTORTICONAudioProcessor();
}
