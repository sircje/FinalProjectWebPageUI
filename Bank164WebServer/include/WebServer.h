#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "Bank.h"

class WebServer {
private:
    Bank *bank;
public:
    WebServer(Bank &bank, int argc, char **argv);
};

#endif // WEB_SERVER_H
