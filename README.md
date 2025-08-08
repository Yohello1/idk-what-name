# idk-what-name
noodles

## What libs do you need to compile?
- Ncurses
- (Might also need to be on linux)
- SDL
- Go look at `SDL/multi_array_test.cpp` and look at the includes, I am not listing them all out
- Sycl, CUDA, ????, icpx :3

Note: For SDL related projects, just change the file to compile in the Makefile
## What files do I need to compile?
- `path_finding` has a bunch of random algos I played around with, you'll need sfml, boost, and sycl, and icpx (or adaptive cpp!) to compile them. Just edit the make files, or run the rules, should generate a binary 
- `randomAlgos` is the exact same 
- `SDL` is just stuff that ALSO has SDL stuff, and some ncurses I think 
- `SDL/jeux-video` i where v3 of the game engine lied 

## What about the "game engine"
It's under `jeux-video`.
Make a `build` dir, enter it, and then run `cmake ..` under that Directory
Then run `make`, or wtv you used. 
