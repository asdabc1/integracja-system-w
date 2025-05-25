//
// Created by asdab on 24/05/2025.
//

#ifndef COMPARISONDATA_H
#define COMPARISONDATA_H
#include <string>
#include "helperFunctions.h"


class ComparisonData {
private:
    std::string date;
    double value;
    std::pair<double, double> currencies;

public:
    ComparisonData(const std::string& date, const double& value) : date(date), value(value) {}
    void setCurrencies(std::pair<double, double> currencies) {this->currencies = currencies;}
    void setCurrencies(const double& usd, const double& eur) {this->currencies = std::make_pair(usd, eur);}

    [[nodiscard]] std::string getDate() const;
    [[nodiscard]] double getValue() const;
    [[nodiscard]] std::pair<double, double> getCurrencies() const;
};



#endif //COMPARISONDATA_H
