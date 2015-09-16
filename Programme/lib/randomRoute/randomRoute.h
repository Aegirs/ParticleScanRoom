#include "moteur.h"
#include "scan.h"

class RandomRoute{
protected:
  Scan* scanner;
  unsigned long timeout;

public:
  RandomRoute(Scan* scan);
  void randomRedirection(Moteur<RandomRoute>* moteur);

  bool onStop();
  bool onRunBack();
  bool testForward();
};
