## 4.Procesamiento de imagenes básico
#### Objetivo
* Elegir un objetivo virtual y fiajarlo. 

Trabajaremos con dos programas principales:

findLocation.cpp
: Este vera los inputs de la camara y detectara que locacion se e interesante para el objetivo planteado.

lookAtLocation.cpp
: este tendra la locacion de la imagen, e intentara mover la camara a donde queremos apuntar con el robot. 

Para el primer programa (findLocation) podemos usar la camara que sea. Para el segundo programa, necesitaremos una camara del robot o del simulador. Para el simulador, en caso de que no este activado, deberemos activar la "flag" "world on", en el archivo de configuracion del entorno (conf/iCub_parts_activation.ini) para que tenga una pelota en frente a el. 
Ya podemos apuntar con la camara a la mesa con el comando 
```bash
yarp rpc /icubSim/head/rpc:i
```
y luego setteamos los grados que moveremos la vision del robot 

```bash
set pos 0 -60
```

#### Encontar ubicación
En YARP, las imagenes de las camaras, son enviadas de un programa a otro usando puertos. En C++, podemos crear un puerto para leer o escribir imagenes con la siguiente linea de codigo
```C++
BufferedPort<ImageOf<PixelRgb> > port;
```
Esto quiere decir "Haz un puerto buffering para enviar/recibir imagenes en formato RGB"

El siguiente codigo podria ser un ejemplo para ogtener imagenes repetidamente:

```C++
#include <stdio.h>
/* Get all OS and signal processing YARP classes */
#include <yarp/os/all.h>
#include <yarp/sig/all.h>
using namespace yarp::os;
using namespace yarp::sig;
int main() {
  Network yarp; // set up yarp
  BufferedPort<ImageOf<PixelRgb> > imagePort;  // make a port for reading images
  imagePort.open("/tutorial/image/in");  // give the port a name
  while (1) { 
    ImageOf<PixelRgb> *image = imagePort.read();  // read an image
    if (image!=NULL) { // check we actually got something
       printf("We got an image of size %dx%d\n", image->width(), image->height());
    }
  }
  return 0;
}
```
Puedes compilarlo de la forma que quieras. Una forma facil de acerlo, es ir al directorio donde tienes el codigo y tipear en la consola 
```bash
yarp cmake
```
Esto creara un CMakeList basico que sera suficiente para compilar este programa. Si luego corres cmake en esta direccion obtendras un programa llamado yarpy (Puedes cambiar este nombres en el archivo CMakeLists.txt). Si lo corres , veras un puerto llamado "/tutorial/image/in" .
Ahora podras conectar una imagen a ese puerto usando `yarp connect <name of image port> /tutorial/image/in`
por ejemplo, para el simulador, deberia ser algo asi 
`yarp connect /icubSim/cam/left /tutorial/image/in`

deberiamos obtener un mensaje como este en el terminal que estamos utilizando :
```
  We got an image of size 320x240
  We got an image of size 320x240
  We got an image of size 320x240
  ...
```

Ahora extenderemos un poco el programa basico, para que el robot sea capaz de capturar el objeto azul .

```C++
#include <stdio.h>
// Get all OS and signal processing YARP classes
#include <yarp/os/all.h>
#include <yarp/sig/all.h>
using namespace yarp::os;
using namespace yarp::sig;
int main() {
  Network yarp; // set up yarp
  BufferedPort<ImageOf<PixelRgb> > imagePort;  // make a port for reading images
  imagePort.open("/tutorial/image/in");  // give the port a name
  while (1) { // repeat forever
    ImageOf<PixelRgb> *image = imagePort.read();  // read an image
    if (image!=NULL) { // check we actually got something
       printf("We got an image of size %dx%d\n", image->width(), image->height());
       double xMean = 0;
       double yMean = 0;
       int ct = 0;
       for (int x=0; x<image->width(); x++) {
         for (int y=0; y<image->height(); y++) {
           PixelRgb& pixel = image->pixel(x,y);
           // very simple test for blueishness
           // make sure blue level exceeds red and green by a factor of 2
           if (pixel.b>pixel.r*1.2+10 && pixel.b>pixel.g*1.2+10) {
            // there's a blueish pixel at (x,y)!
            // let's find the average location of these pixels
            xMean += x;
            yMean += y;
            ct++;
           }
         }
       }
       if (ct>0) {
         xMean /= ct;
         yMean /= ct;
       }
       if (ct>(image->width()/20)*(image->height()/20)) {
         printf("Best guess at blue target: %g %g\n", xMean, yMean);
       }
    }
  }
  return 0;
}
```

Ahora que tenemos un objetivo, hagamos que el robot vea este objetivo. Podemos settearlo como output en un vector de YARP, usando un vector como este. 

```C++
BufferedPort<Vector> targetPort;
```
Agregamos algunas lineas al principio del programa :

```C++
Network yarp;
BufferedPort<ImageOf<PixelRgb> > imagePort;
BufferedPort<Vector> targetPort;  // ADD THIS LINE
targetPort.open("/tutorial/target/out");  // ADD THIS LINE
```
Cuando sabemos cual es nuestro objetivo, lo enviamos:
```
printf("Best guess at blue target: %g %g\n", xMean, yMean);
Vector& v = targetPort.prepare();
v.resize(3);
v[0] = xMean;
v[1] = yMean;
v[2] = 1;       // a confidence value, always good practice to add.  In this case, we pretend to be very confident
targetPort.write();  // send our data
```
Cuando no se selecciona ningun objetivo, igualmente es bueno poner un mensaje que refleje que no se ha seleccionado nada, ya que es dificil diferenciar de que no lo detecte y que se haya muerto el programa si no escribimos mensajes. 

```
* dd this for the condition where no target is picked */
Vector& v = targetPort.prepare();
v.resize(3);
v[0] = 0;
v[1] = 0;
v[2] = 0;
targetPort.write();  // send our data
```
Ahora podemos leer desde este puerto (solo para probar) :
`yarp read ... /tutorial target/out`
#### Mirar al objetivo 


