# Vector Phaseshaping Synthesis (VPS) Methods
The following programs provide **VPS** methods with various applications for the **Korg NTS-1 Nu:Tekt digital synthesizer**. This work is based upon the research carried out by Jari Kleimola, Victor Lazzarini, Joseph Timoney, and Vesa Välimäki that was presented in their 2011 paper “Vector Phaseshaping Synthesis“. This work represents only a partial treatment of the contents of their work and further experimentation is required to implement those methods not covered here.

## [1 - Basic Phase Distortion Synthesis](https://github.com/GrahamJamesKeane/VPS/tree/main/1%20-%20Basic%20Phase%20Distortion)

In order to establish a basis for **VPS** we will begin with the classic phase distortion technique which utilises a single inflection point with one variable.

## [2 - Basic VPS](https://github.com/GrahamJamesKeane/VPS/tree/main/2%20-%20VPS)

**VPS** extends the basic phase distortion method and introduces a 2-dimensional inflection point which expands the potential for waveshaping synthesis techniques.

## [3 - Formant Synthesis](https://github.com/GrahamJamesKeane/VPS/tree/main/3%20-%20Formant%20Synthesis)

Manipulation of the **v-component** of the 2D inflection point around certain values produces formant peaks which create unusual and pleasant vowel effects in the output.

## [4 - Multiple Inflection Points (MVPS)](https://github.com/GrahamJamesKeane/VPS/tree/main/4%20-%20Multiple%20Inflection%20points)

We can expand **VPS** with multiple inflection points which provides even greater variety in the waveshaping we can perform.

## 5 - Adaptive Vector Control (AVC)

The aspects of **AVC** covered here involve modulating the inflection points with low frequency oscillators.

### [4.1 - 1D Control](https://github.com/GrahamJamesKeane/VPS/tree/main/4%20-%20Multiple%20Inflection%20points/mvps1D)

A 1D LFO modulation of the **d-components** in the **MVPS** program.

### [4.2 - 2D LFM](https://github.com/GrahamJamesKeane/VPS/tree/main/4%20-%20Multiple%20Inflection%20points/mvps2D)

A 2D LFO modulation of both component groups in the **MVPS** program.

### [4.3 - Lissajous Modulation](https://github.com/GrahamJamesKeane/VPS/tree/main/5%20-%20Lissajous%20Modulation)

A complete modulation of the **d** and **v** components via LFO which generates values from paired trigonometric function with synchronised phases that scales amplituides associated with the **d** and **v** components. The path of the inflection point traces **Lissajous** figures with interesting audio qualities.
