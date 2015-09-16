#include "clientTCP.h"

ClientTCP::ClientTCP() {

}

TCPClient ClientTCP::getClient() {
  return client;
}

int ClientTCP::ipArrayFromString(byte ipArray[], String ipString) {
  int dot1 = ipString.indexOf('.');
  ipArray[0] = ipString.substring(0, dot1).toInt();

  int dot2 = ipString.indexOf('.', dot1 + 1);
  ipArray[1] = ipString.substring(dot1 + 1, dot2).toInt();

  dot1 = ipString.indexOf('.', dot2 + 1);
  ipArray[2] = ipString.substring(dot2 + 1, dot1).toInt();

  dot2 = ipString.indexOf(':', dot1 + 1);
  ipArray[3] = ipString.substring(dot1 + 1,dot2).toInt();

  return ipString.substring(dot2+1).toInt();
}

int ClientTCP::connectToMyServer(String ipPort) {
  byte serverAddress[4];
  int port = ipArrayFromString(serverAddress, ipPort);

  if (client.connect(serverAddress, port)) {
    return 1; // successfully connected
  } else {
    return -1; // failed to connect
  }
}
