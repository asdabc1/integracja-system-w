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

void ComparisonData::save(mysqlx::Session& session) const{
    auto schema = session.getSchema("integracja");
    auto table = schema.getTable("comparison_data");

    auto res = table.select("date").where("date = :date").bind("date", date).execute();

    if (res.count() == 0) {
        // Insert
        table.insert("date", "value", "usd", "eur")
            .values(date, value, currencies.first, currencies.second)
            .execute();
    } else {
        // Update
        table.update()
            .set("value", value)
            .set("usd", currencies.first)
            .set("eur", currencies.second)
            .where("date = :date")
            .bind("date", date)
            .execute();
    }
}

std::optional<ComparisonData> ComparisonData::loadByDate(mysqlx::Session& session, const std::string& date) {
    auto schema = session.getSchema("your_database_name");
    auto table = schema.getTable("comparison_data");

    auto res = table.select("date", "value", "usd", "eur")
        .where("date = :date")
        .bind("date", date)
        .execute();

    auto row = res.fetchOne();
    if (!row) {
        return std::nullopt;
    }

    ComparisonData data(row[0].get<std::string>(), row[1].get<double>());
    data.setCurrencies(row[2].get<double>(), row[3].get<double>());
    return data;
}

void ComparisonData::removeByDate(mysqlx::Session& session, const std::string& date) {
    auto schema = session.getSchema("your_database_name");
    auto table = schema.getTable("comparison_data");
    table.remove().where("date = :date").bind("date", date).execute();
}