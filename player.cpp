#include "player.h"

Player::Player(GameMechs* thisGMRef)
{
    mainGameMechsRef = thisGMRef;
    myDir = STOP;

    playerPosList = new objPosArrayList;

    int rows = mainGameMechsRef->getBoardSizeY();
    int cols = mainGameMechsRef->getBoardSizeX();

    // set player to middle
    int initialX = cols / 2;
    int initialY = rows / 2;

    // force player on the board interior
    if (initialX <= 0) initialX = 1;
    if (initialY <= 0) initialY = 1;
    if (initialX >= cols - 1) initialX = cols - 2;
    if (initialY >= rows - 1) initialY = rows - 2;

    objPos head(initialX, initialY, '*');
    playerPosList->insertHead(head);
}

Player::~Player()
{
    delete playerPosList;
}

objPosArrayList* Player::getPlayerPos() const
{
    return playerPosList;
}

void Player::updatePlayerDir()
{
    char inputChar = mainGameMechsRef->getInput();

    if (inputChar != 0)
    {
        // beginner switch-based FSM input logic
        switch (inputChar)
        {
            case 'w':
            case 'W':
                if (myDir == STOP || myDir == LEFT || myDir == RIGHT) myDir = UP;
                break;

            case 's':
            case 'S':
                if (myDir == STOP || myDir == LEFT || myDir == RIGHT) myDir = DOWN;
                break;

            case 'a':
            case 'A':
                if (myDir == STOP || myDir == UP || myDir == DOWN) myDir = LEFT;
                break;

            case 'd':
            case 'D':
                if (myDir == STOP || myDir == UP || myDir == DOWN) myDir = RIGHT;
                break;

            default:
                break;
        }

        mainGameMechsRef->clearInput();
    }
}

void Player::movePlayer()
{
    int rows = mainGameMechsRef->getBoardSizeY();
    int cols = mainGameMechsRef->getBoardSizeX();

    objPos currentHead = playerPosList->getHeadElement();

    int newX = currentHead.pos->x;
    int newY = currentHead.pos->y;

    // movement based on direction
    if (myDir == UP)        newY -= 1;
    else if (myDir == DOWN) newY += 1;
    else if (myDir == LEFT) newX -= 1;
    else if (myDir == RIGHT)newX += 1;

    // If you want border-collision death (more "real game"):
    // Treat border as wall: if hit wall -> lose + exit.
    if (newX <= 0 || newX >= cols - 1 || newY <= 0 || newY >= rows - 1)
    {
        mainGameMechsRef->setLoseFlag();

        return;
    }

    objPos newHead(newX, newY, '*');

    // insert head only (growth is controlled by main loop)
    playerPosList->insertHead(newHead);
}

void Player::getHeadPos(objPos &returnPos) const
{
    if (playerPosList->getSize() > 0)
    {
        objPos head = playerPosList->getHeadElement();
        returnPos.setObjPos(head);
    }
}

void Player::removeTail()
{
    if (playerPosList->getSize() > 0)
    {
        playerPosList->removeTail();
    }
}

Player::Dir Player::getDir() const
{
    return myDir;
}
