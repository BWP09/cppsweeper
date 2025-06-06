# Cppsweeper

A simple, keyboard-controlled minesweeper game!

## Build

This project uses the [Meson Build system](https://mesonbuild.com/)  along with [SFML](https://www.sfml-dev.org/) for graphics, both must be installed.
Run `meson compile -C build` to compile.

## Usage

Run `./minesweeper <width> <height> <number_of_mines>`.
Use arrow keys to move the cursor, press `z` to flag the tile, `x` to excavate the tile, and `c` to mark the tile as a guess. Press `space` when you have won/lost to start a new game.
You can also chord a correctly flagged tile by pressing `x` while it's selected.

## Images

### Win
Ran with `./minesweeper 50 50 500`

![win image](https://github.com/BWP09/cppsweeper/blob/aca739d86c9ab9cd43eb35ae4b486c2d34b3c0b1/images/win.png)

### Loss
Ran with `./minesweeper 50 50 500`

![loss image](https://github.com/BWP09/cppsweeper/blob/aca739d86c9ab9cd43eb35ae4b486c2d34b3c0b1/images/lose.png)