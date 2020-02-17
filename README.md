# Hive-TSP
<h2>INTRO:</h2>
Swarm intelligence has become a research interest to many research scientists of related fields
in recent years. Bonabeau has defined the swarm intelligence as “any attempt to design
algorithms or distributed problem-solving devices inspired by the collective behaviour of
social insect colonies and other animal societies” . Bonabeau et al. focused their viewpoint
on social insects alone such as termites, bees, wasps as well as other different ant species.
However, the term swarm is used in a general manner to refer to any restrained collection of
interacting agents or individuals. The classical example of a swarm is bees swarming around
their hive; nevertheless the metaphor can easily be extended to other systems with a similar
architecture. An ant colony can be thought of as a swarm whose individual agents are ants.
Similarly a flock of birds is a swarm of birds. An immune system  is a swarm of cells and
molecules as well as a crowd is a swarm of people . Particle Swarm Optimization (PSO)
Algorithm models the social behaviour of bird flocking or fish schooling . <br>
<h2>PROPOSED APPROACH</h2>
In this work, a particular intelligent behaviour of a honey bee swarm, foraging behaviour, is
considered and a new artificial bee colony (ABC) algorithm simulating this behaviour of real
honey bees is described for solving multidimensional and multimodal optimisation problems.
In the model, the colony of artificial bees consists of three groups of bees: employed bees,
onlookers and scouts. The first half of the colony consists of the employed artificial bees and
the second half includes the onlookers. For every food source, there is only one employed
bee. In other words, the number of employed bees is equal to the number of food sources
around the hive. The employed bee whose food source has been exhausted by the bees
becomes a scout. The main steps of the algorithm are given below:<br> 
-----------------------------------------------------------------------------------------------------------------------------------------
<h4>Algorithm:</h4>
<b><br>
Send the scouts onto the initial food sources<br>
REPEAT<br>
Send the employed bees onto the food sources and determine their nectar amounts<br>
Calculate the probability value of the sources with which they are preferred by the
onlooker bees<br>
Stop the exploitation process of the sources abandoned by the bees<br>
Send the scouts into the search area for discovering new food sources, randomly<br>
Memorize the best food source found so far<br>
UNTIL (requirements are met)</b><br>
-----------------------------------------------------------------------------------------------------------------------------------------
Each cycle of the search consists of three steps: moving the employed and onlooker bees onto
the food sources and calculating their nectar amounts; and determining the scout bees and
directing them onto possible food sources. A food source position represents a possible
solution to the problem to be optimized. The amount of nectar of a food source corresponds to
the quality of the solution represented by that food source. Onlookers are placed on the food
sources by using a probability based selection process. As the nectar amount of a food source
increases, the probability value with which the food source is preferred by onlookers
increases, too. Every bee colony has scouts that are the colony’s explorers . The explorers
do not have any guidance while looking for food. They are primarily concerned with finding
any kind of food source. As a result of such behaviour, the scouts are characterized by low 
search costs and a low average in food source quality. Occasionally, the scouts can
accidentally discover rich, entirely unknown food sources. In the case of artificial bees, the
artificial scouts could have the fast discovery of the group of feasible solutions as a task. In
this work, one of the employed bees is selected and classified as the scout bee. The selection
is controlled by a control parameter called "limit". If a solution representing a food source is
not improved by a predetermined number of trials, then that food source is abandoned by its
employed bee and the employed bee is converted to a scout. The number of trials for releasing
a food source is equal to the value of "limit" which is an important control parameter of ABC.
In a robust search process exploration and exploitation processes must be carried out together.
In the ABC algorithm, while onlookers and employed bees carry out the exploitation process
in the search space, the scouts control the exploration process.
In the case of real honey bees, the recruitment rate represents a “measure” of how quickly the
bee swarm locates and exploits the newly discovered food source. Artificial recruiting process
could similarly represent the “measurement” of the speed with which the feasible solutions or
the optimal solutions of the difficult optimization problems can be discovered. The survival
and progress of the real bee swarm depended upon the rapid discovery and efficient utilization
of the best food resources. Similarly the optimal solution of difficult engineering problems is
connected to the relatively fast discovery of “good solutions” especially for the problems that
need to be solved in real time. 
