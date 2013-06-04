JediChess
CS 174A Term Group Project
=========
Team 3: 
------
Yuta Kai
Evan Shi
Daniel Ichwan


=======================================
Project Introduction
=======================================
Our group implemented the game of chess with a Star Wars battle look and feel. The game is played by two human users using the standard rules of chess with the added element of animated battles when a piece is captured. The program displays a bird’s eye view of an 8x8 chess grid with a realistic battlefield overlay. Each player’s turn will consist of the player selecting a piece with the mouse; the program will display the available moves for that piece by highlighting the valid squares on the board; the player would then move the piece as desired by clicking on a highlighted square. Should the player make a move that will capture the opponent’s piece the program will show the two pieces engaging in animated combat. At the end of the player’s turn, the board will rotate for the opponent’s turn.  The game will continue until checkmate is reached.

User Interaction
---------------
To play the game, merely assign one player per team (white or black).  The white team will start the game.

In each player's turn, the player can select either one of their pieces with the mouse, which will cause the board to highlight all the valid squares the piece can move towards.  If the player would like to change their selection, merely click the other piece in the same manner.  To actually make a move after selecting the piece, click on one of the highlighted squares.  This will cause the piece to transition to that location.  If an unhighlighted square is selected, no action will occur and the player must make a valid move.  If the move performed by that piece involves the capture of an enemy piece, an animation will occur.  This animation involve the attacking piece either shooting at the victim piece then walking to occupy the destination square or walk towards the destination square and strike the victim piece with a lightsaber.  The victim piece will disappear at the appropriate time.  After a player's turn has ended from a valid action (either a move or an attack), the board will rotate 180 degrees and the other player's turn will ensue.

Once a player's king piece is placed in checkmate, the game ends with a display of the victorious team's flag.  


Keyboard buttons
--------
Q, q, Esc = Quit the application
I, i = Camera zoom in
O, o = Camera zoom out
W, w = Camera move vertically up in y-direction
A, a = Camera move left in x-direction
S, s = Camera move vertially down in y-direction
D, d = Camera move right in x-direction
U, u = unhighlight all squares on chess board
Space key = reset camera to original position



==================================
Project Result 
==================================
Advanced Topic
-----------
- Color buffer based selection


Overall, the team was very happy with the results of the project's implementation.  The game of chess could be played seamlessly with fun and moderately realistic chess pieces.  The game successfully implemented color buffer selection for the pieces/chess squares, animation in the piece capturing, texture mapping, and successful chess rule implementation.

In a high-level view, the game mainly involves an interaction of a chess Board class, Piece classes, and a GameManager class.  The Board was composed of many Squares that kept track of the pieces occupying them and the location of themselves.  The GameManager kept track of the valid moves the Pieces could make as well as whether the King was in check or in checkmate.  The Piece class maintained their own state and performed the animations required during a capture.  Supplementary files were used to generate the geometry and apply the corresponding textures to each piece.  The main body comprised of the traditional GLUT loop that responded to mouse clicks and keyboard input.  This loop began by intializing a base scene, populating the board with the appropriate pieces and positioning the light and camera appropriately.  The loop continually responded to player selection, rendered the pieces and allowed for the indirect control of the animation sequences as appropriate.

The game could possibly be improved with more advanced geometries for the pieces, shadows or more elaborate animation, but overall, the game met and pleased the expectations set forth by the team.  The game is completely functional, manifests a lively battle between good and evil and embodies the culmination of a quarter of computer graphics knowledge.  Overall, the game presented a venue to exercise graphical techniques and creativity learned in this class in an integrated and purposeful setting.
