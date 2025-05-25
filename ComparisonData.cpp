//
// Created by asdab on 24/05/2025.
//

#include "ComparisonData.h"

std::string ComparisonData::getDate() const {
    return date;
}

double ComparisonData::getValue() const {
    return value;
}

std::pair<double, double> ComparisonData::getCurrencies() const {
    return currencies;
}
