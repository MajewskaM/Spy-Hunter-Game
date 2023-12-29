#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <time.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

//screen parameters
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define DPI     120.0

//upper table
#define TABLE_HEIGHT    40
#define DIST_X  7
#define DIST_Y  10

//number of pixels to ommit while 
//printing narrower & wider road
#define STEP    20

//state of field & positioning
#define NONE    0
#define ONE     1

//parameters of load screen & pop window
#define LOAD_WIDTH 200
#define LOAD_HEIGHT 150
#define MAX_SAVES       10
#define POP_WINDOW_HEIGHT 200
#define POP_WINDOW_WIDTH 500

//deafult text size
#define TEXT_SIZE       128
//number of bits
#define BITS    8

//useful keys pressed
#define ENTER   -1
#define ESC     -2

//road
#define OFF_ROAD        50
#define ROAD_WIDTH      100
//start lenght of a road without performing any shifts
#define ROAD_LOOP 8
#define DISTANCE_POINTS 50
#define SIDES   3

//break lenght from changing look of the road
#define ISLAND_TIME 1

//player's car & other cars parameters
#define OTHER_CAR_W     20
#define OTHER_CAR_H     30
#define PLAYER_CAR_W    24
#define PLAYER_CAR_H    37
//start position y
#define CAR_Y   150
//numbers of parameters about each car
#define CAR_PARAMETERS  3
#define NEW_CARS_START_BORDER   30

//lenght of one shoot
#define SHOT_SIZE       20
#define SHOT_WIDTH      2
//lenght of distance between shots
#define SHOT_BREAK      20

//points earned by destroying proper car
#define DESTROY_POINTS  100
#define POINTS_CHANGE   300.0
//break for destroying a wrong car
#define POINTS_BREAK    10
//number of positions showed in TOP scores
#define SCORES  10

//number of frames
#define CHANGE_ROAD_FRAMES 200
#define WHITE_CAR_FRAMES 1200
#define BLUE_CAR_FRAMES 900
#define FRAMES 500

//time for which player has infinetely many lives
#define NO_DIE_TIME     10.0
#define BASIC_CHANGE    0.01
#define BASIC_CHANGE_DELTA      0.001
#define TIMES_CHANGE    0.5

//speed values
#define SPEED_CASUAL    1.5
#define SPEED_UP        2.0
#define SPEED_DOWN      1.0

//color values
#define DEPTH   32
#define RGBA    255
#define RMASK   0x00FF0000
#define GMASK   0x0000FF00
#define BMASK   0x000000FF
#define AMASK   0xFF000000


//structures with info about game, road, shots
struct gameInfo {
    int currentScore;
    int carXPos;
    int carYPos;
    int distancePoints;
    double distance;
    double gameTime;

};

struct roadInfo {
    int currentShift;
    int shiftSide;
    int whenNextShift;
    int roadLoop;
    int islandTime;
    int startTimer;
    int roadX;
};

struct shotInfo {
    int shotBreak;
    int shotsToPerform;
};

//colors that can be displayed
struct colors {
    int darkGreen;
    int red;
    int orange;
    int yellow;
    int black;
    int white;
    int blue;
};

struct screenElements {
    SDL_Surface* screen;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* scrtex;
};

struct drawElements {
    SDL_Surface* charset;
    SDL_Surface* playerCar1;
    SDL_Surface* playerCar2;
    SDL_Surface* playerCar3;
    SDL_Surface* enemyCar;
    SDL_Surface* otherCar;
    SDL_Surface* playerActiveCar;
};

struct timeInfo {
    double delta;
    int time1;
    int time2;
    int inPause = NONE;
    int pauseTime2 = NONE;
    int pauseTime1 = NONE;
};

struct speedInfo {
    double speed = SPEED_CASUAL;
    const double casualSpeed = SPEED_CASUAL;
    const double speedingUp = SPEED_UP;
    const double slowingDown = SPEED_DOWN;
};


enum CARS { BLUE_CAR = 1, WHITE_CAR = 2 };

enum GAME_STATE { START_NEW_GAME = 0, PLAYING_GAME = 1, PAUSE_GAME = 2, FINISH_GAME = 3, LOAD_GAME = 4, SAVE_GAME = 5, END_GAME = 6 };

enum ADD_POINTS_STATES { COUNT = 1, NO_COUNT = 2 };

enum SORT_LIST { POINTS = 1, TIME = 2 };

enum ROAD_STATE { WIDER = 1, NARROWER = 2 };

//"window & screen" functions
int prepareGame(screenElements* screen, drawElements* draw, colors* var);
int checkForErrors(screenElements* screen);
void windowSettings(screenElements* screen);
void destroySurfaces(screenElements* screen, drawElements* draw);
//void destroyBMPSurfaces(screenElements* screen);
void updateScreen(screenElements* screen);
int checkLoadedBMP(SDL_Surface* surface, screenElements* screen, drawElements* draw, char text[]);
int BMPGraphics(screenElements* screen, drawElements* draw);

void copyArray(char source[], char destination[], int size);
void fillArrayWithGivenValue(int* array, const int size, int value);
void fillArrayWithGivenValueTwo(int** array, const int size, int value);
void fillArrayWithGivenValueDouble(double* array, const int size, double value);
void sortArray(int* array, const int size);
void sortArrayDouble(double* array, const int size);

void drawTable(SDL_Surface* screen, SDL_Surface* charset, gameInfo game, colors var);
void createColorVariables(SDL_Surface* screen, colors* var);

//"starting new game" functions
void startNewGame(int* roadArray, int** carsArray, const int size);
void newGameInfoParameters(gameInfo* game, roadInfo* road, shotInfo* shot, drawElements* draw);

//"shots" & "other cars" functions
void generateShot(int* shots, const int size, gameInfo game, shotInfo* shot, int* deleteShot);
void checkDestroyingCars(int* shotArray, int** carsArray, const int size, int* carType, int* deleteShot);
void checkGenratingNewCar(int** carsArray, const int size, gameInfo* game, roadInfo* road, colors var);
void generateCar(int** carsArray, const int size, gameInfo* game, CARS carType, colors var, int newPosition);

//"road loop" functions
int checkIfCarIsOffRoad(int* roadArray, const int size, gameInfo* game, int distanceOff);
void positionCarCenter(gameInfo* game);
void changeCar(drawElements* draw, gameInfo* game);
void handleEvents(timeInfo time, shotInfo* shot, speedInfo* speed, int* carX, int* state);
void changeGameTime(timeInfo* time, gameInfo* game, double speed);

//"visualizating road" funcitions
void applyRoad(int* roadArray, const int size, SDL_Surface* screen, colors var);
void applyShots(int* shotsArray, const int size, SDL_Surface* screen, colors var);
void applyCars(int** carsArray, const int size, SDL_Surface* screen, colors var, drawElements* draw);

void checkForNewShift(gameInfo* game, roadInfo* road);
void moveRoad(int* roadArray, const int size, roadInfo* road);
void moveCars(int** carsArray, const int size, roadInfo* road);
void addDistanceScore(gameInfo* game, roadInfo road, double currentSpeed, int* pointsType);

//"loading" & "saving" game functions
int loadingGameScreen(screenElements* screen, drawElements* draw, int* state, colors var, char* file, int* inPause);
int chooseSavedGame(int* options, int* currentOption, int* state);
void drawSavesList(SDL_Surface* screen, SDL_Surface* charset, colors var, int* options, int currentOption, char fileName[]);
void saveGameToFile(int* roadArray, const int roadSize, int** carsArray, const int carsSize,
    gameInfo* game, roadInfo* road);
void loadGameFromFile(int* roadArray, const int roadSize, int** carsArray, const int carsSize,
    gameInfo* game, roadInfo* road, char fileName[]);

//pausing a game
void pauseGame(int* state, int* inPause);

//"finishing game screen" functions
void endOfGame(screenElements* screen, drawElements* draw, gameInfo* game, int* state, colors var, int* inPause);
void handleEventsEndGame(screenElements* screen, drawElements* draw, colors var, int* state);
void saveResults(gameInfo* game);
void showTopScores(screenElements* screen, drawElements* draw, colors var, int type, int* state);
void drawTopScores(screenElements* screen, drawElements* draw, int* pointsArray,
    double* timeArray, int type, colors var, char txt[], int* state);

//"drawing" functions
void drawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
void drawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
void drawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
void drawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
void drawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
void drawCar(SDL_Surface* screen, SDL_Surface* sprite, int carX, int carY);

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv) {

    //creating game structures 
    //according to road, shots, cars, printing colors, screen, sprites
    gameInfo currentGame;
    roadInfo currentRoad;
    shotInfo currentShot;
    timeInfo currentTime;
    speedInfo currentSpeed;
    colors current;
    screenElements surfaces;
    drawElements drawSurfaces;


    char file[TEXT_SIZE];
    int pointsState = NONE, carXchange = NONE, gameState = START_NEW_GAME, deleteShot = NONE;

    //arrays sizes
    const int roadSize = SCREEN_HEIGHT - TABLE_HEIGHT, shotSize = roadSize - CAR_Y - (PLAYER_CAR_H / 2), carsSize = roadSize;
    double times = NONE;
    //the speed of changes on the screen - "global" speed of moving elements
    const double changes = BASIC_CHANGE;

    //checking if game if well prepared
    //whether there occured some errors, we close the program
    if (prepareGame(&surfaces, &drawSurfaces, &current)) return ONE;

    //creating arrays with information about road, shots and cars 
    int* roadStatus = new int[roadSize];
    int* shotStatus = new int[shotSize];


    int** carsStatus = new int* [carsSize];

    for (int i = 0; i < carsSize; i++) {

        // Declare a memory block
        carsStatus[i] = new int[CAR_PARAMETERS];
    }

    //filling arrays with 0
    fillArrayWithGivenValueTwo(carsStatus, carsSize, NONE);
    fillArrayWithGivenValue(roadStatus, roadSize, NONE);
    fillArrayWithGivenValue(shotStatus, shotSize, NONE);

    //starting counting time
    currentTime.time1 = SDL_GetTicks();

    while (gameState != END_GAME) {

        //performing functions according to actual game state
        switch (gameState) {

        case START_NEW_GAME:
            startNewGame(roadStatus, carsStatus, carsSize);
            newGameInfoParameters(&currentGame, &currentRoad, &currentShot, &drawSurfaces);
            //deleting shots!
            fillArrayWithGivenValue(shotStatus, shotSize, NONE);
            carXchange = NONE;
            gameState = PLAYING_GAME;
            break;

        case PLAYING_GAME:

            changeGameTime(&currentTime, &currentGame, currentSpeed.speed);

            //draw black screen
            SDL_FillRect(surfaces.screen, NULL, current.black);

            //looking if car is on the road - then we can add points normally
            if (checkIfCarIsOffRoad(roadStatus, roadSize, &currentGame, NONE) == NONE) {
                addDistanceScore(&currentGame, currentRoad, currentSpeed.speed, &pointsState);
            }

            //looking whether car is to far from the road
            if (checkIfCarIsOffRoad(roadStatus, roadSize, &currentGame, OFF_ROAD)) {
                if (currentGame.gameTime <= NO_DIE_TIME) changeCar(&drawSurfaces, &currentGame);
                else gameState = FINISH_GAME;
            }

            checkDestroyingCars(shotStatus, carsStatus, shotSize, &pointsState, &deleteShot);

            checkForNewShift(&currentGame, &currentRoad);

            //changes determines how fast we are doing the changes to our screen
            //moving all elements on the road
            if (currentGame.distance >= changes) {
                times = currentSpeed.speed;
                //how many times we need to move arrays, depending on the current speed
                while (times) {
                    moveRoad(roadStatus, roadSize, &currentRoad);
                    times -= TIMES_CHANGE;
                }
                generateShot(shotStatus, shotSize, currentGame, &currentShot, &deleteShot);
                moveCars(carsStatus, carsSize, &currentRoad);
                currentGame.distance -= changes;
            }


            //apply all road elements - visualisation of: road shifts, shots, cars
            applyRoad(roadStatus, roadSize, surfaces.screen, current);
            applyShots(shotStatus, shotSize, surfaces.screen, current);
            applyCars(carsStatus, carsSize, surfaces.screen, current, &drawSurfaces);

            drawTable(surfaces.screen, drawSurfaces.charset, currentGame, current);
            drawCar(surfaces.screen, drawSurfaces.playerActiveCar, currentGame.carXPos, currentGame.carYPos);

            updateScreen(&surfaces);

            checkGenratingNewCar(carsStatus, carsSize, &currentGame, &currentRoad, current);

            handleEvents(currentTime, &currentShot, &currentSpeed, &carXchange, &gameState);

            currentGame.carXPos += carXchange;
            currentRoad.whenNextShift++;

            break;

        case PAUSE_GAME:
            pauseGame(&gameState, &(currentTime.inPause));
            break;

        case FINISH_GAME:
            endOfGame(&surfaces, &drawSurfaces, &currentGame, &gameState, current, &(currentTime.inPause));
            break;

        case LOAD_GAME:

            //check if we choosed a game to load
            if (loadingGameScreen(&surfaces, &drawSurfaces, &gameState, current, file, &(currentTime.inPause)) == ONE) {
                loadGameFromFile(roadStatus, roadSize, carsStatus, carsSize, &currentGame, &currentRoad, file);
                //deleting shots!
                fillArrayWithGivenValue(shotStatus, shotSize, NONE);
            }
            gameState = PLAYING_GAME;
            break;

        case SAVE_GAME:
            saveGameToFile(roadStatus, roadSize, carsStatus, carsSize, &currentGame, &currentRoad);
            gameState = PLAYING_GAME;
            break;

        case END_GAME:
            break;

        };

    };

    destroySurfaces(&surfaces, &drawSurfaces);
    // Delete the arrays created
    delete[] roadStatus;
    delete[] shotStatus;


    for (int i = 0; i < carsSize; i++)
        delete[] carsStatus[i];
    delete[] carsStatus;



    SDL_Quit();
    return NONE;
};


//handling of events (if there were any)
//performing appropriate action
void handleEvents(timeInfo time, shotInfo* shot, speedInfo* speed, int* carX, int* state) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            //depending on pressed key, performing actions
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) *state = END_GAME;
            else if (event.key.keysym.sym == SDLK_UP) speed->speed = speed->speedingUp;
            else if (event.key.keysym.sym == SDLK_DOWN) speed->speed = speed->slowingDown;
            //just in case delta will be 0, we need add minimum value of change!
            else if (event.key.keysym.sym == SDLK_LEFT) *carX = -DPI * (time.delta + BASIC_CHANGE);
            else if (event.key.keysym.sym == SDLK_RIGHT) *carX = DPI * (time.delta + BASIC_CHANGE);
            else if (event.key.keysym.sym == SDLK_n) *state = START_NEW_GAME;
            else if (event.key.keysym.sym == SDLK_p) *state = PAUSE_GAME;
            else if (event.key.keysym.sym == SDLK_SPACE) shot->shotsToPerform++;
            else if (event.key.keysym.sym == SDLK_s) *state = SAVE_GAME;
            else if (event.key.keysym.sym == SDLK_l) *state = LOAD_GAME;
            else if (event.key.keysym.sym == SDLK_f) *state = FINISH_GAME;
            break;
        case SDL_KEYUP:
            speed->speed = speed->casualSpeed;
            *carX = NONE;
            break;
        case SDL_QUIT:
            *state = END_GAME;
            break;
        };
    };
}


//chaging the passing game, time, and counting passed distance
void changeGameTime(timeInfo* time, gameInfo* game, double speed) {
    //get time ticks
    (*time).time2 = SDL_GetTicks();

    //here t2-t1 is the time in milliseconds since
    //the last screen was drawn
    //delta is the same time in seconds
    (*time).delta = (((*time).time2 - (*time).time1) + (*time).inPause) * BASIC_CHANGE_DELTA;
    (*time).time1 = (*time).time2;
    (*time).inPause = NONE;

    //storing information about current game in structure
    game->gameTime += (*time).delta;
    game->distance += speed * (*time).delta;

}


//checking if we have shot a car
//depending on destroyed car changing counting points in proper way
void checkDestroyingCars(int* shotArray, int** carsArray, const int size, int* points, int* deleteShot) {

    int shotPos, carPos, carSize, color = NONE, shotSize = NONE;

    //looking for positions of corresponding elements in both arrays
    for (int i = 0; i < size; i++) {
        carPos = carsArray[i][1];
        carSize = carsArray[i][2];
        color = carsArray[i][0];

        //checking if we have car on i-th position
        if (color != NONE) {

            //check every "row" of the car height for possible shot
            for (int j = 0; j <= carSize * 2; j++) {
                //in case to not go to far in array
                if (i + j >= size) break;
                shotPos = shotArray[i + j];

                //we are looking if shot in corresponding hight position is shooting a car
                if (shotPos >= carPos && shotPos <= carPos + OTHER_CAR_W) {

                    //depending on destroyed car we change counting points
                    if (color == WHITE_CAR) *points = NO_COUNT;
                    else if (color == BLUE_CAR) *points = COUNT;

                    //car now shold not be displayed
                    carsArray[i][0] = NONE;
                    carsArray[i][1] = NONE;
                    carsArray[i][2] = NONE;
                    shotSize = NONE;

                    //we also need to delete the shot
                    while (shotSize <= SHOT_SIZE) {
                        shotArray[i + j + shotSize] = NONE;
                        shotSize++;

                        //if the "killing shot" hasnt been printed yet
                        //we also need to remember to stop printing it
                        if (i + j + shotSize >= size) {
                            *deleteShot = ONE;
                            break;
                        }
                    }
                    break;
                }
            }
        }

    }

}


//we are changing current player's car to next BMP graphics
//after that position car to the center
void changeCar(drawElements* draw, gameInfo* game) {
    if ((*draw).playerActiveCar == (*draw).playerCar3) {
        (*draw).playerActiveCar = (*draw).playerCar1;
        positionCarCenter(game);
    }
    else if ((*draw).playerActiveCar == (*draw).playerCar2) {
        (*draw).playerActiveCar = (*draw).playerCar3;
        positionCarCenter(game);
    }
    else if ((*draw).playerActiveCar == (*draw).playerCar1) {
        (*draw).playerActiveCar = (*draw).playerCar2;
        positionCarCenter(game);
    }
}


//showing the end of game screen with possible other options to display or perform
void endOfGame(screenElements* screen, drawElements* draw, gameInfo* game, int* state, colors var, int* inPause) {

    int pauseTime1, pauseTime2;
    //counting the time "lost" for event
    pauseTime1 = SDL_GetTicks();

    *state = FINISH_GAME;

    //buffer for printing text
    char buffer[TEXT_SIZE];
    //distances for printing
    int distanceY = SCREEN_HEIGHT / 2 - POP_WINDOW_HEIGHT / 2, distanceX = SCREEN_WIDTH / 2 - POP_WINDOW_WIDTH / 2;

    saveResults(game);

    //drawing "end of game" screen
    drawRectangle((*screen).screen, distanceX, distanceY, POP_WINDOW_WIDTH, POP_WINDOW_HEIGHT / 2, var.yellow, var.orange);

    distanceY += STEP / 2;
    sprintf(buffer, "GAME OVER");
    drawString((*screen).screen, (*screen).screen->w / 2 - strlen(buffer) * BITS / 2, distanceY, buffer, (*draw).charset);
    distanceY += STEP;

    sprintf(buffer, "(n) - start new game");
    drawString((*screen).screen, (*screen).screen->w / 2 - strlen(buffer) * BITS / 2, distanceY, buffer, (*draw).charset);
    distanceY += STEP / 2;

    sprintf(buffer, "(ESC) - quit");
    drawString((*screen).screen, (*screen).screen->w / 2 - strlen(buffer) * BITS / 2, distanceY, buffer, (*draw).charset);
    distanceY += STEP;

    sprintf(buffer, "(p) - TOP 10 scores (by points)");
    drawString((*screen).screen, (*screen).screen->w / 2 - strlen(buffer) * BITS / 2, distanceY, buffer, (*draw).charset);
    distanceY += STEP / 2;

    sprintf(buffer, "(t) - TOP 10 scores (by most time alive)");
    drawString((*screen).screen, (*screen).screen->w / 2 - strlen(buffer) * BITS / 2, distanceY, buffer, (*draw).charset);
    distanceY += STEP / 2;

    updateScreen(screen);

    handleEventsEndGame(screen, draw, var, state);

    pauseTime2 = SDL_GetTicks();

    //time "in pause"
    //tells us how much time we spended while being in pause
    *inPause = (pauseTime1 - pauseTime2);

}


//handling events in end game screen, allows to show top scores
void handleEventsEndGame(screenElements* screen, drawElements* draw, colors var, int* state) {
    SDL_Event event;

    while (*state == FINISH_GAME) {

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_n) *state = START_NEW_GAME;
                else if (event.key.keysym.sym == SDLK_ESCAPE) *state = END_GAME;
                else if (event.key.keysym.sym == SDLK_p) showTopScores(screen, draw, var, POINTS, state);
                else if (event.key.keysym.sym == SDLK_t) showTopScores(screen, draw, var, TIME, state);
                break;
            case SDL_QUIT:
                *state = END_GAME;
                break;
            }
            break;
        }
    };
}


//preparing an array with top 10 points or times in last 10 games
void showTopScores(screenElements* screen, drawElements* draw, colors var, int type, int* state) {

    int i = NONE;
    //dynamic arrays with top scores
    int* pointScores = new int[SCORES];
    double* timeScores = new double[SCORES];
    fillArrayWithGivenValue(pointScores, SCORES, NONE);
    fillArrayWithGivenValueDouble(timeScores, SCORES, NONE);
    //buffer for window title
    char txt[TEXT_SIZE];

    //filling appropriate array
    if (type == POINTS) {

        //file with saved scores
        FILE* pointFile = fopen("files/TOP_points", "r");

        sprintf(txt, "TOP GAMES BY POINTS");
        i = NONE;

        //checking if the file is well opened
        if (pointFile != NULL) {

            //reading file till the end, getting only last 10 values to array
            while (i < SCORES) {
                if (fread(&pointScores[i], sizeof(int), 1, pointFile) == 1) i++;
                //that means we read whole file
                else break;
                //there are still some results to read
                if (i >= SCORES) i = NONE;
            }

            sortArray(pointScores, SCORES);
            fclose(pointFile);

        }


    }
    else {

        //file with saved scores
        FILE* timeFile = fopen("files/TOP_time", "r");

        sprintf(txt, "TOP GAMES BY TIME");
        i = NONE;

        //checking if the file is well opened
        if (timeFile != NULL) {

            //reading file till the end, getting only last 10 values to array
            while (i < SCORES) {
                if (fread(&timeScores[i], sizeof(double), 1, timeFile) == 1) i++;
                //that means we read whole file
                else break;
                //there are still some results to read
                if (i >= SCORES) i = NONE;
            }
            sortArrayDouble(timeScores, SCORES);
            fclose(timeFile);
        }

    }

    //now we can just draw top scores
    drawTopScores(screen, draw, pointScores, timeScores, type, var, txt, state);

    //deleting allocated memory
    delete[] timeScores;
    delete[] pointScores;

}


//drawing top scores of time or points to the screen, we use information from given arrays
void drawTopScores(screenElements* screen, drawElements* draw, int* pointsArray, double* timeArray,
    int type, colors var, char txt[], int* state) {

    int distanceY = SCREEN_HEIGHT / 2 - LOAD_HEIGHT, distanceX = SCREEN_WIDTH / 2 - LOAD_WIDTH;

    //printing window, and its title
    drawRectangle((*screen).screen, distanceX, distanceY, LOAD_WIDTH * 2, LOAD_HEIGHT * 2, var.yellow, var.orange);
    drawString((*screen).screen, (*screen).screen->w / 2 - strlen(txt) * BITS / 2, distanceY + STEP / 2, txt, (*draw).charset);
    distanceY += STEP * 2;

    for (int i = 0; i < SCORES; i++) {

        //printing double and int values in proper way
        if (type == POINTS) {
            if (pointsArray[i] == NONE) sprintf(txt, "%d. -- ", i + 1);
            else sprintf(txt, "%d.\t %d", i + 1, pointsArray[i]);
        }
        else {
            if (timeArray[i] == NONE) sprintf(txt, "%d. -- ", i + 1);
            else sprintf(txt, "%d.\t %.1lf s", i + 1, timeArray[i]);
        }

        drawString((*screen).screen, (*screen).screen->w / 2 - strlen(txt) * BITS / 2, distanceY, txt, (*draw).charset);
        distanceY += STEP;
    }

    distanceY += STEP;
    sprintf(txt, "(n) - new game, (ESC) - end game");
    drawString((*screen).screen, (*screen).screen->w / 2 - strlen(txt) * BITS / 2, distanceY + STEP / 2, txt, (*draw).charset);

    updateScreen(screen);
    handleEventsEndGame(screen, draw, var, state);
}


//loading all BMP game graphics, returns 1 if something went wrong - there occured some errors
int BMPGraphics(screenElements* screen, drawElements* draw) {

    //loading graphics
    (*draw).charset = SDL_LoadBMP("./cs8x8.bmp");
    if (checkLoadedBMP((*draw).charset, screen, draw, "./cs8x8.bmp")) return ONE;
    SDL_SetColorKey((*draw).charset, true, 0x000000);

    (*draw).playerCar1 = SDL_LoadBMP("./playerCar1.bmp");
    if (checkLoadedBMP((*draw).playerCar1, screen, draw, "./playerCar1.bmp")) return ONE;

    (*draw).playerCar2 = SDL_LoadBMP("./playerCar2.bmp");
    if (checkLoadedBMP((*draw).playerCar2, screen, draw, "./playerCar2.bmp")) return ONE;

    (*draw).playerCar3 = SDL_LoadBMP("./playerCar3.bmp");
    if (checkLoadedBMP((*draw).playerCar3, screen, draw, "./playerCar3.bmp")) return ONE;

    (*draw).enemyCar = SDL_LoadBMP("./enemyCar.bmp");
    if (checkLoadedBMP((*draw).enemyCar, screen, draw, "./enemyCar.bmp")) return ONE;

    (*draw).otherCar = SDL_LoadBMP("./otherCar.bmp");
    if (checkLoadedBMP((*draw).otherCar, screen, draw, "./otherCar.bmp")) return ONE;

    return NONE;
}


//simply sorting given array
void sortArray(int* array, const int size) {
    int temp;
    //then we need to sort an array till i-th element
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++)
        {
            if (array[i] < array[j]) {
                temp = array[j];
                array[j] = array[i];
                array[i] = temp;
            }
        }
    }
}


//sorting array with double values
void sortArrayDouble(double* array, const int size) {
    double temp;
    //then we need to sort an array till i-th element
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++)
        {
            if (array[i] < array[j]) {
                temp = array[j];
                array[j] = array[i];
                array[i] = temp;
            }
        }
    }
}


//saving information about score and time to files
void saveResults(gameInfo* game) {

    //saving current score to file, and current time
    FILE* filePoints = fopen("files/TOP_points", "a+");
    FILE* fileTime = fopen("files/TOP_time", "a+");

    //saving game only if file is well opened!
    if (filePoints != NULL) {
        //writing all informations about ponints scored
        fwrite(&game->currentScore, sizeof(int), 1, filePoints);
        //closing a file
        fclose(filePoints);
    }

    if (fileTime != NULL) {
        //writing information about time alive
        fwrite(&game->gameTime, sizeof(double), 1, fileTime);
        //closing a file
        fclose(fileTime);
    }

}


//displaying a screen from which we can choose one of the previous saved games, 
//return 1 if we have choosen one of them, or 0 if we haven't
//name of choosed file is stored in file array
int loadingGameScreen(screenElements* screen, drawElements* draw, int* state, colors var, char* file, int* inPause) {

    int currentSave = NONE, choosedPosition, numberOfSaves, pauseTime1, pauseTime2;
    //counting the time "lost" for event
    pauseTime1 = SDL_GetTicks();

    *state = LOAD_GAME;
    drawSavesList((*screen).screen, (*draw).charset, var, &numberOfSaves, currentSave, file);

    //determines if we have any saved games and if so we are displaying them
    if (numberOfSaves > NONE) {
        currentSave = ONE;
        choosedPosition = NONE;

        //waiting for action: we choosed position of save or changed state of the game
        while (choosedPosition == NONE && *state == LOAD_GAME) {
            numberOfSaves = NONE;
            //drawing everything to the screen
            drawSavesList((*screen).screen, (*draw).charset, var, &numberOfSaves, currentSave, file);
            updateScreen(screen);
            choosedPosition = chooseSavedGame(&numberOfSaves, &currentSave, state);
        };

        pauseTime2 = SDL_GetTicks();
        //time "in pause"
        //tells us how much time we spended while being in pause
        *inPause = (pauseTime1 - pauseTime2);

        //we choosed a saved game, char array keeps its name
        if (choosedPosition == ENTER) {
            return ONE;
        }
        //otherwise we just pressed ESC or quit the game
        return NONE;

    }



}


//updating changes to the screen
void updateScreen(screenElements* screen) {

    SDL_UpdateTexture((*screen).scrtex, NULL, (*screen).screen->pixels, (*screen).screen->pitch);
    //SDL_RenderClear(renderer);
    SDL_RenderCopy((*screen).renderer, (*screen).scrtex, NULL, NULL);
    SDL_RenderPresent((*screen).renderer);
}


//destroying all surfaces - ending a program
void destroySurfaces(screenElements* screen, drawElements* draw) {

    SDL_FreeSurface((*draw).charset);
    SDL_FreeSurface((*draw).playerCar1);
    SDL_FreeSurface((*draw).playerCar2);
    SDL_FreeSurface((*draw).playerCar3);
    SDL_FreeSurface((*draw).enemyCar);
    SDL_FreeSurface((*draw).otherCar);
    SDL_FreeSurface((*screen).screen);
    SDL_DestroyTexture((*screen).scrtex);
    SDL_DestroyRenderer((*screen).renderer);
    SDL_DestroyWindow((*screen).window);

}


//visualizating road shifts and whole road on the screen
void applyRoad(int* roadArray, const int size, SDL_Surface* screen, colors var) {

    int move, color = var.darkGreen;

    //printing road according to content in array
    for (int i = 0; i < size; i++) {
        move = roadArray[i];
        drawRectangle(screen, NONE, i + TABLE_HEIGHT, SCREEN_WIDTH / 2 - ROAD_WIDTH - move, ONE, color, color);
        drawRectangle(screen, SCREEN_WIDTH - (SCREEN_WIDTH / 2 - ROAD_WIDTH) + move,
            i + TABLE_HEIGHT, SCREEN_WIDTH / 2 - ROAD_WIDTH - move, ONE, color, color);
    }

}


//visualizating shots
void applyShots(int* shotArray, const int size, SDL_Surface* screen, colors var) {

    int color = var.yellow;
    for (int i = 0; i < size; i++) {

        if (shotArray[i] != NONE) {
            //we need to generate a shot on the screen
            drawRectangle(screen, shotArray[i] - 1, TABLE_HEIGHT + i, SHOT_WIDTH, ONE, color, color);
        }
    }

}


//visualizating cars on the road
void applyCars(int** carsArray, const int size, SDL_Surface* screen, colors var, drawElements* draw) {

    int difference = NONE, carType = NONE, carSize = NONE, carPos = NONE, color = NONE;
    SDL_Surface* car;
    for (int i = 0; i < size; i++) {
        carType = carsArray[i][0];
        carPos = carsArray[i][1];
        carSize = carsArray[i][2];

        //depending on car type we prepare its printing parameters
        switch (carType) {

        case BLUE_CAR:
            color = var.blue;
            car = (*draw).enemyCar;
            break;
        case WHITE_CAR:
            color = var.white;
            car = (*draw).otherCar;
            break;

        }

        //determines whether we have a car in i-th position on the screen
        if (carType != NONE) {

            //we are only printing a car if it can be fully displayed on the screen
            if (i < size - (carSize * 2)) {
                drawCar(screen, car, carPos + OTHER_CAR_W / 2, (TABLE_HEIGHT + i) + OTHER_CAR_H / 2);
            }

        }
        //otherwise we do not have a car on current position

    }
}


//preparing all pre-game parameters
//screen, surfaces, colors, and BMP graphics
//returns one if there occured some errorrs
int prepareGame(screenElements* screen, drawElements* draw, colors* var) {

    //check for possible errors connected to screen
    if (checkForErrors(screen)) return ONE;

    //preparing window properties
    windowSettings(screen);

    //creating necessary color variables
    createColorVariables((*screen).screen, var);

    //making cursor not visible
    SDL_ShowCursor(SDL_DISABLE);

    //loading charset and all necessary car graphics from bmp
    if (BMPGraphics(screen, draw)) return ONE;

    //no errors
    return NONE;

}


//checking for possible errors that may occur during generating a game window
int checkForErrors(screenElements* screen) {

    int rc;

    //checking for possible errors that may occur during generating a game window
    //if so we are ending the program
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return ONE;
    }

    //fullscreen mode
    //rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
    rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
        &(*screen).window, &(*screen).renderer);

    if (rc != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return ONE;
    };


    return NONE;
}


//setting window properties
void windowSettings(screenElements* screen) {

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize((*screen).renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor((*screen).renderer, NONE, NONE, NONE, RGBA);
    SDL_SetWindowTitle((*screen).window, "Spy Hunter - Martyna Majewska");

    (*screen).screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, DEPTH,
        RMASK, GMASK, BMASK, AMASK);

    (*screen).scrtex = SDL_CreateTexture((*screen).renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT);
}


//looking whether car is far from the road depending on given distance
//returns value ONE or NONE
int checkIfCarIsOffRoad(int* road, const int roadSize, gameInfo* game, int distanceOff) {

    //positions in array of the player's car, to check how the road looks like there
    int arrPosLow = roadSize - CAR_Y - PLAYER_CAR_H / 2, arrPosUp = roadSize - CAR_Y + PLAYER_CAR_H / 2;
    int difference = PLAYER_CAR_H;

    //posiitons of sides of the player's car
    int leftSide = game->carXPos - PLAYER_CAR_W / 2, rightSide = game->carXPos + PLAYER_CAR_W / 2;
    //road "borders", to know where the road ends
    int xLeft, xRight;

    //checking all pixels of the player's car height and comparing each with road at that point
    while (difference > NONE) {

        //calculating current left right x of the road and applyng given distance
        xLeft = SCREEN_WIDTH / 2 - ROAD_WIDTH - road[arrPosLow + difference - ONE] - distanceOff;
        xRight = road[arrPosLow + difference - 1] + SCREEN_WIDTH / 2 + ROAD_WIDTH + distanceOff;

        //if car is off the road
        if (leftSide < xLeft) return ONE;
        if (rightSide > xRight) return ONE;

        difference--;
    }

    //we passed the loop, car is on the road!
    return NONE;
}


//drawing the upper table with info about game: points, time left, etc...
void drawTable(SDL_Surface* screen, SDL_Surface* charset, gameInfo game, colors var) {

    char text[TEXT_SIZE];
    int distanceX = DIST_X, distanceY = DIST_Y;
    drawRectangle(screen, NONE, NONE, SCREEN_WIDTH, TABLE_HEIGHT, var.yellow, var.orange);

    //print text to screen with given charset
    sprintf(text, "Spy-Hunter-Game");
    //center of the screen
    drawString(screen, screen->w / 2 - strlen(text) * BITS / 2, distanceY, text, charset);

    //elapsed time
    sprintf(text, "TIMER: %.1lf s", game.gameTime);
    //print at left side
    drawString(screen, distanceX, distanceY, text, charset);

    //displaying current scoring
    sprintf(text, "%06d", game.currentScore);
    //print at right side
    drawString(screen, screen->w - strlen(text) * BITS - distanceX, distanceY, text, charset);

    //implemented functions
    sprintf(text, "a, b, c, d, e, f, g");
    //print at right side
    drawString(screen, screen->w - strlen(text) * BITS - distanceX, SCREEN_HEIGHT - (2 * STEP), text, charset);

    sprintf(text, "h, i, j, k, m, o");
    //print at right side
    drawString(screen, screen->w - strlen(text) * BITS - distanceX, SCREEN_HEIGHT - STEP, text, charset);
}


//creating variables that are responsible for printing colours
void createColorVariables(SDL_Surface* screen, colors* var) {
    var->darkGreen = SDL_MapRGB(screen->format, 0x4c, 0xaf, 0x50);
    var->red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
    var->orange = SDL_MapRGB(screen->format, 0xFF, 0x4c, 0x00);
    var->yellow = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
    var->black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    var->white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
    var->blue = SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF);
}


//adding score in appriopriate way scored by distance
void addDistanceScore(gameInfo* game, roadInfo road, double currentSpeed, int* pointsType) {

    //static value depended on value of speed, lenght of break if we destroyed wrong car
    static int points = NONE, pointsBreak = NONE;

    //destroying wrong car starts points break
    if (*pointsType == NO_COUNT && pointsBreak == NONE) {
        pointsBreak = POINTS_BREAK;
    }

    //we are looking if we can add points
    if (pointsBreak > NONE) {
        pointsBreak--;
        points -= POINTS_CHANGE;
        //the break ended, we need to change state of additional points
        if (pointsBreak == NONE) *pointsType = NONE;
    }
    //we are checking if enough loops passed and we can add distance points
    //value depends on current speed
    else if (points >= (POINTS_CHANGE / currentSpeed) && *pointsType != NO_COUNT) {
        points -= (POINTS_CHANGE / currentSpeed);
        game->currentScore += DISTANCE_POINTS;

        //that means we killed a car and we need to add additional points
        if (*pointsType == COUNT) {
            if (pointsBreak == NONE) {
                game->currentScore += DESTROY_POINTS;
            }
            *pointsType = NONE;
        }
    }

    //points are depending on the speed of car
    //so if road speed is greater the value added to points is grater
    points += currentSpeed;

}


//we are checking if appriopriate amount of frames passed to generate new car if so we are generating new one
void checkGenratingNewCar(int** carsArray, const int size, gameInfo* game, roadInfo* road, colors var) {

    //start position
    static int position = ONE;
    int newPos = NONE, startShift = NEW_CARS_START_BORDER, currRoadWidthL, currRoadWidthR;

    //choosing different position for newly created car
    if (road->whenNextShift % FRAMES == NONE) position++;

    //looking what are the "borders" of our current road in first position on the screen
    currRoadWidthL = SCREEN_WIDTH / 2 - ROAD_WIDTH - road->roadX + STEP;
    currRoadWidthR = SCREEN_WIDTH - (SCREEN_WIDTH / 2 - ROAD_WIDTH) + road->roadX - STEP;

    //getting different cars positions
    if (position % SIDES == NONE) newPos = currRoadWidthR - startShift;
    else if (position % SIDES == ONE) newPos = currRoadWidthL + startShift;
    else
    {
        //we start from center
        //also checking if we should go right or left
        if (position % (SIDES - ONE) == NONE) newPos = SCREEN_WIDTH / 2 + startShift;
        else newPos = SCREEN_WIDTH / 2 - startShift;
    }

    //looking if generated new car position is located on the road
    //generates a car depending if the given amount of frames passed
    if (newPos > currRoadWidthL && newPos < currRoadWidthR) {
        //generating casual white
        if (road->whenNextShift % WHITE_CAR_FRAMES == NONE) {
            generateCar(carsArray, size, game, WHITE_CAR, var, newPos);
        }
        //generating blue car
        else if (road->whenNextShift % BLUE_CAR_FRAMES == NONE) {
            generateCar(carsArray, size, game, BLUE_CAR, var, newPos);
        }
    }

}


//we are assinging parameters about new car in the cars array
void generateCar(int** carsArray, const int size, gameInfo* game, CARS carType, colors var, int newPosition) {

    //car size is actually the height of the car devided by 2
    int carSize = OTHER_CAR_H / 2, color = NONE;

    //assinging values by given car type
    switch (carType) {

    case BLUE_CAR:
        color = BLUE_CAR;
        break;
    case WHITE_CAR:
        color = WHITE_CAR;
        break;

    }

    //car color
    carsArray[carSize - ONE][0] = color;

    //position on road
    carsArray[carSize - ONE][1] = newPosition;

    //car size
    carsArray[carSize - ONE][2] = carSize;

}


//provide an information about current shifts on the road, checks for a new shift (NARROWER OR WIDER)
void checkForNewShift(gameInfo* game, roadInfo* road) {


    if (road->currentShift == NONE) {

        //we can look for next shift, depending on the frames
        if (road->whenNextShift % CHANGE_ROAD_FRAMES == NONE) {

            //apply lenght of current shift
            road->currentShift = STEP;

            //what should be the next shift wider or narrower
            if (road->shiftSide == NARROWER) {
                //checks if we have to perform any changes
                if (road->roadLoop > NONE) {
                    road->roadLoop--;
                }
                else {
                    road->shiftSide = WIDER;
                    road->roadLoop = ROAD_LOOP;
                    road->islandTime = ISLAND_TIME;
                    road->startTimer = NONE;
                }
            }
            //wider road mode
            else {
                //checks if we have to perform any changes
                if (road->roadLoop > NONE) {
                    //last loop of changes
                    if (road->roadLoop == ONE) {
                        //we stop the changes
                        road->startTimer = ONE;
                        if (road->islandTime > NONE) {
                            //starting a stop operation
                            road->islandTime--;
                        }
                        //we can start shift again, stop ended
                        else {
                            road->startTimer = NONE;
                            road->roadLoop--;
                        }
                        road->currentShift = NONE;
                    }
                    else {
                        road->roadLoop--;
                    }

                }
                else {
                    road->shiftSide = NARROWER;
                    road->roadLoop = ROAD_LOOP - ONE;

                }
            }

        }
    }
}


//preparing start parameters of the game structures
//they are according to road, game, shots and current player car
void newGameInfoParameters(gameInfo* game, roadInfo* road, shotInfo* shot, drawElements* draw) {

    game->currentScore = NONE;
    game->carXPos = SCREEN_WIDTH / 2;
    game->carYPos = SCREEN_HEIGHT - CAR_Y;
    game->gameTime = NONE;
    game->distancePoints = NONE;
    game->distance = NONE;

    road->currentShift = NONE;
    road->shiftSide = NARROWER;
    road->roadLoop = CAR_PARAMETERS;
    road->whenNextShift = ONE;
    road->islandTime = ONE;
    road->startTimer = NONE;

    shot->shotBreak = NONE;
    shot->shotsToPerform = NONE;

    //choose first player's car
    draw->playerActiveCar = draw->playerCar1;

}


//positioning car on the center of the screen
void positionCarCenter(gameInfo* game) {
    game->carXPos = SCREEN_WIDTH / 2;
}


//checks if there is still a shot to perform and if so put information about its position in the array
void generateShot(int* shots, const int size, gameInfo game, shotInfo* shot, int* deleteShot) {

    //array tell us about the place where to pring shoots, on which position on screen
    static int shotSize = SHOT_SIZE, shotBreak = NONE, lastPos = game.carXPos, newPos = ONE;

    //move fields by one field upwards
    for (int i = ONE; i < size; i++) shots[i - ONE] = shots[i];

    if (*deleteShot == ONE) {
        shotSize = NONE;
        shot->shotsToPerform--;
        shotBreak = SHOT_BREAK;
        *deleteShot = NONE;
    }
    else {
        //we need to also generate new field - shot[0] depending on player's car position
//checking if we still have some shots to perform, or there is a need to still print last one
        if (shotSize > NONE && shot->shotsToPerform > NONE) {
            shotSize--;

            //we need to get new position
            if (newPos == ONE) {
                lastPos = game.carXPos;
                newPos = NONE;
            }
            //we eneded printing a shot
            if (shotSize == NONE) {
                shot->shotsToPerform--;
                shotBreak = SHOT_BREAK;
            }
            shots[size - ONE] = lastPos;
        }
        //we can not print a shot, we need to print a break
        else if (shotBreak > NONE) {
            shotBreak--;

            //the break ended, we can start printing a new shot in "queue"
            if (shotBreak == NONE) {
                shotSize = SHOT_SIZE;
                newPos = ONE;
            }
            //we are not printing a shot
            shots[size - ONE] = NONE;
        }
        //otherwise we are also not printing a shot
        else shots[size - ONE] = NONE;

    }

}


//starting new game - filling arrays with 0
void startNewGame(int* roadArray, int** carsArray, const int size) {

    fillArrayWithGivenValue(roadArray, size, NONE);
    fillArrayWithGivenValueTwo(carsArray, size, NONE);

}


//moving all cars - fields in array one lower - moving cars on the road
void moveCars(int** carsArray, const int size, roadInfo* road) {

    //moving all fields one downwards
    for (int i = size - 2; i >= 0; i--) {
        for (int j = 0; j < CAR_PARAMETERS; j++) {
            carsArray[i + 1][j] = carsArray[i][j];
        }
    }

    //new field is empty
    carsArray[0][0] == NONE;
    carsArray[0][1] == NONE;
    carsArray[0][2] == NONE;

}


//putting all fields one field lower - moving the road
void moveRoad(int* roadArray, const int size, roadInfo* road) {

    //move fields by one downwards
    for (int i = size - 2; i >= NONE; i--) roadArray[i + ONE] = roadArray[i];

    //the change will be wider or narrow
    //we need to generate new field - array[0]
    //also store info about last field
    int lastField = roadArray[ONE];

    //looking if we still need to display some shift
    //and if the timer is not telling to stop for a while
    if (road->currentShift > NONE && road->startTimer == NONE) {

        //looking what shift to perform
        if (road->shiftSide == WIDER) {
            //we are adding current shift to both sides of the road
            roadArray[NONE] = lastField + road->currentShift;
        }
        else if (road->shiftSide == NARROWER) {
            //we are substract current shift from both sides of the road
            roadArray[NONE] = lastField - road->currentShift;
        }
        //there is no other shift to perform
        road->currentShift = NONE;

    }
    //no change in next field, we are stopping for a while
    else {
        roadArray[NONE] = lastField;
    }

    //storing information about current appearance of first field on the road
    road->roadX = roadArray[NONE];

}


//saving game to a file, using current structures and informations from arrays about: road & other cars
void saveGameToFile(int* roadArray, const int roadSize, int** carsArray, const int carsSize,
    gameInfo* game, roadInfo* road) {

    //store information about current year, month, day, hour, minute and second
    time_t n = time(NULL);
    struct tm* now = localtime(&n);
    char buffer[TEXT_SIZE];
    //put the information to the buffer in proper way
    strftime(buffer, TEXT_SIZE, "%Y %m %d %H %M %S", now);

    char filePath[TEXT_SIZE];
    const char* folderPath = "files/saves/";

    strcpy(filePath, folderPath);
    strcat(filePath, buffer);

    //crating a file with saved current game status
    FILE* file = fopen(filePath, "w");

    //saving game only if file is well opened!
    if (file != NULL) {

        //adding this save to the list of all saves
        FILE* fileS = fopen("files/saves/ListOfSaves", "a+");

        //write the name of save to file with all saves names
        //only if file is well opened!
        if (fileS != NULL) {
            fwrite(buffer, sizeof(char), TEXT_SIZE, fileS);
            fclose(fileS);
        }

        //writing all informations about: game, road, and arrays to file
        fwrite(&game->currentScore, sizeof(int), 1, file);
        fwrite(&game->carXPos, sizeof(int), 1, file);
        fwrite(&game->carYPos, sizeof(int), 1, file);
        fwrite(&game->distancePoints, sizeof(int), 1, file);
        fwrite(&game->gameTime, sizeof(double), 1, file);
        fwrite(&game->distance, sizeof(double), 1, file);

        //road informations
        fwrite(&road->currentShift, sizeof(int), 1, file);
        fwrite(&road->islandTime, sizeof(int), 1, file);
        fwrite(&road->roadLoop, sizeof(int), 1, file);
        fwrite(&road->shiftSide, sizeof(int), 1, file);
        fwrite(&road->startTimer, sizeof(int), 1, file);
        fwrite(&road->whenNextShift, sizeof(int), 1, file);

        //road & other cars arrays with informations about them
        fwrite(roadArray, sizeof(int), roadSize, file);

        for (int i = 0; i < carsSize; i++) {
            fwrite(carsArray[i], sizeof(int), CAR_PARAMETERS, file);
        }

        //closing a file
        fclose(file);
    }

}


//drawing load screen with list of saves, showing current choosed option
void drawSavesList(SDL_Surface* screen, SDL_Surface* charset, colors var,
    int* options, int currentOption, char fileName[]) {

    //buffer for keeping current file name
    char buffer[TEXT_SIZE];
    int distanceY = SCREEN_HEIGHT / 2 - LOAD_HEIGHT, distanceX = SCREEN_WIDTH / 2 - LOAD_WIDTH, n = 0;
    *options = NONE;

    //opening file with names of current saves
    FILE* fileS = fopen("files/saves/ListOfSaves", "r");

    //drawing load screen
    drawRectangle(screen, distanceX, distanceY, LOAD_WIDTH * 2, LOAD_HEIGHT * 2, var.orange, var.orange);
    sprintf(buffer, "SAVED GAMES (choose - PRESS SPACE)");
    drawString(screen, screen->w / 2 - strlen(buffer) * BITS / 2, distanceY + STEP / 2, buffer, charset);
    distanceY += STEP / 2;

    //checking if file is well opened - if we can open this file
    if (fileS != NULL) {

        //reading file till the end or to max possibvle number of saves that can be displayed
        while (fread(buffer, sizeof(char), TEXT_SIZE, fileS) == TEXT_SIZE && n != MAX_SAVES) {
            n++;
            distanceY += STEP;
            (*options)++;

            //draw current choosed option in differrent color
            if (*options == currentOption) {
                drawRectangle(screen, distanceX, distanceY - STEP / 2, LOAD_WIDTH * 2, STEP, var.red, var.red);
                //getting current choosed file name
                copyArray(buffer, fileName, TEXT_SIZE);
            }
            //drawing file name to the screen - list of saves
            drawString(screen, distanceX + STEP / 2, distanceY, buffer, charset);
        }
        //closing a file
        fclose(fileS);
    }

}


//copy given array - source to destination array 
void copyArray(char source[], char destination[], int size) {
    for (int i = 0; i < size; i++) {
        destination[i] = source[i];
    }
}


//pausing a game and waiting for a key to REASUME
void pauseGame(int* state, int* inPause) {

    int pauseTime1, pauseTime2;
    //counting the time "lost" for event
    pauseTime1 = SDL_GetTicks();

    *state = PAUSE_GAME;
    SDL_Event event;

    //waiting only for one key - the one that changes value of key
    //loop stops if we RESUMED game
    while (*state == PAUSE_GAME) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_p) *state = PLAYING_GAME;
                break;
            case SDL_QUIT:
                *state = END_GAME;
                break;
            }
            break;
        }
    };

    pauseTime2 = SDL_GetTicks();
    //time "in pause"
    //tells us how much time we spended while being in pause
    *inPause = (pauseTime1 - pauseTime2);
}


//returns option the user choosed while being on the load screen - NONE, ENTER or ESC
int chooseSavedGame(int* options, int* currentOption, int* state) {

    //possible options to choose at the current time!
    static int upOpt = NONE, downOpt = *options - ONE;
    SDL_Event event;

    //when we are starting choosing options we need to "zero" possible upper options
    if (*currentOption == ONE) {
        upOpt = NONE, downOpt = *options - ONE;
    }

    //looking what key the user pressed
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) return ESC;
            //space - we choosed currentOption - position - as new file to load
            else if (event.key.keysym.sym == SDLK_SPACE) {
                return ENTER;
            }
            //switch to upper option if possible
            else if (event.key.keysym.sym == SDLK_UP) {
                if (upOpt > NONE) {
                    (*currentOption)--;
                    upOpt--;
                    downOpt++;
                }
            }
            //switch to lower option if possible
            else if (event.key.keysym.sym == SDLK_DOWN) {
                if (downOpt > NONE) {
                    (*currentOption)++;
                    downOpt--;
                    upOpt++;
                }
            }
            break;
        case SDL_QUIT:
            //we need to close whole program!
            *state = END_GAME;
            break;
        }
        break;
    };
    //we have not choosed a save to load
    return NONE;

}


//returning new array and changing current game parameters with data from given file
void loadGameFromFile(int* roadArray, const int roadSize, int** carsArray, const int carsSize,
    gameInfo* game, roadInfo* road, char fileName[]) {

    char filePath[TEXT_SIZE];
    const char* folderPath = "files/saves/";

    strcpy(filePath, folderPath);
    strcat(filePath, fileName);

    //opening file with given file name
    FILE* file = fopen(filePath, "rb");

    //in case our file did not open well - we can not read from it!
    if (file != NULL) {

        //reading all parameters about:
        // 
        //game state, player position, score, time, etc.
        fread(&game->currentScore, sizeof(int), 1, file);
        fread(&game->carXPos, sizeof(int), 1, file);
        fread(&game->carYPos, sizeof(int), 1, file);
        fread(&game->distancePoints, sizeof(int), 1, file);
        fread(&game->gameTime, sizeof(double), 1, file);
        fread(&game->distance, sizeof(double), 1, file);

        //road informatin about current appearance etc.
        fread(&road->currentShift, sizeof(int), 1, file);
        fread(&road->islandTime, sizeof(int), 1, file);
        fread(&road->roadLoop, sizeof(int), 1, file);
        fread(&road->shiftSide, sizeof(int), 1, file);
        fread(&road->startTimer, sizeof(int), 1, file);
        fread(&road->whenNextShift, sizeof(int), 1, file);

        //reading information from arrays
        fread(roadArray, sizeof(int), roadSize, file);

        for (int i = 0; i < carsSize; i++) {
            fread(carsArray[i], sizeof(int), CAR_PARAMETERS, file);
        }

        //we need to close our file
        fclose(file);

    }
}


//filling all fields in array with given value - one dimensional
void fillArrayWithGivenValue(int* array, const int size, int value) {
    for (int i = 0; i < size; i++) {
        array[i] = value;
    }
}


//filling all fields in array with given value - one dimensional, double type
void fillArrayWithGivenValueDouble(double* array, const int size, double value) {
    for (int i = 0; i < size; i++) {
        array[i] = value;
    }
}


//filling all fields in array with given value - two dimensional
void fillArrayWithGivenValueTwo(int** array, const int size, int value) {

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < CAR_PARAMETERS; j++) {
            array[i][j] = value;
        }
    }
}


// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void drawString(SDL_Surface* screen, int x, int y, const char* text,
    SDL_Surface* charset) {
    int px, py, c;
    SDL_Rect s, d;
    s.w = 8;
    s.h = 8;
    d.w = 8;
    d.h = 8;
    while (*text) {
        c = *text & 255;
        px = (c % 16) * 8;
        py = (c / 16) * 8;
        s.x = px;
        s.y = py;
        d.x = x;
        d.y = y;
        SDL_BlitSurface(charset, &s, screen, &d);
        x += 8;
        text++;
    };
};


//checks if loaded BMP is well build, returns 1 is no
int checkLoadedBMP(SDL_Surface* surface, screenElements* screen, drawElements* draw, char text[]) {

    if (surface == NULL) {
        printf("SDL_LoadBMP%s error: %s\n", text, SDL_GetError());
        destroySurfaces(screen, draw);
        SDL_Quit();
        return ONE;
    };
    return NONE;
}


//draw cars - sprite on the road on given position
void drawCar(SDL_Surface* screen, SDL_Surface* sprite, int carX, int carY) {
    SDL_Rect dest;
    dest.x = carX - sprite->w / 2;
    dest.y = carY - sprite->h / 2;
    dest.w = sprite->w;
    dest.h = sprite->h;
    SDL_BlitSurface(sprite, NULL, screen, &dest);
};

// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void drawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
    SDL_Rect dest;
    dest.x = x - sprite->w / 2;
    dest.y = y - sprite->h / 2;
    dest.w = sprite->w;
    dest.h = sprite->h;
    SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// draw a single pixel
void drawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32*)p = color;
};


// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void drawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
    for (int i = 0; i < l; i++) {
        drawPixel(screen, x, y, color);
        x += dx;
        y += dy;
    };
};


// draw a rectangle of size l by k
void drawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
    Uint32 outlineColor, Uint32 fillColor) {
    int i;
    drawLine(screen, x, y, k, 0, 1, outlineColor);
    drawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
    drawLine(screen, x, y, l, 1, 0, outlineColor);
    drawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
    for (i = y + 1; i < y + k - 1; i++)
        drawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};
