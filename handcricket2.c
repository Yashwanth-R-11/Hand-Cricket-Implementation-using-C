#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include"rules (1).h"
typedef struct {
    int runs;
    int balls;
} BatterStats;
typedef struct {
    int wickets;
} BowlerStats;
typedef struct {
    int freq[7];
} FrequencyStats;

char team1[PLAYERS_PER_TEAM][50];
char team2[PLAYERS_PER_TEAM][50];

void printHeader() {
    printf("-------\n");
    printf("       TEAM NAME, PR0JECT NAME       \n");
    printf("        HANDCRICKET             \n");
    printf("--------\n\n");
}

void waitForEnter() {
    printf("Press Enter to continue...");
    getchar();
    getchar();
}

void inputPlayerNames() {
    printf("Enter player names for Team 1:\n");
    for (int i = 0; i < PLAYERS_PER_TEAM; i++) {
        printf("Player %d: ", i + 1);
        scanf(" %[^\n]", team1[i]);
    }

    printf("\nEnter player names for Team 2:\n");
    for (int i = 0; i < PLAYERS_PER_TEAM; i++) {
        printf("Player %d: ", i + 1);
        scanf(" %[^\n]", team2[i]);
    }
}

int toss() {
    int call, tossResult;
    printf("%s, call the toss (0 for Heads, 1 for Tails): ", team1[0]);
    scanf("%d", &call);

    tossResult = rand() % 2;
    printf("Toss result: %s\n", tossResult == 0 ? "Heads" : "Tails");

    if (call == tossResult) {
        printf(" %s's Team wins the toss!\n", team1[0]);
        return 1;
    } else {
        printf(" %s's Team wins the toss!\n", team2[0]);
        return 2;
    }
}

int chooseBatOrBowl(char captainName[]) {
    int choice;
    printf("%s, choose:\n", captainName);
    printf("0. Bat first\n");
    printf("1. Bowl first\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int getHiddenInput() {
    int num;
    char ch;
    do {
        ch = getch();
        num = ch - '0';
    } while (num < 1 || num > 6);
    printf("*\n");
    return num;
}

int getVisibleInput(const char* playerName) {
    int num;
    printf("%s (Bowling), enter a number (1-6): ", playerName);
    scanf("%d", &num);

    while (num < 1 || num > 6) {
        printf("Invalid input. Enter a number between 1 and 6: ");
        scanf("%d", &num);
    }

    return num;
}

int playInnings(
    char battingTeam[][50], char bowlingTeam[][50], int targetToChase,
    BatterStats batterStats[], BowlerStats bowlerStats[], FrequencyStats freqStats[]
) {
    int totalRuns = 0, wicketsLost = 0, strikerIndex = 0, ballCounter = 0;

    printf("\n------\n");
    printf("  Batting: %s & %s\n", battingTeam[0], battingTeam[1]);
    printf("  Bowling: %s & %s\n", bowlingTeam[0], bowlingTeam[1]);
    printf("-------\n");

    for (int over = 0; over < OVERS && wicketsLost < PLAYERS_PER_TEAM; over++) {
        int currentBowlerIndex = over % 2;
        printf("\n--- Over %d: Bowler is %s ---\n", over + 1, bowlingTeam[currentBowlerIndex]);

        for (int ball = 0; ball < BALLS_PER_OVER && wicketsLost < PLAYERS_PER_TEAM; ball++) {
            ballCounter++;
            printf("\nBall %d\n", ballCounter);

            printf("%s (Batting), choose a number (1-6): ", battingTeam[strikerIndex]);
            int bat = getHiddenInput();
            freqStats[strikerIndex].freq[bat]++;

            int bowl = getVisibleInput(bowlingTeam[currentBowlerIndex]);
            freqStats[currentBowlerIndex + 2].freq[bowl]++;

            batterStats[strikerIndex].runs += bat;
            batterStats[strikerIndex].balls++;

            if (bat == bowl) {
                printf(" OUT! %s is out!\n", battingTeam[strikerIndex]);
                bowlerStats[currentBowlerIndex].wickets++;
                wicketsLost++;
                strikerIndex++;
            } else {
                totalRuns += bat;
                printf(" %d Runs Scored! Total: %d/%d\n", bat, totalRuns, wicketsLost);
            }

            if (targetToChase != -1 && totalRuns > targetToChase) {
                printf(" Target surpassed! Innings ends early.\n");
                return totalRuns;
            }
        }
    }

    printf("\n Innings Over! Total Score: %d/%d\n", totalRuns, wicketsLost);
    return totalRuns;
}

void saveToFile(
    char winner[], int team1Score, int team2Score,
    BatterStats team1Batters[], BatterStats team2Batters[],
    BowlerStats team1Bowlers[], BowlerStats team2Bowlers[],
    FrequencyStats freqStats[]
) {
    FILE *file = fopen("hand_cricket_score.txt", "a");
    if (file == NULL) {
        printf(" Error opening file.\n");
        return;
    }

    fprintf(file, " Match Summary:\n");
    fprintf(file, "Team 1 (%s & %s): %d runs\n", team1[0], team1[1], team1Score);
    for (int i = 0; i < 2; i++) {
        float sr = team1Batters[i].balls ? (team1Batters[i].runs / (float)team1Batters[i].balls) * 100 : 0;
        fprintf(file, "  %s - %d runs, %d balls, SR: %.2f\n", team1[i], team1Batters[i].runs, team1Batters[i].balls, sr);
    }

    fprintf(file, "Team 2 (%s & %s): %d runs\n", team2[0], team2[1], team2Score);
    for (int i = 0; i < 2; i++) {
        float sr = team2Batters[i].balls ? (team2Batters[i].runs / (float)team2Batters[i].balls) * 100 : 0;
        fprintf(file, "  %s - %d runs, %d balls, SR: %.2f\n", team2[i], team2Batters[i].runs, team2Batters[i].balls, sr);
    }

    fprintf(file, "\nBowler Stats:\n");
    fprintf(file, "  %s - %d wickets\n", team1[0], team1Bowlers[0].wickets);
    fprintf(file, "  %s - %d wickets\n", team1[1], team1Bowlers[1].wickets);
    fprintf(file, "  %s - %d wickets\n", team2[0], team2Bowlers[0].wickets);
    fprintf(file, "  %s - %d wickets\n", team2[1], team2Bowlers[1].wickets);

    fprintf(file, "\nInput Frequency:\n");
    for (int i = 0; i < 4; i++) {
        char* name = (i < 2) ? team1[i] : team2[i - 2];
        fprintf(file, "%s: ", name);
        for (int j = 1; j <= 6; j++) {
            fprintf(file, "%d(%dx) ", j, freqStats[i].freq[j]);
        }
        fprintf(file, "\n");
    }

    fprintf(file, " Winner: %s\n", winner);
    fprintf(file, "-----------------------------------\n");

    fclose(file);
    printf(" Detailed match result saved to 'hand_cricket_score.txt'\n");
}

void showFinalResult(int team1Score, int team2Score,
    BatterStats team1Batters[], BatterStats team2Batters[],
    BowlerStats team1Bowlers[], BowlerStats team2Bowlers[],
    FrequencyStats freqStats[]) {

    printf("\n==============================\n");
    printf("          FINAL RESULT\n");
    printf("==============================\n");
    printf("Team 1 (%s & %s): %d runs\n", team1[0], team1[1], team1Score);
    printf("Team 2 (%s & %s): %d runs\n", team2[0], team2[1], team2Score);

    if (team1Score > team2Score) {
        printf(" Team 1 Wins! (%s & %s)\n", team1[0], team1[1]);
        saveToFile("Team 1", team1Score, team2Score, team1Batters, team2Batters, team1Bowlers, team2Bowlers, freqStats);
    } else if (team2Score > team1Score) {
        printf("Team 2 Wins! (%s & %s)\n", team2[0], team2[1]);
        saveToFile("Team 2", team1Score, team2Score, team1Batters, team2Batters, team1Bowlers, team2Bowlers, freqStats);
    } else {
        printf(" It's a Tie!\n");
        saveToFile("Tie", team1Score, team2Score, team1Batters, team2Batters, team1Bowlers, team2Bowlers, freqStats);
    }
}

int main() {
    srand(time(NULL));
    int tossWinner, choice, team1BatsFirst;
    int team1Score, team2Score;

    BatterStats team1Batters[2] = {{0}};
    BatterStats team2Batters[2] = {{0}};
    BowlerStats team1Bowlers[2] = {{0}};
    BowlerStats team2Bowlers[2] = {{0}};
    FrequencyStats freqStats[4] = {{{0}}};

    printHeader();
    inputPlayerNames();
    tossWinner = toss();

    if (tossWinner == 1) {
        choice = chooseBatOrBowl(team1[0]);
        team1BatsFirst = (choice == 0);
    } else {
        choice = chooseBatOrBowl(team2[0]);
        team1BatsFirst = !(choice == 0);
    }

    waitForEnter();

    if (team1BatsFirst) {
        printf("\n-- First Innings: Team 1 Batting --\n");
        team1Score = playInnings(team1, team2, -1, team1Batters, team2Bowlers, freqStats);

        printf("\n-- Second Innings: Team 2 Batting --\n");
        team2Score = playInnings(team2, team1, team1Score, team2Batters, team1Bowlers, freqStats);
    } else {
        printf("\n-- First Innings: Team 2 Batting --\n");
        team2Score = playInnings(team2, team1, -1, team2Batters, team1Bowlers, freqStats);

        printf("\n-- Second Innings: Team 1 Batting --\n");
        team1Score = playInnings(team1, team2, team2Score, team1Batters, team2Bowlers, freqStats);
    }

    showFinalResult(team1Score, team2Score, team1Batters, team2Batters, team1Bowlers, team2Bowlers, freqStats);
    return 0;
}
