# 1 - Basic Vector Phaseshaping Synthesis (VPS)

This custom oscillator was designed for the **Korg NTS-1 Nu:Tekt digital synthesizer**. This programs implements the Vector Phaseshaping (VPS) technique. It utilises pitch-tracked filters from the BiQuad utility to reduce noise in the output.

- [Basic VPS Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/2%20-%20VPS/vps_basic)

- [Basic VPS unit file](https://github.com/GrahamJamesKeane/VPS/blob/main/2%20-%20VPS/vps_basic/vps_basic.ntkdigunit)

## 1.1 - Basic VPS Controls 
This program has three user parameters.

- Slot 1: D value, range [0-1].
- Slot 2: V value, range [0-1].
- Slot 3: Frequency ratio, range [1-101].

These parameters can be accessed via the shift function of the OSC button and the TYPE knob. The parameters are modified via the B knob once selected. 

# 2 - Basic VPS with Interpolation

This version of the basic VPS program introduces linear interpolation to help smoothen the transition between phase values which otherwise result in abrupt and unpleasant changes in the curvature of the waveform.

- [VPS with Interpolation Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/2%20-%20VPS/vps_intpl)

- [VPS with Interpolation Unit File](https://github.com/GrahamJamesKeane/VPS/blob/main/2%20-%20VPS/vps_intpl/vps_intpl.ntkdigunit)

## 2.1 - Basic VPS with Interpolation Controls 
This program shares the same controls as the Basic version above.

# 3 - Project Build
