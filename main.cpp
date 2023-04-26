#include<iostream>
#include<fstream>
#include<string>
#include<chrono>

#include"message_queue.h"
#include"message_queue_analyzer.h"

#include<thread>

using namespace std::chrono_literals;

int main()
{
    MessageQueue<std::string> msg_q(10);

    msg_q.push(Message<std::string>("10", Urgency::Major, std::chrono::minutes(10)));

    msg_q.push(Message<std::string>("35", Urgency::Medium, std::chrono::minutes(35)));

    msg_q.push(Message<std::string>("3", Urgency::Critical, std::chrono::minutes(3)));

    msg_q.push(Message<std::string>("11", Urgency::Major, std::chrono::minutes(11)));

    msg_q.push(Message<std::string>("45", Urgency::Minor, std::chrono::minutes(5)));

    msg_q.push(Message<std::string>("35", Urgency::Medium, std::chrono::minutes(35)));

    msg_q.push(Message<std::string>("3", Urgency::Critical, std::chrono::minutes(3)));

    msg_q.push(Message<std::string>("1", Urgency::Critical, std::chrono::minutes(1)));

    msg_q.push(Message<std::string>("5", Urgency::Major, std::chrono::minutes(5)));

    msg_q.push(Message<std::string>("54", Urgency::Minor, std::chrono::minutes(54)));


    // std::this_thread::sleep_for(90s);

    // try
    // {
    //     msg_q.push(Message<std::string>("54", Urgency::Minor, std::chrono::minutes(54)));
    // }
    // catch(const std::exception& e)
    // {
    //     std::cout << e.what();
    // }


    // auto size = msg_q.size();
    // for (size_t i = 0; i < size; ++i)
    // {
    //     std::cout << msg_q.get().getMessage() << "\n";
    // }

    Analyzer a;
    std::ofstream fout("Output.txt");

    if (!fout.is_open())
    {
        std::cerr << "Cannot open the file";
    }
    else
    {
        a.showInfo(msg_q, fout);
    }

    return 0;
}