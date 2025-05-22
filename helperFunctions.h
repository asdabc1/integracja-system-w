#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "includes.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace ssl = boost::asio::ssl;

std::string extract_query_param(const std::string& target, const std::string& param_name);
void removeObjectById(boost::json::value& data, const std::string& targetId);
std::pair<double, double> fetchDollarAndEuro(boost::asio::io_context &ioc, std::string date);

#endif //HELPERFUNCTIONS_H
