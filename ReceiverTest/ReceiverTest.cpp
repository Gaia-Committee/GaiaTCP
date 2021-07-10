#include <GaiaTCP/GaiaTCP.hpp>
#include <iostream>
#include <GaiaTCP/Acceptor.hpp>
#include <thread>

using namespace Gaia;

int main()
{
    TCP::Acceptor acceptor(8000);

    auto connection = acceptor.Accept();

    connection.OnReceive.Add(Events::Functor<const std::string&>([](const std::string& text){
        std::cout << text << std::endl;
    }));

    connection.StartListen();

    Gaia::Background::BackgroundWorker sender([&connection](const std::atomic_bool& flag){
        while (flag)
        {
            std::string heart_beat = "HeartBeat";
            std::cout << "Heart beat enqueue." << std::endl;
            connection.Write(heart_beat.data(), heart_beat.size());
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    sender.Start();

    connection.StartListen();

    connection.WaitError();

    return 0;
}