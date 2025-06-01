# Final project: Systems integration
Group:
- Adam Franciszek Bożek
- Frantsysk-Yan Boika

## Stack:
- C++ (CMake, vcpkg)
    - Boost
        - Beast
        - asio
        - json
    - mysql connector C++
- Python (matplotlib)
- Docker

## How to run:
### 1. Download the project
```git bash
git clone https://github.com/asdabc1/integracja-system-w
```

### 2. Start the app
```terminal
cd integracja
docker-compose up --build
```

### 3. Wait for the app to turn on
It will probably take a while. When the docker containers are up, wait for the processes inside them to finish their set-up

### 4. All is ready
You may begin to use the application. To start, go to `http://localhost:8080/index`, and log in using credentials: login: `admin`, password: `haslo123`.