#pragma once

#include<algorithm>
#include<list>
#include<stdexcept>

#include"message.h"

template<typename T>
class MessageQueue
{
private:
    std::list<Message<T>> msg_queue;
    const size_t msg_limit;

    void pushInOrder(const Message<T>& msg);
    void pushInOrder(Message<T>&& msg);

public:
    MessageQueue(const size_t msg_limit);

// ================== Push ==================

    void push(const Message<T>& msg);
    void push(Message<T>&& msg);



// ================== Get ==================

    Message<T> get();

    size_t size() const;
    size_t limit() const;
    bool empty() const;

    friend class Analyzer;
};


template<typename T>
void MessageQueue<T>::pushInOrder(const Message<T>& msg)
{
    msg_queue.insert(std::find_if(msg_queue.cbegin(), msg_queue.cend(), [&msg](const auto& msg_in_queue)
                                                                        { return msg.moreUrgentThan(msg_in_queue); }), msg);
}

template<typename T>
void MessageQueue<T>::pushInOrder(Message<T>&& msg)
{
    msg_queue.insert(std::find_if(msg_queue.cbegin(), msg_queue.cend(), [&msg](const auto& msg_in_queue)
                                                                        { return msg.moreUrgentThan(msg_in_queue); }), std::move(msg));
}


template<typename T>
MessageQueue<T>::MessageQueue(const size_t msg_limit) : msg_limit(msg_limit) {}

// ================== Push ==================

template<typename T>
void MessageQueue<T>::push(const Message<T>& msg)
{
    if (msg_queue.empty())
    {
        msg_queue.push_front(msg);
        return;
    }

    if (size() < limit())
    {
        pushInOrder(msg);
    }
    else
    {
        msg_queue.remove_if([](const auto& msg_in_queue)
                            { return msg_in_queue.expired(); });

        if (size() == limit())
            throw std::length_error("Message queue is full");
        else
            pushInOrder(msg);
    }
}

template<typename T>
void MessageQueue<T>::push(Message<T>&& msg)
{
    if (msg_queue.empty())
    {
        msg_queue.push_front(std::move(msg));
        return;
    }

    if (size() < limit())
    {
        pushInOrder(std::move(msg));
    }
    else
    {
        msg_queue.remove_if([](const auto& msg_in_queue)
                            { return msg_in_queue.expired(); });

        if (size() == limit())
            throw std::length_error("Message queue is full");
        else
            pushInOrder(std::move(msg));
    }
}

// ================== Get ==================

template<typename T>
Message<T> MessageQueue<T>::get()
{
    if (msg_queue.empty())
        throw std::runtime_error("Message queue is empty");

    while (!msg_queue.empty() && msg_queue.front().expired())
    {
        msg_queue.pop_front();
    }

    if (!msg_queue.empty())
    {
        Message<T> msg(std::move(msg_queue.front()));
        msg_queue.pop_front();
        return msg;
    }
    else
        throw std::runtime_error("All messages had terminated");
}

template<typename T>
size_t MessageQueue<T>::size() const { return msg_queue.size(); }

template<typename T>
size_t MessageQueue<T>::limit() const { return msg_limit; }

template<typename T>
bool MessageQueue<T>::empty() const { return msg_queue.empty(); }

