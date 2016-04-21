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
    T item;
    _mutex.lock();
    if(!_list.isEmpty()){
        item = _list.takeFirst();
    }

    _mutex.unlock();
    return item;
}

template <class T>
int Fifo<T>::Count() const
{
    return _list.count();
}
