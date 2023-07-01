#pragma once
#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

class DataFrameError : public std::exception {
private:
	std::string msg;
public:
	DataFrameError(); 
	DataFrameError(std::string msg_);
	const char* what() const throw(); 
};

class Valor {
    std::string data;
public:
    Valor() = default;
    Valor(std::string data_) : data(data_) {};
    template <typename T>
    T get() const;
    operator std::string() const { return data; }
    operator float() const { return std::stof(data.c_str()); }
    operator int() const { return std::stoi(data.c_str()); }
	friend std::ostream& operator<<(std::ostream& os, const Valor& val) {
		os << val.data;
		return os;
	}
};

class Columna {
	friend class DataFrame;
    std::vector<Valor> valores;

public:
    std::string nombre;
    Columna() = default;
    Columna(const std::vector<Valor> &vals, std::string name = "UnNamed") : valores(vals), nombre(name) {};
    int size() const;
	void add(const Valor& val);

	const Valor& operator[](unsigned int valuePosition) const;
	friend std::ostream& operator<<(std::ostream& os, const Columna& col) {
        os << col.nombre << std::endl;
		for (int i = 0; i < col.size(); i++)
			os << col.valores.at(i).get<std::string>() << std::endl;
		return os;
	}
};

class Fila {
	std::vector<std::string> columnas;
	std::vector<Valor> vals;

public:
	Fila(); 
	Fila(const std::vector<std::string>& cols);
	
	int size() const; 
	void push(const std::string& val); 
	bool set(const std::string& col, const std::string& val); 

	Valor operator[](const std::string col) const;
	const Valor operator[](unsigned int valuePosition) const;

	friend std::ostream& operator<<(std::ostream& os, const Fila& fila) {
		for (int i = 0; i < fila.size(); i++) {
			os << fila.columnas[i] << ": " << fila[i].get<std::string>() << std::endl;
		}
		return os;
	}
};

class DataFrame {
	std::vector<std::string> nombre_columnas;
	std::vector<Fila> filas;
    std::vector<Columna> columnas;
	std::vector<std::string> data;
	char separador = ',';
	int length = 0;
	std::string filename;
public: 
	DataFrame(); 
	DataFrame read_csv(const std::string filename, const char sep = ','); 
	void addRow(std::vector<std::string> data); 
	void save();
	int size(); 
    
    Columna& getCol(const std::string col);
    Columna& operator[](const std::string col);
	Fila getRow(int i); 
	Fila& operator[](int i) { return filas[i]; }

private:
	void parseHeader();
	std::vector<std::string> procesarLinea(std::string linea);
	void parseContenido(); 
};
#endif