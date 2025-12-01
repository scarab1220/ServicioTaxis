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

struct Cola {
    int indices[MAX_TAXIS]; // Guarda los índices al array de taxis
    int tam; // Tamaño actual de la cola
};

Cola colaEsperaEjectivo;
Cola colaEsperaTradicional;
Cola colaRutaEjecutivo;
Cola colaRutaTradicional;

// Funciones basicas de cola

void inicializarCola(Cola &cola) {
    cola.tam = 0;
}

bool colaVacia(const Cola &cola) {
    return cola.tam == 0;
}

bool colaLlena(const Cola &cola) {
    return cola.tam == MAX_TAXIS;
}

bool encolar(Cola &cola, int indiceTaxi) {
    if (colaLlena(cola)) 
        return false; // Cola llena
    cola.indices[cola.tam++] = indiceTaxi;
    cola.tam++;
    return true;
}

bool desencolar(Cola &cola) {
    if (colaVacia(cola)) return -1; // Cola vacía
    int primero = cola.indices[0];
    // se mueven una posicion a la izquierda
    for (int i = 1; i < cola.tam; i++) {
        cola.indices[i - 1] = cola.indices[i];
    }
    cola.tam--;
    return primero;
}

//Declaracion de funciones adicionales

void mostrarMenu();
void opcionAgregarTaxi();
void opcionVerDatos();
void opcionVerificarTaxisEnRuta();
void opcionReinsertarTaxiEnCola();
void opcionVerReportes();

int main() {
    inicializarCola(colaEsperaEjectivo);
    inicializarCola(colaEsperaTradicional);
    inicializarCola(colaRutaEjecutivo);
    inicializarCola(colaRutaTradicional);
    
    char opcion;

    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore(); // Limpiar el buffer de entrada

        switch (opcion) {
            case '1':
                opcionAgregarTaxi();
                break;
            case '2':
                opcionVerDatos();
                break;
            case '3':
                opcionVerificarTaxisEnRuta();
                break;
            case '4':
                opcionReinsertarTaxiEnCola();
                break;
            case '5':
                opcionVerReportes();
                break;
            case '6':
                cout << "Saliendo del sistema..." << endl;
                break;
            default:
                cout << "Opcion invalida. Por favor intente de nuevo." << endl;
        }
    } while (opcion != '6');

    return 0;


}

// Fin del archivo codeMain.cpp