Quote from the Docs (read them for more info):

<blockquote>Simulated Annealing Framework: A class to help users implement simulated annealing in an easy and flexible way.<br>
<br>
Simulated Annealing Class<br>
<br>
This class offers the user a flexible framework to use for the implementation of applications of Simulated Annealing. To use it the user needs to create a child class that implements the 3 required functions listed below.<br>
<br>
Require overriding:<br>
<br>
<ul><li>giveRandomNeighbour()<br>
</li><li>calcDistanceToTarget()</li></ul>

Optional overriding:<br>
<br>
<ul><li>shouldStopHook()<br>
</li><li>printStatus()<br>
</li><li>calcProbability()<br>
</li><li>calcNewTemp()</li></ul>

The type of the candidate solutions can be set using template parameters. The type of the target value we want to achieve can also be set using template parameters. The link between these two types is made by the calcDistanceToTarget function which is one of the functions that require overriding.<br>
</blockquote>

You'll also find a couple of examples of applications of the algorythm in the code.

The simplest being SimulatedAnnealingSin which searches for the angle with sin(angle)==1.

The second one is SimulatedAnnealingNQueens which searches a solution for the NQueens problem for large N's.

The third one tries to find the point in a PRQuadtree that is furthest from all other points.