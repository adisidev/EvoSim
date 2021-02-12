#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>

// MAKE CHANGES HERE
#define height 10 // height of foraging board
#define width 10 // width of foraging board
#define n_moves 200 // number of moves per game
#define n_code 243 // size of code (fixed)
#define n_cross 5 // size of visibile cross + (fixed)
#define chance 4 // chance of mutation (in percentage)
#define f_chance 50 // chance of square being food (in percentage)
#define n_sim 100 // number of simulations per genetic code
#define n_gen 500 // number of genetic codes per generation
#define speed 200 // speed of real time simulation in miliseconds

// struct to store one instance of board for (for one game)
struct board {

  // variables
  bool grid[height][width]; // store whether food or not
  int pos[2]; // store position of player
  int score; // store score (eat: +10, move: 0, dirt: -1, wall: -5)
  int moves; // store moves remaining

  // functions
  void init_grid(); // initialise the grid based on random chance
  void move(char action); // move pos once
  std::vector<int> cross(); // returns what is around (visibile cross)
  void print();

  // constructor
  board() {
    init_grid();
  }

};

// struct to store genetic code and average score of n_sim simlulations
struct genetic_code {

  // variables
  char code[n_code]; // store genetic code
  double avg_score; // store average score based on calc_score

  // functinos
  genetic_code mutate(); // mutates the useful genetic code
  char action(std::vector<int>);
  int simulate(); // simulate upto n_moves
  void simulate_rt(); // simulates in real time
  void calc_score(); // runs n_sim simulations and calculates average score

  // constructor
  genetic_code(char g_code[n_code]) {
    for (size_t i = 0; i < n_code; i++) {
      code[i] = g_code[i];
    }

  }

  // overloading "<" operator for this data type to compare genetic codes
  bool operator < (const genetic_code &rhs) const {

    // whichver has lower avg_score will be smaller
    return avg_score > rhs.avg_score;
  }

};

// struct to store current generation with n_gen genetic codes
struct generation {

  // variables
  int gen_count; // What generation we are on
  std::vector<genetic_code> gen; // store all genetic codes for generation

  // TODO: Show percentile values

  double worst_score;
  // double percentile_25; // 25th percentile
  double median_score; // 50th percentile
  // double percentile_75; // 75th percentile
  double best_score;

  // functions
  void next_gen(); // advances by one generation
  void best(bool show_best); // calculates best genetic code of that generation
                             // set show_best to simulate best in real time
  void adv_gens(int n, bool show_best); // advance n generations

  // constructor
  generation(genetic_code initial) {

    gen_count = 0;

    // reserving memory for vector
    gen.reserve(n_gen);
    gen[0] = initial;

    // mutating n_gen times for remainder of generation
    for (size_t i = 1; i < n_gen; i++) {
      gen[i] = initial.mutate();
    }

  }

};

// MAKE CHANGES HERE
int main() {

  srand((unsigned int)time(NULL)); // Seeding random number generator

  // getting starting genetic code
  char g_code_string[243];
  std::cin >> g_code_string;

  genetic_code initial(g_code_string); // creating 0th generation

  initial.simulate_rt(); // showing simulation of inital code

  generation curr_gen(initial);
  curr_gen.best(false); // find best version but not simulating in real time

  // Keeps on advancing 50 generations and showing progress until exited
  while (true) {
    curr_gen.adv_gens(50, false);
              // change number (50) to advance lesser generations per iteration
              // change to true to simulate best of every gen in real time

    curr_gen.best(true); // simulating best version in real time
                  // change to false to not simulate best version in real time
  }

  return 0;
}

// initialises grid
void board::init_grid() {

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {

      if ((rand() % 100) < f_chance) {
        grid[i][j] = true;
      } else {
        grid[i][j] = false;
      }

    }
  }

  pos[0] = rand() % height;
  pos[1] = rand() % width;

  moves = n_moves;
  score = 0;

}

// moves pos/player
void board::move(char action) {

  switch (action) {

    case '0': // move north
      moves--;
      if (pos[0] + 1 < height) {

        // can move
        pos[0]++;

      } else {

        // bumped into wall
        score -= 5;
      }
    break;

    case '1': // move south
      moves--;
      if (pos[0] - 1 > -1) {

        // can move
        pos[0]--;

      } else {

        // bumped into wall
        score -= 5;
      }
    break;

    case '2': // move east
      moves--;
      if (pos[1] + 1 < width) {

        // can move
        pos[1]++;

      } else {

        // bumped into wall
        score -= 5;
      }
    break;

    case '3': // move west
      moves--;
      if (pos[1] - 1 > -1) {

        // can move
        pos[1]--;

      } else {

        // bumped into wall
        score -= 5;
      }
    break;

    case '4': // stay put
      moves--;
    break;

    case '5': // eat
      moves--;
      if (grid[pos[0]][pos[1]]) {

        // ate food
        score += 10;
        grid[ pos[0] ] [ pos[1] ] = false;

      } else {

        // ate dirt
        score -= 1;
      }
    break;

    case '6': // random move

      // making a random move
      move('0' + rand() % 5);

    break;


  }
}

// returns what is on cross (north, south, east, west, here) near pos
std::vector<int> board::cross() {

  std::vector<int> vis_cross(n_cross);

  // north
  if (pos[0] + 1 < height) {
    if (!grid[pos[0] + 1][pos[1]]) {

      // empty
      vis_cross[0] = 0;
    } else {

      // food
      vis_cross[0] = 1;
    }
  } else {

    // wall
    vis_cross[0] = 2;
  }

  // south
  if (pos[0] - 1 > -1) {
    if (!grid[pos[0] - 1][pos[1]]) {

      // empty
      vis_cross[1] = 0;
    } else {

      // food
      vis_cross[1] = 1;
    }
  } else {

    // wall
    vis_cross[1] = 2;
  }

  // east
  if (pos[1] + 1 < width) {
    if (!grid[pos[0]][pos[1] + 1]) {

      // empty
      vis_cross[2] = 0;
    } else {

      // food
      vis_cross[2] = 1;
    }
  } else {

    // wall
    vis_cross[2] = 2;
  }

  // west
  if (pos[1] - 1 > -1) {
    if (!grid[pos[0]][pos[1] - 1]) {

      // empty
      vis_cross[3] = 0;
    } else {

      // food
      vis_cross[3] = 1;
    }
  } else {

    // wall
    vis_cross[3] = 2;
  }

  // here
  if (pos[0] < height && pos[0] > -1 && pos[1] < width && pos[1] > -1) {
    if (!grid[pos[0]][pos[1]]) {

      // empty
      vis_cross[4] = 0;
    } else {

      // food
      vis_cross[4] = 1;
    }
  } else {

    // wall
    vis_cross[4] = 2;
  }

  return vis_cross;

}

// Printing current state of board
void board::print() {

  std::cout << "moves: " << moves << '\n';
  std::cout << "score: " << score << "\n\n";

  for (int i = height - 1; i >= 0; i--) {
    for (int j = 0; j < width; j++) {

      if (pos[0] == i && pos[1] == j) {
        std::cout << "| ";
      } else if (grid[i][j]) {
        std::cout << "F ";
      } else {
        std::cout << "+ ";
      }
    }

    std::cout << "\n\n";
  }
}

// Code to mutate a genetic code based on chance
genetic_code genetic_code::mutate() {

  // Copying initial genetic code to be mutating
  genetic_code mutated(code);

  // Mutating only first two columns (relevant part)
  for (size_t i = 0; i < n_code; i++) {

    // Mutating based on chance
    if (i % 3 == 1) {
      if ((rand() % 100) < chance) {
        mutated.code[i] = '0' + (rand() % 6);
      }
      i++;

    } else if ((rand() % 100) < chance) {
      mutated.code[i] = '0' + (rand() % 4);
    }



  }

  return mutated;

}

// Calculating action based on genetic code (base 3 to base 10)
char genetic_code::action(std::vector<int> vis_cross) {

  int rule = vis_cross[4] +
              vis_cross[3] * 3 +
              vis_cross[2] * 9 +
              vis_cross[1] * 27 +
              vis_cross[0] * 81;

  // std::cout << "rule: " << rule + 1 << '\n';

  return code[rule];

}

// Simulating one instance in background
int genetic_code::simulate() {

  // Initialising a temporary board
  board temp;

  // Exhausting all moves
  while (temp.moves > 0) {

    // move based on action based on cross
    temp.move(action(temp.cross()));
  }

  // Returning score
  return temp.score;

}

// Simulating one instance in rael time
void genetic_code::simulate_rt() {

  // Initialising a temporary board
  board temp;

  // Exhausting all moves
  while (temp.moves > 0) {

    system("clear");

    temp.print();

    // move based on action based on cross
    temp.move(action(temp.cross()));
    // std::cout << action(temp.cross()) << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(speed));

  }

  system("clear");
  temp.print();

}

// Calculating average score of n_sim simulations
void genetic_code::calc_score() {

  avg_score = 0;

  for (size_t i = 0; i < n_sim; i++) {
    avg_score += simulate();
  }

  avg_score /= n_sim;

}

void generation::next_gen() {

  std::cout << "previous_gen: " << gen_count << '\n';
  gen_count++;
  std::cout << "new gen: " << gen_count << '\n';

  // temporary vector to store new generation
  std::vector<genetic_code> temp;

  // killing by chance
  for (size_t i = 0; i < n_gen; i++) {
    if (i < ((rand() % n_gen) + 1)) {
      temp.push_back(gen[i]);
    }
  }

  // adding mutants
  int g_code_count = 0;
  while (temp.size() < n_gen) {
    temp.push_back(temp[g_code_count].mutate());
    g_code_count++;
  }

  // replacing old generation with new generation
  gen.clear();
  gen = temp;

  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}

void generation::best(bool show_best = false) {

  std::cout << "current generation: " << gen_count << '\n';

  for (size_t i = 0; i < n_gen; i++) {
    gen[i].calc_score();
  }

  // sorts in descending order as constructor overloaded the wrong way
  std::sort(gen.begin(), gen.end());

  worst_score = gen[n_gen - 1].avg_score;
  median_score = gen[(int) (n_gen/2) + 1].avg_score;
  best_score = gen[0].avg_score;
  std::cout << "worst score: " << worst_score << '\n';
  std::cout << "median score: " << median_score << '\n';
  std::cout << "best score: " << best_score << '\n';

  if (show_best) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    gen[0].simulate_rt();
  }

  std::cout << "code (for best score): " << '\n';
  std::cout << gen[0].code << "\n\n";

  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}

void generation::adv_gens(int n, bool show_best) {

  best(show_best);

  for (size_t i = 0; i < n; i++) {
    next_gen();
    best(show_best);
  }

}
