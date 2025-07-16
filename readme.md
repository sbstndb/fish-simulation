# Fish Simulation

A simple fish school simulation using C++ with SDL2 and ImGui. This project demonstrates the boids algorithm for emergent behavior.

## Demo

![Fish Simulation Demo](output.webm)

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

This project is built and tested on Ubuntu. You will need the following packages installed:

- `build-essential`
- `cmake`
- `libsdl2-dev`
- `libsdl2-image-dev`

You can install them using `apt`:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libsdl2-dev libsdl2-image-dev
```

### Compilation

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/sbstndbs/fish-simulation.git
    cd fish-simulation
    ```

2.  **Initialize submodules:**

    ```bash
    git submodule update --init --recursive
    ```

3.  **Create a build directory:**

    ```bash
    mkdir build
    cd build
    ```

4.  **Run CMake and build the project:**

    ```bash
    cmake ..
    make
    ```

5.  **Run the simulation:**

    ```bash
    ./sdl-imgui
    ```

## TODO

- [ ] Add cohesion behavior to the boids algorithm.
- [ ] Implement a quadtree to optimize neighbor searching.
- [ ] Add more fish species with different behaviors.
- [x] Create a basic simulation with SDL2.
- [x] Integrate ImGui for real-time parameter tuning.

## Important Note

The fish texture path is set to `../img/fleche.png` in `src/test_imgui.cpp`. Make sure to run the executable from the `build` directory so it can find the image files correctly.


