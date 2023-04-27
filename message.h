#pragma once

#include<chrono>

enum class Urgency
{
    Critical,
    Major,
    Medium,
    Minor
};


template<typename T>
class Message
{
private:
    T msg;
    Urgency urgency;
    std::chrono::system_clock::time_point expiration_time;

public:
    // ================== Ctors with arguments ==================

    Message(const T& msg, const Urgency urgency, const std::chrono::system_clock::time_point& expiration_time);
    Message(const T& msg, const Urgency urgency, const std::chrono::minutes& expiration_duration);

    
    // ================== Getters ==================

    const T& getMessage() const &;
    T&& getMessage() &&;
    Urgency getUrgency() const;
    const std::chrono::system_clock::time_point& getExpirationTime() const;


    // ================== Setters ==================

    void setMessage(const T& m);
    void setMessage(T&& m);
    void setUrgency(const Urgency urg);
    void setExpirationTime(const std::chrono::system_clock::time_point& exp_time);



// ================== Checkers ==================
    
    bool expired(const std::chrono::system_clock::time_point& time) const;
    bool expired() const;
    bool moreUrgentThan(const Message& other) const;
};


template<typename T>
Message<T>::Message(const T& msg, const Urgency urgency, const std::chrono::system_clock::time_point& expiration_time)
: msg(msg)
, urgency(urgency)
, expiration_time(expiration_time) {}


template<typename T>
Message<T>::Message(const T& msg, const Urgency urgency, const std::chrono::minutes& expiration_duration)
: msg(msg)
, urgency(urgency)
, expiration_time(std::chrono::system_clock::now() + expiration_duration) {}


template<typename T>
const T& Message<T>::getMessage() const & { return msg; }

template<typename T>
T&& Message<T>::getMessage() && { return std::move(msg); }

template<typename T>
Urgency Message<T>::getUrgency() const { return urgency; }

template<typename T>
const std::chrono::system_clock::time_point& Message<T>::getExpirationTime() const { return expiration_time; }



template<typename T>
void Message<T>::setMessage(const T& m) { msg = m; }

template<typename T>
void Message<T>::setMessage(T&& m) { msg = std::move(m); }

template<typename T>
void Message<T>::setUrgency(const Urgency urg) { urgency = urg; }

template<typename T>
void Message<T>::setExpirationTime(const std::chrono::system_clock::time_point& exp_time) { expiration_time = exp_time; }



template<typename T>
bool Message<T>::expired(const std::chrono::system_clock::time_point& time) const { return time >= expiration_time; }

template<typename T>
bool Message<T>::expired() const { return expired(std::chrono::system_clock::now()); }

template<typename T>
bool Message<T>::moreUrgentThan(const Message& rhs) const { return static_cast<int>(this->urgency) < static_cast<int>(rhs.urgency); }

