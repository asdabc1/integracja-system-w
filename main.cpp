#include "includes.h"

#include "helperFunctions.h"
#include "requestHandlingAndRouting.h"
#include "server.h"
#include "ComparisonData.h"

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

        auto li = std::make_shared<Listener>(ioc, tcp::endpoint{address, port});

        std::cout << "Serwer zostal uruchomiony na porcie " << port << "." << std::endl;

        std::thread contextThread([&ioc](){ioc.run();});
        contextThread.detach();

        std::map<std::string, std::pair<double, double>> fetchedRates;
        auto* store = new std::vector<ComparisonData>();
        auto xd = data.as_object()["data"].as_array();

        for (const auto& element : xd) {
            store->emplace_back(element.as_object().at("date").as_string().c_str(), std::stod(element.as_object().at("awarded_value").as_string().c_str()));
        }

        std::cout << "Zapisano dane o przetargach do obiektow\n";

        for (int i = 0; i < store->size(); i++) {
            if (fetchedRates.contains((*store)[i].getDate()))
                (*store)[i].setCurrencies(fetchedRates[(*store)[i].getDate()]);

            else {
                (*store)[i].setCurrencies(fetchDollarAndEuro((*store)[i].getDate()));
                fetchedRates[(*store)[i].getDate()] = (*store)[i].getCurrencies();
            }
        }
        std::cout << "Kazdy obiekt danych posiada dane o walucie";



        std::cin.get();
        ioc.stop();

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
