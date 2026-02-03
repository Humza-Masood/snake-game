#ifndef PLAYER_H
#define PLAYER_H

#include "GameMechs.h"
#include "objPos.h"
#include "objPosArrayList.h"

class Player
{
    public:
        enum Dir {UP, DOWN, LEFT, RIGHT, STOP};
        Dir getDir() const;

        Player(GameMechs* thisGMRef);
        ~Player();

        void getHeadPos(objPos &returnPos) const;

        void updatePlayerDir();
        void movePlayer();

        objPosArrayList* getPlayerPos() const;
        void removeTail();

    private:
        objPosArrayList* playerPosList;
        enum Dir myDir;

        GameMechs* mainGameMechsRef;
};

#endif
