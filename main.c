#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>


void debug(void) {
    printf("\ndebug\n");
}


typedef struct {

    char name[20];
    int score;

} user;


// games option
int
        WIDTH = 10,
        HEIGHT = 10,
        total_number_ships = 10,
        turn = 1,
        largest_length = 5,
        run = true,
        separator_code = 4,
        number_saved_games = 0,
        isBot = false;

// ships information
int
        count_GreenBay = 4,
        count_Freedom = 3,
        count_Pioneer = 2,
        count_VellaGulf = 0,
        count_Independence = 1,
        count_Frragut = 0,
        count_Virginia = 0,
        count_GeorgeWashington = 0;

int
        HP_GreenBay = 1,
        HP_Freedom = 2,
        HP_Pioneer = 3,
        HP_VellaGulf = 4,
        HP_Independence = 5,
        HP_Frragut = 6,
        HP_Virginia = 7,
        HP_GeorgeWashington = 10;

// rules
int missile_is_used_1 = false, missile_is_used_2 = false;

// players information
int player2_score = 0, player1_score = 0, number_users = 0, place_player1_List = -2, place_player2_List = -2;
user List_Users[100];


struct node {

    int x;
    int y;
    char status;

    struct node *next;
    struct node *prev;
};


struct SHIP {

    int length;
    char *dir;

    struct node *head;
    struct node *tail;
};


// this function is for linking the ships parts in the map_player
void append(int x, int y, struct SHIP *ship, char status) {

    struct node *new_node = malloc(sizeof(struct node));
    if (ship->head == NULL && ship->tail == NULL) {

        ship->tail = new_node;
        ship->head = new_node;

        ship->tail->x = x;
        ship->tail->y = y;
        ship->tail->status = status;

        ship->head->x = x;
        ship->head->y = y;
        ship->head->status = status;

    } else {

        ship->tail->next = new_node;
        ship->tail->next->prev = ship->tail;
        ship->tail->next->next = NULL;
        ship->tail = ship->tail->next;

        ship->tail->y = y;
        ship->tail->x = x;
        ship->tail->status = status;

    }


}


// this function change turn.
void change_turn() {

    if (turn == 1) {

        turn = 2;
    } else if (turn == 2) {

        turn = 1;
    }
}


// this function find the ship according to a coordinate
struct SHIP *getShip(int x, int y, struct SHIP **ships) {

    struct node *current_node;

    for (int i = 0; i < total_number_ships; i++) {

        current_node = ships[i]->head;
        for (int j = 0; j < ships[i]->length; j++) {

            if (x == current_node->x && y == current_node->y) { return ships[i]; }

            current_node = current_node->next;
        }
    }
}


// this function find the exact node according to a coordinate
struct node *getNode(int x, int y, struct SHIP **ships) {

    struct node *current_node;

    for (int i = 0; i < total_number_ships; i++) {

        current_node = ships[i]->head;
        for (int j = 0; j < ships[i]->length; j++) {

            if (x == current_node->x && y == current_node->y) { return current_node; }

            current_node = current_node->next;
        }
    }
}


// this function check if the function in completely exploded or not
int isExploded(struct SHIP *ship) {

    struct node *current_node;
    current_node = ship->head;

    for (int i = 0; i < ship->length; i++) {

        if (current_node->status == 'f') { return false; }
        current_node = current_node->next;
    }

    return true;
}


// check if the ship fits in the map_player
int isInside(const char *map_player, struct SHIP* ship, int _x_, int _y_, char *dir) {

    if (strcmp(dir, "d") == 0) {
        for (int i = 0; i < ship->length; i++) {

            if (!((_y_ + i < HEIGHT && _y_ + i >= 0) && (0 <= _x_ && _x_ < WIDTH))) {
                printf("the ship does not fit into the map_player\n");
                return false;
            }
        }
    }

    if (strcmp(dir, "u") == 0) {

        for (int i = 0; i < ship->length; i++) {

            if (!((_y_ - i < HEIGHT && _y_ - i >= 0) && (0 <= _x_ && _x_ < WIDTH))) {
                printf("the ship does not fit into the map_player\n");
                return false;
            }
        }
    }

    if (strcmp(dir, "r") == 0) {

        for (int i = 0; i < ship->length; i++) {

            if (!((_y_ < HEIGHT && _y_ >= 0) && (0 <= _x_ + i && _x_ + i < WIDTH))) {
                printf("the ship does not fit into the map_player\n");
                return false;
            }
        }
    }

    if (strcmp(dir, "l") == 0) {

        for (int i = 0; i < ship->length; i++) {

            if (!((_y_ < HEIGHT && _y_ >= 0) && (0 <= _x_ - i && _x_ - i < WIDTH))) {
                printf("the ship does not fit into the map_player\n");
                return false;
            }
        }
    }
    return true;
}


// check if a node is inside the map
int isIn(int x, int y) {

    if (x < WIDTH && y < HEIGHT && x >= 0 && y >= 0) return true;
    else return false;
}


// check if is there any collision between ships space
int ifCollision(const char *map_player, struct SHIP *ship, int _x_, int _y_, char *dir) {

    if (strcmp(dir, "d") == 0) {
        for (int i = -1; i <= ship->length; i++) {

            for (int j = -1; j < 2; j++) {

                if (map_player[(_y_ + i) * WIDTH + _x_ + j] == 'f' &&
                    ((_y_ + i < HEIGHT && _y_ + i >= 0) && (_x_ + j < WIDTH && _x_ + j >= 0))) {
                    printf("there is a collision between ships\n");
                    return true;
                }
            }
        }
    }


    if (strcmp(dir, "u") == 0) {

        for (int i = -1; i <= ship->length; i++) {

            for (int j = -1; j < 2; j++) {

                if (map_player[(_y_ - i) * WIDTH + _x_ + j] == 'f' &&
                    ((_y_ - i < HEIGHT && _y_ - i >= -1) && (_x_ + j < WIDTH && _x_ + j >= 0))) {
                    printf("there is a collision between ships\n");
                    return true;
                }
            }
        }
    }


    if (strcmp(dir, "r") == 0) {

        for (int i = -1; i <= ship->length; i++) {

            for (int j = -1; j < 2; j++) {

                if (map_player[(_y_ + j) * WIDTH + _x_ + i] == 'f' &&
                    ((_y_ + j < HEIGHT && _y_ + j >= 0) && (_x_ + i < WIDTH && _x_ + i >= 0))) {
                    printf("there is a collision between ships\n");
                    return true;
                }
            }
        }
    }


    if (strcmp(dir, "l") == 0) {

        for (int i = -1; i <= ship->length; i++) {

            for (int j = -1; j < 2; j++) {

                if (map_player[(_y_ + j) * WIDTH + _x_ - i] == 'f' &&
                    ((_y_ + j < HEIGHT && _y_ + j >= 0) && (_x_ - i < WIDTH && _x_ - i >= 0))) {
                    printf("there is a collision between ships\n");
                    return true;
                }
            }
        }
    }
    return false;
}


// this function checks if the ship fits in its place in 2 condition:
//                                                                   first if the ship is completely in the map_player.
//                                                                   second if the ship does not have any collision with other ships.
int isFit(const char *map_player, struct SHIP *ship, int _x_, int _y_, char *dir) {

    if (isInside(map_player, ship, _x_, _y_, dir) && !(ifCollision(map_player, ship, _x_, _y_, dir))) {

        return true;
    } else {

        return false;
    }
}


// this function put the ship in its place
int make_ship(struct SHIP *ship, char *dir, char *map_player, int _x_, int _y_) {

    struct node *current_node;
    current_node = ship->head;

    // u : up
    if (strcmp(dir, "u") == 0) {
        if (isFit(map_player, ship, _x_, _y_, dir) == 0) {      // this check that if the ship fits in the place or not
            printf("invalid place for putting your ship\n");
            return 0;
        }

        ship->dir = strdup(dir);
        append(_x_, _y_, ship, 'f');
        map_player[_x_ + WIDTH * _y_] = 'f';

        for (int i = 1; i < ship->length; i++) {
            append(_x_, _y_ - i, ship, 'f');
            map_player[_x_ + WIDTH * (_y_ - i)] = 'f';
        }
    }

    // d : down
    if (strcmp(dir, "d") == 0) {
        if (isFit(map_player, ship, _x_, _y_, dir) == 0) {      // this check that if the ship fits in the map_player or not
            printf("invalid place for putting your ship\n");
            return 0;
        }

        ship->dir = strdup(dir);
        append(_x_, _y_, ship, 'f');
        map_player[_x_ + WIDTH * _y_] = 'f';

        for (int i = 1; i < ship->length; i++) {
            append(_x_, _y_ + i, ship, 'f');
            map_player[_x_ + WIDTH * (_y_ + i)] = 'f';
        }
    }

    // r : right
    if (strcmp(dir, "r") == 0) {
        if (isFit(map_player, ship, _x_, _y_, dir) == 0) {      // this check that if the ship fits in the map_player or not
            printf("invalid place for putting your ship\n");
            return 0;
        }

        ship->dir = strdup(dir);
        append(_x_, _y_, ship, 'f');
        map_player[_x_ + WIDTH * _y_] = 'f';

        for (int i = 1; i < ship->length; i++) {
            append(_x_ + i, _y_, ship, 'f');
            map_player[_x_ + i + WIDTH * _y_] = 'f';
        }
    }

    // l : left
    if (strcmp(dir, "l") == 0) {
        if (isFit(map_player, ship, _x_, _y_, dir) == 0) {      // this check that if the ship fits in the map_player or not
            printf("invalid place for putting your ship\n");
            return 0;
        }

        ship->dir = strdup(dir);
        append(_x_, _y_, ship, 'f');
        map_player[_x_ + WIDTH * _y_] = 'f';

        for (int i = 1; i < ship->length; i++) {
            append(_x_ - i, _y_, ship, 'f');
            map_player[_x_ - i + WIDTH * _y_] = 'f';
        }
    }
    return 1;
}


// this function locate the ships into the map_player manually
void map_player_ships_manually(struct SHIP **ships, char *map_player) {
    // the code below is for players mapping term

    int length_1_counter = 0;
    int length_2_counter = 0;
    int length_3_counter = 0;
    int length_5_counter = 0;

    int length;                           // length is for the length of the boat that the player select
    int _x_;                                // x of head of the ship
    int _y_;                                // y of head of the ship
    char *dir = malloc(sizeof(char) *2);     // the direction that the ship has, must choose between u(up), d(down), r(right), l(left)

    while (length_1_counter + length_2_counter + length_3_counter + length_5_counter < total_number_ships) {

        //----------------------------------------------------- this part is for getting inputs from user (player)
        // scanning a valid length
        do {
            printf("select the size of the ship\n");
            scanf("%d", &length);
        } while (length != 1 && length != 2 && length != 3 && length != 5);

        // scanning a valid x

        do {
            printf("enter a number for x between 0 and %d\n", WIDTH - 1);
            scanf("%d", &_x_);
        } while (_x_ < 0 || _x_ > WIDTH - 1);

        // scanning a valid y

        do {
            printf("enter a number for y between 0 and %d\n", HEIGHT - 1);
            scanf("%d", &_y_);
        } while (_y_ < 0 || _y_ > HEIGHT - 1);

        // scanning a valid directory

        do {
            printf("enter the direction that the ship has, must choose between u(up), d(down), r(right), l(left)\n");
            scanf("%s", dir);
        } while (strcmp(dir, "u") != 0 && strcmp(dir, "d") != 0 && strcmp(dir, "r") != 0 && strcmp(dir, "l") != 0);

        //----------------------------------------------------- this part is placing ships in the map_player

        //----------------------length 1

        if (length == 1 && length_1_counter < 4) {

            ships[6 + length_1_counter]->dir = strdup(dir);
            if (make_ship(ships[6 + length_1_counter], dir, map_player, _x_, _y_) == 0) continue;

            length_1_counter++;

        } else if (length == 1 && length_1_counter == 4) {
            printf("you have used all of your ships with length of 1\n");
            continue;
        }

        //----------------------length 2

        if (length == 2 && length_2_counter < 3) {

            ships[3 + length_2_counter]->dir = strdup(dir);
            if (make_ship(ships[3 + length_2_counter], dir, map_player, _x_, _y_) == 0) continue;

            length_2_counter++;

        } else if (length == 2 && length_2_counter == 3) {
            printf("you have used all of your ships with length of 2\n");
            continue;
        }

        //----------------------length 3

        if (length == 3 && length_3_counter < 2) {

            ships[1 + length_3_counter]->dir = strdup(dir);
            if (make_ship(ships[1 + length_3_counter], dir, map_player, _x_, _y_) == 0) continue;

            length_3_counter++;

        } else if (length == 3 && length_3_counter == 2) {
            printf("you have used all of your ships with length of 3\n");
            continue;
        }

        //----------------------length 5

        if (length == 5 && length_5_counter < 1) {

            ships[0]->dir = strdup(dir);
            if (make_ship(ships[0], dir, map_player, _x_, _y_) == 0) continue;

            length_5_counter++;

        } else if (length == 5 && length_5_counter == 1) {
            printf("you have used all of your ships with length of 5\n");
            continue;
        }

        //--------------------------------------------------------------------------------------------------------

    }
}


// this function locate the ships into the map_player automatically
void map_player_ships_automatically(struct SHIP **ships, char *map_player) {
    // the code below is for mapping automatically

    srand(time(0));

    int length_1_counter = 0;
    int length_2_counter = 0;
    int length_3_counter = 0;
    int length_5_counter = 0;

    int length;  // length is for the length of the boat that the player select
    int _x_;     // x of head of the ship
    int _y_;     // y of head of the ship
    char *dir = malloc(sizeof(char) * 2);
    int dir_int; // the direction that the ship has, must choose between 1(up), 2(down), 3(right), 4(left)
    int i = 0;

    while (length_1_counter + length_2_counter + length_3_counter + length_5_counter < total_number_ships) {

        _x_ = rand() % WIDTH;
        _y_ = rand() % HEIGHT;
        dir_int = rand() % 4 + 1;

        if (i % 10 < 4){
            length = 1;
        }
        else if (i % 10 >= 4 && i % 10 < 7){
            length = 2;
        }
        else if (i % 10 >= 7 && i % 10 < 9){
            length = 3;
        }
        else if (i % 10 == 9){
            length = 5;
        }

        if (dir_int == 1) strcpy(dir, "u");
        else if (dir_int == 2) strcpy(dir, "d");
        else if (dir_int == 3) strcpy(dir, "r");
        else if (dir_int == 4) strcpy(dir, "l");

        //----------------------------------------------------- this part is placing ships in the map_player
        //----------------------length 1
        if (length == 1 && length_1_counter < 4) {

            ships[6 + length_1_counter]->dir = strdup(dir);
            if (make_ship(ships[6 + length_1_counter], dir, map_player, _x_, _y_) == 0) {continue;}
            length_1_counter++;
            i++;
        }
        //----------------------length 2
        else if (length == 2 && length_2_counter < 3) {

            ships[3 + length_2_counter]->dir = strdup(dir);
            if (make_ship(ships[3 + length_2_counter], dir, map_player, _x_, _y_) == 0) {continue;}
            length_2_counter++;
            i++;
        }
        //----------------------length 3
        else if (length == 3 && length_3_counter < 2) {

            ships[1 + length_3_counter]->dir = strdup(dir);
            if (make_ship(ships[1 + length_3_counter], dir, map_player, _x_, _y_) == 0) {continue;}
            length_3_counter++;
            i++;
        }
        //----------------------length 5
        else if (length == 5 && length_5_counter < 1) {

            ships[0]->dir = strdup(dir);
            if (make_ship(ships[0], dir, map_player, _x_, _y_) == 0) {continue;}
            length_5_counter++;
            i++;
        }
        //--------------------------------------------------------------------------------------------------------
    }
}


// this function sort_users the users in due to their scores
void sort_users (){

    int rev = 0;
    char string[20];

    for (int i = 0;i < number_users;i++){

        if (i != 0 && List_Users[i].score > List_Users[i - 1].score){

            rev = List_Users[i].score;
            strcpy(string, List_Users[i].name);

            List_Users[i].score = List_Users[i - 1].score;
            strcpy(List_Users[i].name, List_Users[i - 1].name);

            List_Users[i - 1].score = rev;
            strcpy(List_Users[i - 1].name, string);

            i = 0;
        }
    }
}


// this function saves the information of the users during the game
void save_users_info(void) {

    if (isBot == false) {
        // save_users_info users score and users
        List_Users[place_player1_List].score += player1_score;
        List_Users[place_player2_List].score += player2_score;

        struct SHIP *current_ship = malloc(sizeof(struct SHIP));
        struct node *current_node = malloc(sizeof(struct node));

        sort_users();
        FILE *file_users = fopen("users.txt", "w");

        for (int i = 0; i < number_users - 1; i++) {
            fprintf(file_users, "%s ", List_Users[i].name);
            fprintf(file_users, "%d\n", List_Users[i].score);
        }
        fprintf(file_users, "%s ", List_Users[number_users - 1].name);
        fprintf(file_users, "%d", List_Users[number_users - 1].score);

        fclose(file_users);
    }
    else if (isBot == true){
        // save_users_info users score and users
        List_Users[place_player1_List].score += player1_score;

        struct SHIP *current_ship = malloc(sizeof(struct SHIP));
        struct node *current_node = malloc(sizeof(struct node));

        sort_users();
        FILE *file_users = fopen("users.txt", "w");

        for (int i = 0; i < number_users - 1; i++) {
            fprintf(file_users, "%s ", List_Users[i].name);
            fprintf(file_users, "%d\n", List_Users[i].score);
        }
        fprintf(file_users, "%s ", List_Users[number_users - 1].name);
        fprintf(file_users, "%d", List_Users[number_users - 1].score);

        fclose(file_users);
    }

}


// this function save_users_info the entire game
void record_game(char* map_player1 , char* map_player2){

    FILE* RECORD = fopen("RECORD.txt", "a");

    fprintf(RECORD, "\n               PLAYER 1\n");
    // loop for drawing the players map
    for (int y = 0; y < HEIGHT; y++) {

        for (int x = 0; x < WIDTH; x++) {
            if (x == WIDTH - 1) fprintf(RECORD, " %c \n", map_player1[x + WIDTH * y]);
            else fprintf(RECORD, " %c |", map_player1[x + WIDTH * y]);
        }

        if (y < HEIGHT - 1) {
            for (int x = 0; x < WIDTH; x++) {
                if (x == WIDTH - 1) fprintf(RECORD, "---\n");
                else fprintf(RECORD, "---|");
            }
        }
    }

    // this separates the maps
    fprintf(RECORD,"\n");
    for (int i = 0; i < WIDTH * 4 - 1; i++) {
        fprintf(RECORD, "&");
    }
    fprintf(RECORD, "\n\n");


    fprintf(RECORD, "\n               PLAYER 2\n");
    // loop for drawing the players 2 map
    for (int y = 0; y < HEIGHT; y++) {

        for (int x = 0; x < WIDTH; x++) {

            if (x == WIDTH - 1) fprintf(RECORD, " %c \n", map_player2[x + WIDTH * y]);
            else fprintf(RECORD, " %c |", map_player2[x + WIDTH * y]);

        }

        if (y < HEIGHT - 1) {
            for (int x = 0; x < WIDTH; x++) {

                if (x == WIDTH - 1) fprintf(RECORD, "---\n");
                else fprintf(RECORD, "---|");
            }
        }
    }

    // this separates the maps
    fprintf(RECORD,"\n");
    for (int i = 0; i < WIDTH * 4 - 1; i++) {
        fprintf(RECORD, "&");
    }
    fprintf(RECORD, "\n\n");

    // closing the file
    fclose(RECORD);

}


// this function show the entire games moves
void play_back(char* recorder){

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    FILE* PLAY_BACK = fopen("RECORD.txt", "r");

    while (!(feof(PLAY_BACK))){
        fgets(recorder, 100000, PLAY_BACK);
        printf("%s", recorder);

    }
    fclose(PLAY_BACK);
}


// this function draws the games momentary boards for player
void draw_board(char *map) {

    // theme options
    char separators[] = {'a' + 79, 'a' + 80, 'a' + 81, '|', '$', '*'};
    char *map_name[] = {"Shady1", "Shady2", "Shady3", "Line", "Money", "Star"};

    // loop for drawing the map
    if (separator_code == 4) {
        for (int y = 0; y < HEIGHT; y++) {

            for (int x = 0; x < WIDTH; x++) {

                if (x == WIDTH - 1) printf(" %c \n", map[x + WIDTH * y]);
                else printf(" %c |", map[x + WIDTH * y]);
            }

            if (y < HEIGHT - 1) {
                for (int x = 0; x < WIDTH; x++) {

                    if (x == WIDTH - 1) printf("---\n");
                    else printf("---|");
                }
            }
        }
    }

    else {

        for (int y = 0; y < HEIGHT; y++) {

            for (int x = 0; x < WIDTH; x++) {

                if (x == WIDTH - 1) printf(" %c \n", map[x + WIDTH * y]);
                else printf(" %c %c", map[x + WIDTH * y], separators[separator_code - 1]);
            }

            if (y < HEIGHT - 1) {
                for (int x = 0; x < WIDTH; x++) {

                    if (x == WIDTH - 1) printf("%c%c%c\n", separators[separator_code - 1], separators[separator_code - 1], separators[separator_code - 1]);
                    else printf("%c%c%c%c", separators[separator_code - 1], separators[separator_code - 1], separators[separator_code - 1], separators[separator_code - 1]);
                }
            }
        }
    }

    // this separates the maps
    printf("\n");
    for (int i = 0; i < WIDTH * 4 - 1; i++) {
        printf("&");
    }
    printf("\n\n");
}


// save_users_info game information ( ships and maps)
void save(char *map_player1, char *map_player2, struct SHIP **player1_ships, char *map_player2_for_player1, char *map_player1_for_player2, struct SHIP **player2_ships)
{

    FILE *saved_ships = fopen("saved_ships.txt", "ab");
    FILE *saved_maps = fopen("saved_maps.txt", "ab");
    struct node *current_node = malloc(sizeof(struct node));

    // saving the game info
    // maps
    fwrite(map_player1, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);
    fwrite(map_player2_for_player1, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);
    fwrite(map_player2, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);
    fwrite(map_player1_for_player2, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);
    // ships
    for (int i = 0; i < total_number_ships; i++)
    {
        // player1 ships
        fwrite(player1_ships[i]->dir, sizeof(char), 2, saved_ships);
        fwrite(&player1_ships[i]->length, sizeof(int), 1, saved_ships);
        current_node = player1_ships[i]->head;
        for (int j = 0; j < player1_ships[i]->length; j++)
        {
            fwrite(&current_node->x, sizeof(int), 1, saved_ships);
            fwrite(&current_node->y, sizeof(int), 1, saved_ships);
            fwrite(&current_node->status, sizeof(char), 1, saved_ships);
            current_node = current_node->next;
        }
        // player2 ships
        fwrite(player2_ships[i]->dir, sizeof(char), 2, saved_ships);
        fwrite(&player2_ships[i]->length, sizeof(int), 1, saved_ships);
        current_node = player2_ships[i]->head;

        for (int j = 0; j < player2_ships[i]->length; j++)
        {
            fwrite(&current_node->x, sizeof(int), 1, saved_ships);
            fwrite(&current_node->y, sizeof(int), 1, saved_ships);
            fwrite(&current_node->status, sizeof(char), 1, saved_ships);
            current_node = current_node->next;
        }
    }
    fclose(saved_ships);
    fclose(saved_maps);
}


// this function load the saved games
void load(char *map_player1, char *map_player2, struct SHIP **player1_ships, char *map_player2_for_player1, char *map_player1_for_player2, struct SHIP **player2_ships)
{

    int choice = 1;
    int i = 0;
    struct node *current_node;
    int isLast = 2;

    // ask if should load last game
    printf("1. Load Last game\n"
           "2. show all saved games\n");
    do
    {
        scanf("%d", &choice);
    } while (choice != 1 && choice != 2);
    // initializing

    FILE *saved_maps = fopen("saved_maps.txt", "rb");
    FILE *saved_ships = fopen("saved_ships.txt", "rb");
    char *map_player2_copy = malloc(sizeof(char) * WIDTH * HEIGHT + 1);
    char *map_player1_copy = malloc(sizeof(char) * WIDTH * HEIGHT + 1);
    char *map_player2_for_player1_copy = malloc(sizeof(char) * WIDTH * HEIGHT + 1);
    char *map_player1_for_player2_copy = malloc(sizeof(char) * WIDTH * HEIGHT + 1);

    do
    {
        i++;
        // reading the maps from the file
        fread(map_player1_copy, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);
        fread(map_player2_for_player1_copy, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);
        fread(map_player2_copy, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);
        fread(map_player1_for_player2_copy, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);

        // drawing the maps
        printf("%d.              PLAYER 1\n", i);
        draw_board(map_player1_copy);
        printf("%d.              PLAYER 2\n", i);
        draw_board(map_player2_copy);

        // reallocating the maps
        map_player2_copy = malloc(sizeof(char) * WIDTH * HEIGHT + 1);
        map_player1_copy = malloc(sizeof(char) * WIDTH * HEIGHT + 1);
        map_player2_for_player1_copy = malloc(sizeof(char) * WIDTH * HEIGHT + 1);
        map_player1_for_player2_copy = malloc(sizeof(char) * WIDTH * HEIGHT + 1);
    } while (i < number_saved_games && isLast == 2);
    ///--------------------------------------------------------------------------------
    if (choice == 2)
    {
        printf("\nchoose your game\n");
        scanf("%d", &choice);
    }
    else if (choice == 1)
    {
        choice = number_saved_games;
    }

    fseek(saved_ships, (choice - 1) * (41 * sizeof(char) + 52 * sizeof(int)), SEEK_SET);
    fseek(saved_maps, (choice - 1) * 4 * sizeof(char) * WIDTH * HEIGHT + 1, SEEK_SET);

    //////////////////////////////////////
    int x;
    int y;
    char status;

    // ships
    for (int k = 0; k < total_number_ships; k++)
    {
        // player 1 ships
        fread(player1_ships[k]->dir, sizeof(char), 2, saved_ships);
        fread(&player1_ships[k]->length, sizeof(int), 1, saved_ships);
        printf("%s %d", player1_ships[k]->dir, player1_ships[k]->length);
        current_node = player1_ships[k]->head;

        for (int j = 0; j < player1_ships[i]->length; j++)
        {
            fread(&x, sizeof(int), 1, saved_ships);
            fread(&y, sizeof(int), 1, saved_ships);
            fread(&status, sizeof(char), 1, saved_ships);
            append(x, y, player1_ships[k], status);
        }
        // player 2 ships
        fread(player2_ships[k]->dir, sizeof(char), 2, saved_ships);
        fread(&player2_ships[k]->length, sizeof(int), 1, saved_ships);
        current_node = player1_ships[k]->head;

        for (int j = 0; j < player1_ships[i]->length; j++)
        {
            fread(&x, sizeof(int), 1, saved_ships);
            fread(&y, sizeof(int), 1, saved_ships);
            fread(&status, sizeof(char), 1, saved_ships);
            append(x, y, player2_ships[k], status);
        }
    }
    // maps
    fread(map_player1, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);
    fread(map_player2_for_player1, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);
    fread(map_player2, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);
    fread(map_player1_for_player2, sizeof(char), WIDTH * HEIGHT + 1, saved_maps);

    fclose(saved_maps);
    fclose(saved_ships);
}


// this function adjust score of the players
void add_score(int length) {

    if (turn == 1) {

        player1_score += 5 * ((float)largest_length /(float) length) + 1;

    } else {

        player2_score += 5 * ((float)largest_length /(float) length) + 1;

    }
}


// this function is used for the isBot to shoot its bomb
void shoot_automatically(char *map_for_shooter, char *map_of_shooter, char *map_enemy, struct SHIP **ships) {

    struct node *a_node = malloc(sizeof(struct node));

    int x;
    int y;

    srand(time(NULL));

    do {
        x = rand() % WIDTH;
        y = rand() % HEIGHT;

    } while (map_for_shooter[x + WIDTH * y] != ' ');

    if (map_enemy[x + WIDTH * y] == 'w') {

        map_for_shooter[x + WIDTH * y] = 'w';
        map_enemy[x + WIDTH * y] = 'r';

    } else if (map_enemy[x + WIDTH * y] == 'f') {

        a_node = getNode(x, y, ships);
        a_node->status = 'e';

        if (isExploded(getShip(x, y, ships))) {

            struct node *current_node = getShip(x, y, ships)->head;
            struct SHIP *current_ship = getShip(x, y, ships);

            add_score(current_ship->length);

            for(int i = 0 ; i < current_ship->length ; i++) {

                current_node->status = 'c';

                // these codes are for changing the maps blocks status
                if (strcmp(getShip(x, y, ships)->dir, "r") == 0) {

                    map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                    map_enemy[current_node->x + (current_node->y - 1) * WIDTH] = 'r';
                    map_enemy[current_node->x + (current_node->y + 1) * WIDTH] = 'r';

                    map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                    map_for_shooter[current_node->x + (current_node->y - 1) * WIDTH] = 'w';
                    map_for_shooter[current_node->x + (current_node->y + 1) * WIDTH] = 'w';

                }

                if (strcmp(getShip(x, y, ships)->dir, "l") == 0) {

                    map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                    map_enemy[current_node->x + (current_node->y - 1) * WIDTH] = 'r';
                    map_enemy[current_node->x + (current_node->y + 1) * WIDTH] = 'r';

                    map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                    map_for_shooter[current_node->x + (current_node->y - 1) * WIDTH] = 'w';
                    map_for_shooter[current_node->x + (current_node->y + 1) * WIDTH] = 'w';
                }

                if (strcmp(getShip(x, y, ships)->dir, "u") == 0) {

                    map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                    map_enemy[current_node->x + 1 + current_node->y * WIDTH] = 'r';
                    map_enemy[current_node->x - 1 + current_node->y * WIDTH] = 'r';

                    map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                    map_for_shooter[current_node->x + 1 + (current_node->y - 1) * WIDTH] = 'w';
                    map_for_shooter[current_node->x - 1 + (current_node->y + 1) * WIDTH] = 'w';
                }

                if (strcmp(getShip(x, y, ships)->dir, "d") == 0) {

                    map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                    map_enemy[current_node->x + 1 + current_node->y * WIDTH] = 'r';
                    map_enemy[current_node->x - 1 + current_node->y * WIDTH] = 'r';

                    map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                    map_for_shooter[current_node->x + 1 + (current_node->y - 1) * WIDTH] = 'w';
                    map_for_shooter[current_node->x - 1 + (current_node->y + 1) * WIDTH] = 'w';
                }
                current_node = current_node->next;
            }

            // these are for fixing 1 block upper and lower of head and tail problem of not getting changed in code above
            if (strcmp(current_ship->dir, "r") == 0) {

                map_enemy[current_ship->head->x - 1 + WIDTH * (current_ship->head->y + 1)] = 'r';
                map_enemy[current_ship->head->x - 1 + WIDTH * (current_ship->head->y - 1)] = 'r';
                map_enemy[current_ship->head->x - 1 + WIDTH * current_ship->head->y] = 'r';
                map_enemy[current_ship->tail->x + 1 + WIDTH * (current_ship->tail->y - 1)] = 'r';
                map_enemy[current_ship->tail->x + 1 + WIDTH * (current_ship->tail->y + 1)] = 'r';
                map_enemy[current_ship->tail->x + 1 + WIDTH * current_ship->tail->y] = 'r';

                map_for_shooter[current_ship->head->x - 1 + WIDTH * (current_ship->head->y + 1)] = 'w';
                map_for_shooter[current_ship->head->x - 1 + WIDTH * (current_ship->head->y - 1)] = 'w';
                map_for_shooter[current_ship->head->x - 1 + WIDTH * current_ship->head->y] = 'w';
                map_for_shooter[current_ship->tail->x + 1 + WIDTH * (current_ship->tail->y - 1)] = 'w';
                map_for_shooter[current_ship->tail->x + 1 + WIDTH * (current_ship->tail->y + 1)] = 'w';
                map_for_shooter[current_ship->tail->x + 1 + WIDTH * current_ship->tail->y] = 'w';
            }

            if (strcmp(current_ship->dir, "l") == 0) {

                map_enemy[current_ship->tail->x - 1 + WIDTH * (current_ship->head->y + 1)] = 'r';
                map_enemy[current_ship->tail->x - 1 + WIDTH * (current_ship->head->y - 1)] = 'r';
                map_enemy[current_ship->tail->x - 1 + WIDTH * current_ship->head->y] = 'r';
                map_enemy[current_ship->head->x + 1 + WIDTH * (current_ship->tail->y - 1)] = 'r';
                map_enemy[current_ship->head->x + 1 + WIDTH * (current_ship->tail->y + 1)] = 'r';
                map_enemy[current_ship->head->x + 1 + WIDTH * current_ship->tail->y] = 'r';

                map_for_shooter[current_ship->tail->x - 1 + WIDTH * (current_ship->head->y + 1)] = 'w';
                map_for_shooter[current_ship->tail->x - 1 + WIDTH * (current_ship->head->y - 1)] = 'w';
                map_for_shooter[current_ship->tail->x - 1 + WIDTH * current_ship->head->y] = 'w';
                map_for_shooter[current_ship->head->x + 1 + WIDTH * (current_ship->tail->y - 1)] = 'w';
                map_for_shooter[current_ship->head->x + 1 + WIDTH * (current_ship->tail->y + 1)] = 'w';
                map_for_shooter[current_ship->head->x + 1 + WIDTH * current_ship->tail->y] = 'w';
            }

            if (strcmp(current_ship->dir, "u") == 0) {

                map_enemy[current_ship->tail->x - 1 + WIDTH * current_ship->head->y - 1] = 'r';
                map_enemy[current_ship->tail->x + 1 + WIDTH * current_ship->head->y - 1] = 'r';
                map_enemy[current_ship->tail->x + WIDTH * current_ship->head->y - 1] = 'r';
                map_enemy[current_ship->head->x + 1 + WIDTH * current_ship->tail->y + 1] = 'r';
                map_enemy[current_ship->head->x - 1 + WIDTH * current_ship->tail->y + 1] = 'r';
                map_enemy[current_ship->head->x + WIDTH * current_ship->tail->y + 1] = 'r';

                map_for_shooter[current_ship->tail->x - 1 + WIDTH * current_ship->head->y - 1] = 'w';
                map_for_shooter[current_ship->tail->x + 1 + WIDTH * current_ship->head->y - 1] = 'w';
                map_for_shooter[current_ship->tail->x + WIDTH * current_ship->head->y - 1] = 'w';
                map_for_shooter[current_ship->head->x + 1 + WIDTH * current_ship->tail->y + 1] = 'w';
                map_for_shooter[current_ship->head->x - 1 + WIDTH * current_ship->tail->y + 1] = 'w';
                map_for_shooter[current_ship->head->x + WIDTH * current_ship->tail->y + 1] = 'w';

            }

            if (strcmp(current_ship->dir, "u") == 0) {

                map_enemy[current_ship->tail->x - 1 + WIDTH * current_ship->tail->y - 1] = 'r';
                map_enemy[current_ship->tail->x + 1 + WIDTH * current_ship->tail->y - 1] = 'r';
                map_enemy[current_ship->tail->x + WIDTH * current_ship->tail->y - 1] = 'r';
                map_enemy[current_ship->head->x + 1 + WIDTH * current_ship->head->y + 1] = 'r';
                map_enemy[current_ship->head->x - 1 + WIDTH * current_ship->head->y + 1] = 'r';
                map_enemy[current_ship->head->x + WIDTH * current_ship->head->y + 1] = 'r';

                map_for_shooter[current_ship->tail->x - 1 + WIDTH * current_ship->tail->y - 1] = 'w';
                map_for_shooter[current_ship->tail->x + 1 + WIDTH * current_ship->tail->y - 1] = 'w';
                map_for_shooter[current_ship->tail->x + WIDTH * current_ship->tail->y - 1] = 'w';
                map_for_shooter[current_ship->head->x + 1 + WIDTH * current_ship->head->y + 1] = 'w';
                map_for_shooter[current_ship->head->x - 1 + WIDTH * current_ship->head->y + 1] = 'w';
                map_for_shooter[current_ship->head->x + WIDTH * current_ship->head->y + 1] = 'w';

            }

        } else {

            map_enemy[x + WIDTH * y] = 'e';
            map_for_shooter[x + WIDTH * y] = 'e';

            if (turn == 1) player1_score += 1;
            else if (turn == 2) player2_score += 1;

        }
    }
    change_turn();

}


// this function is used for player to shoot the missiles
void shoot(char *map_for_shooter, char *map_of_shooter, char *map_enemy, struct SHIP **ships, int x, int y) {

    struct node *a_node;


    if (map_enemy[x + WIDTH * y] == ' ') {

        map_for_shooter[x + WIDTH * y] = 'w';
        map_enemy[x + WIDTH * y] = 'w';

    } else if (map_enemy[x + WIDTH * y] == 'f') {

        a_node = getNode(x, y, ships);
        a_node->status = 'e';

        if (isExploded(getShip(x, y, ships))) {

            struct node *current_node = getShip(x, y, ships)->head;
            struct SHIP *current_ship = getShip(x, y, ships);

            add_score(current_ship->length);

            for (int i = 0; i < current_ship->length; i++) {

                current_node->status = 'c';

                // these codes are for changing the maps blocks status
                if (strcmp(getShip(x, y, ships)->dir, "r") == 0) {

                    map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                    map_enemy[current_node->x + (current_node->y - 1) * WIDTH] = 'w';
                    map_enemy[current_node->x + (current_node->y + 1) * WIDTH] = 'w';

                    map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                    map_for_shooter[current_node->x + (current_node->y - 1) * WIDTH] = 'w';
                    map_for_shooter[current_node->x + (current_node->y + 1) * WIDTH] = 'w';

                }

                if (strcmp(getShip(x, y, ships)->dir, "l") == 0) {

                    map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                    map_enemy[current_node->x + (current_node->y - 1) * WIDTH] = 'w';
                    map_enemy[current_node->x + (current_node->y + 1) * WIDTH] = 'w';

                    map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                    map_for_shooter[current_node->x + (current_node->y - 1) * WIDTH] = 'w';
                    map_for_shooter[current_node->x + (current_node->y + 1) * WIDTH] = 'w';
                }

                if (strcmp(getShip(x, y, ships)->dir, "u") == 0) {

                    map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                    map_enemy[current_node->x + 1 + current_node->y * WIDTH] = 'w';
                    map_enemy[current_node->x - 1 + current_node->y * WIDTH] = 'w';

                    map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                    map_for_shooter[current_node->x + 1 + (current_node->y - 1) * WIDTH] = 'w';
                    map_for_shooter[current_node->x - 1 + (current_node->y + 1) * WIDTH] = 'w';
                }

                if (strcmp(getShip(x, y, ships)->dir, "d") == 0) {

                    map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                    map_enemy[current_node->x + 1 + current_node->y * WIDTH] = 'w';
                    map_enemy[current_node->x - 1 + current_node->y * WIDTH] = 'w';

                    map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                    map_for_shooter[current_node->x + 1 + (current_node->y - 1) * WIDTH] = 'w';
                    map_for_shooter[current_node->x - 1 + (current_node->y + 1) * WIDTH] = 'w';
                }
                current_node = current_node->next;
            }

            // these are for fixing 1 block upper and lower of head and tail problem of not getting changed in code above
            if (strcmp(current_ship->dir, "r") == 0) {

                map_enemy[current_ship->head->x - 1 + WIDTH * (current_ship->head->y + 1)] = 'w';
                map_enemy[current_ship->head->x - 1 + WIDTH * (current_ship->head->y - 1)] = 'w';
                map_enemy[current_ship->head->x - 1 + WIDTH * current_ship->head->y] = 'w';
                map_enemy[current_ship->tail->x + 1 + WIDTH * (current_ship->tail->y - 1)] = 'w';
                map_enemy[current_ship->tail->x + 1 + WIDTH * (current_ship->tail->y + 1)] = 'w';
                map_enemy[current_ship->tail->x + 1 + WIDTH * current_ship->tail->y] = 'w';

                map_for_shooter[current_ship->head->x - 1 + WIDTH * (current_ship->head->y + 1)] = 'w';
                map_for_shooter[current_ship->head->x - 1 + WIDTH * (current_ship->head->y - 1)] = 'w';
                map_for_shooter[current_ship->head->x - 1 + WIDTH * current_ship->head->y] = 'w';
                map_for_shooter[current_ship->tail->x + 1 + WIDTH * (current_ship->tail->y - 1)] = 'w';
                map_for_shooter[current_ship->tail->x + 1 + WIDTH * (current_ship->tail->y + 1)] = 'w';
                map_for_shooter[current_ship->tail->x + 1 + WIDTH * current_ship->tail->y] = 'w';
            }

            if (strcmp(current_ship->dir, "l") == 0) {

                map_enemy[current_ship->tail->x - 1 + WIDTH * (current_ship->head->y + 1)] = 'w';
                map_enemy[current_ship->tail->x - 1 + WIDTH * (current_ship->head->y - 1)] = 'w';
                map_enemy[current_ship->tail->x - 1 + WIDTH * current_ship->head->y] = 'w';
                map_enemy[current_ship->head->x + 1 + WIDTH * (current_ship->tail->y - 1)] = 'w';
                map_enemy[current_ship->head->x + 1 + WIDTH * (current_ship->tail->y + 1)] = 'w';
                map_enemy[current_ship->head->x + 1 + WIDTH * current_ship->tail->y] = 'w';

                map_for_shooter[current_ship->tail->x - 1 + WIDTH * (current_ship->head->y + 1)] = 'w';
                map_for_shooter[current_ship->tail->x - 1 + WIDTH * (current_ship->head->y - 1)] = 'w';
                map_for_shooter[current_ship->tail->x - 1 + WIDTH * current_ship->head->y] = 'w';
                map_for_shooter[current_ship->head->x + 1 + WIDTH * (current_ship->tail->y - 1)] = 'w';
                map_for_shooter[current_ship->head->x + 1 + WIDTH * (current_ship->tail->y + 1)] = 'w';
                map_for_shooter[current_ship->head->x + 1 + WIDTH * current_ship->tail->y] = 'w';
            }

            if (strcmp(current_ship->dir, "u") == 0) {

                map_enemy[current_ship->tail->x - 1 + WIDTH * current_ship->head->y - 1] = 'w';
                map_enemy[current_ship->tail->x + 1 + WIDTH * current_ship->head->y - 1] = 'w';
                map_enemy[current_ship->tail->x + WIDTH * current_ship->head->y - 1] = 'w';
                map_enemy[current_ship->head->x + 1 + WIDTH * current_ship->tail->y + 1] = 'w';
                map_enemy[current_ship->head->x - 1 + WIDTH * current_ship->tail->y + 1] = 'w';
                map_enemy[current_ship->head->x + WIDTH * current_ship->tail->y + 1] = 'w';

                map_for_shooter[current_ship->tail->x - 1 + WIDTH * current_ship->head->y - 1] = 'w';
                map_for_shooter[current_ship->tail->x + 1 + WIDTH * current_ship->head->y - 1] = 'w';
                map_for_shooter[current_ship->tail->x + WIDTH * current_ship->head->y - 1] = 'w';
                map_for_shooter[current_ship->head->x + 1 + WIDTH * current_ship->tail->y + 1] = 'w';
                map_for_shooter[current_ship->head->x - 1 + WIDTH * current_ship->tail->y + 1] = 'w';
                map_for_shooter[current_ship->head->x + WIDTH * current_ship->tail->y + 1] = 'w';

            }

            if (strcmp(current_ship->dir, "u") == 0) {

                map_enemy[current_ship->tail->x - 1 + WIDTH * current_ship->tail->y - 1] = 'w';
                map_enemy[current_ship->tail->x + 1 + WIDTH * current_ship->tail->y - 1] = 'w';
                map_enemy[current_ship->tail->x + WIDTH * current_ship->tail->y - 1] = 'w';
                map_enemy[current_ship->head->x + 1 + WIDTH * current_ship->head->y + 1] = 'w';
                map_enemy[current_ship->head->x - 1 + WIDTH * current_ship->head->y + 1] = 'w';
                map_enemy[current_ship->head->x + WIDTH * current_ship->head->y + 1] = 'w';

                map_for_shooter[current_ship->tail->x - 1 + WIDTH * current_ship->tail->y - 1] = 'w';
                map_for_shooter[current_ship->tail->x + 1 + WIDTH * current_ship->tail->y - 1] = 'w';
                map_for_shooter[current_ship->tail->x + WIDTH * current_ship->tail->y - 1] = 'w';
                map_for_shooter[current_ship->head->x + 1 + WIDTH * current_ship->head->y + 1] = 'w';
                map_for_shooter[current_ship->head->x - 1 + WIDTH * current_ship->head->y + 1] = 'w';
                map_for_shooter[current_ship->head->x + WIDTH * current_ship->head->y + 1] = 'w';

            }

        } else {

            map_enemy[x + WIDTH * y] = 'e';
            map_for_shooter[x + WIDTH * y] = 'e';

            if (turn == 1){

                player1_score += 1;

            }

            else if (turn == 2){

                player2_score += 1;

            }

        }
    }
}


// this function is used for player to shoot its bomb
void shoot_manually(char *map_for_shooter, char *map_of_shooter, char *map_enemy, struct SHIP **ships) {

    // showing the board of enemy
    printf("                 ENEMY\n");
    draw_board(map_for_shooter);
    printf("                 YOU\n");
    draw_board(map_of_shooter);

    struct node *a_node;

    int x;
    int y;

    char *ans = malloc(sizeof(char) * 2);
    char *dir = malloc(sizeof(char) * 2);


    printf("\nPlayer %d Turn\n\n", turn);

    ans = "n";
    // checking if the player wants to shoot the missile.
    if (turn == 1 && player1_score >= 100 && missile_is_used_1 == false) {

        free(ans);
        ans = malloc(sizeof(char) * 2);

        printf("do you want to shoot your missile?(Y/N):");
        scanf("%s", ans);

    } else if (turn == 2 && player2_score >= 100 && missile_is_used_2 == false) {

        free(ans);
        ans = malloc(sizeof(char) * 2);

        printf("do you want to shoot your missile?(Y/N):");
        scanf("%s", ans);
    }

    // scanning the coordination of the place that the player wants to shoot to
    do {
        printf("choose a block that is not revealed yet!\n");
        printf("enter x between %d and 0:", WIDTH - 1);
        scanf("%d", &x);
        printf("enter y between %d and 0:\n", HEIGHT - 1);
        scanf("%d", &y);

    } while ((map_for_shooter[x + WIDTH * y] != ' ' || x < 0 || y < 0 || x > WIDTH - 1 || y > HEIGHT - 1) && strcmpi(ans, "n") == 0 );

    // doing the shooting process
    if (strcmp(ans, "n") == 0 || strcmp(ans, "N") == 0) {

        if (map_enemy[x + WIDTH * y] == ' ') {
            map_for_shooter[x + WIDTH * y] = 'w';
            map_enemy[x + WIDTH * y] = 'w';

        } else if (map_enemy[x + WIDTH * y] == 'f') {

            a_node = getNode(x, y, ships);
            a_node->status = 'e';

            if (isExploded(getShip(x, y, ships))) {

                struct node *current_node = getShip(x, y, ships)->head;
                struct SHIP *current_ship = getShip(x, y, ships);

                add_score(current_ship->length);

                for (int i = 0 ; i < current_ship->length ; i++) {

                    current_node->status = 'c';

                    // these codes are for changing the maps blocks status
                    if (strcmp(getShip(x, y, ships)->dir, "r") == 0) {

                        map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                        if (isIn(current_node->x, current_node->y - 1))
                            map_enemy[current_node->x + (current_node->y - 1) * WIDTH] = 'w';
                        if (isIn(current_node->x, current_node->y + 1))
                            map_enemy[current_node->x + (current_node->y + 1) * WIDTH] = 'w';

                        map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                        if (isIn(current_node->x, current_node->y - 1))
                            map_for_shooter[current_node->x + (current_node->y - 1) * WIDTH] = 'w';
                        if (isIn(current_node->x, current_node->y + 1))
                            map_for_shooter[current_node->x + (current_node->y + 1) * WIDTH] = 'w';

                    }

                    if (strcmp(getShip(x, y, ships)->dir, "l") == 0) {

                        map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                        if (isIn(current_node->x, current_node->y - 1))
                            map_enemy[current_node->x + (current_node->y - 1) * WIDTH] = 'w';
                        if (isIn(current_node->x, current_node->y + 1))
                            map_enemy[current_node->x + (current_node->y + 1) * WIDTH] = 'w';

                        map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                        if (isIn(current_node->x, current_node->y - 1))
                            map_for_shooter[current_node->x + (current_node->y - 1) * WIDTH] = 'w';
                        if (isIn(current_node->x, current_node->y + 1))
                            map_for_shooter[current_node->x + (current_node->y + 1) * WIDTH] = 'w';
                    }
                    if (strcmp(getShip(x, y, ships)->dir, "u") == 0) {

                        map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                        if (isIn(current_node->x + 1, current_node->y))
                            map_enemy[current_node->x + 1 + current_node->y * WIDTH] = 'w';
                        if (isIn(current_node->x - 1, current_node->y))
                            map_enemy[current_node->x - 1 + current_node->y * WIDTH] = 'w';

                        map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                        if (isIn(current_node->x + 1, current_node->y))
                            map_for_shooter[current_node->x + 1 + current_node->y * WIDTH] = 'w';
                        if (isIn(current_node->x - 1, current_node->y))
                            map_for_shooter[current_node->x - 1 + current_node->y * WIDTH] = 'w';
                    }

                    if (strcmp(getShip(x, y, ships)->dir, "d") == 0) {

                        map_enemy[current_node->x + current_node->y * WIDTH] = 'c';
                        if (isIn(current_node->x + 1, current_node->y))
                            map_enemy[current_node->x + 1 + current_node->y * WIDTH] = 'w';
                        if (isIn(current_node->x - 1, current_node->y))
                            map_enemy[current_node->x - 1 + current_node->y * WIDTH] = 'w';

                        map_for_shooter[current_node->x + current_node->y * WIDTH] = 'c';
                        if (isIn(current_node->x + 1, current_node->y))
                            map_for_shooter[current_node->x + 1 + current_node->y * WIDTH] = 'w';
                        if (isIn(current_node->x - 1, current_node->y))
                            map_for_shooter[current_node->x - 1 + current_node->y * WIDTH] = 'w';
                    }

                    current_node = current_node->next;

                }

                int xt = current_ship->tail->x;
                int yt = current_ship->tail->y;

                int xh = current_ship->head->x;
                int yh = current_ship->head->y;

                // these are for fixing 1 block upper and lower of head and tail problem of not getting changed in code above

                if (strcmp(current_ship->dir, "r") == 0) {

                    if (isIn(xh - 1, yh + 1)) map_enemy[xh - 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xh - 1, yh - 1)) map_enemy[xh - 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xh - 1, yh)) map_enemy[xh - 1 + WIDTH * yh] = 'w';
                    if (isIn(xt + 1, yh - 1)) map_enemy[xt + 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xt + 1, yh + 1)) map_enemy[xt + 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xt + 1, yh)) map_enemy[xt + 1 + WIDTH * yh] = 'w';

                    if (isIn(xh - 1, yh + 1)) map_for_shooter[xh - 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xh - 1, yh - 1)) map_for_shooter[xh - 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xh - 1, yh)) map_for_shooter[xh - 1 + WIDTH * yh] = 'w';
                    if (isIn(xt + 1, yh - 1)) map_for_shooter[xt + 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xt + 1, yh + 1)) map_for_shooter[xt + 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xt + 1, yh)) map_for_shooter[xt + 1 + WIDTH * yh] = 'w';

                }

                if (strcmp(current_ship->dir, "l") == 0) {

                    if (isIn(xt - 1, yh + 1)) map_enemy[xt - 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xt - 1, yh - 1)) map_enemy[xt - 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xt - 1, yh)) map_enemy[xt - 1 + WIDTH * yh] = 'w';
                    if (isIn(xh + 1, yh - 1)) map_enemy[xh + 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xh + 1, yh + 1)) map_enemy[xh + 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xh + 1, yh)) map_enemy[xh + 1 + WIDTH * yh] = 'w';

                    if (isIn(xt - 1, yh + 1)) map_for_shooter[xt - 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xt - 1, yh - 1)) map_for_shooter[xt - 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xt - 1, yh)) map_for_shooter[xt - 1 + WIDTH * yh] = 'w';
                    if (isIn(xh + 1, yh - 1)) map_for_shooter[xh + 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xh + 1, yh + 1)) map_for_shooter[xh + 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xh + 1, yh)) map_for_shooter[xh + 1 + WIDTH * yh] = 'w';

                }

                if (strcmp(current_ship->dir, "u") == 0) {

                    if (isIn(xh - 1, yh + 1)) map_enemy[xh - 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xh + 1, yh + 1)) map_enemy[xh + 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xh, yh + 1)) map_enemy[xh + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xt + 1, yt - 1)) map_enemy[xt + 1 + WIDTH * (yt - 1)] = 'w';
                    if (isIn(xt - 1, yt - 1)) map_enemy[xt - 1 + WIDTH * (yt - 1)] = 'w';
                    if (isIn(xt, yt - 1)) map_enemy[xt + WIDTH * (yt - 1)] = 'w';

                    if (isIn(xh - 1, yh + 1)) map_for_shooter[xh - 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xh + 1, yh + 1)) map_for_shooter[xh + 1 + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xh, yh + 1)) map_for_shooter[xh + WIDTH * (yh + 1)] = 'w';
                    if (isIn(xt + 1, yt - 1)) map_for_shooter[xt + 1 + WIDTH * (yt - 1)] = 'w';
                    if (isIn(xt - 1, yt - 1)) map_for_shooter[xt - 1 + WIDTH * (yt - 1)] = 'w';
                    if (isIn(xt, yt - 1)) map_for_shooter[xt + WIDTH * (yt - 1)] = 'w';

                }

                if (strcmp(current_ship->dir, "d") == 0) {

                    if (isIn(xh - 1, yt + 1)) map_enemy[xh - 1 + WIDTH * (yt + 1)] = 'w';
                    if (isIn(xh + 1, yt + 1)) map_enemy[xh + 1 + WIDTH * (yt + 1)] = 'w';
                    if (isIn(xh, yt + 1)) map_enemy[xh + WIDTH * (yt + 1)] = 'w';
                    if (isIn(xt + 1, yh - 1)) map_enemy[xt + 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xt - 1, yh - 1)) map_enemy[xt - 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xt, yh - 1)) map_enemy[xt + WIDTH * (yh - 1)] = 'w';

                    if (isIn(xh - 1, yt + 1)) map_for_shooter[xh - 1 + WIDTH * (yt + 1)] = 'w';
                    if (isIn(xh + 1, yt + 1)) map_for_shooter[xh + 1 + WIDTH * (yt + 1)] = 'w';
                    if (isIn(xh, yt + 1)) map_for_shooter[xh + WIDTH * (yt + 1)] = 'w';
                    if (isIn(xt + 1, yh - 1)) map_for_shooter[xt + 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xt - 1, yh - 1)) map_for_shooter[xt - 1 + WIDTH * (yh - 1)] = 'w';
                    if (isIn(xt, yh - 1)) map_for_shooter[xt + WIDTH * (yh - 1)] = 'w';
                }

            } else {

                map_enemy[x + WIDTH * y] = 'e';
                map_for_shooter[x + WIDTH * y] = 'e';

                if (turn == 1)player1_score++;
                else if (turn == 2)player2_score++;
            }
        }
    }

    else if (strcmp(ans, "y") == 0 || strcmp(ans, "Y") == 0) {

        printf("add the direction ( r, l, u, d )");
        scanf("%s", dir);

        if (strcmp(dir, "d") == 0 || strcmp(dir, "u") == 0) {

            for (int i = 0; i < HEIGHT; i++) {

                shoot(map_for_shooter, map_of_shooter, map_enemy, ships, x, i);
            }
        }

        if (strcmp(dir, "r") == 0 || strcmp(dir, "l") == 0) {

            for (int i = 0; i < WIDTH; i++) {

                shoot(map_for_shooter, map_of_shooter, map_enemy, ships, i, y);
            }
        }

        // paying the price
        if (turn == 1) {
            player1_score -= 100;
            missile_is_used_1 = true;
        }
        else if (turn == 2) {
            player2_score -= 100;
            missile_is_used_2 = true;
        }
    }
    change_turn();
}


// this function check if the program should run again by checking if ech of players have lost all of his/her ships.
int ifRun(struct SHIP **player1_ships, struct SHIP **player2_ships) {

    for (int i = 0 ; i < total_number_ships ; i++) {

        if (isExploded(player1_ships[i]) == false) break;
        if (i == total_number_ships - 1) return false;

    }

    for (int i = 0 ; i < total_number_ships ; i++) {

        if (isExploded(player2_ships[i]) == false) break;
        if (i == total_number_ships - 1) return false;

    }

    return true;

}


// this function show the sample for the maps to the player while choosing the theme
void show_themes(){

    char separator[] = {'a' + 79, 'a' + 80, 'a' + 81, '|', '$', '*'};
    char *map_name[] = {"Shady1", "Shady2", "Shady3", "Line", "Money", "Star"};
    int x = 0 , y = 0 ;

    for( int i = 0 ; i < 6 ; i++) {
        printf("                 %d.%s\n\n", i + 1, map_name[i]);
        while (y < 2 * HEIGHT - 1) {
            if (y % 2 != 0) {
                if (x < WIDTH - 1) {
                    if (i == 3) {
                        printf("---%c", separator[i]);
                        x++;
                    }
                    else {
                        printf("%c%c%c%c", separator[i], separator[i], separator[i], separator[i]);
                        x++;
                    }
                } else {
                    if (i == 3) {
                        printf("---\n");
                        x = 0;
                        y++;
                        continue;
                    }
                    else {
                        printf("%c%c%c\n", separator[i],separator[i],separator[i]);
                        x = 0;
                        y++;
                        continue;
                    }
                }
            } else {
                if (x < WIDTH - 1) {
                    printf(" w %c", separator[i]);
                    x++;
                } else {
                    printf(" w \n");
                    y++;
                    x = 0;
                }

            }
        }
        x = 0 ;
        y = 0 ;
        printf("\n\n");
    }
}


// this function show the main menu of the game.
void show_menu(struct SHIP **player1_ships, char *map_player1, struct SHIP **player2_ships, char *map_player2, char *map_player1_for_player2, char *map_player2_for_player1) {

    // initializing
    int choice;
    int k = 0;

    char *new_user1 = malloc(sizeof(char) * 100);
    char *new_user2 = malloc(sizeof(char) * 100);


    FILE *users_file;

    users_file = fopen("users.txt", "r");
    while (!(feof(users_file))) {

        fscanf(users_file, "%s", List_Users[k].name);
        fscanf(users_file, "%d", &List_Users[k].score);
        k++;
    }
    fclose(users_file);
    number_users = k;
    k = 0;

    while (choice != 6) {

        // showing the main menu
        printf("\n"
               "1. Play with a Friend\n"
               "2. Play with Bot\n"
               "3. load game\n"
               "4. Settings\n"
               "5. Score Board\n"
               "6. Exit\n");
        do {
            scanf("%d", &choice);
        }while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6);


        //--------------------------------------------------------
        //--------------------------------------------------------

        if (choice == 1) {

            //-----------------------------------------------------------------------
            //-----------------------------------------------------------------------
            // first player choose his user
            printf("\nFirst Player\n");

            printf("choose user\n");

            printf("            1. choose from available users\n"
                   "            2. new user\n");

            // choosing the user info
            scanf("%d", &choice);

            // new user
            if (choice == 2) {

                printf("username:");
                scanf("%s", new_user1);
                int i = 0;

                // check if the user is already in use or not
                while (i < number_users) {

                    if (strcmpi(new_user1, List_Users[i].name) == 0) {
                        printf("%s is already in use\n", new_user1);
                        printf("username: ");
                        scanf("%s", new_user1);

                        i = 0;
                    }
                    i++;
                }
                // affecting the new users to games information
                number_users++;
                strcpy(List_Users[number_users - 1].name, new_user1);
                List_Users[number_users - 1].score = 0;

                place_player1_List = number_users - 1;
            }

                // choose from available users
            else if (choice == 1) {
                printf("           Users List\n");

                for (int j = 0; j < number_users; j++) {
                    printf("%d. %s %10d\n", j + 1, List_Users[j].name, List_Users[j].score);
                }

                printf("enter your user number:");
                do {
                    scanf("%d", &choice);
                } while (choice > number_users || choice < 1);
                place_player1_List = choice - 1;

            }
            //--------------------------------------------------------
            //--------------------------------------------------------
            // first player put his ships
            // showing the menu

            printf("put ships\n");
            printf("           1.Auto\n"
                   "           2.Manual\n");

            do {

                scanf("%d", &choice);

            } while (choice != 1 && choice != 2);

            if (choice == 1) {

                map_player_ships_automatically(player1_ships, map_player1);

            } else if (choice == 2) {

                map_player_ships_manually(player1_ships, map_player1);

            }
            //-----------------------------------------------------------------------
            //-----------------------------------------------------------------------
            // second player choose his user
            // showing the menu
            printf("\nSecond Player\n");
            printf("choose user\n");
            printf("            1. choose from available users\n"
                   "            2. new user\n");
            // choosing the user information
            do {

                scanf("%d", &choice);

            } while (choice != 1 && choice != 2);
            // new user
            if (choice == 2) {

                printf("username:");
                scanf("%s", new_user2);
                int i = 0;

                // check if the user is already in use or not
                while (i < number_users) {

                    if (strcmpi(new_user2, List_Users[i].name) == 0) {

                        printf("%s is already in use\n", new_user2);
                        printf("username: ");
                        scanf("%s", new_user2);
                        i = 0;
                    }
                    i++;
                }
                // affecting the new users to the games information
                number_users++;
                strcpy(List_Users[number_users - 1].name, new_user2);
                List_Users[number_users - 1].score = 0;

                place_player2_List = number_users - 1;
            }

                // choose from available users
            else if (choice == 1) {
                printf("           Users List\n");

                for (int j = 0; j < number_users; j++) {
                    printf("%d. %s %10d\n", j + 1, List_Users[j].name, List_Users[j].score);
                }

                printf("enter your user number:");
                do {
                    scanf("%d", &choice);
                    if (choice == place_player1_List)printf("user is already in use\n");
                } while (choice - 1== place_player1_List || choice > number_users || choice < 1);
                place_player2_List = choice - 1;
            }

            //-----------------------------------------------------------------------
            //-----------------------------------------------------------------------
            // second player put his ships
            // showing the menu


            printf("put ships\n");
            printf("           1.Auto\n"
                   "           2.Manual\n");

            do {

                scanf("%d", &choice);

            } while (choice != 1 && choice != 2);

            if (choice == 1) {

                map_player_ships_automatically(player2_ships, map_player2);

            } else if (choice == 2) {

                map_player_ships_manually(player2_ships, map_player2);

            }

            // showing the main menu
            printf("\n"
                   "4. Settings\n"
                   "5. Score Board\n"
                   "6. Exit\n");

            do {

                scanf("%d", &choice);

            } while (choice != 4 && choice != 5 && choice != 6);

        }

        //--------------------------------------------------------
        //--------------------------------------------------------

        if (choice == 2) {

            isBot = true;
            //-----------------------------------------------------------------------
            // first player choose his user
            printf("\nFirst Player\n");
            printf("choose user\n");
            printf("            1. choose from available users\n"
                   "            2. new user\n");

            // choosing the user info
            do {
                scanf("%d", &choice);
            } while (choice != 1 && choice != 2);

            // new user
            if (choice == 2) {

                printf("username:");
                scanf("%s", new_user2);
                int i = 0;
                // check if the user is already in use or not
                while (i < number_users) {

                    if (strcmpi(new_user2, List_Users[i].name) == 0) {

                        printf("%s is already in use\n", new_user2);
                        printf("username: ");
                        scanf("%s", new_user2);
                        i = 0;
                    }
                    i++;
                }

                // affecting the new users to games information
                number_users++;
                strcpy(List_Users[number_users - 1].name, new_user2);
                List_Users[number_users - 1].score = 0;

                place_player1_List = number_users - 1;

            }
            // choose from available users
            else if (choice == 1) {
                printf("           Users List\n");

                for (int j = 0; j < number_users; j++) {
                    printf("%d. %s %10d\n", j + 1, List_Users[j].name, List_Users[j].score);
                }

                printf("enter your user number:");
                do {
                    scanf("%d", &choice);
                } while (choice > number_users || choice < 1);
                place_player1_List = choice - 1;
            }
            // first player put his ships
            // showing the menu
            printf("\n"
                   "1. put ships\n"
                   "2. load\n");

            do {
                scanf("%d", &choice);
            } while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6);

            if (choice == 1) {
                printf("put ships\n");
                printf("           1.Auto\n"
                       "           2.Manual\n");
                // putting players ships
                do {
                    scanf("%d", &choice);
                } while (choice != 1 && choice != 2);

                if (choice == 1) {
                    map_player_ships_automatically(player1_ships, map_player1);
                } else if (choice == 2) {
                    map_player_ships_manually(player1_ships, map_player1);
                }
                // put the bots ships
                map_player_ships_automatically(player2_ships, map_player2);

            } else if (choice == 2) {
                load(map_player1, map_player2, player1_ships, map_player2_for_player1, map_player1_for_player2,
                     player2_ships);

            }
            //-----------------------------------------------------------------------
            //-----------------------------------------------------------------------
            // showing the menu again
            printf("\n"
                   "4. Settings\n"
                   "5. Score Board\n"
                   "6. Exit\n");

            do {
                scanf("%d", &choice);
            } while (choice != 4 && choice != 5 && choice != 6);

        }

        //--------------------------------------------------------
        //--------------------------------------------------------

        if (choice == 3) {

            if (number_saved_games == 0) {
                printf("there is no saved game in memory!\n");
                continue;
            }
            // putting ships
            //----------------------------------------------------------------------
            //----------------------------------------------------------------------
            load(map_player1, map_player2, player1_ships, map_player2_for_player1, map_player1_for_player2,
                 player2_ships);
            //-----------------------------------------------------------------------
            //-----------------------------------------------------------------------
            // first player choose his user
            printf("\nFirst Player\n");

            printf("choose user\n");
            printf("            1. choose from available users\n"
                   "            2. new user\n");

            // choosing the user info
            scanf("%d", &choice);

            // new user
            if (choice == 2) {

                printf("username:");

                scanf("%s", new_user1);
                int i = 0;

                // check if the user is already in use or not
                while (i < number_users) {

                    if (strcmpi(new_user1, List_Users[i].name) == 0) {

                        printf("%s is already in use\n", new_user1);
                        printf("username: ");
                        scanf("%s", new_user1);

                        i = 0;
                    }
                    i++;
                }

                // affecting the new users to games information
                number_users++;
                strcpy(List_Users[number_users - 1].name, new_user1);
                List_Users[number_users - 1].score = 0;

                place_player1_List = number_users - 1;

            }

                // choose from available users
            else if (choice == 1) {

                users_file = fopen("users.txt", "r");
                printf("           Users List\n");

                for (int j = 0; j < number_users; j++) {
                    printf("%d. %s %10d\n", j + 1, List_Users[j].name, List_Users[j].score);
                }

                printf("enter your user number:");
                do {
                    scanf("%d", &choice);
                    if (choice == place_player1_List)printf("user is already in use\n");
                } while (choice > number_users || choice < 1);
                place_player1_List = choice - 1;
            }

            //-----------------------------------------------------------------------
            //-----------------------------------------------------------------------
            // second player choose his user
            //-----------------------------------------------------------------------
            //-----------------------------------------------------------------------
            // second player choose his user
            // showing the menu

            printf("\nSecond Player\n");

            printf("choose user\n");

            printf("            1. choose from available users\n"
                   "            2. new user\n");

            // choosing the user information

            do {

                scanf("%d", &choice);

            } while (choice != 1 && choice != 2);

            // new user
            if (choice == 2) {

                printf("username:");

                scanf("%s", new_user2);
                int i = 0;

                // check if the user is already in use or not
                while (i < number_users) {

                    if (strcmpi(new_user2, List_Users[i].name) == 0) {

                        printf("%s is already in use\n", new_user2);
                        printf("username: ");
                        scanf("%s", new_user2);


                        i = 0;
                    }
                    i++;
                }

                // affecting the new users to the games information
                number_users++;
                strcpy(List_Users[number_users - 1].name, new_user2);
                List_Users[number_users - 1].score = 0;

                place_player2_List = number_users - 1;

            }

                // choose from available users
            else if (choice == 1) {

                users_file = fopen("users.txt", "r");

                printf("           Users List\n");

                for (int j = 0; j < number_users; j++) {

                    printf("%d. %s %10d\n", j + 1, List_Users[j].name, List_Users[j].score);
                }

                printf("enter your user number");
                do {
                    scanf("%d", &choice);
                    place_player2_List = choice - 1;
                } while (place_player1_List == place_player2_List || place_player2_List > number_users || place_player2_List < 1);

            }
            //------------------------------------------------------
            //------------------------------------------------------
            // showing the main menu
            printf("\n"
                   "1. Play with a Friend\n"
                   "2. Play with Bot\n"
                   "3. load game\n"
                   "4. Settings\n"
                   "5. Score Board\n"
                   "6. Exit\n");

            do {

                scanf("%d", &choice);

            } while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6);


        }

        //--------------------------------------------------------
        //--------------------------------------------------------

        if (choice == 4) {

            printf("\n"
                   "1.Ships\n"
                   "2.Map Size\n"
                   "3.Theme\n"
                   "4.return\n");

            do {
                scanf("%d", &choice);
            } while (choice < 1 || choice > 4);

            while (choice != 4) {

                if (choice == 1) {
                    printf("\nchoose the number of each ship");

                    printf("\nGreenBay(HP = 1):");
                    scanf("%d", &count_GreenBay);

                    printf("\nFreedom(HP = 2):");
                    scanf("%d", &count_Freedom);

                    printf("\nPioneer(HP = 3):");
                    scanf("%d", &count_Pioneer);

                    printf("\nVellaGulf(HP = 4):");
                    scanf("%d", &count_VellaGulf);

                    printf("\nIndependence(HP = 5):");
                    scanf("%d", &count_Independence);

                    printf("\nFrragut(HP = 6):");
                    scanf("%d", &count_Frragut);

                    printf("\nVirginia(HP = 7):");
                    scanf("%d", &count_Virginia);

                    printf("\nGeorgeWashington(HP = 10):");
                    scanf("%d", &count_GeorgeWashington);

                    printf("\n"
                           "1.Ships\n"
                           "2.Map Size\n"
                           "3.Theme\n"
                           "4.return\n");
                }

                if (choice == 2) {

                    printf("WIDTH:");
                    scanf("%d", &WIDTH);

                    printf("HEIGHT:");
                    scanf("%d", &HEIGHT);

                    free(map_player1);
                    free(map_player2);
                    free(map_player1_for_player2);
                    free(map_player2_for_player1);
                    map_player1 = malloc(sizeof(char) * (WIDTH * HEIGHT + 1));
                    map_player2 = malloc(sizeof(char) * (WIDTH * HEIGHT + 1));
                    map_player1_for_player2 = malloc(sizeof(char) * (WIDTH * HEIGHT + 1));
                    map_player2_for_player1 = malloc(sizeof(char) * (WIDTH * HEIGHT + 1));

                    printf("\n"
                           "1.Ships\n"
                           "2.Map Size\n"
                           "3.Theme\n"
                           "4.return\n");

                }

                if (choice == 3) {

                    show_themes();

                    // choosing the theme
                    printf("choose your theme:");
                    do {

                        scanf("%d", &choice);

                    } while (choice > 6 || choice < 1);
                    //
                    separator_code = choice;

                    printf("\n"
                           "1.Ships\n"
                           "2.Map Size\n"
                           "3.Theme\n"
                           "4.return\n");

                }

                scanf("%d", &choice);

            }

            //showing menu
            printf("\n"
                   "1. Play with a Friend\n"
                   "2. Play with Bot\n"
                   "3. load game\n"
                   "4. Settings\n"
                   "5. Score Board\n"
                   "6. Exit\n");

            do {

                scanf("%d", &choice);

            }while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6);

        }

        //--------------------------------------------------------
        //--------------------------------------------------------

        if (choice == 5) {

            sort_users();
            for (int i = 0; i < number_users; i++) {

                printf("%d. %s %4d\n", i, List_Users[i].name, List_Users[i].score);
            }

            // showing menu
            printf("\n"
                   "1. Play with a Friend\n"
                   "2. Play with Bot\n"
                   "3. load game\n"
                   "4. Settings\n"
                   "5. Score Board\n"
                   "6. Exit\n");

            do {
                scanf("%d", &choice);
            } while (choice != 4 && choice != 5 && choice != 6 && choice != 1 && choice != 2 && choice != 3);

        }
        //--------------------------------------------------------
        //--------------------------------------------------------
    }
}


// this function runs the game
void Main(void) {

    // games initial options
    char *junk = malloc(sizeof(char) * 100);
    int choice = 0;
    int x;
    int y;

//    FILE* PLAY_BACK = fopen("RECORD.txt", "w");
//    fclose(PLAY_BACK);

    srand(time(NULL));
    FILE *file = fopen("data.txt", "r");
    fscanf(file, "%d", &number_saved_games);
    fclose(file);
    //--------------------- must add code for the WIDTH and HEIGHT input
    //-----------------initializing ships array
    char *map_player1 = malloc(sizeof(char) * (WIDTH * HEIGHT + 1));
    char *map_player2 = malloc(sizeof(char) * (WIDTH * HEIGHT + 1));

    char *map_player1_for_player2 = malloc(sizeof(char) * (WIDTH * HEIGHT + 1));
    char *map_player2_for_player1 = malloc(sizeof(char) * (WIDTH * HEIGHT + 1));


    for (int i = 0; i < WIDTH * HEIGHT; i++) {

        map_player1[i] = ' ';
        map_player2[i] = ' ';

        map_player1_for_player2[i] = ' ';
        map_player2_for_player1[i] = ' ';

    }


    struct SHIP *player2_ships[10];
    struct SHIP *player1_ships[10];

    for (int i = 0; i < total_number_ships; i++) {

        player2_ships[i] = malloc(sizeof(struct SHIP));
        player2_ships[i]->head = NULL;
        player2_ships[i]->tail = NULL;

        player1_ships[i] = malloc(sizeof(struct SHIP));
        player1_ships[i]->head = NULL;
        player1_ships[i]->tail = NULL;
    }

    player1_ships[0]->length = 5;

    player1_ships[1]->length = 3;
    player1_ships[2]->length = 3;

    player1_ships[3]->length = 2;
    player1_ships[4]->length = 2;
    player1_ships[5]->length = 2;

    player1_ships[6]->length = 1;
    player1_ships[7]->length = 1;
    player1_ships[8]->length = 1;
    player1_ships[9]->length = 1;


    player2_ships[0]->length = 5;

    player2_ships[1]->length = 3;
    player2_ships[2]->length = 3;

    player2_ships[3]->length = 2;
    player2_ships[4]->length = 2;
    player2_ships[5]->length = 2;

    player2_ships[6]->length = 1;
    player2_ships[7]->length = 1;
    player2_ships[8]->length = 1;
    player2_ships[9]->length = 1;


    char *recorder = malloc(sizeof(char) * 100000);
    show_menu(player1_ships, map_player1, player2_ships, map_player2, map_player1_for_player2, map_player2_for_player1);

    //start the game
    while (run) {

        // first of all we check that if we should continue or not
        run = ifRun(player1_ships, player2_ships);

        if (run) {

            // recording the game
            record_game(map_player1, map_player2);

            shoot_manually(map_player2_for_player1, map_player1, map_player2, player2_ships);
            save_users_info();
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

            if (isBot == true){
                do {
                    x = rand() % WIDTH;
                    y = rand() % HEIGHT;
                } while (map_player1_for_player2[x + WIDTH * y] != ' ');
                shoot(map_player1_for_player2, map_player2, map_player1, player1_ships, x, y);
            }


            else {shoot_manually(map_player1_for_player2, map_player2, map_player1, player1_ships);}
            save_users_info();
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

        }

        printf("save game(1 : yes /0 : no):");
        scanf("%d", &choice);

        if (choice == 1){
            number_saved_games++;
            file = fopen("data.txt", "w");
            fprintf(file, "%d", number_saved_games);
            fclose(file);
            save(map_player1, map_player2, player1_ships, map_player2_for_player1, map_player1_for_player2, player2_ships );
        }

    }

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                      GAME OVER");
    printf("                 THIS IS THE PLAY BACK\n\n\n");

    play_back(recorder);
}


int main() {
    // the reason im doing this , is getting more control on my program
    Main();
}


// a sample for the game display

//               Shady1
//
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
//
//
//                Shady2
//
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
//
//
//                 Shady3
//
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w



// a sample for the game display
//               Shady1
//
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
// ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w ░ w
//
//
//                Shady2
//
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
//  w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w ▒ w
//
//
//                 Shady3
//
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
// ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
//  w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w ▓ w
//
//
//                 Line
//
//  w | w | w | w | w | w | w | w | w | w
// ---|---|---|---|---|---|---|---|---|---
//  w | w | w | w | w | w | w | w | w | w
// ---|---|---|---|---|---|---|---|---|---
//  w | w | w | w | w | w | w | w | w | w
// ---|---|---|---|---|---|---|---|---|---
//  w | w | w | w | w | w | w | w | w | w
// ---|---|---|---|---|---|---|---|---|---
//  w | w | w | w | w | w | w | w | w | w
// ---|---|---|---|---|---|---|---|---|---
//  w | w | w | w | w | w | w | w | w | w
// ---|---|---|---|---|---|---|---|---|---
//  w | w | w | w | w | w | w | w | w | w
// ---|---|---|---|---|---|---|---|---|---
//  w | w | w | w | w | w | w | w | w | w
// ---|---|---|---|---|---|---|---|---|---
//  w | w | w | w | w | w | w | w | w | w
// ---|---|---|---|---|---|---|---|---|---
//  w | w | w | w | w | w | w | w | w | w
//
//
//                 Money
//
//  w $ w $ w $ w $ w $ w $ w $ w $ w $ w
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//  w $ w $ w $ w $ w $ w $ w $ w $ w $ w
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//  w $ w $ w $ w $ w $ w $ w $ w $ w $ w
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//  w $ w $ w $ w $ w $ w $ w $ w $ w $ w
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// w $ w $ w $ w $ w $ w $ w $ w $ w $ w
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//  w $ w $ w $ w $ w $ w $ w $ w $ w $ w
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//  w $ w $ w $ w $ w $ w $ w $ w $ w $ w
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//  w $ w $ w $ w $ w $ w $ w $ w $ w $ w
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//  w $ w $ w $ w $ w $ w $ w $ w $ w $ w
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//  w $ w $ w $ w $ w $ w $ w $ w $ w $ w
//
//
//                 Star
//
//  w * w * w * w * w * w * w * w * w * w
// ***************************************
//  w * w * w * w * w * w * w * w * w * w
// ***************************************
//  w * w * w * w * w * w * w * w * w * w
// ***************************************
//  w * w * w * w * w * w * w * w * w * w
// ***************************************
//  w * w * w * w * w * w * w * w * w * w
// ***************************************
//  w * w * w * w * w * w * w * w * w * w
// ***************************************
//  w * w * w * w * w * w * w * w * w * w
// ***************************************
//  w * w * w * w * w * w * w * w * w * w
// ***************************************
//  w * w * w * w * w * w * w * w * w * w
// ***************************************
//  w * w * w * w * w * w * w * w * w * w