#include <iostream>
#include <fstream>
#include <string>   
#include <cmath>
#include <filesystem>

namespace fs = std::filesystem;
const int MAX_LENGTH = 100; // Longitud m�xima de cada palabra
const char DELIMITER = ' '; // Car�cter delimitador
const char DELIMITER2 = ','; // Car�cter delimitador
const int MAX_WORDS = 900;
const int tam_num = 15;
const int MAX_LINE_SIZE = 1024;

class Directorio {
public:
	static void crearDirectorio(const std::string& directorio) {
		std::filesystem::create_directory(directorio);
	}

	static void eliminarDirectorio(const std::string& directorio) {
		std::filesystem::remove_all(directorio);
	}
};

class Pagina {
private:
	std::string direccion;
	std::string direccion_copia_bloque;
public:
	Pagina(const std::string& direccion, const std::string& direccion_copia_bloque)
		: direccion(direccion), direccion_copia_bloque(direccion_copia_bloque) {}
	void escribirPagina(const std::string& direccionPagina, const std::string& direccionBloque) {
		std::ifstream bloqueFile(direccionBloque);
		if (!bloqueFile) {
			std::cout << "Error al abrir el archivo del bloque." << std::endl;
			return;
		}
		std::ofstream inputFile(direccionPagina, std::ios::out | std::ios::binary | std::ios::app); // Abrir el archivo en modo "append"
		if (!inputFile) {
			std::cout << "Error al abrir el archivo de la p�gina." << std::endl;
			return;
		}

		char c;
		while (bloqueFile.get(c)) {
			if (c != '\n') { // Omitir caracteres de salto de l�nea
				inputFile.put(c);
			}
			else {
				inputFile << '\n';
			}
		}
		bloqueFile.close();
		inputFile << '\n';
		inputFile.close();
		return;
	}
	void imprimirPagina() {
		std::ifstream bloqueFile(direccion);
		if (!bloqueFile) {
			std::cout << "Error al abrir el archivo del bloque." << std::endl;
			return;
		}
		char c;
		while (bloqueFile.get(c)) {
			std::cout << c;
		}

		bloqueFile.close();
	}

	std::string getDireccion() const {
		return direccion;
	}
	std::string getDireccionBloque() const {
		return direccion_copia_bloque;
	}
	void buscarPagina() {
		// Implementa la l�gica para buscar un bloque espec�fico en el bloque actual
		std::cout << "Buscando bloque" << std::endl;
	}

};

class Bloque {
private:
	int sectoresPorBloque;
	int capacidadSectorBytes;
	int capacidadBytes;
	int sectoresPorPista;
	int pistasPorSuperficie;
	int nPlatos;
	std::string direccion;
public:
	Bloque(const std::string& direccion, int pistasPorSuperficie, int sectoresPorPista, int sectoresPorBloque, int capacidadSectorBytes)
		: direccion(direccion), pistasPorSuperficie(pistasPorSuperficie), sectoresPorPista(sectoresPorPista), sectoresPorBloque(sectoresPorBloque), capacidadSectorBytes(capacidadSectorBytes) {
		std::ofstream file(direccion, std::ios::out | std::ios::binary);
		if (!file) {
			std::cout << "Error al crear el archivo del bloque: " << direccion << std::endl;
			return;
		}
		int cant_sectores = sectoresPorPista * pistasPorSuperficie * 2 * nPlatos;

		/////////////////////////////////////

		double resultado = static_cast<double>(cant_sectores) / sectoresPorBloque;
		int cocienteRedondeado = std::ceil(resultado);

		//std::cout <<"-"<< capacidadSectorBytes << "-" << sectoresPorBloque;
		capacidadBytes = capacidadSectorBytes * sectoresPorBloque;
		const char nullByte = '\0';
		char* buffer = new char[capacidadBytes];
		std::fill_n(buffer, capacidadBytes, nullByte);

		//file.write(buffer, capacidadBytes);
		file.close();

		delete[] buffer;
	}
	
	bool escribirArchivo(std::ifstream& inputFile, const std::string& nombreSector) {
		std::ofstream sectorFile(direccion, std::ios::out | std::ios::binary | std::ios::app); // Abrir el archivo en modo "append"
		if (!sectorFile) {
			std::cout << "Error al abrir el archivo del sector." << std::endl;
			return false;
		}
		// Ignorar la primera l�nea
		//std::string firstLine;
		//std::getline(inputFile, firstLine);

		//sectorFile << "#CABECERA #" << "\n";
		char c;
		int count = 0;
		while (count < capacidadBytes && inputFile.get(c)) {
			if (c != '\n') { // Omitir caracteres de salto de l�nea
				//std::cout << c;
				sectorFile.put(c);
				++count;
			}
		}
		sectorFile << "\n";
		sectorFile.close();
		return (count == capacidadBytes);
	}
	int getCapacidadBytes() const {
		return capacidadBytes;
	}
	bool bloqueLleno() {
		std::ifstream file(direccion, std::ios::ate | std::ios::binary);
		if (!file) {
			std::cout << "Error al abrir el archivo del bloque: " << direccion << std::endl;
			return false;
		}

		int fileSize = file.tellg();  // Obtiene el tama�o actual del archivo
		file.close();
		if (fileSize < capacidadBytes) {
			return false;
		}
		else {
			return true;
		}

	}
	void imprimirBloque() {
		std::ifstream bloqueFile(direccion, std::ios::in | std::ios::binary);
		if (!bloqueFile) {
			std::cout << "Error al abrir el archivo del bloque." << std::endl;
			return;
		}
		char c;
		while (bloqueFile.get(c)) {
			std::cout << c;
		}

		bloqueFile.close();
	}



	int getCapacidadOcupada() {
		std::ifstream bloqueFile(direccion, std::ios::ate | std::ios::binary);
		if (!bloqueFile) {
			std::cout << "Error al abrir el archivo del bloque: " << direccion << std::endl;
			return 0;
		}

		int fileSize = bloqueFile.tellg();  // Obtiene el tama�o actual del archivo
		bloqueFile.close();
		return fileSize;
	}
	std::string getDireccion() const {
		return direccion;
	}
	void buscarBloque() {
		// Implementa la l�gica para buscar un bloque espec�fico en el bloque actual
		std::cout << "Buscando bloque" << std::endl;
	}

	
};

class ManagerBuffer {
private:
	int sectoresPorBloque;
	int capacidadSectorBytes;
	std::vector<std::string> paginas;
	int capacidad;

public:
	ManagerBuffer(int capacidad,int sectoresBloque,int capacidadSectorBytes) : capacidad(capacidad), sectoresPorBloque(sectoresPorBloque), capacidadSectorBytes(capacidadSectorBytes){
		Directorio::eliminarDirectorio("Memoria_Principal"); // Elimina disco si ya existe
		Directorio::crearDirectorio("Memoria_Principal");
	}

	~ManagerBuffer() {
		for (const auto& pagina : paginas) {
			std::remove(pagina.c_str());
		}
		Directorio::eliminarDirectorio("Memoria_Principal"); // Elimina disco si ya existe
	}

	void generarPagina(Pagina* pagina, std::string numero) {
		std::string direccion_pag = pagina->getDireccion();
		std::string direccion_blo = pagina->getDireccionBloque();

		// Verificar si la p�gina ya est� en el vector
		auto it = std::find(paginas.begin(), paginas.end(), direccion_pag);
		if (it != paginas.end()) {
			// La p�gina ya est� en el vector, moverla al inicio
			std::rotate(paginas.begin(), it, it + 1);
		}
		else {
			// La p�gina no est� en el vector, eliminar el �ltimo elemento si se alcanz� la capacidad m�xima
			if (paginas.size() >= capacidad) {
				eliminarPagina();
			}
			// Agregar la nueva p�gina al inicio del vector
			paginas.insert(paginas.begin(), direccion_pag);
		}

		pagina->escribirPagina(direccion_pag, direccion_blo);  // Escribir el bloque en el archivo de la p�gina
		std::cout << "Contenido del buffer de p�ginas:" << std::endl;
		for (const auto& pagina : paginas) {
			std::cout << pagina << std::endl;
		}
	}

	void eliminarPagina() {
		if (!paginas.empty()) {
			std::string paginaDir = paginas.front();
			paginas.erase(paginas.begin());
			// Eliminar el archivo de la p�gina
			std::remove(paginaDir.c_str());
		}
	}

	void mostrarPagina(Pagina* pagina, std::string numero) {
		pagina->imprimirPagina();
	}

	/*void buscarPagina(int numeroPagina) {
		if (numeroPagina >= 1 && numeroPagina <= paginas.size()) {
			std::string direccionPagina = paginas[numeroPagina - 1];
			Bloque bloque(direccionPagina, sectoresPorBloque, capacidadSectorBytes);
			bloque.buscarBloque();
			// Mover la p�gina al final del vector (actualizar LRU)
			std::rotate(paginas.begin() + numeroPagina - 1, paginas.begin() + numeroPagina, paginas.end());
		}
		else {
			std::cout << "La p�gina especificada no existe." << std::endl;
		}
	}*/

	void imprimirBuffer() {
		std::cout << "Contenido del buffer de p�ginas:" << std::endl;
		for (const auto& pagina : paginas) {
			std::cout << pagina << std::endl;
		}
	}

	int ubicar_registro(int contador2,int& ubi) {
		int total_lineas = 0;
		std::string registro = "";
		int i = 1;
		int valor = 0;
		std::string archivo_path;
		while (true) {
			archivo_path = "Directorio/bloque_" + std::to_string(i) + ".txt";
			std::ifstream archivo(archivo_path);
			if (!archivo) {
				break;  // No hay m�s archivos para leer
			}

			std::string linea;

			int contador = 0;
			int let = 0;
			while (std::getline(archivo, linea)) {
				++contador;

				if (valor != contador2) {
					valor += 1;
					++let;
					registro = linea;
				}

			}
			total_lineas += contador;

			if (total_lineas >= contador2) {
				ubi = let;
				break;
			}
			archivo.close();
			++i;  // Pasar al siguiente archivo
		}
		return i;
	}

	void imprimir_registro(int contador2) {
		int total_lineas = 0;
		std::string registro = "";
		int i = 1;
		int valor = 0;
		std::string archivo_path;
		std::cout << "impri" << std::endl;
		while (true) {
			archivo_path = "Directorio/bloque_" + std::to_string(i) + ".txt";

			std::ifstream archivo(archivo_path);
			if (!archivo) {
				break;  // No hay m�s archivos para leer
			}

			std::string linea;
		
			int contador = 0;
			int let = 0;
			while (std::getline(archivo, linea)) {
				++contador;
				
				if (valor != contador2) {
					valor += 1;
					++let;
					registro = linea;
				}
								
			}
			total_lineas += contador;
			
			if (total_lineas >= contador2) {
				std::cout << "DATOS DEL REGISTRO "<<std::endl;
				std::cout << "BLOQUE: " << i << std::endl;
				std::cout << "LINEA " << let << std::endl;
				std::cout << "---------------------" << std::endl;
				std::cout << registro << std::endl;
				
				//std::cout << linea;
				break;
			}
			archivo.close();
			++i;  // Pasar al siguiente archivo
		}
		return ;  // Retornar la ruta al archivo
	}

	void tam_registro() {
		std::ifstream archivo("esquema.txt"); // Nombre del archivo de texto
		if (archivo.is_open()) {
			int suma = 0;
			char c;

			// Leer hasta encontrar el primer salto de l�nea
			while (archivo.get(c) && c != '\n') {
				if (c == '#') {
					int numero = 0;
					bool hayNumero = false;

					// Leer los caracteres entre los delimitadores '#'
					while (archivo.get(c) && c != '\n') {
						if (c >= '0' && c <= '9') {
							numero = numero * 10 + (c - '0');
							hayNumero = true;
						}
						else if (c == '#') {
							if (hayNumero) {
								suma += numero;
							}
							numero = 0;
							hayNumero = false;
						}
					}

					if (hayNumero) {
						suma += numero;
					}
				}
			}

			suma += 1;
			std::cout << "Bytes por Registro: " << suma << std::endl;
			std::cout << "Bits por Registro: " << suma * 8 << std::endl;
		}
		else {
			std::cout << "No se pudo abrir el archivo." << std::endl;
		}

	}

	void tam_file() {
		std::ifstream archivo("Directorio/bloque_1.txt");
		if (!archivo) {
			std::cout << "No se pudo abrir el archivo." << std::endl;
			return;
		}

		char caracter;
		int tamano = 0;

		while (archivo.get(caracter)) {
			tamano++;
		}
		std::cout << "Bits por File: " << tamano * 8 << std::endl;

		archivo.close();

	}

	void eliminar_registro(int registro) {
		int linea;
		char reemplazo = '-';
		int ubicacion=ubicar_registro(registro,linea);
		std::string posi = std::to_string(ubicacion);
		std::string direc = "Memoria_Principal/pagina_" + posi + ".txt";
		std::fstream archivo(direc, std::ios::in | std::ios::out);

		if (!archivo) {
			std::cout << "No se pudo abrir el archivo." << std::endl;
			return;
		}

		int contador = 1;
		std::string lineaActual;
		archivo <<"\n";
		while (std::getline(archivo, lineaActual)) {
			contador++;
			if (contador == linea) {
				for (char& caracter : lineaActual) {
					caracter = reemplazo;
				}
				archivo.seekp(archivo.tellg());
				archivo << lineaActual;
				break;
			}
		}
		archivo << "\n";
		archivo.close();
	}
};

class Cabezal {
private:
	std::string direccion;
	int capacidadBytes;
public:
	Cabezal(const std::string& _direccion, int _capacidadBytes) : direccion(_direccion), capacidadBytes(_capacidadBytes) {}

	void leerArchivo() {
		std::fstream sectorFile(direccion, std::ios::in | std::ios::binary);
		if (!sectorFile) {
			std::cout << "Error al abrir el archivo del sector." << std::endl;
			return;
		}

		char c;
		while (sectorFile.get(c)) {
			std::cout << c;
		}
		sectorFile.close();
	}

	bool escribirArchivo(std::ifstream& inputFile) {
		//std::cout << direccion;
		std::ofstream sectorFile(direccion, std::ios::out | std::ios::binary);
		if (!sectorFile) {
			std::cout << "Error al abrir el archivo del sector." << std::endl;
			return false;
		}

		/*ESCRIBIR REGISTROS COMPLETOS*/

		std::string registro;
		int bytesEscritos = 0;
		std::string linea;
		std::streampos posAnterior;
		std::streampos posActual;
		while (bytesEscritos < capacidadBytes && std::getline(inputFile, linea)) {
			posAnterior = posActual;
			posActual = inputFile.tellg(); // Guardar la posici�n despu�s de leer la l�nea
			if (linea.empty()) {
				continue;  // Omitir l�neas vac�as
			}
			if (linea.length() > capacidadBytes - bytesEscritos) {
				inputFile.seekg(posAnterior); // Retroceder al principio de la l�nea anterior
				break;  // El registro no cabe en el sector actual, salir del bucle
			}
			registro += linea + '\n';
			bytesEscritos += linea.length();
		}
		sectorFile << registro;
		sectorFile.close();
		return true;

	}

};

class Sector {
private:
	int bytesOcupados;
	int capacidadBytes;
	std::string direccion;
public:
	Sector(const std::string& direccion, int capacidadBytes)
		: direccion(direccion), capacidadBytes(capacidadBytes), bytesOcupados(0) {
		std::ofstream file(direccion, std::ios::out | std::ios::binary);
		if (!file) {
			std::cout << "Error al crear el archivo del sector: " << direccion << std::endl;
			return;
		}

		/*INCIALIZAR ESPACIOS*/
		const char nullByte = '\0';
		char* buffer = new char[capacidadBytes];
		std::fill_n(buffer, capacidadBytes, nullByte);

		//file.write(buffer, capacidadBytes);
		file.close();

		delete[] buffer;
	}
};

class Pista {
private:
	int pistasPorSuperficie;
	int sectoresPorPista;
	int capacidadSectorBytes;
	std::string baseDir;
public:
	Pista(const std::string& baseDir, int sectoresPorPista, int capacidadSectorBytes)
		: baseDir(baseDir), sectoresPorPista(sectoresPorPista), capacidadSectorBytes(capacidadSectorBytes) {
		Directorio::crearDirectorio(baseDir);

		/*INICIALIZAR SECTORES DE CADA PISTA*/
		for (int m = 1; m <= sectoresPorPista; m++) {
			std::string sectorDir = baseDir + "/sector_" + std::to_string(m) + ".txt";
			Sector* mySector = new Sector(sectorDir, capacidadSectorBytes);
			delete mySector;
		}
	}
};

class Superficie {
private:
	int pistasPorSuperficie;
	int sectoresPorPista;
	int capacidadSectorBytes;
	std::string baseDir;
public:
	Superficie(const std::string& baseDir, int pistasPorSuperficie, int sectoresPorPista, int capacidadSectorBytes)
		: baseDir(baseDir), pistasPorSuperficie(pistasPorSuperficie), sectoresPorPista(sectoresPorPista), capacidadSectorBytes(capacidadSectorBytes) {
		Directorio::crearDirectorio(baseDir);

		/*INICIALIZAR PISTA DE CADA SUPERFICIE*/
		for (int k = 1; k <= pistasPorSuperficie; k++) {
			std::string pistaDir = baseDir + "/pista_" + std::to_string(k);
			Pista* myPista = new Pista(pistaDir, sectoresPorPista, capacidadSectorBytes);
			delete myPista;
		}
	}
};

class Plato {
private:
	std::string baseDir;
	int pistasPorSuperficie;
	int sectoresPorPista;
	int capacidadSectorBytes;

public:
	Plato(const std::string& baseDir, int pistasPorSuperficie, int sectoresPorPista, int capacidadSectorBytes)
		: baseDir(baseDir), pistasPorSuperficie(pistasPorSuperficie), sectoresPorPista(sectoresPorPista), capacidadSectorBytes(capacidadSectorBytes) {
		Directorio::crearDirectorio(baseDir);

		/*INICIALIZAR SUPERFICIE DE CADA PLATO*/
		for (int j = 1; j <= 2; j++) {
			std::string superficieDir = baseDir + "/superficie_" + std::to_string(j);
			Superficie* mySuperficie = new Superficie(superficieDir, pistasPorSuperficie, sectoresPorPista, capacidadSectorBytes);
			delete mySuperficie;
		}
	}
};

class Disco {
private:
	std::string nombreDisco;
	int nPlatos;
	int pistasPorSuperficie;
	int sectoresPorPista;
	int capacidadSectorBytes;
	int sectoresPorBloque;
	Plato** platos;

public:
	Disco(const std::string& nombre, int nPlatos, int pistasPorSuperficie, int sectoresPorPista, int capacidadSectorBytes, int sectoresPorBloque)
		: nombreDisco(nombre), nPlatos(nPlatos), pistasPorSuperficie(pistasPorSuperficie), sectoresPorPista(sectoresPorPista), capacidadSectorBytes(capacidadSectorBytes), sectoresPorBloque(sectoresPorBloque) {
		Directorio::eliminarDirectorio(nombreDisco); // Elimina disco si ya existe
		Directorio::crearDirectorio(nombreDisco);

		/*INICIALIZAR PLATOS DEL DISCO*/
		platos = new Plato * [nPlatos];
		for (int i = 0; i < nPlatos; i++) {
			std::string platoDir = nombreDisco + "/plato_" + std::to_string(i + 1);
			platos[i] = new Plato(platoDir, pistasPorSuperficie, sectoresPorPista, capacidadSectorBytes);
		}

		/*INICIALIZAR BLOQUES*/
		std::string direct = "./Directorio";
		Directorio::eliminarDirectorio(direct); // Elimina directorio si ya existe
		Directorio::crearDirectorio(direct);
		int cant_sectores = sectoresPorPista * pistasPorSuperficie * 2 * nPlatos;

		/////////////////////////////////////

		double resultado = static_cast<double>(cant_sectores) / sectoresPorBloque;
		int cocienteRedondeado = std::ceil(resultado);
		//std::cout << cant_sectores << "-" << sectoresPorBloque << "-" << cocienteRedondeado;
		for (int m = 1; m <= cocienteRedondeado; m++) {
			std::string bloqueDir = direct + "/bloque_" + std::to_string(m) + ".txt";
			Bloque* mybloque = new Bloque(bloqueDir, pistasPorSuperficie, sectoresPorPista, sectoresPorBloque, capacidadSectorBytes);
			delete mybloque;
		}
	}

	~Disco() {
		Directorio::eliminarDirectorio("Directorio");

		//for (int i = 0; i < nPlatos; i++) {
		//	delete platos[i];
		//}
		//delete[] platos;
	}

	void almacenar_archivo(const std::string& archivoTxt) {
		std::ifstream inputFile(archivoTxt, std::ios::in | std::ios::binary);
		if (!inputFile) {
			std::cout << "Error al abrcir el archivo: " << archivoTxt << std::endl;
			return;
		}

		/*UBICACION ACTUAL*/
		int platoActual = 0, superficieActual = 0, pistaActual = 0, sectorActual = 0;
		bool discoLleno = false;
		/*RECORRER COMPONENTES*/
		while (!inputFile.eof()) {
			std::string sectorDir = nombreDisco + "/plato_" + std::to_string(platoActual + 1) + "/superficie_" + std::to_string(superficieActual + 1) + "/pista_" + std::to_string(pistaActual + 1) + "/sector_" + std::to_string(sectorActual + 1) + ".txt";
			Cabezal* cabezal = new Cabezal(sectorDir, capacidadSectorBytes);
			if (!cabezal->escribirArchivo(inputFile)) {
				delete cabezal;
				break;
			}

			delete cabezal;
			//GUARDAR EN EL DISCO 
			if (sectorActual != sectoresPorPista) {
				if (pistaActual != pistasPorSuperficie) {
					if (superficieActual == 1) {
						platoActual += 1;
						superficieActual = 0;
						if (platoActual == (nPlatos)) {
							pistaActual += 1;
							if (pistaActual == pistasPorSuperficie) {
								sectorActual += 1;
								pistaActual = 0;
							}
							platoActual = 0;

						}
					}
					else {
						superficieActual += 1;
					}

				}
			}
			else {
				discoLleno = true;
			}



			if (discoLleno) {
				std::cout << "El disco est� lleno. No se pueden almacenar m�s datos." << std::endl;
				break;
			}
		}

		inputFile.close();
	}

	void obtener_archivo_Bloque() {
		std::vector<Bloque*> bloques; // Cambio: Almacenar punteros a Bloque en lugar de objetos Bloque
		std::string direct = "./Directorio/";
		int cantSectores = sectoresPorPista * pistasPorSuperficie * 2 * nPlatos;
		double resultado = static_cast<double>(cantSectores) / sectoresPorBloque;
		int platoActual = 0, superficieActual = 0, pistaActual = 0, sectorActual = 0;
		bool bloquelleno = false;
		///////////////////////////////////////////////////////////////////////////////////////////
		int cantBloques = std::ceil(resultado); // cantidad de bloques
		int contador = 0;
		int bloqueActual = 0;
		int contador_sectores_tipo_completo = 0;
		std::string bloqueDir = direct + "/bloque_" + std::to_string(bloqueActual + 1) + ".txt";
		Bloque* bloque = new Bloque(bloqueDir, pistasPorSuperficie, sectoresPorPista, sectoresPorBloque, capacidadSectorBytes);
		int contador_sector = 1;
		while (contador < cantSectores) {
			std::string sectorDir = "disco/plato_" + std::to_string(platoActual + 1) + "/superficie_" + std::to_string(superficieActual + 1) + "/pista_" + std::to_string(pistaActual + 1) + "/sector_" + std::to_string(sectorActual + 1) + ".txt";
			std::ifstream inputFile(sectorDir, std::ios::in | std::ios::binary);
			if (!inputFile) {
				std::cout << "Error al abrir el archivo: " << std::endl;
				delete bloque; // Liberar memoria antes de salir de la funci�n
				return;
			}
			if (contador_sector > sectoresPorBloque) {
				contador_sector = 0;
				//bloques.push_back(bloque);
				delete bloque;
				bloqueActual += 1;
				bloqueDir = direct + "/bloque_" + std::to_string(bloqueActual + 1) + ".txt";
				bloque = new Bloque(bloqueDir, pistasPorSuperficie, sectoresPorPista, sectoresPorBloque, capacidadSectorBytes);
				bloque->escribirArchivo(inputFile, sectorDir);
				contador_sector += 1;
			}
			else {
				bloque->escribirArchivo(inputFile, sectorDir);
			}
			if (sectorActual != sectoresPorPista) {
				if (pistaActual != pistasPorSuperficie) {
					if (superficieActual == 1) {
						platoActual += 1;
						superficieActual = 0;
						if (platoActual == (nPlatos)) {
							pistaActual += 1;
							if (pistaActual == pistasPorSuperficie) {
								sectorActual += 1;
								pistaActual = 0;
							}
							platoActual = 0;

						}
					}
					else {
						superficieActual += 1;
					}

				}
			}

			contador += 1;
			inputFile.close();
			contador_sector += 1;
		}

		delete bloque; // Liberar memoria antes de salir de la funci�n
	}

	void imprimirBloque(const std::string& direccionBloque) {
		std::ifstream bloqueFile(direccionBloque, std::ios::in | std::ios::binary);
		if (!bloqueFile) {
			std::cout << "Error al abrir el archivo del bloque." << std::endl;
			return;
		}
		capacidadOcupadaEnBloque(direccionBloque);
		char c;
		while (bloqueFile.get(c)) {
			std::cout << c;
		}

		bloqueFile.close();
	}

	void capacidadOcupadaEnBloque(const std::string& direccionBloque) {
		std::ifstream bloqueFile(direccionBloque, std::ios::ate | std::ios::binary);
		if (!bloqueFile) {
			std::cout << "Error al abrir el archivo del bloque: " << direccionBloque << std::endl;
			return;
		}
		int fileSize = bloqueFile.tellg();  // Obtiene el tama�o actual del archivo
		std::cout << std::endl << "CAPACIDAD DEL BLOQUE ES:" << fileSize << std::endl;
		bloqueFile.close();
		return;
	}

	long long calcularTamanoCarpeta(const std::string& rutaCarpeta) {
		long long tamanoTotal = 0;

		for (const auto& archivo : fs::recursive_directory_iterator(rutaCarpeta)) {
			if (fs::is_regular_file(archivo)) {
				tamanoTotal += fs::file_size(archivo.path());
			}
		}

		return tamanoTotal;
	}

	int inicializarDisco() {
		int capacidadDisco = nPlatos * pistasPorSuperficie * sectoresPorPista * capacidadSectorBytes;
		return capacidadDisco;
	}

	void imprimir_sector(int sector) {
		int platoActual = 0, superficieActual = 0, pistaActual = 0, sectorActual = 0;
		int cantSectores = sectoresPorPista * pistasPorSuperficie * 2*nPlatos;
		int contador = 0;
		while (contador < cantSectores) {
			if (contador == (sector - 1)) {
				std::string sectorDir = nombreDisco + "/plato_" + std::to_string(platoActual + 1) + "/superficie_" + std::to_string(superficieActual + 1) + "/pista_" + std::to_string(pistaActual + 1) + "/sector_" + std::to_string(sectorActual + 1) + ".txt";
				std::string carpetDir = nombreDisco + "/plato_" + std::to_string(platoActual + 1) + "/superficie_" + std::to_string(superficieActual + 1) + "/pista_" + std::to_string(pistaActual + 1);
				std::cout << "CAPACIDAD DE LA CARPETA DONDE SE UBICA EL SECTOR ES:" << calcularTamanoCarpeta(carpetDir) << std::endl;;
				Cabezal* cabezal = new Cabezal(sectorDir, capacidadSectorBytes);
				///DATOS DEL SECTOR///
				std::ifstream sectorFile(sectorDir, std::ios::in | std::ios::binary);
				if (!sectorFile) {
					std::cout << "Error al abrir el archivo del sector." << std::endl;
					return;
				}
				sectorFile.seekg(0, std::ios::end); // Ir al final del archivo
				int fileSize = sectorFile.tellg();  // Obtener el tama�o actual del archivo
				sectorFile.seekg(0, std::ios::beg); // Volver al principio del archivo

				std::cout << "UBICACION DEL ARCHIVO: " << sectorDir << std::endl;
				std::cout << "CAPACIDAD DEL SECTOR: " << capacidadSectorBytes << " bytes" << std::endl;
				std::cout << "CAPACIDAD USADA DEL SECTOR: " << fileSize - 1 << " bytes" << std::endl;
				std::cout << "CAPACIDAD RESTANTE DEL SECTOR: " << capacidadSectorBytes - (fileSize - 1) << " bytes" << std::endl;
				sectorFile.close();
				cabezal->leerArchivo();
			}
			if (sectorActual != sectoresPorPista) {
				if (pistaActual != pistasPorSuperficie) {
					if (superficieActual == 1) {
						platoActual += 1;
						superficieActual = 0;
						if (platoActual == (nPlatos)) {
							pistaActual += 1;
							if (pistaActual == pistasPorSuperficie) {
								sectorActual += 1;
								pistaActual = 0;
							}
							platoActual = 0;

						}
					}
					else {
						superficieActual += 1;
					}

				}
			}
			contador += 1;
		}
		//std::cout << "No se encuentra el sector" << std::endl; // Si no se encuentra el sector, se devuelve una cadena vac�a
	}

	void generar_esquema() {
		std::ofstream archivo("esquema.txt", std::ios::app | std::ios::ate);// Abre el archivo en modo de escritura y posiciona el cursor al final
		if (archivo.is_open()) {
			std::cout << "Nombre de la Tabla: ";
			char caracter;
			while (std::cin.get(caracter) && caracter != '\n') {
				archivo.put(caracter);
			}
			archivo << "#";
			std::cout << "Esquema: ";
			while (std::cin.get(caracter) && caracter != '\n') {
				archivo.put(caracter == ' ' ? '#' : caracter);
			}
			archivo << "\n";
			archivo.close();
			std::cout << "La frase se ha escrito en el archivo." << std::endl;
		}
		else {
			std::cerr << "No se pudo abrir el archivo." << std::endl;
		}
	}


	void exportar_tabla() {
		int linea_esquema = 1;
		//std::cout << nombreArchivo;
		std::cout << "Nombre de la Tabla que deseas exportar: ";
		std::string res = "";
		char caracter;
		while (std::cin.get(caracter) && caracter != '\n') {
			res+=caracter;
		}
		std::string resultado_ar = res+".txt";
		std::string archivo_dc = res + ".csv";
		std::ofstream resultado(resultado_ar, std::ios::app); // Crea el archivo si no existe 
		char carpeta_Archivo[MAX_LENGTH] = "";

		std::ifstream archivo(archivo_dc);
		std::ifstream esquema("esquema.txt");

		if (!archivo) {
			std::cout << "Error al abrir los archivo." << std::endl;
			return;
		}
		if (!esquema) {
			std::cout << "Error al abrir los archivos es." << std::endl;
			return;
		}
		if (!resultado) {
			std::cout << "Error al abrir los archivos res." << std::endl;
			return;
		}

		if (esquema.is_open()) {
			char caracter;
			int contador = 1;
			int number = 0;
			int index = 0;
			int currentNumber = 0; // N�mero actual que se est� leyendo
			bool isNumber = false;
			int numbers[tam_num];
			while (esquema.get(caracter)) {
				if (caracter == '\n') {
					contador++;
					if (contador > linea_esquema) {
						break;
					}
				}

				if (contador < linea_esquema) {
					continue;  // Ignorar las l�neas
				}
				if (caracter >= '0' && caracter <= '9') {
					number = number * 10 + (caracter - '0');
					//std::cout << "numero" << number;
					isNumber = true;
				}
				else if (isNumber) {
					numbers[index++] = number;
					number = 0;
					isNumber = false;
				}


			}
			esquema.close();

			if (archivo.is_open()) {
				//std::cout << "hola";
				const int MAX_LINE_SIZE = 1024;
				char columnLine[MAX_LINE_SIZE];
				archivo.get(columnLine, MAX_LINE_SIZE, '\n');
				archivo.ignore(MAX_LINE_SIZE, '\n');
				//output << columnLine << std::endl;
				char dataLine[MAX_LINE_SIZE];

				while (archivo.get(dataLine, MAX_LINE_SIZE, '\n')) {
					archivo.ignore(MAX_LINE_SIZE, '\n'); // Descarta el car�cter de nueva l�nea
					int numerador = 0;
					char value[MAX_LINE_SIZE];
					bool inQuotes = false;
					bool firstValue = true;
					int i = 0;
					int j = 0;
					while (dataLine[i] != '\0') {
						if (dataLine[i] == '\"') {
							inQuotes = !inQuotes;
							i++;
							continue;
						}

						//std::cout << "inquotes" << inQuotes;
						if (!inQuotes && dataLine[i] == ',') {

							value[j] = '\0';

							// Determina la cantidad de bytes que representa el valor
							size_t bytes = j; // En este ejemplo, el tama�o en bytes se basa en la longitud del valor

							// Escribe los espacios correspondientes en el archivo de salida
							if (firstValue) {
								firstValue = false;
							}

							//std::cout << "nume" << numbers[numerador] << "-" << std::endl;
							resultado << std::left << std::setw(numbers[numerador]) << value;

							numerador++;
							j = 0;
							//std::cout << j;
							i++;
							//std::cout << i;
							//std::cout << numerador << "-";
							continue;
							//numerador++;

						}


						value[j++] = dataLine[i++];

						//std::cout << numerador << "-";

					}
					value[j] = '\0';

					// Determina la cantidad de bytes que representa el valor
					size_t bytes = j; // En este ejemplo, el tama�o en bytes se basa en la longitud del valor

					// Escribe los espacios correspondientes en el archivo de salida

					if (firstValue) {
						firstValue = false;
					}

					resultado << std::left << std::setw(numbers[10]) << value;
					resultado << std::endl;
					//std::wcout << 20;


					//std::cout << numerador << "-";
				}

				archivo.close();
				resultado.close();

				//std::cout << "Archivo convertido con �xito." << std::endl;
				return;
			}
		}
		else {
			std::cout << "No se pudo abrir el archivo." << std::endl;
			return;
		}


	}



};

void menu() {

	/////////////////////////////////////////////////////
	char tabla[15];
	std::string nombreBloque ;
	std::string nombrePagina;
	std::string nombreDisco = "disco";
	std::string archivoTxt, nombreArchivo;
	int opcion = 0, sector, cantidadBits, linea,registro,block;
	std::string bloque;
	int capacidadTotal;
	std::string str; // Variable para almacenar la tabla a guardar en disco
	std::vector<Pagina*> paginas;
	int nPlatos = 0, pistasPorSuperficie = 0, sectoresPorPista = 0, capacidadSectorBytes = 0, sectoresPorBloque = 0;
	Disco* myDisco = nullptr;
	ManagerBuffer* buffer = new ManagerBuffer(4, sectoresPorPista, capacidadSectorBytes); // Crear buffer de p�ginas con capacidad para 4 p�ginas
	Pagina* pagina = nullptr;
	while (opcion != 11) {
		std::cout << "---------------MENU DISCO---------------" << std::endl;
		std::cout << "1. INICIALIZAR DISCO" << std::endl;
		std::cout << "2. ESQUEMA TABLA" << std::endl;
		std::cout << "3. GENERAR FILE" << std::endl;
		std::cout << "4. GUARDAR TABLA EN DISCO" << std::endl;
		std::cout << "5. DATOS POR REGISTRO" << std::endl;
		std::cout << "6. IMPRIMIR REGISTRO" << std::endl;
		std::cout << "7. BIT POR FILE" << std::endl;
		std::cout << "8. IMPRIMIR SECTOR" << std::endl;
		std::cout << "9. IMPRIMIR BLOQUE" << std::endl;
		std::cout << "10. ELIMINAR REGISTRO" << std::endl;
		std::cout << "11. SALIR" << std::endl;
		std::cout << "Ingrese una opci�n: ";
		std::cin >> opcion;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpiar el b�fer del cin
		switch (opcion) {
		case 1:
			std::cout << "Cantidad de platos:" << std::endl;
			std::cin >> nPlatos;
			std::cout << "Cantidad de pistas:" << std::endl;
			std::cin >> pistasPorSuperficie;
			std::cout << "Cantidad de sectores:" << std::endl;
			std::cin >> sectoresPorPista;
			std::cout << "Tama�o de sectores:" << std::endl;
			std::cin >> capacidadSectorBytes;
			std::cout << "Cantidad de sectores en un bloque:" << std::endl;
			std::cin >> sectoresPorBloque;

			if (myDisco != nullptr) {
				delete myDisco; // Liberar memoria si ya se hab�a inicializado un disco anteriormente
			}
			myDisco = new Disco(nombreDisco, nPlatos, pistasPorSuperficie, sectoresPorPista, capacidadSectorBytes, sectoresPorBloque);
			buffer = new ManagerBuffer(4, sectoresPorPista, capacidadSectorBytes); // Crear buffer de p�ginas con capacidad para 4 p�ginas

			capacidadTotal = myDisco->inicializarDisco();
			std::cout << "La capacidad total del disco es: " << capacidadTotal << " bytes." << std::endl;
			break;

		case 2:
			myDisco->generar_esquema();
			break;
		case 3:
			myDisco->exportar_tabla();
			break;
		case 4:
			std::cout << "Tabla a guardar en disco:";
			std::cin.get(tabla, 20);
			myDisco->almacenar_archivo(tabla);
			myDisco->obtener_archivo_Bloque();
			break;
		case 5:

			buffer->tam_registro();
			break;

		case 6:
			std::cout << "Imprmir Registro: ";
			std::cin >> registro;
			buffer->imprimir_registro(registro);
			break;

		case 7:

			buffer->tam_file();
			break;
		case 8:
			std::cout << "SECTOR:";
			std::cin >> sector;
			myDisco->imprimir_sector(sector);
			break;
		case 9:
			std::cout << "BLOQUE:";
			std::cin >> bloque;
			nombreBloque = "Directorio/bloque_" + bloque + ".txt";
			nombrePagina = "Memoria_Principal/pagina_" + bloque + ".txt";
			if (nombreBloque == "Directorio/bloque_0.txt") {
				break;
			}
			std::cout << nombreBloque;
			pagina = new Pagina(nombrePagina, nombreBloque);
			paginas.push_back(pagina);
			buffer->generarPagina(pagina, bloque);
			buffer->mostrarPagina(pagina, bloque);
			break;
		case 10:
			std::cout << "Eliminar Registro: ";
			std::cin >> registro;
			block=buffer->ubicar_registro(registro,linea);
			bloque = std::to_string(block);
			nombreBloque = "Directorio/bloque_" + bloque + ".txt";
			nombrePagina = "Memoria_Principal/pagina_" + bloque + ".txt";
			if (nombreBloque == "Directorio/bloque_0.txt") {
				break;
			}
			pagina = new Pagina(nombrePagina, nombreBloque);
			paginas.push_back(pagina);
			buffer->generarPagina(pagina, bloque);
			std::cin >>linea;
			buffer->eliminar_registro(registro);
			break;
		case 11:
			std::cout << "Saliendo del programa..." << std::endl;
			break;
		default:
			std::cout << "Opci�n inv�lida. Por favor, ingrese una opci�n v�lida." << std::endl;
			break;
		}
	}

	// Liberar la memoria del objeto Disco antes de salir del programa
	delete myDisco;
	delete buffer;
	delete pagina;
}

int main() {
	menu();
	system("pause");
	return 0;
}
