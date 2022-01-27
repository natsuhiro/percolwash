# percolwash - ncurses-based shoot-and-drop game


     ┌------stage:1-------┐
     |&$&&%&&%$#&#$&$#$&$%|
     |%&%#XX%%$#$X$$%&#%$#|
     |#&&&&&%|%X$%%$#&$##X|
     |$$%##&&| X$&#X$X###X|
     |%$XX$#&|  $&XXX%X&#X|
     |$X%#$&&|  X%X#$X%X&$|
     |&&&&%&X|  X XX$%$X%%|
     | %##$$&|    &#&X&X%$|
     | # #%%%|    XX$#$%X#|
     |       |            |
     n       |            |
     e       |            |
     x       |            |
     t       |            |
     %       %            |
     └score:121---limit: 4┘


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

