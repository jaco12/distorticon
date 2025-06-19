# DISTORTICON

An audio distortion plugin with added downsampling, bit-depth reduction, filtering, and gain capabilities.

## Installation

1. Copy the .vst3 file into the respective plugins folder of your preferred digital audio workstation (DAW).
2. In your DAW, open the plugin manager and run a scan so that it can be discovered (e.g., _"Scan for installed plugins"_ in FL Studio or _"Re-scan"_ in REAPER).
3. Give it a go!

## User Interface

![Screenshot of the DISTORTICON user interface](https://github.com/jaco12/distorticon/blob/main/info/distorticon-ui-screenshot.png)

## User Parameters

1. 	**<ins>Pre-filter</ins>:** low shelf, peak/notch, and high shelf filter before the distortion is added
    - Low shelf transition frequency: _0 Hz – 500 Hz_
      - Gain: _-40 dB – +20 dB_
    - Peak/notch center frequency: _200 Hz – 5,000 Hz_
      - Gain: _-40 dB – +20 dB_
    - High shelf transition frequency: _1,000 Hz – 20,000 Hz_
      - Gain: _-40 dB – +20dB_
2. **<ins>Drive</ins>:** increases the gain and soft clips it at a certain threshold using the std::tanh function
3. **<ins>Post-filter</ins>:** filters after the distortion
   - Same filters and gain control as pre-filters
3. **<ins>Downsampling</ins>:** reduces the sample rate resolution by passing through every n samples only
5. **<ins>Bit depth</ins>:** quantizes input sample to a limited number of levels
6. **<ins>Dry gain</ins>:** Gain control for the original, dry audio input. Summed with the wet gain
7. **<ins>Wet gain</ins>:** Gain control for the affected, wet audio input. Summed with the dry gain

## Use Cases

To test this effect, a simple drum break was used as the input audio because the transients of the drums allow for the distortion to sound more noticeable. This drum break was used to create five simple use cases to showcase the features of the plugin:

- A test case that emphasizes the drive effect
- A test case that emphasizes the bit depth reduction
- A test case that emphasizes the downsampling functionality
- A test case that uses all three effects to an extreme in order to showcase how much my plugin can transform the original audio
- A use case that uses all three effects, but more subtly and within the context of a larger piece of music
