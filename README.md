# DataFrame C++

DataFrame C++ es un framework que se basa en simular la funcionalidad del framework o módulo de Python "Pandas". Esta implementación tiene como finalidad dar al usuario una herramienta con la cual pueda manejar de forma efectiva y sencilla archivos `.csv` en C++.

## Uso
Solo necesita crear una instancia de la clase y llamar al método `.read_csv()` que como parametros recibe el nombre del archivo y el separador del archivo. Por defecto es `,`

```c++
int main(){
    DataFrame df = DataFrame().read_csv("test.csv");
    return 0;
}
```
Para acceder a los valores podemos hacerlo de 2 formas:
1. Mediante el índice de la fila y luego el nombre de la columna
2. Mediante el nombre de la columna y luego mediante el índice

```c++
int main(){
    DataFrame df = DataFrame().read_csv("test.csv");
    // primera forma
    std::cout << df[0]["id"] << std::endl;

    //segunda forma
    std::cout << "id: " << df["id"][0]; << std::endl;
    return 0;
}
```
## Inferencia de datos?
Tratando de acercarce más a Pandas se trató de implementar una inferencia de datos. Para el caso de este framework se logra implementando los operadores de conversión adecuados de los tipos más comunes `int`, `string` y `float`. Aquí un ejemplo:
```c++
int main(){
    DataFrame df = DataFrame().read_csv("test.csv");

    // se guarda el dato en una variable de tipo int
    int id = df["id"][0]; // se hace un cast implícito
    std::cout << "id: " << id << std::endl;
    return 0;
}
```
Otra forma de obtener el valor como un tipo de dato específico es mediante el método `.get`. De esta forma:
```c++
int main(){
    DataFrame df = DataFrame().read_csv("test.csv");

    std::cout << "id: " << df["id"][0].get<string>() << std::endl;
    return 0;
}
```
Este método puede realizar la conversión a cualquier tipo de dato. Incluso uno personalizado que usted haya creado.