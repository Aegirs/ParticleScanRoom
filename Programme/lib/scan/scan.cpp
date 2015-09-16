#include "scan.h"

Scan::Scan(int capteur,int servo) {
	scanner = initScan(capteur,servo);
}

Scanner* Scan::initScan(int capteur,int pinServo) {
	Scanner* scan = (Scanner*)malloc(sizeof(Scanner));

	scan->capteur = capteur;
	scan->servo.attach(pinServo);
	scan->data = (char*)calloc(MAXSIZE,sizeof(char));
	scan->taille = 0;

	return scan;
}

char* Scan::scanServo() {
	int i = 0;

	for(i = 0; i <= 180 ; i += INCRANGLE ) {
		writeAngle(i);
	}

	for(i = 180; i >= 0 ; i -= INCRANGLE ) {
		writeAngle(i);
	}

	return scanner->data;
}

// a faire lecture plusieur fois de la distance et moy var et exclusion des valeur extreme sur genre 10 val
// 20 ms max par boucle si rien autour => 200 ms par mesure de 10
void Scan::writeAngle(int angle) {
	scanner->servo.write(angle);
	double distance = calculDistance();

	if ( scanner->taille < MAXSIZE ) {
		scanner->taille += snprintf(scanner->data + (scanner->taille),MAXSIZE-(scanner->taille),"%.2f,",distance);
	}
}

double Scan::calculDistance() {
	double moy = 0.0;
	double var = 0.0;
	int N = 10;

	for(int i=0; i < N ; i++) {
		double distance = capteurDistance(scanner->capteur);
		delay(TIME);

		moy += distance;
		var += distance * distance;
	}

	moy /= N;
	var /= N;
	var -= moy*moy;

	return moy;
}
