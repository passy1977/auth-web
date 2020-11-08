#include <drogon/drogon.h>
int main() {
    using drogon::app;

    app()
    .loadConfigFile("../config.json")
    .run();
    return 0;
}
