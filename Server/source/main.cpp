#include "CssWork.h"
#include "Server.h"

int main(int argc, char *argv[])
{
    // - res = 38f6
    // CssWork::testCrc("A590006C00000000000004000000");
    // CssWork::testCrc("A50000400002a000000000000000");
    try {
        boost::asio::io_context io_context;
        auto Server{std::make_shared<BoostServer>(io_context, proto_project::kServerAddr, proto_project::kServerPort)};
        Server->ListenTcp();
        io_context.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
