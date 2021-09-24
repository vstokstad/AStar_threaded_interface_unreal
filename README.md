# AI A* assignment
Unreal Engine 4.26.2


Main algorithm is in Astar/FGAStar.cpp, the node i use is FGNode.cpp.
The FGPlayer is calling the methods via its AStar interface. the synchronos version returns an array of Nodes, while the async version returns a "smoothed" path in an array of vectors.
in the async version the player spawns an actor to move along the path. The FGPathfinder, which further smooths the path using arbitary velocity and draws its path along the way.

I have merged my unthreaded branch with the threaded version, so there is only the main branch to consider.
when calling clicking the points on the grid, the player calls both methods now, and prints timers when they are done, for profiling.
