#include "randomRoute.h"

//scanner
char* name = "Scan Room";
char AngleServo[MAXSIZE];

bool run = false;

Moteur<RandomRoute>* moteur = new Moteur<RandomRoute>(&RandomRoute::testForward,NULL);
RandomRoute* randomRoute;
Scan* scanner;

int afficheMotor(String instr) {
  //return moteur->afficheMotor(instr);
}

void setup() {

  Serial.begin(9600);

  // capteur = A7, servo = A6
  scanner = new Scan(A7,A6);
  randomRoute = new RandomRoute(scanner);
  moteur->setContext(randomRoute);

  //motors
  moteur->addMotor(A0,D0,0);
  moteur->addMotor(A1,D1,1);

  Spark.function("sparkCommand", runProgramm);
  Spark.function("afficheInit", afficheMotor );

  Spark.variable("name",name,STRING);
  Spark.variable("AngleServo", AngleServo, STRING);
}

void loop() {
  if ( run ) {
    if ( randomRoute->testForward() ) {
      moteur->setOnRunT(&RandomRoute::testForward);
      moteur->commandMotors("forward");
    }
    else {
      char* coordpolaire = scanner->scanServo();
      memcpy(AngleServo,coordpolaire,MAXSIZE);
      Spark.publish("allData", "ok");

      randomRoute->randomRedirection(moteur);
      delay(1000);
      free(coordpolaire);
    }
  }
}

int runProgramm(String instruction) {
  run = !run;
  moteur->setOnRunT(&RandomRoute::onStop);

  return 1;
}
