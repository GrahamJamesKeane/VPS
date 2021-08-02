# 1 - VPS with Lissajous 2D LFO Modulation (VPSLJ)

This custom oscillator was designed for the **Korg NTS-1 Nu:Tekt digital synthesizer**. The programs implements Lissajous modulation via 2D LFO for the Vector Phaseshaping (VPS) 
synthesis technique. It also utilises pitch-tracked filters from the BiQuad utility and interpolation to reduce noise in the output.

- [VPSLJ Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/5%20-%20Lissajous%20Modulation/vpslj_1)

- [VPSLJ unit file](https://github.com/GrahamJamesKeane/VPS/blob/main/5%20-%20Lissajous%20Modulation/vpslj_1/vpslj_1.ntkdigunit)

## 1.1 - VPSLJ Controls 
This program has six user parameters;

- Slot 1: Base LFO Freq, range [0-30]Hz.
- Slot 2: D Shift, range [0-6].
- Slot 3: V Shift, range [0-6].
- Slot 4: Amp D, range [0-1].
- Slot 5: Amp V, range [0-5].
- Slot 6: Phase Offest, range [0-1].

These parameters can be accessed via the **shift** function of the **OSC** button and the **TYPE** knob. The parameters are modified via the **B** knob once selected. 

- Shape: Frequency ratio, range [1-101].

This parameter can be accessed via the primary function of the **A** knob.

# 2 - Project Build
- [Vector Phaseshaping Synthesis](https://korgnts1beginnersguide.wordpress.com/?p=760)
