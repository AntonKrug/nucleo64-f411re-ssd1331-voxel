# Benchmarks

Doing whole run on the voxel map (1571 frames) each time and measuring how many miliseconds it took (and calculating the average FPS), Debug configurations are slower but allow better debuging while Release configuration is using -Ofast and is significantly faster.
What is bizzare that some improvements made it slower, yet when testing the most upto date Release candidate and undoing these improvements caused slow down by 7fps. So that looks like even when at that moment that change was not showing significant result when they combined together they were making measurable impact 

| What changed | Debug time | Debug delta  | Debug FPS | Release time | Release delta | Release FPS | 
| --- | --- | --- | --- | --- | --- | --- |
| Living dangerously and removing display DMA sync blockers                                                          | 28079ms | -0.3%   | 55fps | 10043ms | 0%    | 156fps|
| Hardcoding screen WIDTH                                                                                            | 28164ms | -1.7%   | 55fps | 10043ms | -3.3% | 156fps|
| Using more of the fixed-point calculations                                                                         | 28660ms | -37%    | 54fps | 10383ms | -7.7% | 151fps|
| Removing redudant multiplication (release version already did that optimalisation in the toolchain)                | 39222ms | -0.07%  | 40fps | 11186ms |  0%   | 140fps| 
| Few fixed-point calculations                                                                                       | 39248ms | -0.3%   | 40fps | 11186ms | -6%   | 140fps|
| Skybox clean up done through DMA (but doing the full screen)                                                       | 39371ms | -2.7%   | 39fps | 11902ms | -2.6% | 131fps|
| Particial skybox cleanup (not wasting cycles on parts which do not need to be cleaned)                             | 40471ms | -2.4%   | 38fps | 12230ms | -4.2% | 128fps|
| Better DMA and double buffering                                                                                    | 41447ms | -0.8%   | 37fps | 12762ms |       | 123fps|
| Badly done DMA transfers (the overhead is not making it as efficient as it could be)                               | 41797ms | -3.8%   | 37fps | | | |
| Instead of adding whole MAP_SIZE on each calculation pre-hardcode it before so less additions will have to be made | 43386ms | +0.004% | 36fps | | | |
| Aligning buffer does slow down, even when it shouldn't                                                             | 43384ms | +0.018% | 36fps | | | |
| Vertical increments suits the renderer more (see below)                                                            | 43376ms | -0.3%   | 36fps | | | |
| Horizontal increment screen                                                                                        | 43507ms |         | 36fps | | | |

## Vertical incrementing
  - It does align better to the render engine which often writes vertical lines
  - Instead of increment the WIDTH now it can increment just by +1
  - Calculation of the first ofset is simpler as well because 
    - now we do y + (x * HEIGHT), which can be simplied into a shift operation << HEIGHT_BITS because the HEIGHT is power of 2 (64), while WIDTH was not (96)
    - and because the x << HEIGHT_BITS is power of 2 then instead of addition just binary OR can be used (depending on the arch this might have the same performance)



# Possible improvements

 - Do fixed point aproximation for sin/cos
    - https://en.wikipedia.org/wiki/CORDIC
 	- http://www.coranac.com/2009/07/sines/
 	- https://www.nullhardware.com/blog/fixed-point-sine-and-cosine-for-embedded-systems/

 