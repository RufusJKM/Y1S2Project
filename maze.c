/**
 * @file maze.c
 * @author Rufus Kitt McGrath
 * @brief Code for the maze game for COMP1921 Assignment 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

// defines for the required autograder exit codes
#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

typedef struct __Coord
{
    int x;
    int y;
} coord;

typedef struct __Maze
{
    char **map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

/**
 * @brief Initialise a maze object - allocate memory and set attributes
 *
 * @param this pointer to the maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return int 0 on success, 1 on fail
 */
int create_maze(maze *this, int width, int height)
{
    //validate dimensions
    if (height > 100 || height < 5){
        printf("Usage: dimension out of bounds (5-100)\n");
        return 1;
    } else if (width > 100 || width < 5){
        printf("Usage: dimension out of bounds (5-100)\n");
        return 1;
    }

    //initialize fields
    int i = 0;
    (*this).map = (char**)malloc(width * sizeof(char*));
    for (i = 0; i < width; i+=1){
        (*this).map[i] = (char*)malloc(height * sizeof(char));
    }

    (*this).start.x = 0;
    (*this).start.y = 0;
    (*this).end.x = 0;
    (*this).end.y = 0;
    (*this).height = height;
    (*this).width = width;

    return 0;
}

/**
 * @brief Free the memory allocated to the maze struct
 *
 * @param this the pointer to the struct to free
 */
void free_maze(maze *this)
{
    free(this);
}

/**
 * @brief Validate and return the width of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid width (5-100)
 */
int get_width(FILE *file)
{
    //read in a line up to 101 characters
    fseek(file, 0, SEEK_SET);
    int buffer_size = 101;
    char line_buffer[buffer_size];
    int firstWidth = 0;
    int i = 0;

    //find the length of the first line
    fgets(line_buffer, buffer_size, file);
    while(line_buffer[i] != '\n'){
        i+=1;
        if (i > 100){  
            printf("Error: Invalid width\n");
            return 1;
        }
    }
    firstWidth = i;
    
    //validate width
    if (firstWidth > 100 || firstWidth < 5){
        printf("Error: Invalid width\n");
        return 1;
    } else {
        return firstWidth;
    }
}

/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid height (5-100)
 */
int get_height(FILE *file)
{
    int buffer_size = 100;
    char line_buffer[buffer_size];
    int i = 0;
    //determine number of lines
    while (fgets(line_buffer, buffer_size, file) != NULL) {
        i+=1;
    }

    //validate height
    int height = i;
    if (height > 100 || height < 5){
        printf("Error: Invalid height\n");
        return 1;
    } else {
        return height;
    }
}

/**
 * @brief read in a maze file into a struct
 *
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(maze *this, FILE *file)
{
    //get dimensions
    int height = get_height(file);
    (*this).height = height;
    int width = get_width(file);
    (*this).width = width;
    //produce error if dimesnions are invalid
    if (height == 1 || width == 1){
        return 1;
    }

    //read each line into the map
    int buffer_size = 100;
    char line_buffer[buffer_size];
    int i = 0;
    fseek(file, 0, SEEK_SET);

    while (fgets(line_buffer, buffer_size, file) != NULL) {
        for (int j = 0; j < width; j+=1){
            (*this).map[i][j] = line_buffer[j];
        }
        i+=1;
    }
    
    //close the file
    fclose(file);

    int foundS = 0;
    int foundE = 0;
    for (i = 0; i < height; i+=1){
        for (int j = 0; j < width; j+=1){
            
            //check for multiple start/end points
            if ((*this).map[i][j] == 83 && foundS == 1){
                printf("Error: Multiple start points S\n");
                return 1;
            } else if ((*this).map[i][j] == 69 && foundE == 1){
                printf("Error: Multiple end points E\n");
                return 1;
            }

            //locate and position start and end
            if (foundS == 0 || foundE == 0){
                if ((*this).map[i][j] == 83){
                    (*this).start.x=j;
                    (*this).start.y=i;
                    foundS = 1;
                } else if ((*this).map[i][j] == 69){
                    (*this).end.x=j;
                    (*this).end.y=i;
                    foundE = 1;
                }
            }
           
            //testing for illegal characters
            if ((*this).map[i][j] != 83){
                if ((*this).map[i][j] != 69){
                    if ((*this).map[i][j] != 32){
                        if ((*this).map[i][j] != 35){
                            if ((*this).map[i][j] != 10){
                                printf("Error: File contains illegal characters\n");
                                return 1;
                            }
                        }
                    }
                }
            } 
        }
    }
    
    //produce error if there is a missing start/end
    if (foundS == 0){
        printf("Error: No start point S\n");
        return 1;
    } else if (foundE == 0){
        printf("Error: No end point E\n");
        return 1;
    }

    printf("File loaded successfully\n");
    return 0;
}

/**
 * @brief Prints the maze out - code provided to ensure correct formatting
 *
 * @param this pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze *this, coord *player)
{
    // make sure we have a leading newline..
    printf("\n");
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            // decide whether player is on this spot or not
            if (player->x == j && player->y == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", this->map[i][j]);
            }
        }
        // end each row with a newline.
        printf("\n");
    }
}

/**
 * @brief Validates and performs a movement in a given direction
 *
 * @param this Maze struct
 * @param player The player's current position
 * @param direction The desired direction to move in
 */
void move(maze *this, coord *player, char direction)
{
    int invalid = 0;
    coord newCoord = *player;

    //validate direction
    if (direction == 'w'){
        newCoord.y-=1;
    } else if (direction == 'm'){
        print_maze(this, player);
    } else if (direction == 's'){
        newCoord.y+=1;
    } else if (direction == 'd'){
        newCoord.x+=1;
    } else if (direction == 'a'){
        newCoord.x-=1;
    } else {
        printf("Error: invalid input\n");
        invalid = 1;
    }

    //ensure move stays within the bounds
    if (newCoord.y < 0 || newCoord.y >= (*this).height){
        printf("Error: out of bounds\n");
        invalid = 1;
    } else if (newCoord.x < 0 || newCoord.x >= (*this).width){
        printf("Error: out of bounds\n");
        invalid = 1;
    } 

    //ensure player doesnt move into wall
    if (invalid == 0){
        if ((*this).map[newCoord.y][newCoord.x] != '#') {
            (*player).x = newCoord.x;
            (*player).y = newCoord.y;
        } else {
            printf("Error: you hit a wall\n");
        }
    }
}

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param this current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze *this, coord *player)
{
    if ((*this).end.x == (*player).x){
        if ((*this).end.y == (*player).y){
            printf("Congratulations, you solved the maze!\n");
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

/**
 * @brief Check whether the an argument is an integer
 *
 * @param argument argument passed in
 * @return int 1 for false, integer value for true
 */
int is_digit(char argument[])
{
    for (int i = 0; i < strlen(argument); i+=1){
        if (argument[i] < 48 || argument[i] > 57){
            printf("Usage: dimensions must be integers");
            return 1;
        }
    }
    return atoi(argument);
}

int main(int argc, char *argv[])
{
    int width = 0;
    int height = 0;
    if (argc != 4){
        printf("Usage: maze <filename> <width> <height>");
        return(1);
    } else {
        //ensure entered dimensions are ints between 5 and 100
        width = is_digit(argv[2]);
        height = is_digit(argv[3]);
        if (width == 1 || height == 1){
            return 1;
        }
    }

    // set up some useful variables (you can rename or remove these if you want)
    coord *player;
    maze *this_maze = malloc(sizeof(maze));
    FILE *f;

    //initialize maze object
    if (create_maze(this_maze, width, height) == 1){
        return 3;
    }

    //open the file and ensure it exists
    f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Error: Could not find or open file\n");
        return 2;
    } 

    //read in the maze, any errors exit with code 3 (maze error)
    if (read_maze(this_maze, f) == 1){
        return 3;
    }

    //initialize player position
    (*player).x = (*this_maze).start.x;
    (*player).y = (*this_maze).start.y;
    
    //start game loop
    char direction;
    while (has_won(this_maze, player) == 0){
        printf("Make your move... \n");
        scanf(" %c", &direction);
        direction = tolower(direction);
        move(this_maze, player, direction);
    }

    //free space
    free(this_maze);
    return 0;
}