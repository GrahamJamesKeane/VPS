# 1 - VPS with Multiple Inflection Points (MVPS)

This custom oscillator was designed for the **Korg NTS-1 Nu:Tekt digital synthesizer**. The programs implements multiple inflection points for Vector Phaseshaping (VPS) 
synthesis. technique. It also utilises pitch-tracked filters from the BiQuad utility and interpolation to reduce noise in the output.

- [MVPS Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/4%20-%20Multiple%20Inflection%20points/mvps)

- [MVPS unit file](https://github.com/GrahamJamesKeane/VPS/blob/main/4%20-%20Multiple%20Inflection%20points/mvps/mvps.ntkdigunit)

## 1.1 - MVPS Controls 
This program has six user parameters;

- Slot 1: D0 value, range [0-1].
- Slot 2: V0 value, range [0-1].
- Slot 3: D1 range, range [0-1].
- Slot 4: V1 value, range [0-1].
- Slot 5: D2 range, range [0-1].
- Slot 6: V2 value, range [0-1].

These parameters can be accessed via the **shift** function of the **OSC** button and the **TYPE** knob. The parameters are modified via the **B** knob once selected. 

- Shape: Frequency ratio, range [1-101].

This parameter can be accessed via the primary function of the **A** knob.

# 2 - VPS with Multiple Inflection Points and 1D LFO Modulation (MVPS1D)

This custom oscillator was designed for the **Korg NTS-1 Nu:Tekt digital synthesizer**. The programs implements multiple inflection points for Vector Phaseshaping (VPS) synthesis technique with 1D LFO modulation of the d-group parameters. It also utilises pitch-tracked filters from the BiQuad utility and interpolation to reduce noise in the output.

- [MVPS1D Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/4%20-%20Multiple%20Inflection%20points/mvps1D)

- [MVPS1D unit file](https://github.com/GrahamJamesKeane/VPS/blob/main/4%20-%20Multiple%20Inflection%20points/mvps1D/mvps1D.ntkdigunit)

## 2.1 - MVPS1D Controls 
This program shares the same six user parameters as MVPS as well as the shape parameter (frequency ratio).

The LFO can accessed via the **shift** function of the **OSC** button. The **A** knob controls the LFO's frequency and the **B** knob controls the amplitude. The overall pitch can also be manipulated here via the B knob (Bipolar parameter).

# 3 - VPS with Multiple Inflection Points and 2D LFO Modulation (MVPS2D)

This custom oscillator was designed for the **Korg NTS-1 Nu:Tekt digital synthesizer**. The programs implements multiple inflection points for Vector Phaseshaping (VPS) synthesis technique with 2D LFO modulation of the d-group and v-group parameters. It also utilises pitch-tracked filters from the BiQuad utility and interpolation to reduce noise in the output.

- [MVPS2D Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/4%20-%20Multiple%20Inflection%20points/mvps2D)

- [MVPS2D unit file](https://github.com/GrahamJamesKeane/VPS/blob/main/4%20-%20Multiple%20Inflection%20points/mvps2D/mvps2D.ntkdigunit)

## 3.1 - MVPS2D Controls 
This program shares the same six user parameters as MVPS as well as the shape parameter (frequency ratio) and native LFO controls (LFO-D).

- Shiftshape: LFO-V frequency, range [0-30]Hz

This parameter can be accessed via the primary function of the **B** knob.

# 4 - Project Build
