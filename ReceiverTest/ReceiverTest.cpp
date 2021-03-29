#include <GaiaTCP/GaiaTCP.hpp>
#include <GaiaByteUtility/GaiaByteUtility.hpp>
#include <iostream>
#include <GaiaTCP/Acceptor.h>

using namespace Gaia;

int main()
{
    TCP::Acceptor acceptor(8000);
    bool bound = acceptor.IsBound();

    auto connection = acceptor.Accept();

    while(true)
    {
        ByteUtility::BytesBuffer data = connection.Read();
        std::cout << ByteUtility::BytesPrinter::PrintToString(ByteUtility::ToBytesAddress(data)) << std::endl;
    }

	return 0;
}