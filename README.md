# ShortestPathVisualizer
A visualizer for the steps that shortest path algorithms do. For now it works with Dijkstra and A* algorithms.
If it finds a shortest path from start to destination it will show it in red.
See example demos in the [Demos](ShortestPathVisualizer/Demos) folder.

## Buttons desription
* White button is for clearing tiles.
* Green Button is for setting the start position.
* Red Button is for setting the destination.
* Gray Button is for setting a blocking tile.

* D Button sets Dijkstra algorithm to be used
* A* Button sets A* algorithm to be used.

* Play Button starts the algorithm.

## How to use
* Select a start position, select a destination, add blocks (optional) and clear tiles (optional) in any order.
* Select algorithm to use (Dijkstra or A*).
* Press Play Button.

You can clear the whole grid with right click.

## Notes
* You need to add one and only one start position (the app will prevent you from adding more).
* You need to add one and only one destination (the app will prevent you from adding more).
* If you want to change the start position or destination you must clear the tile (select white button on top left corner) first.
* Once you press the play button you must wait for the algorithm to end (there is no stop button yet).
* Clearing the grid with right click not only clears the tiles drawn when showing the algorithm but it clears all of them (even start position and destination).
