# percolwash - ncurses-based shoot-and-drop game

<img width="156" alt="pw_ss" src="https://user-images.githubusercontent.com/81680557/151484808-a34a7201-7f92-43e2-bbbb-928b67c6d244.png">

- Matches of three or more identical symbols (two at top line) vertically, horizontally and diagonally disappear.
- The symbol with a blank top also disappears.
- `X` is an obstacle.

##  Keys
Action|Key
--------------|-------------------
Move left|     `KEY_LEFT` `h`
Move right|    `KEY_RIGHT` `l`
Shoot|         `KEY_UP` `SPACE` `k`
Exchange next| `KEY_DOWN` `j`
New game|      `n`
Quit|          `q`

## Install
     > git clone https://github.com/natsuhiro/percolwash.git
     > cd percolwash
     > autoreconf -fi
     > ./configure
     > make
     > sudo make install
