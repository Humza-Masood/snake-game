#include "gameMechs.h"

GameMechs::GameMechs()
{
    boardSizeX = 30;
    boardSizeY = 30;

    resetGameState();
    speed = 6;

    food.symbol = 'F';
}

GameMechs::GameMechs(int boardX, int boardY)
{
    boardSizeX = boardX;
    boardSizeY = boardY;

    resetGameState();
    speed = 6;

    food.symbol = 'F';
}

GameMechs::~GameMechs()
{
    // no heap members directly inside GameMechs
}

void GameMechs::resetGameState()
{
    exitFlag = false;
    loseFlag = false;
    score = 0;
    input = 0;
}

bool GameMechs::getExitFlagStatus() const
{
    return exitFlag;
}

void GameMechs::setExitTrue()
{
    exitFlag = true;
}

bool GameMechs::getLoseFlagStatus() const
{
    return loseFlag;
}

void GameMechs::setLoseFlag()
{
    loseFlag = true;
}

char GameMechs::getInput() const
{
    return input;
}

void GameMechs::setInput(char this_input)
{
    input = this_input;
}

void GameMechs::clearInput()
{
    input = 0;
}

int GameMechs::getBoardSizeX() const
{
    return boardSizeX;
}

int GameMechs::getBoardSizeY() const
{
    return boardSizeY;
}

int GameMechs::getScore() const
{
    return score;
}

void GameMechs::incrementScore()
{
    score++;
}

int GameMechs::getSpeed() const
{
    return speed;
}

void GameMechs::setSpeed(int newSpeed)
{
    if (newSpeed < 1) newSpeed = 1;
    if (newSpeed > 10) newSpeed = 10;
    speed = newSpeed;
}

// Food generation: block only 1 position
void GameMechs::generateFoodPos(objPos blockOff)
{
    food.pos->x = (rand() % (boardSizeX - 2)) + 1;
    food.pos->y = (rand() % (boardSizeY - 2)) + 1;

    while (food.isPosEqual(&blockOff))
    {
        food.pos->x = (rand() % (boardSizeX - 2)) + 1;
        food.pos->y = (rand() % (boardSizeY - 2)) + 1;
    }

    food.symbol = 'F';
}

// Food generation: block entire list (recommended for real game)
void GameMechs::generateFoodPos(objPosArrayList* blockList)
{
    if (blockList == nullptr || blockList->getSize() == 0)
    {
        objPos dummy(1, 1, '0');
        generateFoodPos(dummy);
        return;
    }

    bool ok = false;

    while (!ok)
    {
        food.pos->x = (rand() % (boardSizeX - 2)) + 1;
        food.pos->y = (rand() % (boardSizeY - 2)) + 1;

        ok = true;

        for (int i = 0; i < blockList->getSize(); i++)
        {
            objPos seg = blockList->getElement(i);

            if (seg.pos->x == food.pos->x && seg.pos->y == food.pos->y)
            {
                ok = false;
                break;
            }
        }
    }

    food.symbol = 'F';
}

objPos GameMechs::getFoodPos() const
{
    return food;
}
