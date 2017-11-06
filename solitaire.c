#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>
int DEBUG = 0;

void shuffle(void);
void deal(void);
void print_screen(void);
void draw_card(int num);
void move_pointer(void);
void turn_deck(void);
int get_command(void);
char deck[52] = {0};
char tableaux[7][20] = {0};
char graphics[12] = {0};
char foundations[4];
char ranks[] = "A23456789TJQK";
char suits[] = "DCHS";
char * exposed_pointers[7];
char * deck_pointer;
char * showing_card;
int help = 1;
int main()
{
    
    shuffle();
    deal();
    deck_pointer = &deck[0];
    showing_card = NULL;
    int status = 1;
    while (1)
      {
        print_screen();
        status = get_command();
        if (status == 2)
            break;
      }


}
void show_help(void)
{
    printf("<enter>\tFlip over a card from the deck\n");
    printf("23\tMove subpile from pile 2 to bottom of pile 3\n");
    printf("w5\tMove from waste to pile 5\n");
    printf("1f\tMove the bottom card from pile 1 to respective foundation\n");
    printf("wf\tMove from waste to respective foundation.\n");
    printf("h\tShow this help.\n");
    printf("q\tQuit game.\n");
    help = 0;

}
char * pointer_to_end(int dest_pile)
{
    char * pointer;
    pointer = tableaux[dest_pile];
    if (!*pointer)
        return NULL;
    while (*pointer)
      {
        pointer++;
      }
    pointer--;
    return pointer;

}
int is_valid_descending(int source, char * dest_pointer, int dest_pile)
{
    int source_suit, source_rank;
    char dest;
    int dest_suit, dest_rank;
    source--;
    source_suit = source / 13;
    source_rank = source % 13;

    // Moving a king to an empty tableau.
    if (dest_pointer == NULL && source_rank == 12)
        return 2;
    // Otherwise, you can't move the card to an empty tableau.
    else if (dest_pointer == NULL)
        return 0;
    dest = *dest_pointer;
    dest--;
    dest_suit = dest / 13;
    dest_rank = dest % 13;
    if ((source_suit % 2 != dest_suit % 2) &&
      (dest_rank - source_rank == 1))
        return 1;
    return 0;

}
void waste_to_tableau(int dest_pile)
{
    int source, dest;
    char * dest_pointer;
    source = *showing_card;
    dest_pointer = pointer_to_end(dest_pile);
    if (is_valid_descending(source, dest_pointer, dest_pile))
      {
        if (!dest_pointer)
            tableaux[dest_pile][0] = source;
        else
            *++dest_pointer = source; 
        *showing_card = 0;
        showing_card--;
      }

}
void turn_deck(void)
{
    char temp[52] = {0};
    char * temp_pointer = &temp[0];
    int i;
    for (i = 0; i < 52; i++)
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
    deck_pointer++;
    if (deck_pointer >= &deck[51] || (*deck_pointer == 0 &&
      *(deck_pointer - 1) == 0))
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
void draw_card(int num)
{
    int suit;
    int rank;
    char color;
    num--;
    suit = num / 13;
    rank = num % 13;
    // diamonds and hearts
    if (suit == 0 || suit == 2)
        color = '1';
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
    printf("%s", graphics);
}
void print_screen(void)
{
    int i, j;
    printf("\n\033c\033[35m\t W\t\t D\t C\t H\t S\033[0m\n");
    if (*deck_pointer)
        printf("x x\t");
    else
        printf("\t");
    if (showing_card)
      {
        draw_card(*(showing_card));
        printf("\t\t");
      }
    else
        printf("\t\t");
    for (i = 0; i < 4; i++)
      {
        draw_card(foundations[i]);
        printf("\t");
      }
    printf("\n\n");
    printf("\033[35m 1\t 2\t 3\t 4\t 5\t 6\t 7\t\033[0m\n");
    for (j = 0; j < 20; j++)
      {
        for (i = 0; i <= 6; i++)
          {
            if (tableaux[i][j] && (&tableaux[i][j] >= exposed_pointers[i]))
                draw_card(tableaux[i][j]);
            else if (tableaux[i][j])
                printf("x x");
            printf("\t");
          }
        printf("\n");
      }
    if (help)
        show_help();
}
int in_tableaux_range(char ch)
{
    if (isdigit(ch) && 0 <= ch - '1' <= 6)
        return 1;
    return 0;
}

void tableau_to_tableau(int source_pile, int dest_pile)
{
    int is_valid = 0;
    char * source_pointer;
    char * dest_pointer;
    char * bottom_pointer;
    source_pointer = pointer_to_end(source_pile);
    if (!source_pointer)
        return;
    bottom_pointer = source_pointer;
    dest_pointer = pointer_to_end(dest_pile);
    while (source_pointer >= exposed_pointers[source_pile])
      {
        if (is_valid = is_valid_descending(*source_pointer, dest_pointer, dest_pile))
            break;
        source_pointer--;
      }
    if (!is_valid)
        return;
    
    if (source_pointer == exposed_pointers[source_pile])
      {
        exposed_pointers[source_pile]--;
        if (exposed_pointers[source_pile] < tableaux[source_pile])
            exposed_pointers[source_pile]++;
      }
    while (source_pointer <= bottom_pointer)
      {
        if (!dest_pointer)
            dest_pointer = tableaux[dest_pile];
        else
            dest_pointer++;
            
        *dest_pointer = *source_pointer;
        *source_pointer = 0;
        source_pointer++;

      
      }


}
void fix_tableau(int source_pile)
{
    char * end_of_tableau;
    end_of_tableau = pointer_to_end(source_pile);
    if (!end_of_tableau)
      {
        exposed_pointers[source_pile] = NULL;
        return;
      }
    if (end_of_tableau < exposed_pointers[source_pile])
        exposed_pointers[source_pile]--;
}
void tableau_to_foundation(int source_pile)
{
    char * end_of_tableau;
    char source, foundation_card;
    int source_suit, source_rank, foundation_rank, valid;
    end_of_tableau = pointer_to_end(source_pile);
    if (!end_of_tableau)
        return;
    source = *end_of_tableau;
    source--;
    source_suit = source / 13;
    source_rank = source % 13;
    foundation_card = foundations[source_suit];
    valid = 0;
    if (!foundation_card && source_rank == 0)
        valid = 1;
    else if (!foundation_card)
        valid = 0;
    else
      {
        foundation_card--;
        foundation_rank = foundation_card % 13;
        if ((source_rank - foundation_rank) == 1)
            valid = 1;
      }
    if (!valid)
        return;
    foundations[source_suit] = ++source;
    *end_of_tableau = 0;
    fix_tableau(source_pile);
    
        


    
}
void waste_to_foundation(void)
{
    char source, foundation_card;
    int source_suit, source_rank;
    int valid;
    int foundation_rank;

    source = *showing_card;
    source--;
    source_suit = source / 13;
    source_rank = source % 13;
    foundation_card = foundations[source_suit];
    if (!foundation_card && source_rank == 0)
        valid = 1;
    else if (!foundation_card) 
        valid = 0;
    else
      {
        foundation_card--;
        foundation_rank = foundation_card % 13;
        if ((source_rank - foundation_rank) == 1)
            valid = 1;
      }
    if (!valid)
        return;
    foundations[source_suit] = ++source;
    *showing_card = 0;
    showing_card--;

}
int get_command(void)
{
    char str[100];
    int string_length;
    fgets(str, sizeof(str), stdin);
    if (strlen(str) == 1)
        move_pointer();
    else if (strlen(str) == 2 && str[0] == 'q')
        return 2;
    else if (strlen(str) == 2 && str[0] == 'h')
        help = 1;
    else if (strlen(str) == 3)
      {
        // If it's a tableau-to-tableau command.
        if (in_tableaux_range(str[0]) &&
          in_tableaux_range(str[1]))
            tableau_to_tableau(str[0] - '1', str[1] - '1');
        // tableau to foundation.
        if (in_tableaux_range(str[0]) &&
          str[1] == 'f')
            tableau_to_foundation(str[0]-'1');
        // waste to tableaux
        if (str[0] == 'w' && 
          in_tableaux_range(str[1]))
            waste_to_tableau(str[1] - '1');
        // waste to foundation
        if (str[0] == 'w' &&
          str[1] == 'f')
            waste_to_foundation();
        // foundation to tableaux
        if (str[0] == 'f' && 
          in_tableaux_range(str[1]))
            ;
      }
    return 0;
}
void deal(void)
{
    int column, row;
    deck_pointer = &deck[51];
    for (column = 0; column <= 6; column++)
      {
        for (row = 0; row <= column; row++)
          {
            tableaux[column][row] = *deck_pointer;
            *deck_pointer = 0;
            deck_pointer--;
          }
        exposed_pointers[column] = &tableaux[column][row-1];
      }
    if (DEBUG)
      {
        tableaux[0][0] = 0;
        tableaux[1][1] = 1;
        tableaux[1][0] = 2;
        tableaux[2][2] = 3;
        tableaux[3][3] = 4 + 13;
        tableaux[4][4] = 5;
        tableaux[5][5] = 2 + 0;
        tableaux[6][6] = 1 + 13;
        tableaux[5][5] = 2 + 13;
        deck[0] = 13;
      }

}

void shuffle()
{
    time_t t;
    int position, i;
    
    srand((unsigned) time(&t));
    i = 1;
    while (i <= 52)
      {
        position = rand() % 52;
        if (!deck[position])
          {
            deck[position] = i;
            i++;
          }

      }

}
