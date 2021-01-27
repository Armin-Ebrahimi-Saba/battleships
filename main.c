#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

int WIDTH = 10 , HEIGHT = 10 ,total_number_ships = 10;


struct node {

    int x;
    int y;

    struct node* next;
    struct node* prev;
};


struct SHIP {

    int length;
    char *dir;

    struct node* head;
};


// this function checks if the ship fits in its place in 2 condition:
//                                                                  first if the ship is completely in the map.
//                                                                  second if the ship does not have any collision with other ships.
int isFit(const char *map, struct SHIP* ship){

    if (strcmp(ship->dir, "d") == 0){
        for (int i = -1;i < ship->length;i++){

            for (int j = -1;j < 2;j++){

                if (map[(ship->head->y + i) * WIDTH + ship->head->x + j] == 'f' || !(((ship->head->y + i) * WIDTH <= HEIGHT && (ship->head->y + i) * WIDTH >= -1) && (-1 <= ship->head->x + j && ship->head->x + j <= WIDTH))){
                    return 0;}
            }
        }
    }


    if (strcmp(ship->dir, "u") == 0){
        for (int i = -1;i < ship->length;i++){

            for (int j = -1;j < 2;j++){

                if (map[(ship->head->y - i) * WIDTH + ship->head->x + j] == 'f' || !(((ship->head->y - i) * WIDTH <= HEIGHT && (ship->head->y - i) * WIDTH >= -1) && (-1 <= ship->head->x + j && ship->head->x + j <= WIDTH))){
                    return 0;}
            }
        }
    }


    if (strcmp(ship->dir, "r") == 0){
        for (int i = -1;i < ship->length;i++){

            for (int j = -1;j < 2;j++){

                if (map[(ship->head->y + j) * WIDTH + ship->head->x + i] == 'f' || !(((ship->head->y + j) * WIDTH <= HEIGHT && (ship->head->y + j) * WIDTH >= -1) && (-1 <= ship->head->x + i && ship->head->x + i <= WIDTH))){
                    return 0;}
            }
        }
    }


    if (strcmp(ship->dir, "l") == 0){
        for (int i = -1;i < ship->length;i++){

            for (int j = -1;j < 2;j++){

                if (map[(ship->head->y + j) * WIDTH + ship->head->x - i] == 'f' || !(((ship->head->y + j) * WIDTH <= HEIGHT && (ship->head->y + j) * WIDTH >= -1) && (-1 <= ship->head->x - i && ship->head->x - i <= WIDTH))){
                    return 0;}

            }
        }
    }
    return 1;
}

// this function put the ship in its place
int make_ship(struct SHIP* ship, char *dir, char *map, int _x_, int _y_){

    struct node* current_node;
    current_node = ship->head->next;

    ship->head->x = _x_;
    ship->head->y = _y_;
    ship->dir = dir;

    for (int i = 0;i < ship->length;i++){
        // u : up
        if (strcmp(dir, "u") == 0){
            
            if (isFit(map, ship) == 0){      // this check that if the fits in the place or not
                printf("invalid place for putting your ship\n");
                return 0;}

            map[_x_ + WIDTH * _y_] = 'f';

            current_node->x = ship->head->x;
            current_node->y = current_node->prev->y - 1;
            map[current_node->x + current_node->y * WIDTH] = 'f';

            current_node = current_node->next;

        }
        // d : down
        if (strcmp(dir, "d") != 0){

            if (isFit(map, ship) == 0){      // this check that if the ship fits in the map or not
                printf("invalid place for putting your ship\n");
                return 0;}

            map[_x_ + WIDTH * _y_] = 'f';

            current_node->x = ship->head->x;
            current_node->y = current_node->prev->y + 1;
            map[current_node->x + current_node->y * WIDTH] = 'f';

            current_node = current_node->next;

        }
        // r : right
        if (strcmp(dir, "r") != 0){

            if (isFit(map, ship) == 0){      // this check that if the ship fits in the map or not
                printf("invalid place for putting your ship\n");
                return 0;}

            map[_x_ + WIDTH * _y_] = 'f';

            current_node->y = ship->head->y;
            current_node->x = current_node->prev->x + 1;
            map[current_node->x + current_node->y * WIDTH] = 'f';

            current_node = current_node->next;

        }
        // l : left
        if (strcmp(dir, "l") != 0){
            
            if (isFit(map, ship) == 0){      // this check that if the ship fits in the map or not
                printf("invalid place for putting your ship\n");
                return 0;}

            map[_x_ + WIDTH * _y_] = 'f';

            current_node->y = ship->head->y;
            current_node->x = current_node->prev->x - 1;
            map[current_node->x + current_node->y * WIDTH] = 'f';


            current_node = current_node->next;

        }
    }
    return 1;
}


int make_ship_auto(struct SHIP* ship, char *dir, char *map, int _x_, int _y_){

    struct node* current_node;
    current_node = ship->head->next;

    ship->head->x = _x_;
    ship->head->y = _y_;
    ship->dir = dir;

    for (int i = 0;i < ship->length;i++){
        // u : up
        if (strcmp(dir, "u") == 0){

            if (isFit(map, ship) == 0){      // this check that if the fits in the place or not
                return 0;}

            map[_x_ + WIDTH * _y_] = 'f';

            current_node->x = ship->head->x;
            current_node->y = current_node->prev->y - 1;
            map[current_node->x + current_node->y * WIDTH] = 'f';

            current_node = current_node->next;

        }
        // d : down
        if (strcmp(dir, "d") == 0){

            if (isFit(map, ship) == 0){      // this check that if the ship fits in the map or not
                return 0;}

            map[_x_ + WIDTH * _y_] = 'f';

            current_node->x = ship->head->x;
            current_node->y = current_node->prev->y + 1;
            map[current_node->x + current_node->y * WIDTH] = 'f';

            current_node = current_node->next;

        }
        // r : right
        if (strcmp(dir, "r") == 0){

            if (isFit(map, ship) == 0){      // this check that if the ship fits in the map or not
                return 0;}

            map[_x_ + WIDTH * _y_] = 'f';

            current_node->y = ship->head->y;
            current_node->x = current_node->prev->x + 1;
            map[current_node->x + current_node->y * WIDTH] = 'f';

            current_node = current_node->next;

        }
        // l : left
        if (strcmp(dir, "l") == 0){

            if (isFit(map, ship) == 0){      // this check that if the ship fits in the map or not
                return 0;}

            map[_x_ + WIDTH * _y_] = 'f';

            current_node->y = ship->head->y;
            current_node->x = current_node->prev->x - 1;
            map[current_node->x + current_node->y * WIDTH] = 'f';

            current_node = current_node->next;

        }
    }
    return 1;
}


void map_ships_manually(struct SHIP* ships[], char* map){
    // the code below is for players mapping term

    int length_1_counter = 0;
    int length_2_counter = 0;
    int length_3_counter = 0;
    int length_5_counter = 0;

    int selected;                           // selected is for the length of the boat that the player select
    int _x_;                                // x of head of the ship
    int _y_;                                // y of head of the ship
    char *dir = malloc(sizeof(char)*2);     // the direction that the ship has, must choose between u(up), d(down), r(right), l(left)

    while (length_1_counter + length_2_counter + length_3_counter + length_5_counter < total_number_ships){

        //----------------------------------------------------- this part is for getting inputs from user (player)

        printf("select the size of the ship");
        scanf("%d",&selected);

        // scanning a valid x

        do{
            printf("enter a number between 0 and %d\n",WIDTH - 1);
            scanf("%d",&_x_);
        }while (_x_ < 0 || _x_ > WIDTH - 1);

        // scanning a valid y

        do{
            printf("enter a number between 0 and %d\n",HEIGHT - 1);
            scanf("%d",&_y_);
        }while (_y_ < 0 || _y_ > HEIGHT - 1);

        // scanning a valid directory

        do {
            printf("enter the direction that the ship has, must choose between u(up), d(down), r(right), l(left)\n");
            scanf("%s",dir);
        }while (strcmp(dir, "u") != 0 && strcmp(dir, "d") != 0 && strcmp(dir, "r") != 0 && strcmp(dir, "l") != 0);

        //----------------------------------------------------- this part is placing ships in the map

        //----------------------length 1

        if (selected == 1 && length_1_counter < 4){

            ships[6 + length_1_counter]->dir = dir;
            if (make_ship(ships[6 + length_1_counter], dir, map, _x_, _y_) == 0) continue;

            length_1_counter++;

        }else if (selected == 1 && length_1_counter == 4) {printf("you have used all of your ships with length of 1");continue;}

        //----------------------length 2

        if (selected == 2 && length_2_counter < 3){

            ships[3 + length_2_counter]->dir = dir;
            if (make_ship(ships[3 + length_2_counter], dir, map, _x_, _y_) == 0 ) continue;

            length_2_counter++;

        }else if (selected == 2 && length_2_counter == 3) {printf("you have used all of your ships with length of 2");continue;}

        //----------------------length 3

        if (selected == 3 && length_3_counter < 2){

            ships[3 + length_3_counter]->dir = dir;
            if (make_ship(ships[3 + length_3_counter], dir, map, _x_, _y_) == 0) continue;

            length_3_counter++;

        }else if (selected == 3 && length_3_counter == 2) {printf("you have used all of your ships with length of 3");continue;}

        //----------------------length 5

        if (selected == 5 && length_5_counter < 1){

            ships[0]->dir = dir;
            if (make_ship(ships[0], dir, map, _x_, _y_) == 0) continue;

            length_5_counter++;

        }else if (selected == 5 && length_5_counter == 1) {printf("you have used all of your ships with length of 5");continue;}

        //--------------------------------------------------------------------------------------------------------

    }
}


void map_ships_automatically(struct SHIP* ships[], char* map){
        // the code below is for mapping automatically

        srand(time(0));

        int length_1_counter = 0;
        int length_2_counter = 0;
        int length_3_counter = 0;
        int length_5_counter = 0;

        int selected = rand() % 4;              // selected is for the length of the boat that the player select
        int _x_ = rand() % 10;                  // x of head of the ship
        int _y_ = rand() % 10;                  // y of head of the ship
        char *dir = malloc (sizeof(char) * 2);
        int dir_int = rand() % 4 + 1;           // the direction that the ship has, must choose between 1(up), 2(down), 3(right), 4(left)

        if (dir_int == 1) dir = "u";
        else if (dir_int == 2) dir = "d";
        else if (dir_int == 3) dir = "r";
        else if (dir_int == 4) dir = "l";

        // using the built function for putting the ships in their places.
        while (length_1_counter + length_2_counter + length_3_counter + length_5_counter < total_number_ships){

            //----------------------length 1

            if (selected == 1 && length_1_counter < 4){

                ships[6 + length_1_counter]->head->x = _x_;
                ships[6 + length_1_counter]->head->y = _y_;

                length_1_counter++;
            }

            //----------------------length 2

            if (selected == 2 && length_2_counter < 3){

                ships[3 + length_2_counter]->head->x = _x_;
                ships[3 + length_2_counter]->head->y = _y_;
                if (make_ship_auto(ships[3 + length_2_counter], dir, map, _x_, _y_) == 0) continue;

                length_2_counter++;
            }

            //----------------------length 3

            if (selected == 3 && length_3_counter < 2){

                ships[1 + length_3_counter]->head->x = _x_;
                ships[1 + length_3_counter]->head->y = _y_;
                if (make_ship_auto(ships[3 + length_3_counter],dir , map, _x_, _y_) == 0) continue;

                length_3_counter++;
            }

            //----------------------length 5

            if (selected == 5 && length_5_counter < 1){

                ships[0]->head->x = _x_;
                ships[0]->head->y = _y_;
                if (make_ship_auto(ships[0], dir, map, _x_, _y_) == 0) continue;

                length_5_counter++;
            }
            //----------------------finished getting the ships coordinates
    }
}


void draw_board(){

    int x = 0, y = 0;
    while (y < 2 * HEIGHT - 1){

        if (y % 2 != 0){

            if (x < WIDTH - 1) {
                printf("---|");
                x++;
            }else {
                printf("---\n");
                x = 0;
                y++;
                continue;}

        }else{
            if (x < WIDTH - 1) {
                printf("   |");
                x++;
            }else{
                printf("   \n");
                y++;
                x = 0;}

        }
    }
}

void Main (){

    // games initial options
    int run = true;
    //--------------------- must add code for the WIDTH and HEIGHT input
    //-----------------initializing ships array
    char *map = malloc(sizeof(char) * WIDTH * HEIGHT);

    for (int i = 0;i < WIDTH * HEIGHT;i++){

        map[i] = 'e';}

    struct SHIP* ships[10];

    ships[0] = malloc (sizeof(struct SHIP));
    ships[1] = malloc (sizeof(struct SHIP));
    ships[2] = malloc (sizeof(struct SHIP));
    ships[3] = malloc (sizeof(struct SHIP));
    ships[4] = malloc (sizeof(struct SHIP));
    ships[5] = malloc (sizeof(struct SHIP));
    ships[6] = malloc (sizeof(struct SHIP));
    ships[7] = malloc (sizeof(struct SHIP));
    ships[8] = malloc (sizeof(struct SHIP));
    ships[9] = malloc (sizeof(struct SHIP));

    ships[0]->length = 5;

    ships[1]->length = 3;
    ships[2]->length = 3;

    ships[3]->length = 2;
    ships[4]->length = 2;
    ships[5]->length = 2;

    ships[6]->length = 1;
    ships[7]->length = 1;
    ships[8]->length = 1;
    ships[9]->length = 1;

    map_ships_manually(ships, map);
    draw_board();
}

int main( )
{
    Main();
}


// a sample for the game display

//                         e | e | w | w | w
//                        ---|---|---|---|---
//                         e | e | w | w | w
//                        ---|---|---|---|---
//                         w | w | w | w | w
//                        ---|---|---|---|---
//                         w | w | w | w | w


//                         e | e | w | c | w
//                        ---|---|---|---|---
//                         e | e | w | c | w
//                        ---|---|---|---|---
//                         w | w | c | c | w
//                        ---|---|---|---|---
//                         w | w | c | c | w