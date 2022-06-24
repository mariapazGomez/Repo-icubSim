
## Tutorial Hello World 

En este tutorial se muestra como usar `Cmake` . En este primer tutorial aun no se utiliza el robot. 

En las indicaciones se asume que se tiene :
* YARP funcionando y compilando sin problemas 
* La direccion en el archivo de configuracion de las herramientas de YARP de YARP_DIR 

Trabajamos solo con un archivo, el cual llamamos main.cpp que contiene el siguiente codigo.

```
#include <cstdio>
#include <yarp/os/SystemClock.h>

using namespace std;
using namespace yarp::os;

int main()
{
    printf("Starting the application\n");
    int times=10;

    while(times--)
    {
        printf("Hello iCub\n");
        SystemClock::delaySystem(0.5); //wait 0.5 seconds
    }
    printf("Goodbye!\n");
}

```

Este muestra la frace "hello world" varias veces, esperando 0.5 segundos entre cada repeticion.

Luego se nos muestra como compilar el codigo. 
Debemos crear un archivo con las dependencias de este programa, para que este este conectado con YARP y con el iCub. El archivo debe llamarse `CMakeLists.txt` 

```
project(HELLO_ICUB)

find_package(YARP REQUIRED)

add_executable(hello main.cpp)

# we now add the YARP libraries to our project.
target_link_libraries(hello ${YARP_LIBRARIES})

```
Este ultimo codigo le dice a cmake que project files compilar para que funcione todo correctamente sin problemas.
Ahora para correr el programa debemos usar el comando `cmake ./ `   .

Notaremos que generará un seguidillo de archivos :
* Makefile
* CMakeCache.txt
* cmake_install.cmake
* CMakeFiles Foleder

Para compilar Solo debemos ecribir en la terminal (ubicada en la misma ubicacion de los archivos creados) `make` y luego `./hello`  .




