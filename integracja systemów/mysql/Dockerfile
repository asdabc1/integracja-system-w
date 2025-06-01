FROM mysql:8.0

ENV MYSQL_ROOT_PASSWORD=rootpassword
ENV MYSQL_USER=appuser
ENV MYSQL_PASSWORD=apppassword
ENV MYSQL_DATABASE=integracja

COPY my.cnf /etc/mysql/conf.d/my.cnf
COPY init.sql /docker-entrypoint-initdb.d/

EXPOSE 3306 33060
