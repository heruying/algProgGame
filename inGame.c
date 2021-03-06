#include "inGame.h"

void foundPos (gameState *save, WINDOW *game, int y, int x) {
    for (int i = 0; i < (save->level + 2); i++) {
        if (save->positions[i].x == x && save->positions[i].y == y && save->positions[i].found == 0) {
            save->scenario[(y / 2) * 26 + x / 2] = 'x';
            mvwaddch(game, y, x, 'x');
            mvwaddch(game, y + 1, x, 'x');
            mvwaddch(game, y, x + 1, 'x');
            mvwaddch(game, y + 1, x + 1, 'x');
            save->positions[i].found = 1;
            save->found++;
            if (gotBlock(y, x, game)) {
                save->blocked--;
            }
        }
    }
}

void deletePlayer (character player) {
    mvwaddch(player.curwin, player.y, player.x, ' ');
    mvwaddch(player.curwin, player.y + 1, player.x, ' ');
    mvwaddch(player.curwin, player.y, player.x + 1, ' ');
    mvwaddch(player.curwin, player.y + 1, player.x + 1, ' ');
}

void moveBlock (gameState *save, WINDOW *game, int y, int x, int oldY, int oldX) {
    save->scenario[(oldY / 2) * 26 + oldX / 2] = '0';
    save->scenario[(y / 2) * 26 + x / 2] = '2';
    mvwaddch(game, y, x, '2');
    mvwaddch(game, y + 1, x, '2');
    mvwaddch(game, y, x + 1, '2');
    mvwaddch(game, y + 1, x + 1, '2');
}

void createPlayer (character player) {
    mvwaddch(player.curwin, player.y, player.x, player.character);
    mvwaddch(player.curwin, player.y + 1, player.x, player.character);
    mvwaddch(player.curwin, player.y, player.x + 1, player.character);
    mvwaddch(player.curwin, player.y + 1, player.x + 1, player.character);
}

int checkChar (int y, int x, WINDOW *game, char way) {
    char block;
    switch(way) {
        case 'u':
            block = mvwinch(game, y - 1, x);
            if (block == ' ')
                block = mvwinch(game, y - 1, x + 1);
        break;
        case 'd':
            block = mvwinch(game, y + 2, x);
            if (block == ' ')
                block = mvwinch(game, y + 2, x + 1);
        break;
        case 'l':
            block = mvwinch(game, y, x - 1);
            if (block == ' ')
                block = mvwinch(game, y + 1, x - 1);
        break;
        case 'r':
            block = mvwinch(game, y, x + 2);
            if (block == ' ')
                block = mvwinch(game, y + 1, x + 2);
        break;
    }
    return block;
}

void moveUp(gameState *save, WINDOW *game) {
    int y, x;
    char validMove;
    validMove = checkChar(save->player.y, save->player.x, game, 'u');
    if (validMove == ' ') {
        deletePlayer(save->player);
        save->player.y -= 2;
        save->movement[save->level - 1]++;
    } else if (validMove == '2') {
        y = save->player.y - 2;
        x = save->player.x;
        validMove = checkChar(y, x, game, 'u');
        if (validMove == ' ') {
            moveBlock(save, game, y - 2, x, y, x);
            if (gotBlock(y - 2, x, game)) {
                save->blocked++;
            }
            deletePlayer(save->player);
            save->player.y -= 2;
            save->movement[save->level - 1]++;
        }
        foundPos(save, game, y - 2, x);
    }
};


void moveDown(gameState *save, WINDOW *game) {
    int y, x;
    char validMove;
    validMove = checkChar(save->player.y, save->player.x, game, 'd');
    if (validMove == ' ') {
        deletePlayer(save->player);
        save->player.y += 2;
        save->movement[save->level - 1]++;
    } else if (validMove == '2') {
        y = save->player.y + 2;
        x = save->player.x;
        validMove = checkChar(y, x, game, 'd');
        if (validMove == ' ') {
            moveBlock(save, game, y + 2, x, y, x);
            if (gotBlock(y + 2, x, game)) {
                save->blocked++;
            }
            deletePlayer(save->player);
            save->player.y += 2;
            save->movement[save->level - 1]++;
        }
        foundPos(save, game, y + 2, x);
    }
};

void moveLeft(gameState *save, WINDOW *game) {
    int y, x;
    char validMove;
    validMove = checkChar(save->player.y, save->player.x, game, 'l');
    if (validMove == ' ') {
        deletePlayer(save->player);
        save->player.x -= 2;
        save->movement[save->level - 1]++;
    } else if (validMove == '2') {
        y = save->player.y;
        x = save->player.x - 2;
        validMove = checkChar(y, x, game, 'l');
        if (validMove == ' ') {
            moveBlock(save, game, y, x - 2, y, x);
            if (gotBlock(y, x - 2, game)) {
                save->blocked++;
            }
            deletePlayer(save->player);
            save->player.x -= 2;
            save->movement[save->level - 1]++;
        }
        foundPos(save, game, y, x - 2);
    }
};

void moveRight(gameState *save, WINDOW *game) {
    int y, x;
    char validMove;
    validMove = checkChar(save->player.y, save->player.x, game, 'r');
    if (validMove == ' ') {
        deletePlayer(save->player);
        save->player.x += 2;
        save->movement[save->level - 1]++;
    } else if (validMove == '2') {
        y = save->player.y;
        x = save->player.x + 2;
        validMove = checkChar(y, x, game, 'r');
        if (validMove == ' ') {
            moveBlock(save, game, y, x + 2, y, x);
            if (gotBlock(y, x + 2, game)) {
                save->blocked++;
            }
            deletePlayer(save->player);
            save->player.x += 2;
            save->movement[save->level - 1]++;
        }
        foundPos(save, game, y, x + 2);
    }
};

int gotBlock(int y, int x, WINDOW *game) {
    int blocks = 0;
    if (checkChar(y, x, game, 'u') == 'x' || (checkChar(y, x, game, 'd') == 'x')) {
        if (checkChar(y, x, game, 'r') == 'x' || (checkChar(y, x, game, 'l') == 'x')) {
            blocks = 1;
        }
    }
    return blocks;
}


int randomPositionsGenerator(char *txt) {
    int randxy, aleatorios = 0;
    for (int i = 0; i < 259; i++) {
        if (txt[i] == '2') {
            aleatorios++;
        }
    }
    for (int i = 0; i < aleatorios; i++) {
        do {
            randxy = rand() % 250;
        } while (txt[randxy] != '0');
        txt[randxy] = '*';
    }
    return aleatorios;
}

int movePlayer(gameState *save, WINDOW *game) {
    int option = ERR;
    time_t initialTime = clock();
    while (option == ERR && (clock() - initialTime)/CLOCKS_PER_SEC < 1) {
        option = wgetch(game);
    }
    switch (option) {
        case KEY_UP:
            moveUp(save, game);
        break;
        case KEY_DOWN:
            moveDown(save, game);
        break;
        case KEY_LEFT:
            moveLeft(save, game);
        break;
        case KEY_RIGHT:
            moveRight(save, game);
        break;
        default:
        break;
    }
    createPlayer(save->player);
    return (int)option;
}

void startCharacter(character *player, WINDOW *gameWindow) {
    player->x = 1;
    player->y = 1;
    player->curwin = gameWindow;
    player->character = '+';
    getmaxyx(gameWindow, player->height, player->width);
}

void blockCreator(char letter, WINDOW *game) {
    int x, y;
    if (letter == '0') {
        letter = ' ';
    }
    getyx(game, y, x);
    waddch(game, letter);
    waddch(game, letter);
    mvwaddch(game, y + 1, x, letter);
    waddch(game, letter);
    wmove(game, y, x + 2);
}

void createLevel (gameState *save) {
    FILE * textFile;
    if (save->level == 1) {
        textFile = fopen("nivel1.txt", "r");
    } else if (save->level == 2) {
        textFile = fopen("nivel2.txt", "r");
    } else {
        textFile = fopen("nivel3.txt", "r");
    }
    if (textFile == NULL) {
        printf("erro na abertura do arquivo texto");
    } else {
        for (int i = 0; i < 259; i++) {
            save->scenario[i] = fgetc(textFile);
        }
        randomPositionsGenerator(save->scenario);
    }
    fclose(textFile);
}

void setWindow(WINDOW* game, gameState *save) {
    int x, y, j = 0, k = 0;
    mvwinch(game, 0, 0);
    for (int i = 0; i < 259; i++) {
        if (save->scenario[i] == 'x' || save->scenario[i] == '0' || save->scenario[i] == '2') {
            blockCreator(save->scenario[i], game);
        } else if (save->scenario[i] == '\n') {
            getyx(game, y, x);
            wmove(game, y + 2, 0);
        } else if (save->scenario[i] == '1') {
            getyx(game, save->player.y, save->player.x);
            blockCreator('+', game);
            save->scenario[i] = '0';
        } else if (save->scenario[i] == '*') {
            getyx(game, save->positions[j].y, save->positions[j].x);
            save->positions[j].found = 0;
            j++;
            blockCreator('0', game);
        } else if (save->scenario[i] == '2') {
            k++;
            blockCreator(save->scenario[i], game);
        }
    }
}

void infoUpdate (WINDOW *info, gameState *save) {
    mvwprintw(info, 3, 9, "Level %d", save->level);
    mvwprintw(info, 5, 5, "Found positions:");
    mvwprintw(info, 6, 10, "%d of %d", save->found, save->level + 2);
    if (save->level == 1) {
        mvwprintw(info, 9, 5, "Remaining time: %d", 90 - (int)save->timeSpent[save->level - 1]);
    } else if (save->level == 2) {
        mvwprintw(info, 9, 5, "Remaining time: %d", 60 - (int)save->timeSpent[save->level - 1]);
    } else {
        mvwprintw(info, 9, 5, "Remaining time: %d", 45 - (int)save->timeSpent[save->level - 1]);
    }
    mvwprintw(info, 12, 4, "%s", save->score.name);
    mvwprintw(info, 13, 4, "Score: %.1f", save->score.score);
    mvwprintw(info, 15, 4, "Movements: %d", save->movement[save->level - 1]);
}

void newGame(gameState *save) {
    character player;
    save->level = 1;
    save->found = 0;
    save->blocked = 0;
    for (int i = 0; i < 5; i++) {
        save->positions[i].y = 0;
        save->positions[i].x = 0;
        save->positions[i].found = 0;
        if ( i < 3) {
            save->timeSpent[i] = 0;
            save->movement[i] = 0;
        }
    }
    mvwprintw(stdscr, 23, 3, "PLAYER NAME: ");
    wrefresh(stdscr);
    echo();
    scanw("%s", save->score.name);
    noecho();
    move(23, 0);
    clrtoeol();
    wrefresh(stdscr);
    save->score.score = 0;
    refresh();
    createLevel(save);
    save->player = player;
}
