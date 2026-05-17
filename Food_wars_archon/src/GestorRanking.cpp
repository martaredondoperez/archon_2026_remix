#include "GestorRanking.h"

GestorRanking::GestorRanking(const std::string& ruta) : rutaArchivo(ruta) {
    // Intentar cargar el ranking si existe
    if (archivoExiste()) {
        cargarDelArchivo();
    }
}

bool GestorRanking::archivoExiste() const {
    std::ifstream archivo(rutaArchivo, std::ios::binary);
    return archivo.good();
}

void GestorRanking::cargarDelArchivo() {
    ranking.clear();
    std::ifstream archivo(rutaArchivo, std::ios::binary);

    if (!archivo.is_open()) {
        std::cout << "DEBUG: No se pudo abrir el archivo de ranking: " << rutaArchivo << std::endl;
        return;
    }

    // Leer el número de entradas
    int numEntradas = 0;
    archivo.read(reinterpret_cast<char*>(&numEntradas), sizeof(int));

    // Leer cada entrada
    for (int i = 0; i < numEntradas; i++) {
        // Leer longitud del nombre
        int lenNombre = 0;
        archivo.read(reinterpret_cast<char*>(&lenNombre), sizeof(int));
        std::string nombre(lenNombre, '\0');
        archivo.read(&nombre[0], lenNombre);

        // Leer turnos
        int turnos = 0;
        archivo.read(reinterpret_cast<char*>(&turnos), sizeof(int));

        // Leer longitud del bando
        int lenBando = 0;
        archivo.read(reinterpret_cast<char*>(&lenBando), sizeof(int));
        std::string bando(lenBando, '\0');
        archivo.read(&bando[0], lenBando);

        ranking.emplace_back(nombre, turnos, bando);
    }

    archivo.close();
    std::cout << "DEBUG: Cargadas " << numEntradas << " entradas del ranking." << std::endl;
}

void GestorRanking::guardarEnArchivo() {
    std::ofstream archivo(rutaArchivo, std::ios::binary);

    if (!archivo.is_open()) {
        std::cout << "ERROR: No se pudo crear el archivo de ranking: " << rutaArchivo << std::endl;
        return;
    }

    // Escribir el número de entradas
    int numEntradas = (int)ranking.size();
    archivo.write(reinterpret_cast<const char*>(&numEntradas), sizeof(int));

    // Escribir cada entrada
    for (const auto& entrada : ranking) {
        // Escribir nombre
        int lenNombre = (int)entrada.getNombre().length();
        archivo.write(reinterpret_cast<const char*>(&lenNombre), sizeof(int));
        archivo.write(entrada.getNombre().c_str(), lenNombre);

        // Escribir turnos
        int turnos = entrada.getTurnos();
        archivo.write(reinterpret_cast<const char*>(&turnos), sizeof(int));

        // Escribir bando
        int lenBando = (int)entrada.getBando().length();
        archivo.write(reinterpret_cast<const char*>(&lenBando), sizeof(int));
        archivo.write(entrada.getBando().c_str(), lenBando);
    }

    archivo.close();
    std::cout << "DEBUG: Ranking guardado (" << numEntradas << " entradas)." << std::endl;
}

void GestorRanking::agregarEntrada(const std::string& nombre, int turnos, const std::string& bando) {
    ranking.emplace_back(nombre, turnos, bando);
    // Ordenar automáticamente después de agregar
    std::sort(ranking.begin(), ranking.end(),
        [](const EntradaRanking& a, const EntradaRanking& b) {
            return a.getTurnos() < b.getTurnos();
        });
    guardarEnArchivo();
}

std::vector<EntradaRanking> GestorRanking::obtenerRanking() const {
    return ranking;
}

std::vector<EntradaRanking> GestorRanking::obtenerTop5() const {
    std::vector<EntradaRanking> top5;
    int limite = std::min(5, (int)ranking.size());
    for (int i = 0; i < limite; i++) {
        top5.push_back(ranking[i]);
    }
    return top5;
}

int GestorRanking::obtenerPuestoJugador(const std::string& nombre) const {
    for (int i = 0; i < (int)ranking.size(); i++) {
        if (ranking[i].getNombre() == nombre) {
            return i + 1; // Los puestos empiezan en 1
        }
    }
    return -1; // No encontrado
}

void GestorRanking::limpiarRanking() {
    ranking.clear();
    guardarEnArchivo();
}

bool GestorRanking::guardar() {
    try {
        guardarEnArchivo();
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "ERROR al guardar ranking: " << e.what() << std::endl;
        return false;
    }
}

bool GestorRanking::cargar() {
    try {
        cargarDelArchivo();
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "ERROR al cargar ranking: " << e.what() << std::endl;
        return false;
    }
}
