#include "ultrason.h"

#define MAXSIZE 600
#define INCRANGLE 6
#define TIME 3

typedef struct scanner{
	Servo servo;
	int capteur;
	char* data;
	int taille;
}Scanner;

class Scan{
protected:
	Scanner* scanner;
	Scanner* initScan(int capteur,int servo);
	void writeAngle(int angle);
public:
	Scan(int capteur,int servo);
	double calculDistance();
	char* scanServo ();
};
