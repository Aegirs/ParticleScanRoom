#ifndef moteur_h
#define moteur_h

#include "application.h"
#include "clientTCP.h"

typedef struct motors{
	int pinMotor;
	int pinDir;
	int speed;
	int timeOn;
	int moveForward;

	struct motors* suiv;
}Motor;

template <class T> class Moteur{
	protected:
		ClientTCP* clientTCP;
		Motor* motors;
		int nbMotor;

		bool (*_onRun)(void);
		bool (T::*_onRunT)(void);

		T* context;

		int strInter(String instructions) {

				char *infoMotor = strtok(strdup(instructions.c_str()), ",");
				Motor* listeMotor = motors;

				// boucle sur les moteurs
				while(listeMotor != NULL ) {
					// boucles sur les 3 arguments
						for(int j = 0 ;  j < 3 ;j ++) {
								if ( j == 0 ) {
										int direction = atoi(infoMotor);
										if ( direction > 0 ) {
											digitalWrite(listeMotor->pinDir, HIGH);
										}
										else {
											digitalWrite(listeMotor->pinDir, LOW);
										}
								}
								else if ( j == 1 ) {
									listeMotor->timeOn = atoi(infoMotor);
								}
								else {
									listeMotor->speed = atoi(infoMotor);
								}
								infoMotor = strtok(NULL, ",");
						}

						listeMotor = listeMotor->suiv;
				}

				motorControl();

				return 1;
		}

		bool run() {
			if ( _onRunT != NULL && context != NULL && _onRun != NULL ) {
				return _onRun() || (context->*_onRunT)();
			}
			else  if ( _onRun != NULL ) {
				return _onRun();
			}
			else if ( _onRunT != NULL && context != NULL ) {
				return (context->*_onRunT)();
			}
			else {
				return false;
			}
		}

		void motorControl() {

				int end = 0;

				unsigned long currentMillis = 0;
				unsigned long startMillis = millis();
				TCPClient client = clientTCP->getClient();

				while ( end != nbMotor ) {

						currentMillis = millis();
						end = 0;

						Motor* listeMotor = motors;

						if ( client.connected() ) {
							if ( client.available() > 0 ) {
									char tmp = client.read();
									Serial.println(String(tmp));

									while( listeMotor != NULL ) {
										digitalWrite(listeMotor->pinMotor, LOW);
										listeMotor = listeMotor->suiv;
									}

									return;
							}
						}

						// run all Motor with frequency
						while( listeMotor != NULL ) {
								if ( ( (currentMillis - startMillis) < listeMotor->timeOn ) || run() ) {
									analogWrite(listeMotor->pinMotor, listeMotor->speed);
									// a tester add to onRun if onRun is lower than ten millis
									delay(listeMotor->speed/10);
								}
								else {
									end++;
									digitalWrite(listeMotor->pinMotor, LOW);
								}

								listeMotor = listeMotor->suiv;
						}
				}

				Serial.println();
				Serial.println("Done");
				Serial.println();
		}

		int moveForward(String infos) {
			String instructions = "";

			Motor* listeMotor = motors;
			while( listeMotor != NULL ) {
				instructions += String(listeMotor->moveForward,DEC);
				instructions += String(",");
				instructions += infos;

				if( listeMotor->suiv != NULL ) {
					instructions += String(",");
				}

				listeMotor = listeMotor->suiv;
			}

			strInter(instructions);

			return 1;
		}

		int moveBack(String infos) {
			String instructions = "";

			Motor* listeMotor = motors;
			while( listeMotor != NULL ) {
				instructions += String((listeMotor->moveForward + 1 )%2,DEC);
				instructions += String(",");
				instructions += infos;

				if( listeMotor->suiv != NULL ) {
					instructions += String(",");
				}
				listeMotor = listeMotor->suiv;
			}

			strInter(instructions);

			return 1;
		}

		int turnLeft(String infos) {
			String instructions = "";

			Motor* listeMotor = motors;
			int turn = 0;

			while( listeMotor != NULL ) {

				instructions += String((listeMotor->moveForward + turn )%2,DEC);
				instructions += String(",");
				instructions += infos;

				if( listeMotor->suiv != NULL ) {
					instructions += String(",");
				}
				listeMotor = listeMotor->suiv;
				turn++;
			}

			strInter(instructions);

			return 1;
		}

		int turnRight(String infos) {
			String instructions = "";

			Motor* listeMotor = motors;
			int turn = 1;

			while( listeMotor != NULL ) {
				instructions += String((listeMotor->moveForward + turn )%2,DEC);
				instructions += String(",");
				instructions += infos;

				if( listeMotor->suiv != NULL ) {
					instructions += String(",");
				}
				listeMotor = listeMotor->suiv;
				turn++;
			}

			strInter(instructions);

			return 1;
		}
	public:

		Moteur(bool (T::*callbackT)(void),bool (*callback)(void)) {
			_onRunT = callbackT;
			_onRun = callback;

			clientTCP = new ClientTCP();
			motors = NULL;
			nbMotor = 0;
		}

		void setOnRun(bool (*callback)(void)) {
			_onRun = callback;
		}

		void setOnRunT(bool (T::*callback)(void)) {
			_onRunT = callback;
		}

		void setContext(T* cont) {
			context = cont;
		}

		void commServeur() {
			TCPClient client = clientTCP->getClient();

			if (client.connected()) {
						String message = "";
						while ( client.available() > 0 ) {
								char tmp = client.read();
								message += String(tmp);
						}
						if ( message.length() > 0 ) {
							int reponse = commandMotors(message);
						}
			}
		}

		void addMotor(int pinMotor,int pinDir,int moveForward) {
			Motor* newMotor = (Motor*)malloc(sizeof(Motor));

			pinMode(pinMotor, OUTPUT);
			pinMode(pinDir, OUTPUT);

			newMotor->pinMotor = pinMotor;
			newMotor->pinDir = pinDir;
			newMotor->speed = 0;
			newMotor->timeOn = 0;
			newMotor->moveForward = moveForward;

			newMotor->suiv = motors;

			motors = newMotor;
			nbMotor++;
		}

		int afficheMotor(String affiche) {
			Motor* tmp = motors;
			while( tmp != NULL ) {
				Serial.println("Motor");

				Serial.print("Pin speed: ");
				Serial.println(tmp->pinMotor);

				Serial.print("Pin dir: ");
				Serial.println(tmp->pinDir);

				Serial.print("Seed: ");
				Serial.println(tmp->speed);

				Serial.println("TimeOn: ");
				Serial.println(tmp->timeOn);

				tmp = tmp->suiv;
			}

			return 1;
		}

		int commandMotors(String dir) {
			Serial.println(dir);
			if ( dir == "forward") {
				moveForward("100,100");
			}
			else if ( dir == "back" ) {
				moveBack("100,100");
			}
			else if ( dir == "turnLeft") {
				turnLeft("100,100");
			}
			else if ( dir == "turnRight") {
				turnRight("100,100");
			}
			else {
				Serial.println("Error command not found: " + dir);
				return -1;
			}

			return 1;
		}

		int commandAllMotors(String dir,int timeOf,int speed) {
			Serial.println(dir);
			String instr = String(timeOf,DEC) + "," + String(speed,DEC);

			if ( dir == "forward") {
				moveForward(instr);
			}
			else if ( dir == "back" ) {
				moveBack(instr);
			}
			else if ( dir == "turnLeft") {
				turnLeft(instr);
			}
			else if ( dir == "turnRight") {
				turnRight(instr);
			}
			else {
				Serial.println("Error command not found: " + dir);
				return -1;
			}

			return 1;
		}

};

#endif
