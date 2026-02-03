#ifndef GAMEMECHS_H
#define GAMEMECHS_H

#include <cstdlib>
#include <time.h>

#include "objPos.h"
#include "objPosArrayList.h"

using namespace std;

class GameMechs
{
    private:
        char input;
        bool exitFlag;
        bool loseFlag;
        int score;
        int speed;

        int boardSizeX;
        int boardSizeY;

        objPos food;

    public:
        GameMechs();
        GameMechs(int boardX, int boardY);
        ~GameMechs();

        // Reset flags/input/score to known safe defaults (for restart)
        void resetGameState();

        bool getExitFlagStatus() const;
        void setExitTrue();

        bool getLoseFlagStatus() const;
        void setLoseFlag();          // sets loseFlag true

        char getInput() const;
        void setInput(char this_input);
        void clearInput();

        int getBoardSizeX() const;
        int getBoardSizeY() const;

        int getScore() const;
        void incrementScore();

        int getSpeed() const;
        void setSpeed(int newSpeed);

        // Food (keep your existing style)
        void generateFoodPos(objPos blockOff);
        void generateFoodPos(objPosArrayList* blockList);
        objPos getFoodPos() const;
};

#endif
