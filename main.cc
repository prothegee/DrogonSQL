#include <main.h>

int main() {
    drogon::app().loadConfigFile("../configs/config.json");
    drogon::app().run();
    return 0;
}
