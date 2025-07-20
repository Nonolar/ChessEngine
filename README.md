# Chess engine
This is a CLI tool for a basic chess engine.

## Usage
To play a move you input the algebraic notation for that move. eg. You want to move a bishop to a3 you would input ```Ba3```.
Note that you should not input any indication if a move is a check, a checkmate or a capture, so instead of Bxa3 it would just be ```Ba3```.
Sometimes the program does not find the piece you try to move (Most often if you play a rook), in this case input both the starting and end square for the move. Eg. ```Ra1a3```.

Some special inputs is ```Y``` for activating the bot to play, ```R``` for reverting a move and ```B``` for reverting a move and deactivating the other players bot.

