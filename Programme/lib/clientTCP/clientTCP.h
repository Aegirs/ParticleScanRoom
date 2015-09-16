#include "application.h"

class ClientTCP{
protected:
  int ipArrayFromString(byte ipArray[], String ipString);
  TCPClient client;
public:
  ClientTCP();
  int connectToMyServer(String ip);
  TCPClient getClient();
};
