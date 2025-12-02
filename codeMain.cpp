#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Definiciones y estructuras

const int MAX_TAXIS = 100; // Se puede ajustar segun sea necesario

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
double ingresosEjecutivo = 0.0;    // se muestra como "Ejecutiva"
double ingresosTradicional = 0.0;

int viajesTotales = 0;
int viajesEjecutivo = 0;
int viajesTradicional = 0;

struct Cola {
    int indices[MAX_TAXIS]; // Guarda los indices al array de taxis
    int tam;                // Tamano actual de la cola
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
    if (colaVacia(cola)) return -1; // Cola vacia
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

// Helpers de reportes
void mostrarMenuReportes();
void reporteTotalIngresos();
void reporteIngresosPorCategoria();
void reporteViajesPorCategoria();

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

bool seguroSocialDuplicado(const string &numeroSeguroSocial) {
    for (int i = 0; i < totalTaxis; i++) {
        if (taxis[i].conductor.numeroSeguroSocial == numeroSeguroSocial) {
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
        cout << "Año del True drive (ejemplo 2015): ";
        if (!(cin >> anio)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida. Intente nuevamente." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el resto de la linea
        if (anio < 2010) {
            cout << "Año invalido. Debe ser 2010 o posterior." << endl;
        } else {
            return anio;
        }
    }
}

string determinarCategoria(int anio) {
    if (anio >= 2015) {
        return "Ejecutiva";
    } else {
        return "Tradicional";
    }
}

string leerCategoriaCliente() {
    while (true) {
        cout << "Categoria requerida (E = Ejecutiva, T = Tradicional): ";
        char c;
        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (c == 'E' || c == 'e') return "Ejecutiva";
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
    conductor.numeroSeguroSocial = leerDatoUnico("Numero de Seguro Social", seguroSocialDuplicado);
    cout << "Telefono: ";
    getline(cin, conductor.telefono);
    return conductor;
}

// Helpers de estado de taxis

bool taxiEstaEnCola(const Cola &cola, int indiceTaxi) {
    for (int i = 0; i < cola.tam; ++i) {
        if (cola.indices[i] == indiceTaxi) {
            return true;
        }
    }
    return false;
}

bool taxiEstaEnRuta(int indiceTaxi) {
    return taxiEstaEnCola(colaRutaEjecutivo, indiceTaxi)
        || taxiEstaEnCola(colaRutaTradicional, indiceTaxi);
}

// Implementacion de funciones adicionales

void opcionAgregarTaxi() {
    if (totalTaxis >= MAX_TAXIS) {
        cout << "Limite maximo de True drives alcanzado." << endl;
        return;
    }

    Taxi nuevo;
    nuevo.idCorrelativo = totalTaxis + 1;

    cout << "\n=== Registro de Nuevo True drive ===" << endl;

    // datos unicos con helper generico

    nuevo.placa = leerDatoUnico("Placa del True drive", placaDuplicada);
    nuevo.numeroMotor = leerDatoUnico("Numero de Motor", motorDuplicado);

    cout << "Modelo del True drive: ";
    getline(cin, nuevo.modelo);

    nuevo.anio = leerAnoValido();
    nuevo.categoria = determinarCategoria(nuevo.anio);

    // datos del conductor
    nuevo.conductor = leerDatosConductor();

    // guardar en el array
    int indice = totalTaxis;
    taxis[indice] = nuevo;
    totalTaxis++;

    // encolar segun categoria
    if (nuevo.categoria == "Ejecutiva") {
        encolar(colaEsperaEjecutivo, indice);
    } else {
        encolar(colaEsperaTradicional, indice);
    }

        cout << "\nTrue drive registrado y agregado a la cola de espera correctamente ("
            << nuevo.categoria << ")." << endl;
}

void opcionVerDatos() {
    if (totalTaxis == 0) {
        cout << "No hay True drives registrados." << endl;
        return;
    }

    cout << "Lista de True drives registrados:" << endl;
    for (int i = 0; i < totalTaxis; ++i) {
        const Taxi &t = taxis[i];

        string estado = taxiEstaEnRuta(i) ? "En ruta" : "En espera";

        cout << "ID: " << t.idCorrelativo
             << " | Placa: " << t.placa
             << " | Motor: " << t.numeroMotor
             << " | Modelo: " << t.modelo
             << " | Año: " << t.anio
             << " | Categoria: " << t.categoria
             << " | Estado: " << estado
             << " | Conductor: " << t.conductor.nombre << " " << t.conductor.apellido
             << endl;
    }
}

void opcionEnviarTaxiCliente() {
    cout << "\n=== Enviar True drive a cliente ===" << endl;

    string categoria = leerCategoriaCliente();

    Cola *colaEspera;
    Cola *colaRuta;

    if (categoria == "Ejecutiva") {
        colaEspera = &colaEsperaEjecutivo;
        colaRuta   = &colaRutaEjecutivo;
    } else {
        colaEspera = &colaEsperaTradicional;
        colaRuta   = &colaRutaTradicional;
    }

    if (colaVacia(*colaEspera)) {
        cout << "No hay True drives disponibles en la categoria " << categoria << "." << endl;
        return;
    }

    int indiceTaxi = desencolar(*colaEspera);
    if (indiceTaxi == -1) {
        cout << "Error interno al obtener True drive de la cola." << endl;
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

    if (categoria == "Ejecutiva") {
        ingresosEjecutivo += costo;
        viajesEjecutivo++;
    } else {
        ingresosTradicional += costo;
        viajesTradicional++;
    }

    Taxi &t = taxis[indiceTaxi];
    cout << "\nTrue drive enviado:" << endl;
    cout << "ID: " << t.idCorrelativo
         << " | Placa: " << t.placa
         << " | Categoria: " << t.categoria
         << " | Conductor: " << t.conductor.nombre << " " << t.conductor.apellido
         << endl;
}

void opcionVerTaxisEnRuta() {
    cout << "\n=== True drives en ruta ===" << endl;

    cout << "\nCategoria Ejecutiva:" << endl;
    if (colaVacia(colaRutaEjecutivo)) {
        cout << "  No hay True drives ejecutivos en ruta." << endl;
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
        cout << "  No hay True drives tradicionales en ruta." << endl;
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
    cout << "\n=== Reinsertar True drive a cola de espera ===" << endl;

    string categoria = leerCategoriaCliente();

    Cola *colaEspera;
    Cola *colaRuta;

    if (categoria == "Ejecutiva") {
        colaEspera = &colaEsperaEjecutivo;
        colaRuta   = &colaRutaEjecutivo;
    } else {
        colaEspera = &colaEsperaTradicional;
        colaRuta   = &colaRutaTradicional;
    }

    if (colaVacia(*colaRuta)) {
        cout << "No hay True drives en ruta para la categoria " << categoria << "." << endl;
        return;
    }
    cout << "True drives en ruta en categoria " << categoria << ":" << endl;
    for (int i = 0; i < colaRuta->tam; ++i) {
        int idx = colaRuta->indices[i];
        const Taxi &t = taxis[idx];
        cout << (i + 1) << ") ID: " << t.idCorrelativo
             << " | Placa: " << t.placa
             << " | Conductor: " << t.conductor.nombre << " " << t.conductor.apellido
             << endl;
    }

    int opcion;
    cout << "Seleccione el numero de True drive a reinsertar: ";
    while (!(cin >> opcion) || opcion < 1 || opcion > colaRuta->tam) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Opcion invalida. Seleccione un numero entre 1 y "
             << colaRuta->tam << ": ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int pos = opcion - 1;
    int indiceTaxi = colaRuta->indices[pos];

    for (int i = pos + 1; i < colaRuta->tam; ++i) {
        colaRuta->indices[i - 1] = colaRuta->indices[i];
    }
    colaRuta->tam--;

    encolar(*colaEspera, indiceTaxi);

        cout << "True drive reintegrado a la cola de espera de la categoria "
            << categoria << "." << endl;
}

// Helpers de reportes

void mostrarMenuReportes() {
    cout << "\n=== Menu de reportes ===" << endl;
    cout << "1. Ver total de ingresos" << endl;
    cout << "2. Ver total de ingresos por categoria" << endl;
    cout << "3. Ver total de viajes y por categoria" << endl;
    cout << "4. Volver al menu principal" << endl;
    cout << "Seleccione una opcion: ";
}

void reporteTotalIngresos() {
    cout << "\n[Reporte] Total de ingresos" << endl;
    cout << "Ingresos totales: " << ingresosTotales << endl;
}

void reporteIngresosPorCategoria() {
    cout << "\n[Reporte] Ingresos por categoria" << endl;
    cout << "Ingresos categoria Ejecutiva: " << ingresosEjecutivo << endl;
    cout << "Ingresos categoria Tradicional: " << ingresosTradicional << endl;
}

void reporteViajesPorCategoria() {
    cout << "\n[Reporte] Viajes totales y por categoria" << endl;
    cout << "Viajes totales: " << viajesTotales << endl;
    cout << "Viajes categoria Ejecutiva: " << viajesEjecutivo << endl;
    cout << "Viajes categoria Tradicional: " << viajesTradicional << endl;
}

void opcionVerReportes() {
    char opcion;

    do {
        mostrarMenuReportes();
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case '1':
                reporteTotalIngresos();
                break;
            case '2':
                reporteIngresosPorCategoria();
                break;
            case '3':
                reporteViajesPorCategoria();
                break;
            case '4':
                cout << "Regresando al menu principal..." << endl;
                break;
            default:
                cout << "Opcion invalida. Intente nuevamente." << endl;
        }

    } while (opcion != '4');
}

void mostrarMenu() {
    cout << "\n--- Sistema de Gestion TRUE DRIVE ---" << endl;
    cout << "1. Agregar True drive" << endl;
    cout << "2. Ver Datos de True drives" << endl;
    cout << "3. Enviar True drive a Cliente" << endl;
    cout << "4. Ver True drives en Ruta" << endl;
    cout << "5. Reinsertar True drive en Cola de Espera" << endl;
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
