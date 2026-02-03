#ifndef OBJPOSARRAYLIST_H
#define OBJPOSARRAYLIST_H

#include "objPos.h"

class objPosArrayList
{
    private:
        objPos* aList;
        int arrayCapacity;
        int listSize;

    public:
        objPosArrayList();
        ~objPosArrayList();

        int getSize() const;

        void insertHead(objPos thisPos);
        void insertTail(objPos thisPos);

        void removeHead();
        void removeTail();

        objPos getHeadElement() const;
        objPos getTailElement() const;
        objPos getElement(int index) const;
};

#endif
