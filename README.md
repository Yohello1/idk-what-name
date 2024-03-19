# idk-what-name
noodles

## What libs do you need to compile?
- Ncurses
- (Might also need to be on linux)
- SDL
- Go look at `SDL/multi_array_test.cpp` and look at the includes, I am not listing them all out

Note: For SDL related projects, just change the file to compile in the Makefile
## What files do I need to compile?
- For cellular Automata: `cellular-automata/moores_hood.c`
- For Drunkard Walking: `drunk-walker/drunk.c` (walker.c may also be valid but it is not gaurenteed)
- For Diffusion-Limited-Affregration `Diffusion-limited/diffuse-limit-aggregation.c` 
- For Voroni in terminal go to the voroni and compile the only file
- For Voroni in SDL compile `SDL/voronoi.cpp`
- For my bad attempts at water checkout `SDL/water.cpp, sandy_water.cpp/sandy_water2.cpp`
- I have no idea what I put in `SDL/test.cpp`
- Sand in either `SDL/sand.cpp`, or `SDL/sandy.cpp` I feel like it's the second one
- for random points `SDL/rand.cpp`
- Fixed position `SDL/not_fixed.cpp` ~~Ironic I know~~, `SDL/fixed_pos` is broken
- Spliting up simulation and rendering `SDL/mutli_array_test.cpp` 
- For fire and random positions `SDL/im_on_fire.cpp` with fire direction
- Broken attempt at `cellular-automata.cpp`

## What about the "game engine"
Go to `SDL/jeux-video/` and run the make file
