# Konrad's Notes

## Predator Confusion is sufficient to evolve swarming behaviour

### What problem or question is this paper trying to answer, and why does ot matter?

The paper tries to answer the question, how significant the selective force of predator confusion
for the formation of swarms is. Previous works have already provided insight into swarming behaviour
but most have not isolated the evolutionary pressures which favor the formation of swarms and
none have explored the coevolution of predator and prey behaviour. The paper also explores, how
the evolutionary effect of predator confusion on prey in turn exerts pressure on the predators visual
field, so that they in turn adapt to detect prey in swarms. Finally, the paper shows the changes of
the functional response curve of consumption rate when the swarm density increases. All in all the paper
tries to prove, that the concept of predator confusion has significant influence on the ecological
interaction between predators and prey. The paper is does matter, because the understanding how pervasive 
mechanisms such as predator confusion affect functional response relationships is important in order to 
model the dynamics of predator-prey interactions over ecological and evolutionary time.

### How does the simulation work (the world, the predator, the prey, what they can sense and do, evolution)?


#### The world
The world is a two-dimensional virtual environment, consisting of agents (predator and prey).
Agents decisions are based on a Markov Network (stochastic state machine, which makes control decisions
based sensors and internal states). Certain predator and prey behaviour is constrained (relative movement speed,
turning agility and for predators maximal consumption rate) modelling the differences between predators and prey
observed in nature. Size is 512 x 512 units.


#### The predator
Predator agents can detect prey but no other predators. Their visual field allows them to detect a distance
of 200 virtual meters in front of them. Predators have a rudimentary pre-evolved Markov Network, which 
allows them to track prey. 4 actions (stay still, move forward, turn right 6 degrees and move forward,
turn left 6 degrees and move forward). Automatically attacks prey within 5 meters, consuming it if 
the attack is successful. One possible attack within 10 simulation steps (handling time). Capture
success attack decreased by predator confusion, caused by more prey agents within 30 meters
the predators visual field.


#### The prey
Prey agents can both detect predators and prey. Their visual field allows them to detect a distance
of 100 virtual meters in front of them. Preys Markov Networks are randomly generated based on seeds.
4 actions (stay still, move forward, turn right 8 degrees and move forward,
turn left 8 degrees and move forward).


#### Both
Both predator and prey agents share the same Markov Networks, allowing them a visual field of 180 degrees.
They also have the same moving options (forward, left, right). Retina sensors are organized into 12 layers.
Only the presence of agents in a layer can be detected but not the number. However number of activated
layers can be count.


#### Evolution
Coevolution of Markov Networks of predators and prey with a genetic algorithm, selecting predators
which are good at hunting prey and prey which shows high survivability. Fitness is determined by
selecting pairs of predator and prey genomes, putting them into a simulation with 2000 steps, 50 prey agents
and 1 predator agent. In each step a fitness value for this step is calculated. At the end of a simulation
the sum of all single fitness values is calculated, determining the general fitness of predator and prey.
Asexually reproduction in a Moran process meaning that no recombination is applied.

### What are the main findings?

#### Finding 1: Without predator confusions prey agents disperse
In scenarios where no predator confusion occurs, the prey agents try to disperse as much as possible
evolving no swarm behaviour in a single replicate. The predator tries to hunt for the nearest prey agent
(Figure 3A). The survivability of prey increased over the generations.

#### Finding 2: With predator confusion prey evolve swarming behaviour
In 70% of the replicates, prey exhibited cohesive swarming behaviour. The predator evolved a mechanism
which allows him, to hunt for prey on the outer edges of the swarm (Figure 3B and 3C). He only uses
the two center retina slices in order to avoid predator confusion and constantly turns its direction until
prey enters one of his retinal slices. The survivability of prey significantly increases by evolving 
swarming behaviour as shown in Figure 4C.

#### Finding 3: Prey can evolve swarming behaviour without detecting the predator
The prey from generation 1200 ignored the presence of predators and made their movement decision based on
the position of their conspecifics. 

### What is one thing the paper does well, and one limitation or open question?

#### Advantage: Inverstigates both evolution of prey and predators
The paper investigates the coevolution of predator and prey, instead of focussing only on the prey.
It also explores the interactions between predators and prey.

#### Disadvantage: Only a single predator 
The paper only explores the swarming of prey. Certain predators like wolves hunt in packs in order to
increase their success chance to capture prey. 

### How is this paper similar to and different from our cats-and-mice simulation, and what is one idea from it, we could use in our work?
The simulation of the paper consists of both predators and prey, similar to our cats and mice scenario.
There are however many differences to our scenario:

- we allow multiple cats to exist in a simulation
- genomes can be inherited by two parents or one parent
- individual cats and mice can have different genomes in the same simulation
- there are a lot more sensors which also individual aspects into account (AGE, OSC1, GENETIC_SIM_FWD...)
- neurons are used as memory, storing information from sensors or other neurons
- movement works differently (combination of sensors/neurons decides the final movement into one of eight directions)
- there are a lot more actions which also affect individual aspects (SET_LONGPROBE_DISTANCE, SET_RESPONSIVENESS...)
- individuals can also affect their environment with pheromone (EMIT_SIGNAL0)
- genomes are coded as hexadecimal strings
- there are multiple evolution scenarios with different goals for individuals in order to survive
- we do not have a predator confusion in our scenario

One idea which we could take over from the paper is the handling time of predators. This would make it harder
for the cats to hunt for mice, but it could be a too big advantage for the mice. Another idea we could
take over is developing another visual sensor, which takes the density of individuals 180 degrees in front 
of an individual into account, although we already have a density sensor. 

