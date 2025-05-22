# Hand-Cricket-Implementation-using-C
Code that represents a playable game of hand cricket, implemented using C language.

A lot of people in the world are blisfully unware of the game of hand cricket, and it is a slowly dying out. The goal of this project is to save this gamne that I hold with a lot of love in my heart from extinction. 

We have used several methods of C language, such as header files, both built-in and user defined function modules and file handling I/O.

The format of play is multiplayer (1 v 1).

It is based off of overs that represent a set of 6 balls in a game of traditional cricket, which this code and game aim to represent.

#GAME LOGIC

The first player is able to choose between batting and bowling by a selecting a input, 0 or 1. The computer also randomly selects either 0 or 1. If the numbers match, the player wins the toss, and if the numbers don't match, the second playuer wins the toss.

The winner of the toss decides whether to bat or to bowl. The following rules are dictated as follows:
1) Both players select a number from 1-6.
2) If the numbers are different, then the number selected by the batter is added to the team score.
3) If the numbers are the same, then the batter is out.
4) There are 2 batters and bowlers for each team, however an option can be availed for using a singular batter and bowler.
5) After the batter is out, the bowling team now bats, with the overall total being to beat the score set by the batting team.
6) If the chasing team is able to cross the target set by the batting team they win, and if they aren't the batting team wins.

After reading this README, I hope you have a great time enjoying the game!
