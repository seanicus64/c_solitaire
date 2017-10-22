#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
char deck[52];
char tableaux[7][20];
char waste[52];
char foundations[4][13];
char ranks[] = "A23456789TJQK";
char suits[] = "DCHS";
char graphics[12] = {0};
char * t_pointers[7];
void print_screen(void);
char * deck_pointer;// = &deck[51];
char deck[52] = {38, 24, 7, 35, 42, 30, 26, 49, 29, 43, 11, 48, 19, 36, 23, 45, 37, 46, 44, 27, 12, 17, 39, 14, 2, 13, 6, 16, 32, 20, 52, 28, 31, 40, 18, 22, 15, 21, 10, 1, 8, 50, 51, 5, 41, 47, 34, 3, 25, 33, 4, 9};
void draw_card(int num)
{
    //char graphics[12] = {0};
    int suit;
    int rank;
    char color;
    num--;
    suit = num / 13;
    rank = num % 13;
    // diamonds and hearts
    if (suit == 0 || suit == 2)
        color = '1';
    // clubs and spades
    else
        color = '6';
    graphics[0] = '\033';
    graphics[1] = '[';
    graphics[2] = '3';
    graphics[3] = color;
    graphics[4] = 'm';
    graphics[5] = ranks[rank];
    graphics[6] = '-';
    graphics[7] = suits[suit];
    graphics[8] = '\033';
    graphics[9] = '[';
    graphics[10] = '0';
    graphics[11] = 'm';
}
void deal(void)
{
    deck_pointer = &deck[51];
    printf("%p %p\n", deck, deck_pointer);
    printf("deck_pointer: %p\t&deck[0]: %p\t&deck[51]: %p\n", deck_pointer, &deck[0], &deck[51]);
    int column, row;
    for (column=0; column <= 6; column++)
      {
        for (row=0; row<=column;row++)
          {
            tableaux[column][row] = *deck_pointer;
            *deck_pointer = 0;
            deck_pointer--;
          }
        t_pointers[column] = &tableaux[column][row-1];
      }
    printf("\n");
    print_screen();
}
void print_screen()
{
    int i, j;
    printf("\033[35m\t W\t\t D\t C\t H\t S\033[0m\n");
    if (*deck_pointer)
        printf("!%d\t", *deck_pointer);
    else
        printf("###\t");
    if (*(deck_pointer+1))
      {
        draw_card(*(deck_pointer+1));
        printf("%s\t\t", graphics);
      }
    else
        printf("\t\t");





    printf("\n\n");
    printf("\033[35m 1\t 2\t 3\t 4\t 5\t 6\t 7\033[0m\n");
    for (j=0; j<20; j++)
      {

        for (i=0; i<=6; i++)
          {
            if (tableaux[i][j] && (&tableaux[i][j] >= t_pointers[i]))
              {
                draw_card(tableaux[i][j]);
                printf("%s\t", graphics);
              }
            else if (tableaux[i][j])
                printf("x x\t");
            else
                printf("\t");
          }

        printf("\n");
      }


}
void move_pointer(void)
{
    char * original = deck_pointer;
    deck_pointer--;
    while (1)
      {
        
        if (deck_pointer <= &deck[0])
            deck_pointer = &deck[51];
        else if (!*deck_pointer)
            deck_pointer--;
        else if (deck_pointer == original)
            break;
        else
            break;
      }

}
int is_valid_descending(int source, int dest)
{
    source--;
    dest--;
    int dest_suit = dest / 13;
    int source_suit = source / 13;
    int dest_rank = dest % 13;
    int source_rank = source % 13;
    printf("source_suit = %d\tsource_rank = %d\tdest_suit = %d\tdest_rank = %d\n", 
        source_suit, source_rank, dest_suit, dest_rank);
    if ((source_suit % 2 != dest_suit % 2) && (dest_rank - source_rank == 1))
        return 0;
    return 1;
}
int move_from_waste(int dest_pile)
{
    int source = *(deck_pointer+1);
    int dest = *t_pointers[dest_pile];
    printf("%d\n", is_valid_descending(source, dest));
    
}
int main()
{
    char str[100];
    
    int string_length;
    deal();
    while (1)
      {
        fgets(str, sizeof(str), stdin);
        string_length = strlen(str);
        printf("%d\n", string_length);
        if (string_length == 2 && str[0] == 'q')
            break;
        if (string_length == 1 && deck_pointer > &deck[0])
            move_pointer();
        if (string_length == 3)
          {
            if (str[0] == 'w' && '1' <= str[1] <= '7')
              {
                puts(str);
//                move_from_waste(4);
                move_from_waste(str[1] - '0' - 1);
              }
            ;
          }
//        printf("\033c");
        print_screen();
      }


}
