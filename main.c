#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int WIDTH = 5 , HEIGHT = 4 ;

struct node {

    char status;
    struct node* next;
    struct node* prev;
};

struct SHIP {

    struct node* head;
    struct node* tail;
};

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
                printf(" w |");
                x++;
            }else{
                printf(" w \n");
                y++;
                x = 0;}

        }
    }
}

void Main (struct node* players_ships[], struct node* enemies_ships[]){

    // games initial options
    int run = true;
    //--------------------- must add code for the WIDTH and HEIGHT input
}

int main( )
{
    draw_board();
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














