#include "randomRoute.h"

RandomRoute::RandomRoute(Scan* scan) {
  timeout = 0;
  scanner = scan;
}

void RandomRoute::randomRedirection(Moteur<RandomRoute>* moteur) {
  timeout = millis();

  moteur->setOnRunT(&RandomRoute::onRunBack);
  moteur->commandMotors("back");
  moteur->setOnRunT(&RandomRoute::onStop);

  int nb = random(2,10);
  for(int i=0; i < nb; i++) {
    int turn = random(2);
    int timeRun = random(200,300);
    int speed = random(70,240);

    if ( turn == 1 ) {
      moteur->commandAllMotors("turnLeft",timeRun,speed);
    }
    else {
      moteur->commandAllMotors("turnRight",timeRun,speed);
    }
    delay(100);
  }
}

bool RandomRoute::onStop() {
  delay(10);
  return false;
}

bool RandomRoute::onRunBack() {
  double distance = scanner->calculDistance();
  return (distance < 30) && ( (millis() - timeout) < 3000 );
}

bool RandomRoute::testForward() {
  double distance = scanner->calculDistance();
  return !((distance < 30) && (distance > -1));
}
