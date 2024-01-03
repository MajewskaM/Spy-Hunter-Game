# Spy-Hunter-Game
🌱 University project implemented using C/C++/SDL. Creation of desktop application of an Atari game called Spy Hunter.
The object of the game is to drive down roads and destroy other (enemy) vehicles, using the weapons.
</br> </br>
The assignment is accompanied by a template in which the following functionality is implemented:</br>
• calculating the increment of time, which allows to track the flow;</br>
• displaying the graphics in BMP format;</br>
• drawing a pixel, line, rectangle;</br>
• text display.</br>

<img width="478" alt="gameplay" src="https://github.com/MajewskaM/Spy-Hunter-Game/assets/115587236/662096ef-068e-4497-b27d-f911fa01a008">

## Remarks
**The use of the STL libary is prohibited.**

</br> --------------------------------------------------------------------------------------------------------------------- </br>

## Program controls
🌱 The key controls are utilized in a specified way:</br>
• _**arrows:**_ moving the player in given direction;</br>
• _**esc:**_ quit the program;</br>
• _**n:**_ start a new game;</br>
• _**s:**_ save the game state;</br>
• _**l:**_ load the game state;</br>
• _**p:**_ pause/continue;</br>
• _**space:**_ shooting;</br>
• _**f:**_ finish the game.</br>

<img width="481" alt="top1" src="https://github.com/MajewskaM/Spy-Hunter-Game/assets/115587236/923444df-3807-4fcb-8b9b-a5ae7c5822e9">

</br> --------------------------------------------------------------------------------------------------------------------- </br>

## Implemented functionalities
**(a)** The game board has to be presented in a aestethic and ergonomic way.</br>

**(b)** Displaying the elapsed time and score during gameplay. Both values are reset when the new game is
started.</br>

**(c)** The basic functionality of the game: the movement, the road form (with collisions). The movement is
immediate – responding to the events.</br>

**(d)** Supporting esc and n controls.</br>

**(e)** The game should also keep the score according to the mechanics of the game (points are added faster if the player speeds up, and slower if the player slows down).</br>

**(f)** The right bottom corner of the game screen should contain the letters of implemented elements.</br>

**(g)** Saving and restoring game state. Save and load game from file (keys s and l). Saved game
is identified by the time of the save (date, hour, minute and second). When loading, the list of saved
games is displayed and the user choosed the position from the list in a way chosen by the author.</br>

**(h)** The width of the road changing. The road should change its width, according to the gameplay.</br>

**(i)** Pausing the game. The player should be able to pause the game and then be able to return back
to the play.</br>

**(j)** Enemies and other cars. The gameplay should include enemies that the player can chase, that
can attack and overrun the player, but also regular, non-enemy cars that when destroyed halt the score
counter for a while.</br>

**(k)** Shooting. The player can shoot both enemy and non-enemy cars, this might impact the score.</br>

**(m)** Getting some cars. For a short period of time in the beginning the player should have the unlimited
amount of cars (even if their car is destoyed, the one appears immediately).</br>

**(o)** Keeping the score. Store and display the best results’ list. Single result consist of the number of
points and the time of play. The player has ability to append to the list when exiting the game. The list
is limited only by the memory size (dynamic memory allocation). Best results’ list should be persistent
(i.e. stored in a file and available between the launches). The menu allows you to display the list sorted
by points (option p) and by time (option t).
