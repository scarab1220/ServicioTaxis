#include <iostream>
#include <string>

using namespace std;

const int MAX_TAXIS = 100; // Se puede ajustar según sea necesario

struct Conductor {
    string nombre;
    string apellido;
    string documentoIdentidad;
    string numeroSeguroSocial;
    string telefono;
};

struct Taxi {
    int idCorrelativo;
    string placa;
    string numeroMotor;
    string modelo;
    string year;
    string categoria;
    Conductor conductor;
};

Taxi taxis[MAX_TAXIS];
int totalTaxis = 0;

// variables para reportar

double ingresosTotales = 0.0;
double ingresosEjecutivo = 0.0;
double ingresosTradicional = 0.0;

int viajesTotales = 0;
int viajesEjecutivo = 0;
int viajesTradicional = 0;

int main() {
    cout << "Sistema de Gestion de Taxis Iniciado" << endl;
    return 0;
}

// Fin del archivo codeMain.cpp