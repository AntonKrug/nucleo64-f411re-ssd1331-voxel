# Benchmarks

- DMA skybox clean up speedup (Debug 2.7% 39371ms - 39449ms 39fps, Release 11902ms - 11025ms 131fps)

- Particial skybox cleanup speedup (Debug 2.4% 40471ms 38fps, Release 12230ms - 12247ms 128fps). Compared to the 1st benchmark it's 7.5% 

- Better DMA and double buffering speed up (Debug 0.8% speedup 41447ms - 41524ms 37fps, Release 12762ms - 12785ms 123fps).  Compared to 1st benchmark it's 5% increase on Debug.

- Badly done DMA transfers speedup by 3.8% (Debug 41797ms - 41869ms, getting from average 36 FPS to 37)

- Instead of adding whole MAP_SIZE on each calculation pre-hardcode it before so less additions will have to be made
  - Slowed down by few ms again (Debug 43386 - 43457ms) 

- Aligning buffer does slow down, even when it shouldn't by 0.018% (Debug 43384ms - 43456ms) 

- Vertical increments does 0.3% improvement (Debug 43376ms - 43447ms)
  - It does align better to the render engine which often writes vertical lines
  - Instead of increment the WIDTH now it can increment just by +1
  - Calculation of the first ofset is simpler as well because 
    - now we do y + (x * HEIGHT), which can be simplied into a shift operation << HEIGHT_BITS because the HEIGHT is power of 2 (64), while WIDTH was not (96)
    - and because the x << HEIGHT_BITS is power of 2 then instead of addition just binary OR can be used (depending on the arch this might have the same performance)
    
- Horizontal increment screen (Debug 43507ms - 43580ms 36fps)
 