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
| **GAMEOVER** | On a plain background: "GAME OVER", which paddle missed, both scores, and **RESTART** / **MENU** buttons | Click a button, or press **R** (restart) / **M** (menu) |

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

---

## How the code works

```
PingPong/
  PingPong.cpp    main(): window, game states, buttons, AI, collisions, scores, drawing
  ball.h/.cpp     class Ball: position, direction, bouncing
  paddle.h/.cpp   class Paddle: a 100×10 bar that moves left/right
  Font/Candal.ttf font for the scores and buttons
```

**`Ball`**: keeps a `position` and an x/y velocity worked out from a random starting `angle` (`cos`/`sin` × speed).
- `update()` moves it by its velocity every frame.
- `reboundSides()` flips the x velocity; `reboundPaddleOrTop()` flips the y velocity (and nudges the ball off the paddle).
- `hitBottom()` puts it back in the centre with a new random angle. The restart button uses this too.

**`Paddle`**: `moveLeft()` / `moveRight()` change its position by `paddleSpeed`; `update()` moves the shape there.
`getPosition()` returns its rectangle, used for collisions.

**`main()` (PingPong.cpp)**, every frame:
1. **Events**: closing the window, plus the button clicks and keys for the current **game state** (`MENU`, `PLAYING`, `GAMEOVER`).
2. **MENU / GAMEOVER**: draw that screen's text and buttons, then `continue` (skip the rest of the loop), so the game is paused.
3. **PLAYING**:
   - **Which ball to chase:** for each ball, the distance to each paddle. The top paddle's AI follows the ball nearest to it
     (`nearestBallIndex`), the bottom one's the ball nearest to the bottom (`farthestBallIndex`). With one ball both are ball 0.
   - **Paddles:** keyboard input moves them, or the AI moves a paddle toward its ball's x position (keys **1**/**2**/**0** switch the AI).
   - **Collisions:** the ball bounces off the sides, bounces off a paddle (+1 to that paddle's score), bounces off other balls,
     and getting past a paddle at the top or bottom calls `endGame()`, which switches to `GAMEOVER`.
   - **Draw** the scores, paddles and balls.

**`Button`** (in PingPong.cpp): a rectangle with centred text. `setup()` places it, `contains()` checks a click, and `draw()`
lightens it while the mouse is over it.

**Note:** the game has no frame-rate limit, so it runs as fast as your computer allows, and ball and paddle speed depend on the
machine. Adding `window.setFramerateLimit(60);` after creating the window (and raising the speeds to match) would make it
consistent.
