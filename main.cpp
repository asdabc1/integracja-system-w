#include "includes.h"

#include "helperFunctions.h"
#include "requestHandlingAndRouting.h"
#include "server.h"
#include "ComparisonData.h"
#include "indexAssigners.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace ssl = boost::asio::ssl;

boost::json::value data;

double eurVal[10];
int eurOcc[10];

double usdVal[20];
int usdOcc[20];

int main() {
    try {
        mysqlx::Session session(
            mysqlx::SessionOption::HOST, "127.0.0.1",
            mysqlx::SessionOption::PORT, 33060,
            mysqlx::SessionOption::USER, "root",
            mysqlx::SessionOption::PWD, "rootpassword",
            mysqlx::SessionOption::DB, "integracja"
        );
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

        std::cout << "core data loaded\n";

        for (auto & i : *store) {
            if (fetchedRates.contains(i.getDate()))
                i.setCurrencies(fetchedRates[i.getDate()]);

            else {
                i.setCurrencies(fetchDollarAndEuro(i.getDate()));
                fetchedRates[i.getDate()] = i.getCurrencies();
            }
        }
        std::cout << "assigned exchange rates data\n";

        for (int i = 0, j = 0; i < 20; i++) {
            if (i % 2 == 1)
                ++j;

            eurVal[j] = 0;
            eurOcc[j] = 0;
            usdVal[i] = 0;
            usdOcc[i] = 0;
        }

        eurIndexAssigner e;
        usdIndexAssigner u;

        std::erase_if(*store, [](ComparisonData& x){return (x.getCurrencies().first == -1);});

        for (const auto & i : *store) {
            eurVal[e[i.getCurrencies().second]] += i.getValue();
            eurOcc[e[i.getCurrencies().second]] += 1;

            usdVal[u[i.getCurrencies().first]] += i.getValue();
            usdOcc[u[i.getCurrencies().first]] += 1;

            i.save(session);
        }

        for (int i = 0, j = 0; i < 20; i++) {
            if (i % 2 == 1)
                ++j;

            eurVal[j] /= eurOcc[j];
            usdVal[i] /= usdOcc[i];
        }

        std::cout << "all the values are now calculated\n";

        std::cin.get();
        ioc.stop();
        session.close();

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
