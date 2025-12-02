#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Definiciones y estructuras

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
    int anio;
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

Cola colaEsperaEjecutivo;
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
    cola.indices[cola.tam] = indiceTaxi;
    cola.tam++;
    return true;
}

int desencolar(Cola &cola) {
    if (colaVacia(cola)) return -1; // Cola vacía
    int primero = cola.indices[0];
    // se mueven una posicion a la izquierda
    for (int i = 1; i < cola.tam; i++) {
        cola.indices[i - 1] = cola.indices[i];
    }
    cola.tam--;
    return primero;
}

// Funciones del menu principal

void mostrarMenu();
void opcionAgregarTaxi();
void opcionVerDatos();
void opcionEnviarTaxiCliente();
void opcionVerTaxisEnRuta();
void opcionReinsertarTaxiEnCola();
void opcionVerReportes();


// Funciones para verificar duplicados

bool placaDuplicada(const string &placa) {
    for (int i = 0; i < totalTaxis; i++) {
        if (taxis[i].placa == placa) {
            return true;
        }
    }
    return false;
}

bool motorDuplicado(const string &numeroMotor) {
    for (int i = 0; i < totalTaxis; i++) {
        if (taxis[i].numeroMotor == numeroMotor) {
            return true;
        }
    }
    return false;
}

bool documentoDuplicado(const string &documentoIdentidad) {
    for (int i = 0; i < totalTaxis; i++) {
        if (taxis[i].conductor.documentoIdentidad == documentoIdentidad) {
            return true;
        }
    }
    return false;
}

// Funciones para leer datos con validacion

string leerDatoUnico(const string &etiqueta, bool (*duplicado)(const string &)) {
    string valor;
    while (true) {
        cout << etiqueta << ": ";
        getline(cin, valor);
        if (!duplicado(valor)) {
            return valor;
        } else {
            cout << "El valor ingresado ya existe. Por favor ingrese un valor unico." << endl;
        }
    }
}

int leerAnoValido() {
    int anio;
    while (true) {
        cout << "Año del Taxi (e.g., 2015): ";
        if (!(cin >> anio)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida. Intente nuevamente." << endl;
            continue;
        }
        cin.ignore(); // Limpiar el buffer de entrada
        if (anio < 2010) {
            cout << "Año invalido. Debe ser 2010 o posterior." << endl;
        } else {
            return anio;
        }
    }
}

string determinarCategoria(int anio) {
    if (anio >= 2015) {
        return "Ejecutivo";
    } else {
        return "Tradicional";
    }
}

string leerCategoriaCliente() {
    while (true) {
        cout << "Categoria requerida (E = Ejecutivo, T = Tradicional): ";
        char c;
        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (c == 'E' || c == 'e') return "Ejecutivo";
        if (c == 'T' || c == 't') return "Tradicional";

        cout << "Opcion invalida. Intente nuevamente." << endl;
    }
}


Conductor leerDatosConductor() {
    Conductor conductor;
    cout << "Ingrese los datos del conductor:" << endl;
    cout << "Nombre: ";
    getline(cin, conductor.nombre);
    cout << "Apellido: ";
    getline(cin, conductor.apellido);
    conductor.documentoIdentidad = leerDatoUnico("Documento de Identidad", documentoDuplicado);
    cout << "Numero de Seguro Social: ";
    getline(cin, conductor.numeroSeguroSocial);
    cout << "Telefono: ";
    getline(cin, conductor.telefono);
    return conductor;
}

// Implementacion de funciones adicionales

void opcionAgregarTaxi() {
    if (totalTaxis >= MAX_TAXIS) {
        cout << "Limite maximo de taxis alcanzado." << endl;
        return;
    }

    Taxi nuevo;
    nuevo.idCorrelativo = totalTaxis + 1;

    cout << "\n === Registro de Nuevo Taxi ===" << endl;

    // datos unicos con helper generico

    nuevo.placa = leerDatoUnico("Placa del Taxi", placaDuplicada);
    nuevo.numeroMotor = leerDatoUnico("Numero de Motor", motorDuplicado);

    cout << "Modelo del Taxi: ";
    getline(cin, nuevo.modelo);

    nuevo.anio = leerAnoValido();
    nuevo.categoria = determinarCategoria(nuevo.anio);

    //datos del conductor
    nuevo.conductor = leerDatosConductor();

    // guardar en el array
    int indice = totalTaxis;
    taxis[indice] = nuevo;
    totalTaxis++;

    // encolar segun categoria
    if (nuevo.categoria == "Ejecutivo") {
        encolar(colaEsperaEjecutivo, indice);
    } else {
        encolar(colaEsperaTradicional, indice);
    }

    cout << "\nTaxi registrado y agregado a la cola de espera correctamente (" << nuevo.categoria << ")." << endl;
}

void opcionVerDatos() {
    if (totalTaxis == 0) {
        cout << "No hay taxis registrados." << endl;
        return;
    }
    cout << "Lista de taxis registrados:" << endl;
    for (int i = 0; i < totalTaxis; ++i) {
        const Taxi &t = taxis[i];
        cout << "ID: " << t.idCorrelativo
             << " | Placa: " << t.placa
             << " | Motor: " << t.numeroMotor
             << " | Modelo: " << t.modelo
             << " | Año: " << t.anio
             << " | Categoria: " << t.categoria
             << " | Conductor: " << t.conductor.nombre << " " << t.conductor.apellido
             << endl;
    }
}

void opcionEnviarTaxiCliente() {
    cout << "\n=== Enviar taxi a cliente ===" << endl;

    string categoria = leerCategoriaCliente();

    Cola *colaEspera;
    Cola *colaRuta;

    if (categoria == "Ejecutivo") {
        colaEspera = &colaEsperaEjecutivo;
        colaRuta   = &colaRutaEjecutivo;
    } else {
        colaEspera = &colaEsperaTradicional;
        colaRuta   = &colaRutaTradicional;
    }

    if (colaVacia(*colaEspera)) {
        cout << "No hay taxis disponibles en la categoria " << categoria << "." << endl;
        return;
    }

    int indiceTaxi = desencolar(*colaEspera);
    if (indiceTaxi == -1) {
        cout << "Error interno al obtener taxi de la cola." << endl;
        return;
    }

    encolar(*colaRuta, indiceTaxi);

    string origen, destino;
    double costo;

    cout << "Punto de salida: ";
    getline(cin, origen);
    cout << "Destino: ";
    getline(cin, destino);

    cout << "Costo del viaje: ";
    while (!(cin >> costo)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida. Ingrese un numero: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    ingresosTotales += costo;
    viajesTotales++;

    if (categoria == "Ejecutivo") {
        ingresosEjecutivo += costo;
        viajesEjecutivo++;
    } else {
        ingresosTradicional += costo;
        viajesTradicional++;
    }

    Taxi &t = taxis[indiceTaxi];
    cout << "\nTaxi enviado:" << endl;
    cout << "ID: " << t.idCorrelativo
         << " | Placa: " << t.placa
         << " | Categoria: " << t.categoria
         << " | Conductor: " << t.conductor.nombre << " " << t.conductor.apellido
         << endl;
}

void opcionVerTaxisEnRuta() {
    cout << "\n=== Taxis en ruta ===" << endl;

    cout << "\nCategoria Ejecutivo:" << endl;
    if (colaVacia(colaRutaEjecutivo)) {
        cout << "  No hay taxis ejecutivos en ruta." << endl;
    } else {
        for (int i = 0; i < colaRutaEjecutivo.tam; ++i) {
            int idx = colaRutaEjecutivo.indices[i];
            const Taxi &t = taxis[idx];
            cout << "  ID: " << t.idCorrelativo
                 << " | Placa: " << t.placa
                 << " | Modelo: " << t.modelo
                 << " | Conductor: " << t.conductor.nombre << " " << t.conductor.apellido
                 << endl;
        }
    }

    cout << "\nCategoria Tradicional:" << endl;
    if (colaVacia(colaRutaTradicional)) {
        cout << "  No hay taxis tradicionales en ruta." << endl;
    } else {
        for (int i = 0; i < colaRutaTradicional.tam; ++i) {
            int idx = colaRutaTradicional.indices[i];
            const Taxi &t = taxis[idx];
            cout << "  ID: " << t.idCorrelativo
                 << " | Placa: " << t.placa
                 << " | Modelo: " << t.modelo
                 << " | Conductor: " << t.conductor.nombre << " " << t.conductor.apellido
                 << endl;
        }
    }
}


void opcionReinsertarTaxiEnCola() {
    cout << "\n=== Reinsertar taxi a cola de espera ===" << endl;

    string categoria = leerCategoriaCliente();

    Cola *colaEspera;
    Cola *colaRuta;

    if (categoria == "Ejecutivo") {
        colaEspera = &colaEsperaEjecutivo;
        colaRuta   = &colaRutaEjecutivo;
    } else {
        colaEspera = &colaEsperaTradicional;
        colaRuta   = &colaRutaTradicional;
    }

    if (colaVacia(*colaRuta)) {
        cout << "No hay taxis en ruta para la categoria " << categoria << "." << endl;
        return;
    }

    cout << "Taxis en ruta en categoria " << categoria << ":" << endl;
    for (int i = 0; i < colaRuta->tam; ++i) {
        int idx = colaRuta->indices[i];
        const Taxi &t = taxis[idx];
        cout << (i + 1) << ") ID: " << t.idCorrelativo
             << " | Placa: " << t.placa
             << " | Conductor: " << t.conductor.nombre << " " << t.conductor.apellido
             << endl;
    }

    int opcion;
    cout << "Seleccione el numero de taxi a reinsertar: ";
    while (!(cin >> opcion) || opcion < 1 || opcion > colaRuta->tam) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Opcion invalida. Seleccione un numero entre 1 y " << colaRuta->tam << ": ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int pos = opcion - 1;
    int indiceTaxi = colaRuta->indices[pos];

    for (int i = pos + 1; i < colaRuta->tam; ++i) {
        colaRuta->indices[i - 1] = colaRuta->indices[i];
    }
    colaRuta->tam--;

    encolar(*colaEspera, indiceTaxi);

    cout << "Taxi reintegrado a la cola de espera de la categoria " << categoria << "." << endl;
}


void opcionVerReportes() {
    cout << "\n=== Reportes ===" << endl;

    cout << "Ingresos totales: " << ingresosTotales << endl;
    cout << "Ingresos categoria Ejecutivo: " << ingresosEjecutivo << endl;
    cout << "Ingresos categoria Tradicional: " << ingresosTradicional << endl;
    cout << "Viajes totales: " << viajesTotales << endl;
    cout << "Viajes categoria Ejecutivo: " << viajesEjecutivo << endl;
    cout << "Viajes categoria Tradicional: " << viajesTradicional << endl;
}


void mostrarMenu() {
    cout << "\n--- Sistema de Gestion TRUE DRIVE ---" << endl;
    cout << "1. Agregar Taxi" << endl;
    cout << "2. Ver Datos de Taxis" << endl;
    cout << "3. Enviar Taxi a Cliente" << endl;
    cout << "4. Ver Taxis en Ruta" << endl;
    cout << "5. Reinsertar Taxi en Cola de Espera" << endl;
    cout << "6. Ver Reportes" << endl;
    cout << "7. Salir" << endl;
    cout << "Seleccione una opcion: ";
}


int main() {
    inicializarCola(colaEsperaEjecutivo);
    inicializarCola(colaEsperaTradicional);
    inicializarCola(colaRutaEjecutivo);
    inicializarCola(colaRutaTradicional);

    char opcion;

    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar linea

        switch (opcion) {
            case '1':
                opcionAgregarTaxi();
                break;
            case '2':
                opcionVerDatos();
                break;
            case '3':
                opcionEnviarTaxiCliente();
                break;
            case '4':
                opcionVerTaxisEnRuta();
                break;
            case '5':
                opcionReinsertarTaxiEnCola();
                break;
            case '6':
                opcionVerReportes();
                break;
            case '7':
                cout << "Saliendo del sistema..." << endl;
                break;
            default:
                cout << "Opcion invalida. Por favor intente de nuevo." << endl;
        }
    } while (opcion != '7');

    return 0;
}
