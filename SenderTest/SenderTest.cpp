#include <GaiaTCP/GaiaTCP.hpp>
#include <GaiaByteUtility/GaiaByteUtility.hpp>
#include <iostream>
#include <thread>

using namespace Gaia;

int main()
{
    TCP::Connection connection = TCP::Connector::Connect(TCP::MakeLocalEndpoint(8000));

    unsigned int index = 0;

    while(true)
    {
        std::cout << "Index: " << index << std::endl;

        ByteUtility::BytesBuilder builder;
        builder.AddValue(index);
        connection.Write(ByteUtility::ToBytesAddress(builder.BytesData));

        std::this_thread::sleep_for(std::chrono::seconds(1));

        ++index;
    }

	return 0;
}
