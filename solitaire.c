#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
char deck[52];
char tableaux[7][20] = {0};
char waste[52];
char foundations[4][13];
char ranks[] = "A23456789TJQK";
char suits[] = "DCHS";
char graphics[12] = {0};
char * t_pointers[7];
void print_screen(void);
char * deck_pointer;// = &deck[51];
char * showing_card; // The first card before deck_pointer that is showing. 
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
    int column, row;
    deck_pointer = &deck[51];
    showing_card = NULL;
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
    deck_pointer = &deck[0];
    print_screen();
}
void print_screen()
{
    int i, j;
    printf("\033c\033[35m\t W\t\t D\t C\t H\t S\033[0m\n");
    if (*deck_pointer)
        printf("x x\t");
    else
        printf("\t");
    if (showing_card)
      {
        draw_card(*(showing_card));
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
              {
                draw_card(tableaux[i][j]);
                //printf("%s|\t", graphics);
                printf("x x\t");
              }
            else
                printf("\t");
          }

        printf("\n");
      }


}
void turn_deck(void)
{
    char temp[52] = {0};
    char * temp_pointer = &temp[0];
    int i;
    for (i=0;i<52;i++)
      {
        if (deck[i])
          {
            *temp_pointer = deck[i];
            temp_pointer++;
          }
        deck[i] = 0;
      }

    memcpy(deck, temp, 52);
    deck_pointer = &deck[0];
    showing_card = NULL;
}
void move_pointer(void)
{
    char * original = deck_pointer;
    deck_pointer++;
    // If pointer is at the beginning of deck,
    // we literally can't show any cards.
    if (deck_pointer >= &deck[51] || (*deck_pointer == 0 && *(deck_pointer - 1) == 0))
      {
        turn_deck();
      }
    else
      {
        showing_card = deck_pointer - 1;
        while (showing_card >= &deck[0])
          {
            if (*showing_card)
                break;
            showing_card--;
          }
        if (showing_card < &deck[0])  
            showing_card = NULL;
      }
        


}
int is_valid_descending(int source, int dest_pile)
{
    source--;
    char * dest_pointer;
    dest_pointer = t_pointers[dest_pile];
    while(*dest_pointer)
      {
        dest_pointer++;
      }
    dest_pointer--;
    char dest = *dest_pointer - 1;
    int dest_suit = dest / 13;
    int source_suit = source / 13;
    int dest_rank = dest % 13;
    int source_rank = source % 13;

    if ((source_suit % 2 != dest_suit % 2) && (dest_rank - source_rank == 1))
        return 1;
    return 0;
}
void burn_card(void);
void move_to_end(char, int);
int move_from_waste(int dest_pile)
{
    int source = *(deck_pointer+1);
    int dest = *t_pointers[dest_pile];
    //TODO: check to make sure there's something in waste
    // and destination isn't NULL
    source = *showing_card;
    dest = *t_pointers[dest_pile];
    if (is_valid_descending(source, dest_pile))
      {

        move_to_end(*showing_card, dest_pile);
        burn_card();
      }
    print_screen();
}
void tab_to_tab(int source_pile, int dest_pile)
{
    char * dest_pointer = t_pointers[dest_pile];
    char * source_pointer = t_pointers[source_pile];
    char * bottom_pointer;
    char current_card;
    while (*dest_pointer)
        dest_pointer++;
    dest_pointer--;
    while (*source_pointer)
        source_pointer++;
    source_pointer--;
    bottom_pointer = source_pointer;
    int is_valid = 0;
    while (source_pointer >= t_pointers[source_pile])
      {
        
        if (is_valid_descending(*source_pointer, dest_pile))
          {
            is_valid = 1;
            break;
          }
        source_pointer--;
      }
    if (!is_valid)
      {
        return;
      }
    if (source_pointer == t_pointers[source_pile]) // TODO and isnt empty
      {
        t_pointers[source_pile]--;
      }
    while (source_pointer <= bottom_pointer)
      {
        *++dest_pointer = *source_pointer;
        *source_pointer = 0;
        source_pointer++;
      }

    
    print_screen();



}
void move_to_end(char source, int dest_pile)
{
    char * pointer = t_pointers[dest_pile];
    while (*pointer)
      {
        pointer++;
      }
    *pointer = source;
}   
void burn_card(void)
{
    if (!showing_card)
        return;

    *showing_card = 0;
    while (*showing_card == 0 && showing_card >= &deck[0])
      {
        showing_card--;

      }
    if (showing_card < &deck[0])
      {
//        showing_card = NULL;
        ;
      }
    print_screen();
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
        if (string_length == 2 && str[0] == 'q')
            break;
        if (string_length == 1)
            move_pointer();
        if (string_length == 2 && str[0] == 'k')
//            burn_card();
            tab_to_tab(1, 4);
        if (string_length == 3 && isdigit(str[0]) && isdigit(str[1]))
          {
            tab_to_tab(str[0] - '1', str[1] - '1');
            ;//check if two digits
          }
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
