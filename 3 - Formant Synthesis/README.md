# 1 - Formant Synthesis with VPS (VPSF)

This custom oscillator was designed for the **Korg NTS-1 Nu:Tekt digital synthesizer**. This programs implements formant peak synthesis using the Vector Phaseshaping (VPS) technique. The program utilises pitch-tracked filters from the BiQuad utility to reduce noise in the output. It also uses bipolar parameters to increase the precision of the sweep for the inflection point components.

- [VPSF Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/3%20-%20Formant%20Synthesis/vpsf_1)

- [VPSF unit file](https://github.com/GrahamJamesKeane/VPS/blob/main/3%20-%20Formant%20Synthesis/vpsf_1/vpsf_1.ntkdigunit)

## 1.1 - VPSF Controls 
This program has four user parameters.

- Slot 1: D value, range [0-1].
- Slot 2: V value, range [0-1].
- Slot 3: V range, range [1-10]
- Slot 4: Frequency ratio, range [1-101].

These parameters can be accessed via the **shift** function of the **OSC** button and the **TYPE** knob. The parameters are modified via the **B** knob once selected. 

# 2 - VPSF with Interpolation Gain (VPSFI)

This version of VPSF introduces an interpolation gain to tackle discontinuities in the waveform by linearly mixing two separate VPS signals that are scaled by this interpolation gain. There is high and low-pass processing at the generation point of each signal as well as separate linear interpolation to mask the aliasing produced by the abrupt phase shifts resulting from interval gaps in the d and v parameter ranges.

- [VPSFI Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/3%20-%20Formant%20Synthesis/vpsf_2)

- [VPSFI Unit File](https://github.com/GrahamJamesKeane/VPS/blob/main/3%20-%20Formant%20Synthesis/vpsf_2/vpsf_2.ntkdigunit)

## 2.1 - VPSFI Controls 
This program shares the same controls as VPSF above.

# 3 - Project Build
