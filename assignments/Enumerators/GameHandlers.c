#include <stdio.h>

enum game {
    PAUSED, PLAYING, GAMEOVER, MENU
};

int main() {
    enum game game_state = MENU;
    char input;

    while (game_state != GAMEOVER) {
        switch (game_state) {
            case MENU:
                printf("In menu press 'p' to continue\n");
                scanf(" %c", &input);  // Fixing the getchar() issue
                if (input == 'p') {
                    game_state = PLAYING;
                }
                break;

            case PLAYING:
                printf("Game is playing. Press 'p' to pause or 'q' to quit\n");
                scanf(" %c", &input);
                if (input == 'p') {
                    game_state = PAUSED;
                } else if (input == 'q') {
                    game_state = GAMEOVER;
                }
                break;

            case PAUSED:
                printf("Game is paused. Press 'p' to play or 'q' to quit\n");
                scanf(" %c", &input);
                if (input == 'p') {
                    game_state = PLAYING;
                } else if (input == 'q') {
                    game_state = GAMEOVER;
                }
                break;
        }
    }
    
    printf("Game over.\n");
    return 0;
}
