//
// Created by ms on 2022/10/24.
//

#ifndef MSOPENGLES3_MSQUEUEDEF_H
#define MSOPENGLES3_MSQUEUEDEF_H

#include <list>
#include <mutex>

using namespace std;
template <class T>

class MSMediaQueue : public list<T>{
public:
    MSMediaQueue()
    {
    }

    ~MSMediaQueue()
    {
    }
    void enqueue( const T &t ) {
        m_mutex.lock();
        list<T>::push_back(t);
        m_mutex.unlock();
    }

    T dequeue()
    {
        m_mutex.lock();
        T t = NULL;
        if ( !list<T>::empty() ){
            t = list<T>::front();
            list<T>::pop_front();
        }
        m_mutex.unlock();
        return t;
    }

    bool isEmpty()
    {
        m_mutex.lock();
        bool b = list<T>::empty();
        m_mutex.unlock();
        return b;
    }

private:
    mutex m_mutex;
};



#endif //MSOPENGLES3_MSQUEUEDEF_H
