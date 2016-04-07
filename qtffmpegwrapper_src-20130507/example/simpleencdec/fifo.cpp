#include "fifo.hpp"

template <class T>
Fifo<T>::Fifo() :
    _list(),
    _mutex()
{

}

template <class T>
void Fifo<T>::PushBack(T item)
{
    _mutex.lock();
    _list.append(item);
    _mutex.unlock();
}

template <class T>
T Fifo<T>::PopFront()
{
    if(!_list.isEmpty())
        return _list.takeFirst();
    else
        return NULL;
}

template <class T>
int Fifo<T>::Count() const
{
    return _list.count();
}
