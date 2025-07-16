#define _GNU_SOURCE
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define WIDTH 60
#define HEIGHT 20
#define MAX_SNAKE_LENGTH 100

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[MAX_SNAKE_LENGTH];
    int length;
    int direction;
} Snake;

typedef struct {
    Point position;
    bool exists;
} Food;

// Direction constants
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// Game state
Snake snake;
Food food;
int score = 0;
bool game_over = false;
bool quit_game = false;  // Add this flag for proper quitting

// Color pairs
#define COLOR_SNAKE 1
#define COLOR_FOOD 2
#define COLOR_BORDER 3
#define COLOR_SCORE 4

void init_game() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    
    // Initialize colors
    start_color();
    init_pair(COLOR_SNAKE, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_FOOD, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_BORDER, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_SCORE, COLOR_YELLOW, COLOR_BLACK);
    
    // Initialize snake
    snake.length = 3;
    snake.direction = RIGHT;
    snake.body[0].x = WIDTH / 2;
    snake.body[0].y = HEIGHT / 2;
    snake.body[1].x = WIDTH / 2 - 1;
    snake.body[1].y = HEIGHT / 2;
    snake.body[2].x = WIDTH / 2 - 2;
    snake.body[2].y = HEIGHT / 2;
    
    // Initialize food
    srand(time(NULL));
    food.exists = false;
    
    score = 0;
    game_over = false;
    quit_game = false;  // Reset quit flag when initializing
}

void spawn_food() {
    if (!food.exists) {
        bool valid_position = false;
        while (!valid_position) {
            food.position.x = rand() % (WIDTH - 2) + 1;
            food.position.y = rand() % (HEIGHT - 2) + 1;
            
            // Check if food spawns on snake
            valid_position = true;
            for (int i = 0; i < snake.length; i++) {
                if (snake.body[i].x == food.position.x && 
                    snake.body[i].y == food.position.y) {
                    valid_position = false;
                    break;
                }
            }
        }
        food.exists = true;
    }
}

void draw_border() {
    attron(COLOR_PAIR(COLOR_BORDER));
    
    // Top and bottom borders
    for (int x = 0; x < WIDTH; x++) {
        mvaddch(0, x, '#');
        mvaddch(HEIGHT - 1, x, '#');
    }
    
    // Left and right borders
    for (int y = 0; y < HEIGHT; y++) {
        mvaddch(y, 0, '#');
        mvaddch(y, WIDTH - 1, '#');
    }
    
    attroff(COLOR_PAIR(COLOR_BORDER));
}

void draw_snake() {
    attron(COLOR_PAIR(COLOR_SNAKE));
    
    // Draw head
    mvaddch(snake.body[0].y, snake.body[0].x, '@');
    
    // Draw body
    for (int i = 1; i < snake.length; i++) {
        mvaddch(snake.body[i].y, snake.body[i].x, 'o');
    }
    
    attroff(COLOR_PAIR(COLOR_SNAKE));
}

void draw_food() {
    if (food.exists) {
        attron(COLOR_PAIR(COLOR_FOOD));
        mvaddch(food.position.y, food.position.x, '*');
        attroff(COLOR_PAIR(COLOR_FOOD));
    }
}

void draw_score() {
    attron(COLOR_PAIR(COLOR_SCORE));
    mvprintw(HEIGHT + 1, 0, "Score: %d", score);
    mvprintw(HEIGHT + 2, 0, "Controls: WASD or Arrow Keys, Q to quit");
    attroff(COLOR_PAIR(COLOR_SCORE));
}

void draw_game_over() {
    attron(COLOR_PAIR(COLOR_FOOD));
    mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "GAME OVER!");
    mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 8, "Final Score: %d", score);
    mvprintw(HEIGHT / 2 + 2, WIDTH / 2 - 10, "Press R to restart, Q to quit");
    attroff(COLOR_PAIR(COLOR_FOOD));
}

void update_snake() {
    // Move body
    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }
    
    // Move head based on direction
    switch (snake.direction) {
        case UP:
            snake.body[0].y--;
            break;
        case DOWN:
            snake.body[0].y++;
            break;
        case LEFT:
            snake.body[0].x--;
            break;
        case RIGHT:
            snake.body[0].x++;
            break;
    }
}

bool check_collision() {
    // Check wall collision
    if (snake.body[0].x <= 0 || snake.body[0].x >= WIDTH - 1 ||
        snake.body[0].y <= 0 || snake.body[0].y >= HEIGHT - 1) {
        return true;
    }
    
    // Check self collision
    for (int i = 1; i < snake.length; i++) {
        if (snake.body[0].x == snake.body[i].x && 
            snake.body[0].y == snake.body[i].y) {
            return true;
        }
    }
    
    return false;
}

void check_food_collision() {
    if (food.exists && 
        snake.body[0].x == food.position.x && 
        snake.body[0].y == food.position.y) {
        
        // Grow snake
        if (snake.length < MAX_SNAKE_LENGTH) {
            snake.length++;
        }
        
        // Increase score
        score += 10;
        
        // Remove food
        food.exists = false;
    }
}

void handle_input() {
    int ch = getch();
    
    switch (ch) {
        case 'w':
        case 'W':
        case KEY_UP:
            if (snake.direction != DOWN) snake.direction = UP;
            break;
        case 's':
        case 'S':
        case KEY_DOWN:
            if (snake.direction != UP) snake.direction = DOWN;
            break;
        case 'a':
        case 'A':
        case KEY_LEFT:
            if (snake.direction != RIGHT) snake.direction = LEFT;
            break;
        case 'd':
        case 'D':
        case KEY_RIGHT:
            if (snake.direction != LEFT) snake.direction = RIGHT;
            break;
        case 'q':
        case 'Q':
            quit_game = true;  // Set quit flag instead of just game_over
            break;
        case 'r':
        case 'R':
            if (game_over) {
                // Restart game
                init_game();
                return;
            }
            break;
    }
}

void render() {
    clear();
    
    if (!game_over) {
        draw_border();
        draw_snake();
        draw_food();
        draw_score();
    } else {
        draw_border();
        draw_game_over();
    }
    
    refresh();
}

int main() {
    init_game();
    
    while (!quit_game) {  // Changed condition to check quit_game flag
        handle_input();
        
        if (quit_game) {  // Break immediately if quit is requested
            break;
        }
        
        if (game_over) {
            render();
            usleep(100000); // 100ms delay
            continue;
        }
        
        update_snake();
        
        if (check_collision()) {
            game_over = true;
            continue;
        }
        
        check_food_collision();
        spawn_food();
        
        render();
        
        // Game speed (adjust for difficulty)
        usleep(150000); // 150ms delay
    }
    
    endwin();
    return 0;
}