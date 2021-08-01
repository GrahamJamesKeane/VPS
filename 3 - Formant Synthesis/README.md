# 1 - Formant Synthesis with VPS (VPSF)

This custom oscillator was designed for the **Korg NTS-1 Nu:Tekt digital synthesizer**. This programs implements formant peak synthesis using the Vector Phaseshaping (VPS) technique. It also utilises pitch-tracked filters from the BiQuad utility to reduce noise in the output.

- [VPSF Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/3%20-%20Formant%20Synthesis/vpsf_1)

- [VPSF unit file](https://github.com/GrahamJamesKeane/VPS/blob/main/3%20-%20Formant%20Synthesis/vpsf_1/vpsf_1.ntkdigunit)

## 1.1 - VPSF Controls 
This program has four user parameters.

- Slot 1: D value, range [0-1].
- Slot 2: V value, range [0-1].
- Slot 3: V range, range [1-10]
- Slot 4: Frequency ratio, range [1-101].

These parameters can be accessed via the shift function of the OSC button and the TYPE knob. The parameters are modified via the B knob once selected. 

# 2 - VPSF with Interpolation Gain (VPSFI)

This version of the VPSF introduces an interpolation gain to tackle discontinuities in the waveform.

- [VPSFI Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/3%20-%20Formant%20Synthesis/vpsf_2)

- [VPSFI Unit File](https://github.com/GrahamJamesKeane/VPS/blob/main/3%20-%20Formant%20Synthesis/vpsf_2/vpsf_2.ntkdigunit)

## 2.1 - VPSFI Controls 
This program shares the same controls as VPSF above.

# 3 - Project Build
