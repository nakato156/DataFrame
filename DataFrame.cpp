#include "DataFrame.h" 
using namespace std;

//DataFrameError
DataFrameError::DataFrameError() = default; 
DataFrameError::DataFrameError(string msg_) : msg(msg_){}
const char* DataFrameError::what() const throw() {
	return msg.c_str();
}


// Valor
Valor::operator bool() const { 
	return data == "True" || data == "true";
}
template <typename T>
T Valor::get() const { 
	std::istringstream iss(data);
	T value;
	iss >> value;
	if (iss.fail() || !iss.eof())
		throw std::invalid_argument("Invalid conversion from string to type T");
	return value;
}

// Columna
int Columna::size() const { return valores.size(); }
void Columna::add(const Valor& val){ valores.push_back(val); } 
const Valor& Columna::operator[](unsigned int valuePosition) const {
	if (valuePosition < valores.size())
		return valores[valuePosition];
	throw DataFrameError("Posicion fuera de rango");
}


//Fila
Fila::Fila() = default;
Fila::Fila(const vector<string>& cols) : columnas(cols) {}; 
int Fila::size() const { return columnas.size(); }
void Fila::push(const string& val) { vals.push_back(val); }

bool Fila::set(const string& col, const string& val) {
	int pos = 0;
	for (auto it = columnas.begin(); it != columnas.end(); it++) {
		if (col == *it) {
			vals[pos] = val;
			return true;
		}
		pos++;
	}
	return false;
}

Valor Fila::operator[](const string col) const {
	vector<string>::const_iterator it = columnas.begin();
	int pos = 0;

	for (; it != columnas.end(); it++) {
		if (col == *it)
			return vals[pos];
		pos++;
	}
	throw DataFrameError("Columna no encontrada");
}

const Valor Fila::operator[](unsigned int valuePosition) const {
	if (valuePosition < vals.size()) 
		return vals[valuePosition];
	throw DataFrameError("Posicion fuera de rango");
}

//DataFrame
DataFrame::DataFrame() = default; 
int DataFrame::size() { return length; } 

DataFrame DataFrame::read_csv(const string filename, const char sep){
	separador = sep; 
	this->filename = filename; 
	ifstream archivo(filename); 

	if (!archivo.is_open()) { 
		throw DataFrameError("No se pudo abrir el archivo");
	}
	string linea; 
	while (getline(archivo, linea)) { 
		if (linea != "") { 
			data.push_back(linea); 
			length++; 
		}
	}
	archivo.close(); 
	parseHeader(); 
	length--;   
	parseContenido();
    return *this;
}

void DataFrame::addRow(vector<string> data) {
	if (data.size() != nombre_columnas.size()) 
		throw DataFrameError("La columna agregada no tiene el tamano adecuado"); //error 
	
	Fila nueva_fila = Fila(nombre_columnas); 
	for (auto campo : data)
		nueva_fila.push(campo); 
	filas.push_back(nueva_fila);
	length++;
}

void DataFrame::save() {
	ofstream archivo(filename); 
	if (!archivo.is_open()) throw DataFrameError("No se pudo guardar el archivo");
	//para el header
	for (int c = 0; c < nombre_columnas.size(); c++)
		archivo << nombre_columnas[c] << (c + 1 == nombre_columnas.size() ? '\n' : separador);

	for (int i = 0; i < length; i++) { 
		Fila fila = filas[i]; 
		//para el contenido
		for (size_t c = 0; c < fila.size(); c++) {
			cout << fila[c].get<string>() << endl;
			archivo << fila[c].get<string>() << (c + 1 == fila.size() ? '\n' : separador);
		}
	}
}

Columna& DataFrame::getCol(const string col){
	for(auto it = columnas.begin(); it != columnas.end(); it++){
		if(it->nombre == col) return *it;
	}
	throw DataFrameError("No existe la columna"); //error
}

Columna& DataFrame::operator[](const string col){ return getCol(col); }
Fila DataFrame::getRow(int i) { return filas[i]; }  
vector<string> DataFrame::getColumnas(){ return nombre_columnas; }
void DataFrame::parseHeader() { 
	auto it = data.begin();  
	vector<string> header = procesarLinea(*it); 
	this->nombre_columnas = header;
	for(string &col : header)
		columnas.push_back(Columna({}, col));
}
vector<string> DataFrame::procesarLinea(string linea) {
	vector<string> fila;
	size_t pos;
	while ((pos = linea.find(separador)) != string::npos) {
		string campo = linea.substr(0, pos);
		fila.push_back(campo);
		linea.erase(0, pos + 1);
	}
	fila.push_back(linea);
	return fila;
}
void DataFrame::parseContenido() {
	auto it = data.begin();
	it++;
	for (; it != data.end(); it++) {
		int indexCol = 0;
		string linea = *it;
		Fila fila(nombre_columnas);
		auto row = procesarLinea(linea);
		for (auto campo : row){
			fila.push(Valor(campo));
			columnas.at(indexCol++).add(Valor(campo));
		}

		filas.push_back(fila);
	}
}