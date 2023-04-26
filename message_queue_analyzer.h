#pragma once

#include<iostream>
#include<iomanip>
#include<ctime>

#include"message_queue.h"

class Analyzer
{
public:
    const char* getCurrentTime() const;

    template<typename T>
    double getPercentageByUrgency(const MessageQueue<T>& msg_q, const Urgency urgency);

    template<typename T>
    double getSizeInKb(const MessageQueue<T>& msg_q);

    template<typename T>
    size_t getMaxExpirationTimeDifference(const MessageQueue<T>& msg_q);

    template<typename T>
    void showInfo(const MessageQueue<T>& msg_q, std::ostream& out);
};


const char* Analyzer::getCurrentTime() const
{
    const time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return ctime(&current_time);
}

template<typename T>
double Analyzer::getPercentageByUrgency(const MessageQueue<T>& msg_q, const Urgency urgency)
{
    if (msg_q.empty())
        return 0;

    auto count = std::count_if(msg_q.msg_queue.cbegin(), msg_q.msg_queue.cend(), [urgency](const auto& msg_in_queue)
                               { return msg_in_queue.getUrgency() == urgency; });

    return static_cast<double>(count) / msg_q.size() * 100;
}

template<typename T>
double Analyzer::getSizeInKb(const MessageQueue<T>& msg_q)
{
    if (msg_q.empty())
        return 0;
    return static_cast<double>(sizeof(msg_q.msg_queue.front())) * msg_q.size() / 1024;
}

template<typename T>
size_t Analyzer::getMaxExpirationTimeDifference(const MessageQueue<T>& msg_q)
{
    auto compare = [](const auto& right_msg, const auto& left_msg)
    { return right_msg.getExpirationTime() < left_msg.getExpirationTime(); };
    auto max = std::max_element(msg_q.msg_queue.cbegin(), msg_q.msg_queue.cend(), compare);
    auto min = std::min_element(msg_q.msg_queue.cbegin(), msg_q.msg_queue.cend(), compare);

    return std::chrono::duration_cast<std::chrono::minutes>(max->getExpirationTime() - min->getExpirationTime()).count();
}

template<typename T>
void Analyzer::showInfo(const MessageQueue<T>& msg_q, std::ostream& out)
{
    out << std::setw(23) << std::left << "Current time" << getCurrentTime();
    out << std::setw(23) << std::left << "Queue size" << msg_q.size() << "\n";
    out << std::setw(23) << std::left << "Queue size in Kb" << getSizeInKb(msg_q) << "\n";
    out << std::setw(23) << std::left << "Maximum difference" << getMaxExpirationTimeDifference(msg_q) << " minutes\n";

    out << "\nPersantage\n";
    out << std::setw(23) << ""
        << "Critical - " << std::setprecision(2) << getPercentageByUrgency(msg_q, Urgency::Critical) << "%\n";
    out << std::setw(23) << ""
        << "Major - "    << std::setprecision(2) << getPercentageByUrgency(msg_q, Urgency::Major) << "%\n";
    out << std::setw(23) << ""
        << "Medium - "   << std::setprecision(2) << getPercentageByUrgency(msg_q, Urgency::Medium) << "%\n";
    out << std::setw(23) << ""
        << "Minor - "    << std::setprecision(2) << getPercentageByUrgency(msg_q, Urgency::Minor) << "%\n";
}

