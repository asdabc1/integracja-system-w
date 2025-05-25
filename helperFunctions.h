#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "includes.h"

extern double eurVal[10];
extern int eurOcc[10];

extern double usdVal[20];
extern int usdOcc[20];

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace ssl = boost::asio::ssl;

std::string extract_query_param(const std::string& target, const std::string& param_name);
void removeObjectById(boost::json::value& data, const std::string& targetId);
std::pair<double, double> fetchDollarAndEuro(std::string date);
std::string create_tables();

#endif //HELPERFUNCTIONS_H
