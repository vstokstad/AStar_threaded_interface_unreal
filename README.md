# A* pathfinding
## Threaded/async & sync interface to slap on basically anything.



Main algorithm is in Astar/FGAStar.cpp, the node i use is FGNode.cpp.
The FGPlayer is calling the methods via its AStar interface. the synchronos version returns an array of Nodes, while the async version returns a "smoothed" path in an array of vectors.
in the async version the player spawns an actor to move along the path. The FGPathfinder, which further smooths the path using arbitary velocity and draws its path along the way.
When clicking the points on the grid, the player calls both unthreaded and threaded methods, and prints timers when they are done, for profiling.
### Unreal Engine 4.26.2
