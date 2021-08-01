# Basic Phase Distortion (pd_1)

This custom oscillator was designed for the **Korg NTS-1 Nu:Tekt digital synthesizer**. It is a simple program which implements the classic non-linear phase distortion method. The program serves as the foundation for a series of programs that implement a technique called Vector Phaseshaping.

[Basic PD Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/Basic%20Phase%20Distortion/pd_1)

[Basic PD unit file](https://github.com/GrahamJamesKeane/VPS/blob/main/Basic%20Phase%20Distortion/pd_1/pd_1.ntkdigunit)

# Filtered Phase Distortion (pd_2)

This version of the Phase Distortion program has filters included from the BiQuad header to reduce artefects present in the ouput resulting from gaps in phase transitions.

[Filtered PD Program Files](https://github.com/GrahamJamesKeane/VPS/tree/main/Basic%20Phase%20Distortion/pd_2)

[Filtered PD Unit File](https://github.com/GrahamJamesKeane/VPS/blob/main/Basic%20Phase%20Distortion/pd_2/pd_2.ntkdigunit)

## 1 - Controls 
This program has only one parameter which is located at user parameter 1. This parameter controls the amount of phase distortion. The midpoint of the B knob range is the centre value about which an increase or decrease will distort the phase of the oscillator. To select user parameter 1, use the shift function of the OSC button and then select with the TYPE knob.

## 2 - Project Build
