# Benchmarks

Doing whole run on the voxel map (1571 frames) each time and measuring how many miliseconds it took (and calculating the average FPS), Debug configurations are slower but allow better debuging while Release configuration is using -Ofast and is significantly faster.
What is bizzare that some improvements made it slower, yet when testing the most upto date Release candidate and undoing these improvements caused slow down by 7fps. So that looks like even when at that moment that change was not showing significant result when they combined together they were making measurable impact 

- Pre-rendering path (Debug 28050ms 55fps, Release 10002ms 156fps)

- Living dangerously and removing display sync blockers (Debug 0.3% faster 28079ms 55fps, Release 10043ms 156fps)

- Hardcoding screen WIDTH (Debug 1.7% faster 28164ms 55fps, Release 3.3% faster 10043ms 156fps)   

- Using more fixed-point calculations (Debug 37% faster 28660ms 54fps, Release 7.7% faster 10383ms 151fps)

- Removing redudant multiplication (Debug 0.07% faster 39222ms 40fps,  Release 0% faster 11186ms 140fps), release version already did that optimalisation in the toolchain

- Fixed-point calculations (Debug 0.3% faster 39248ms 40fps, Release 6% faster 11186ms 140fps)  

- DMA skybox clean up (Debug 2.7% faster 39371ms 39fps, Release 11902ms 131fps)

- Particial skybox cleanup (Debug 2.4% faster 40471ms 38fps, Release 12230ms 128fps). Compared to the 1st benchmark it's 7.5% 

- Better DMA and double buffering (Debug 0.8% faster 41447ms 37fps, Release 12762ms 123fps).  Compared to 1st benchmark it's 5% increase on Debug.

- Badly done DMA transfers (Debug 3.8% faster 41797ms, getting from average 36 FPS to 37)

- Instead of adding whole MAP_SIZE on each calculation pre-hardcode it before so less additions will have to be made
  - Slowed down by few ms again (Debug 43386ms) 

- Aligning buffer does slow down, even when it shouldn't (Debug 0.018% slower 43384ms) 

- Vertical increments suits the renderer more (Debug 0.3% faster 43376ms)
  - It does align better to the render engine which often writes vertical lines
  - Instead of increment the WIDTH now it can increment just by +1
  - Calculation of the first ofset is simpler as well because 
    - now we do y + (x * HEIGHT), which can be simplied into a shift operation << HEIGHT_BITS because the HEIGHT is power of 2 (64), while WIDTH was not (96)
    - and because the x << HEIGHT_BITS is power of 2 then instead of addition just binary OR can be used (depending on the arch this might have the same performance)
    
- Horizontal increment screen (Debug 43507ms 36fps)


# Possible improvements

 - Do fixed point aproximation for sin/cos
    - https://en.wikipedia.org/wiki/CORDIC
 	- http://www.coranac.com/2009/07/sines/
 	- https://www.nullhardware.com/blog/fixed-point-sine-and-cosine-for-embedded-systems/

 