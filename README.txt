ENCE260 ASSIGNMENT | TEAM 218

-------BATTLESHIPS-------

A Two-player game using the UCFK4 (UC Fun Kit) LED display, navswitch, and IR 
communications. Sink your opponents ships before they sink yours.




------HOW TO COMPILE------

-Open terminal and navigate to folder that you wish to install game into
-Run command: git clone https://eng-git.canterbury.ac.nz/ence260-2017/team218.git
-Plug UCFK4 into USB slot
-Run command: make program
-Game will start on UCFK4




------PLAYING THE GAME------

---PLACEMENT---

Before play starts, you must place your ships on the board by using the
navswitch. Change position of ships by moving the navswitch, rotate the ships
by pressing the navswitch down, and confirm placement of ship by using the 
button located on the other side of the LED array. 

Once both you and your opponent have placed all four ships (of sizes 2, 3, 4
and 5) one player will automatically be chosen to start the game and will fire 
first.                         


---YOUR TURN---

When it is your turn to fire, the other person's board will appear on your 
LED array, showing positions you have previously fired on. Solid lights
indicate a previous miss, and flashing lights indicate previous hits. Your
aiming reticle is the fast-flashing light. Move the reticle with the navswitch
controls, and push the navswitch to fire on that position - a 'HIT' or 'MISS'
message will show the outcome of the hit, and then the other player will take
their turn.

---OPPONENTS TURN---

When it is not your turn, your screen will clear (to avoid the other player
seeing your ship positions) and the UCFK will wait for the signal to change 
turns back to yours.


---WINNING AND LOSING---

When a player manages to hit all 14 ship positions and sink all four ships, a
signal will be broadcast to the losing player, and the UCFK's will display either
a 'YOU WIN' or 'YOU LOSE' message.



---ACKNOWLEDGEMENTS---

The implementation of this game relies on multiple drivers, fonts and 
utility programs written by M. Hayes.


