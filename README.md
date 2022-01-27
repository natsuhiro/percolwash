# percolwash - ncurses-based shoot-and-drop game


     ┌------stage:1-------┐
     |##%#$#$%%&$%%#%%%%#%|
     |#%%$&%%%&%#%X%%$%#X#|
     |$#####%X#&$&X%&$%X#X|
     |&$X&#$%&&&&&$X$%XX%X|
     |XXX%%$$$X#&%XX$&&&X$|
     |#XX$&&X%X&XX&%#%&X#X|
     |&X&X%XX#X##&$&%%$&%$|
     |XX#$##X$&#%XX&X&#$$$|
     |&#$X##&#$%X$#$##XX$X|
     |         |          |
     n         |          |
     e         |          |
     x         |          |
     t         |          |
     $         #          |
     └score:0-----limit: 5┘

- Matches of three or more identical symbols (two at top line) vertically, horizontally and diagonally disappear.
- The symbol with a blank top also disappears.

##  Keys
  Move left:     KEY_LEFT, 'h'
  Move right:    KEY_RIGHT, 'l'
  Shoot:         KEY_UP, SPACE, 'k'
  Exchange next: KEY_DOWN, 'j'
  New game:      'n'
  Quit:          'q'

## Install
     > git clone https://github.com/natsuhiro/percolwash.git
     > cd percolwash
     > autoreconf -fi
     > ./configure
     > make
     > sudo make install

