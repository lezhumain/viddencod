#ifndef FIFO_H
#define FIFO_H

#include <QList>
#include <QMutex>

template <class T>
class Fifo
{
    public:
        Fifo();
        void PushBack(T item);
        T PopFront();
        int Count() const;

    private:
        QList<T> _list;
        QMutex _mutex;
};
#include "Fifo.cpp"
#endif // FIFO_H
