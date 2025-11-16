#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"

typedef enum {ROLE_BATSMAN=1, ROLE_BOWLER=2, ROLE_ALLROUNDER=3} RoleType;

typedef struct {
    int playerId;
    char name[64];
    char teamName[64];
    RoleType role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    double perfIndex;
} MyPlayer;

typedef struct {
    char name[64];
    int totalPlayers;
    double avgBattingStrikeRate;
} TeamInfo;


double compute_perf_index(const MyPlayer *playerData){
    if(playerData->role == ROLE_BATSMAN){
        return (playerData->battingAverage * playerData->strikeRate) / 100.0;
    } 
    if(playerData->role == ROLE_BOWLER){
        return (playerData->wickets * 2.0) + (100.0 - playerData->economyRate);
    }
    return ((playerData->battingAverage * playerData->strikeRate) / 100.0) +
           (playerData->wickets * 2.0);
}

RoleType role_from_string(const char *roleString){
    if(strcmp(roleString, "Batsman") == 0) return ROLE_BATSMAN;
    if(strcmp(roleString, "Bowler") == 0) return ROLE_BOWLER;
    return ROLE_ALLROUNDER;
}

const char* role_to_string(RoleType roleType){
    if(roleType == ROLE_BATSMAN) return "Batsman";
    if(roleType == ROLE_BOWLER) return "Bowler";
    return "All-rounder";
}


int compare_perf_desc(const void *firstData, const void *secondData){
    const MyPlayer *firstPlayer = firstData;
    const MyPlayer *secondPlayer = secondData;

    if(secondPlayer->perfIndex > firstPlayer->perfIndex) return 1;
    if(secondPlayer->perfIndex < firstPlayer->perfIndex) return -1;
    return 0;
}


int find_team_index(TeamInfo teamsInfoArray[], int totalTeams, const char *teamName){
    for(int teamIndex = 0; teamIndex < totalTeams; teamIndex++){
        if(strcasecmp(teamsInfoArray[teamIndex].name, teamName) == 0)
            return teamIndex;
    }
    return -1;
}


void print_player_list(MyPlayer playerList[], int listSize){
    printf("====================================================================================\n");
    printf("ID Name Role Runs Avg SR Wkts ER Perf.Index\n");
    printf("====================================================================================\n");

    for(int index = 0; index < listSize; index++){
        printf("%d %s %s %d %.1f %.1f %d %.1f %.2f\n",
            playerList[index].playerId,
            playerList[index].name,
            role_to_string(playerList[index].role),
            playerList[index].totalRuns,
            playerList[index].battingAverage,
            playerList[index].strikeRate,
            playerList[index].wickets,
            playerList[index].economyRate,
            playerList[index].perfIndex);
    }

    printf("====================================================================================\n");
}


int main(){

    int totalPlayers = playerCount;
    MyPlayer *allPlayers = malloc(sizeof(MyPlayer) * totalPlayers);

    for(int playerIndex = 0; playerIndex < totalPlayers; playerIndex++){
        allPlayers[playerIndex].playerId = players[playerIndex].id;
        strcpy(allPlayers[playerIndex].name, players[playerIndex].name);
        strcpy(allPlayers[playerIndex].teamName, players[playerIndex].team);
        allPlayers[playerIndex].role = role_from_string(players[playerIndex].role);
        allPlayers[playerIndex].totalRuns = players[playerIndex].totalRuns;
        allPlayers[playerIndex].battingAverage = players[playerIndex].battingAverage;
        allPlayers[playerIndex].strikeRate = players[playerIndex].strikeRate;
        allPlayers[playerIndex].wickets = players[playerIndex].wickets;
        allPlayers[playerIndex].economyRate = players[playerIndex].economyRate;
        allPlayers[playerIndex].perfIndex = compute_perf_index(&allPlayers[playerIndex]);
    }

    int totalTeams = teamCount;
    TeamInfo *teamsInfo = malloc(sizeof(TeamInfo) * totalTeams);

    for(int index = 0; index < totalTeams; index++){
        strcpy(teamsInfo[index].name, teams[index]);
        teamsInfo[index].totalPlayers = 0;
        teamsInfo[index].avgBattingStrikeRate = 0;
    }

    for(int playerIndex = 0; playerIndex < totalPlayers; playerIndex++){
        for(int teamIndex = 0; teamIndex < totalTeams; teamIndex++){
            if(strcmp(allPlayers[playerIndex].teamName, teamsInfo[teamIndex].name) == 0){
                teamsInfo[teamIndex].totalPlayers++;
            }
        }
    }

    for(int teamIndex = 0; teamIndex < totalTeams; teamIndex++){
        double strikeRateSum = 0;
        int strikerCount = 0;

        for(int playerIndex = 0; playerIndex < totalPlayers; playerIndex++){
            if(strcmp(allPlayers[playerIndex].teamName, teamsInfo[teamIndex].name) == 0){
                if(allPlayers[playerIndex].role == ROLE_BATSMAN ||
                   allPlayers[playerIndex].role == ROLE_ALLROUNDER){

                    strikeRateSum += allPlayers[playerIndex].strikeRate;
                    strikerCount++;
                }
            }
        }

        teamsInfo[teamIndex].avgBattingStrikeRate =
            (strikerCount == 0 ? 0 : strikeRateSum / strikerCount);
    }


    while(1){
        printf("\n==============================================================================\n");
        printf("ICC ODI Player Performance Analyzer\n");
        printf("==============================================================================\n");
        printf("1. Add Player to Team (skipped)\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display All Players of a Role Across ALL Teams\n");
        printf("6. Exit\n");
        printf("==============================================================================\n");
        printf("Enter choice: ");

        int userChoice; 
        scanf("%d", &userChoice);

        if(userChoice == 1){
            printf("Add Player feature skipped.\n");
        }

        else if(userChoice == 2){
            char inputTeamName[50];
            printf("Enter Team Name: ");
            scanf(" %[^\n]", inputTeamName);

            int teamIndex = find_team_index(teamsInfo, totalTeams, inputTeamName);
            if(teamIndex == -1){
                printf("Team not found.\n");
                continue;
            }

            MyPlayer *teamPlayerList = malloc(sizeof(MyPlayer) * teamsInfo[teamIndex].totalPlayers);
            int filteredCount = 0;

            for(int playerIndex = 0; playerIndex < totalPlayers; playerIndex++){
                if(strcasecmp(allPlayers[playerIndex].teamName, inputTeamName) == 0){
                    teamPlayerList[filteredCount++] = allPlayers[playerIndex];
                }
            }

            printf("Players of %s:\n", inputTeamName);
            print_player_list(teamPlayerList, filteredCount);

            printf("Total Players: %d\n", filteredCount);
            printf("Average Batting Strike Rate: %.2f\n", teamsInfo[teamIndex].avgBattingStrikeRate);

            free(teamPlayerList);
        }

        else if(userChoice == 3){
            TeamInfo sortedTeamsList[10];
            for(int index = 0; index < totalTeams; index++){
                sortedTeamsList[index] = teamsInfo[index];
            }

            for(int outerIndex = 0; outerIndex < totalTeams; outerIndex++){
                for(int innerIndex = outerIndex + 1; innerIndex < totalTeams; innerIndex++){
                    if(sortedTeamsList[innerIndex].avgBattingStrikeRate >
                       sortedTeamsList[outerIndex].avgBattingStrikeRate){

                        TeamInfo tempInfo = sortedTeamsList[outerIndex];
                        sortedTeamsList[outerIndex] = sortedTeamsList[innerIndex];
                        sortedTeamsList[innerIndex] = tempInfo;
                    }
                }
            }

            printf("Teams Sorted by Average Batting Strike Rate:\n");
            printf("=========================================================\n");
            printf("Team Name | Avg Bat SR | Total Players\n");
            printf("=========================================================\n");

            for(int index = 0; index < totalTeams; index++){
                printf("%s | %.2f | %d\n", 
                    sortedTeamsList[index].name,
                    sortedTeamsList[index].avgBattingStrikeRate,
                    sortedTeamsList[index].totalPlayers);
            }
        }

        else if(userChoice == 4){
            char inputTeamName[50];
            printf("Enter Team Name: ");
            scanf(" %[^\n]", inputTeamName);

            int teamIndex = find_team_index(teamsInfo, totalTeams, inputTeamName);
            if(teamIndex == -1){
                printf("Team not found.\n");
                continue;
            }

            int selectedRoleType;
            int topKCount;

            printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
            scanf("%d", &selectedRoleType);

            printf("Enter K: ");
            scanf("%d", &topKCount);

            MyPlayer *roleFilteredList = malloc(sizeof(MyPlayer) * teamsInfo[teamIndex].totalPlayers);
            int filteredCount = 0;

            for(int playerIndex = 0; playerIndex < totalPlayers; playerIndex++){
                if(strcasecmp(allPlayers[playerIndex].teamName, inputTeamName) == 0 &&
                   allPlayers[playerIndex].role == selectedRoleType){

                    roleFilteredList[filteredCount++] = allPlayers[playerIndex];
                }
            }

            if(filteredCount == 0){
                printf("No such players.\n");
                free(roleFilteredList);
                continue;
            }

            qsort(roleFilteredList, filteredCount, sizeof(MyPlayer), compare_perf_desc);

            if(topKCount > filteredCount) topKCount = filteredCount;

            printf("Top %d %s of %s:\n", 
                topKCount, 
                role_to_string(selectedRoleType), 
                inputTeamName);

            print_player_list(roleFilteredList, topKCount);

            free(roleFilteredList);
        }

        else if(userChoice == 5){
            int selectedRoleType;
            printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
            scanf("%d", &selectedRoleType);

            MyPlayer *roleFilteredList = malloc(sizeof(MyPlayer) * totalPlayers);
            int filteredCount = 0;

            for(int playerIndex = 0; playerIndex < totalPlayers; playerIndex++){
                if(allPlayers[playerIndex].role == selectedRoleType){
                    roleFilteredList[filteredCount++] = allPlayers[playerIndex];
                }
            }

            qsort(roleFilteredList, filteredCount, sizeof(MyPlayer), compare_perf_desc);

            print_player_list(roleFilteredList, filteredCount);

            free(roleFilteredList);
        }

        else if(userChoice == 6){
            break;
        }

        else{
            printf("Invalid choice.\n");
        }
    }

    free(allPlayers);
    free(teamsInfo);

    return 0;
}
