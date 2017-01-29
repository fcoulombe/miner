This project compiles with Visual Studio 2015 Community Update 3. 
Some C++ features might not be available with the original VS2012. (I haven't tested)

The grid is made out of an array of array of blocks. There's an idea 
of position in "grid coordinate" which then gets mapped into "screen coordinate". 
This is an easy way to talk about blocks in 0..7x0..7 coordinates and 
not having to deal with floating points. We can also easily map
those coordinates to the array and easily look at adjacent neighbors.

There's a basic TaskManager which takes care of running interpolation 
tasks (fading, moving) over time by specifying a duration and
passing to the coroutine the percentage to completion.

Alpha support has been added to the engine to support fadein/fadeout.

Game constants are in Utils.h

This was done in roughly 2 days.
Due to life factor, I don't have more time to work on this.

