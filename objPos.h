#ifndef OBJPOS_H
#define OBJPOS_H

typedef struct
{
    int x;
    int y;
} Pos;

class objPos
{
    public:
        Pos* pos;
        char symbol;

        objPos();
        objPos(int xPos, int yPos, char sym);

        ~objPos();
        objPos(const objPos& other);
        objPos& operator=(const objPos& other);

        void setObjPos(objPos o);
        void setObjPos(int xPos, int yPos, char sym);

        objPos getObjPos() const;
        char getSymbol() const;
        char getSymbolIfPosEqual(const objPos* refPos) const;

        bool isPosEqual(const objPos* refPos) const;
};

#endif
