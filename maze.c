#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    char** grid;
    int width;
    int height;
    Point start;
    Point end;
} Maze;

Maze* load_maze(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return NULL;
    }

    Maze* maze = malloc(sizeof(Maze));
    fscanf(file, "%d %d\n", &maze->height, &maze->width);

    // Check for valid maze dimensions
    if (maze->height < 5 || maze->height > 100 || maze->width < 5 || maze->width > 100) {
        printf("Invalid maze dimensions\n");
        free(maze);
        return NULL;
    }

    maze->grid = malloc(maze->height * sizeof(char*));
    for (int i = 0; i < maze->height; i++) {
        maze->grid[i] = malloc(maze->width * sizeof(char));
        fgets(maze->grid[i], maze->width + 1, file);
        fgetc(file); // Read the newline character at the end of each line

        for (int j = 0; j < maze->width; j++) {
            if (maze->grid[i][j] == 'S') {
                maze->start.x = j;
                maze->start.y = i;
            } else if (maze->grid[i][j] == 'E') {
                maze->end.x = j;
                maze->end.y = i;
            }
        }
    }

    fclose(file);
    return maze;
}

void move_player(Maze* maze, char direction) {
    Point new_position = maze->start;

    switch (direction) {
        case 'W':
        case 'w':
            new_position.y--;
            break;
        case 'A':
        case 'a':
            new_position.x--;
            break;
        case 'S':
        case 's':
            new_position.y++;
            break;
        case 'D':
        case 'd':
            new_position.x++;
            break;
        default:
            printf("Invalid direction: %c\n", direction);
            return;
    }

    if (new_position.x < 0 || new_position.x >= maze->width || new_position.y < 0 || new_position.y >= maze->height || maze->grid[new_position.y][new_position.x] == '#') {
        printf("Cannot move in this direction\n");
    } else {
        maze->start = new_position;
    }
}

void display_maze(Maze* maze) {
    printf("\n"); // Newline before the first row
    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->start.x == x && maze->start.y == y) {
                printf("X");
            } else {
                printf("%c", maze->grid[y][x]);
            }
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <maze file>\n", argv[0]);
        return 1; // Argument error
    }

    Maze* maze = load_maze(argv[1]);
    if (maze == NULL) {
        return 2; // File error
    }

    char command;
    do {
        printf("Enter command (WASD to move, M to display maze, Q to quit): ");
        scanf(" %c", &command); // The space before %c skips any whitespace characters, including '\n'

        switch (command) {
            case 'W':
            case 'w':
            case 'A':
            case 'a':
            case 'S':
            case 's':
            case 'D':
            case 'd':
                move_player(maze, command);
                break;
            case 'M':
            case 'm':
                display_maze(maze);
                break;
            case 'Q':
            case 'q':
                printf("Quitting the game...\n");
                break;
            default:
                printf("Invalid command: %c\n", command);
                break;
        }
    } while (command != 'Q' && command != 'q' && !(maze->start.x == maze->end.x && maze->start.y == maze->end.y));

    if (maze->start.x == maze->end.x && maze->start.y == maze->end.y) {
        printf("Congratulations, you reached the end of the maze!\n");
    }

    for (int i = 0; i < maze->height; i++) {
        free(maze->grid[i]);
    }
    free(maze->grid);
    free(maze);

    return 0; // Successful running
}
