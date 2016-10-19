#include "InternalJackClient.h"

int main()
{
//    std::unique_ptr<JackClient> client(new JackClient("Test",2,2));

    std::unique_ptr<InternalJackClient> client(new InternalJackClient("Test",2,2));

    std::cin.get(); // waits for input, press enter to continue
    return 0;
}
