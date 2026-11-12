# PingPong

A Ping Pong game created in C++ with SFML: two paddles (bottom and top) and one ball, with an optional AI for either paddle,
a start menu and a game-over screen.

Created December 2021.

---

## Running it

It builds two ways from the same code: **Visual Studio** (`PingPong.sln`, as before) or **CMake**, which works for the terminal and VS Code
on Linux/macOS. Visual Studio ignores `CMakeLists.txt`, so the two don't interfere.

### What you need (CMake)
A C++17 compiler, CMake ≥ 3.16 and SFML 2.5 or 2.6. On Ubuntu:
```bash
sudo apt install build-essential cmake libsfml-dev
```

### Quickest: `run.sh`
```bash
./run.sh
```
This builds (only what changed) and starts the game. Use `./run.sh --clean` to wipe `build/` and rebuild from scratch first.

### From the terminal, step by step
```bash
cmake -S . -B build
```
```bash
cmake --build build
```
```bash
cd build && ./PingPong
```
Run it from inside `build/`. The game loads `Font/Candal.ttf` from there, and CMake links `build/Font` to `PingPong/Font`.

### From VS Code
1. Open this folder in VS Code. Install the recommended **C/C++** and **CMake Tools** extensions if asked.
2. When CMake Tools asks for a *kit*, pick **GCC**.
3. Click **Build** in the bottom status bar, then **▷ Run** (or the 🐞 button to debug with breakpoints).

### From Visual Studio
Open `PingPong.sln` as before.

---

## Screens (game states)
The game is a small state machine (`enum GameState` in `PingPong/PingPong.cpp`):

```
MENU ──START──▶ PLAYING ──ball gets past a paddle──▶ GAMEOVER ──RESTART──▶ PLAYING
  ▲                                                     │
  └─────────────────────────MENU────────────────────────┘
```

| Screen | What you see | Input |
|---|---|---|
| **MENU** | A centered **START** button | Click it, or press **Enter** |
| **PLAYING** | The game | See controls below |
| **GAMEOVER** | "GAME OVER", which paddle missed, both scores, and **RESTART** / **MENU** buttons over the dimmed game | Click a button, or press **R** (restart) / **M** (menu) |

Restart and Menu both reset the paddles, the ball and the scores. Buttons light up when the mouse is over them.

## Controls (while playing)
| Input | Effect |
|---|---|
| **← / →** | Move the bottom paddle |
| **A / D** | Move the top paddle |
| **1** | Let the AI play the top paddle (on at the start) |
| **2** | Let the AI play the bottom paddle |
| **0** | Turn both AIs off |
| Close window | Quit |

The number of balls is `numBalls` in `PingPong/PingPong.cpp` (currently 1; it was 30).
