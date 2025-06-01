CREATE DATABASE IF NOT EXISTS integracja;

USE integracja;

CREATE TABLE IF NOT EXISTS comparison_data (
    date VARCHAR(20) PRIMARY KEY,
    value DOUBLE,
    usd DOUBLE,
    eur DOUBLE
);