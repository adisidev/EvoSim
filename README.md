# EvoSim
 An evolution simulator for a foraging genome based on Yale-NUS' SI1 foraging exercise.

## Compiling and running (macOS)

#### Compile by runnnig:

1. `g++ main.cpp -o evosim`

#### To use the evolution simulator:

1. `./evosim`

You must paste an initial genome that you want the simulator to evolve. You may make changes in the evolution variables by following the instructions below.

## Changing Evolution Variables

#### You may change the following variables that are defined at the start of `main.cpp`:

- `height`: height of foraging board (default: 10)
- `width`: width of foraging board (default: 10)
- `n_moves`: number of moves per game (default: 200)
- `chance`: chance of mutating per character of genome (in percentage) (default: 4)
- `f_chance` 50 chance of square being food (in percentage) (default: 50)
- `n_sim`: number of simulations per genetic code (default: 100)
- `n_gen`: number of genetic codes per generation (default: 500)
- `speed`: speed of real time simulation in milliseconds (default: 200)

#### Further, you may change the `main` function with the help of the following guide:

##### `curr_gen.adv_gens(50, false)`
- change `50` to number of generations you would like to advance per iteration (before showing best version of that generation)
- change `false` to `true` to see the best version of generation simulated in real time (note: I would recommend you reduce `speed` in the above variables if you do this)

##### `curr_gen.best(true);`
- change `true` to `false` to not simulate the best version after every `50` generations in real time (this is good if you would like to keep the simulator running for a long time)

## Suggested starting genomes

- Useless Genome (this will eventually evolve into Memory Genome)
`000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000`

- Your own genome

## End genomes evolved up till now

- Memory Genome (this yields an average score of ~485, it evolved after ~2000 generations from the useless genome)
`250350050250350250150350120150130250150330250150150310350350050350220000350350000050030050000230050050030230050100050000200050050150320000000000000320050000300110150250250150350250150330020150250120250230150110110330330030210020210320240250210`

- Better Than Memory Genome (this will not evolve into Memory Genome as it yields a higher average score of around 490)
`254354154054354254054354244156356156156354154154054344054354254254254254054054144054354254054354154054354044154354154154254154154354344054354054054254254354354144354354154254354254354354244154354154254354154154154444344244244044244344144344444`

*This was achieved when only rules which required `here` to be `empty` were evolved. That is, rules that had `here` as `food` were mapped to the `eat` action (action `5`)*
