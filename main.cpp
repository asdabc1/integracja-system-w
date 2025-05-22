#include "includes.h"

#include "helperFunctions.h"
#include "requestHandlingAndRouting.h"
#include "server.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace ssl = boost::asio::ssl;

boost::json::value data;

int main() {
    try {
        net::io_context ioc;

        std::ifstream fs("data1.json");
        std::stringstream ss;
        ss << fs.rdbuf();

        data = boost::json::parse(ss.str());

        auto const address = net::ip::make_address("0.0.0.0");
        unsigned short port = 8080;

        auto test = fetchDollarAndEuro(ioc, "2019-08-27");

        auto li = std::make_shared<Listener>(ioc, tcp::endpoint{address, port});

        std::cout << "Serwer zostal uruchomiony na porcie " << port << "." << std::endl;
        std::cout << "Wartosci zfetchowane (testowe): " << test.first << " " << test.second << std::endl;
        ioc.run();

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
