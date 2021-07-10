#include <GaiaTCP/GaiaTCP.hpp>
#include <iostream>
#include <thread>

using namespace Gaia;

int main()
{
    TCP::Connection connection = TCP::Connector::Connect(TCP::MakeLocalEndpoint(8000));

    unsigned int index = 0;
    connection.OnReceive.Add(Events::Functor<const std::string&>([](const std::string& text){
        std::cout << text << std::endl;
    }));
    connection.StartListen();

    while(true)
    {
        std::cout << "Index: " << index << std::endl;

        std::string text;
        std::cin >> text;

        connection.Write(text.data(), text.size());

        std::this_thread::sleep_for(std::chrono::seconds(3));

        ++index;
    }

	return 0;
}
