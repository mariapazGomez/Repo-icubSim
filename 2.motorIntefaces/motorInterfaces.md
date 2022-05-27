## 2. Motores (control de un brazo)
En este tutorial nos enfocamos en el control de los motores, donde buscaremos controlar un brazo, y en este caso si usaremos el simulador. 

En el tutorial se asume que el simulador esta corriendo, por lo que lo primero que debemos hacer es poner a correr el simulador escribiendo 'yarpserver' y luego 'iCub_SIM' .

Luego de esto podemos seguir el paso a paso del tutorial, donde se nos explica paso a paso cada parte del codigo. 

Se comienza setteando los parametros de configuracion o propiedades. 
```
Property options;
options.put("device", "remote_controlboard");
options.put("local", "/test/client");                 //local port names
options.put("remote", "/icubSim/right_arm");         //where we connect to


```

Donde los parametros "Locals" y "remote" se usan para establecer los nombres de los puertos para el lado del servidor (o nodos como lo llamamos en otro repositorio). Para nosotros, viendolo como ROS, significa que estamos creando los siguientes nodos
* /test/client/state:i
* /test/client/command:o
* /test/client/rpc:o

Y se conectaran automaticamente al servidor.

Para crear el driver usamos `PolyDriver` :
```
PolyDriver robotDevice(options);
if (!robotDevice.isValid()) {
    printf("Device not available.  Here are the known devices:\n");
    printf("%s", Drivers::factory().toString().c_str());
    return 1;
}

```

Este pedazo de codigo inicializa el driver y verifica si la conexion y los parametrs han sido establecidos del lado del server y de nuestro lado. El siguiente paso es obtener interfaces para trabajar con ellas. 
por ejemplo :

```
IPositionControl *pos;
IVelocityControl *vel;
IEncoders *enc;
```
y 

```
robotDevice.view(pos);
robotDevice.view(vel);
robotDevice.view(enc);
```
Ya que trabajamos con punteros debemos verificarlos, por lo que agregamos la siguente linea para cada uno de los punteros(aqui mostraremos solo un ejemplo) :

```
if (pos==0) {
  printf("Error getting IPositionControl interface.\n");
  return 1;
}
```
Aqui es donde comenzamos a interactuar con mas cosas cercanas al simulador, por ejemplo ahora podemos ver cuantos ejes podemos usar para mover el brazo del robot (en el sumulador) agregando las siguientes lineas de codigo :


```
int jnts = 0;
pos->getAxes(&jnts); // pos es un puntero con los valores iniciales setteados anteriormente
```
En las siguientes lineas creamos algunos vectores que se utilizaran mas adelante.

Ahora podemos inicializar los controladores y prender los aplificadores :
```
/* we need to set reference accelerations used to generate the velocity */
/* profile, here 50 degrees/sec^2 */
int i;
for (i = 0; i < jnts; i++) {
    tmp[i] = 50.0;
}
pos->setRefAccelerations(tmp.data());
```
Si te fijas estamos cambiando el grado del brazo enpor cada iteracion, es decir en base a un tiempo lo que en realidad significa $[{degree\over seg^2}]$ 

Ademas si lo queremos asi, podemos checkear la posicion de los ejes con :
```
enc->getEncoders(encoders.data());
```
Que lee los valores de los encoders.
Es importante revisar que la comunicacion con el robot es efectiva, esto lo podemos hacer con el siguiente codigo :
```
bool ret=enc->getEncoders(encoders.data());
if (!ret)
{
     fprintf(stderr, "Error reading encoders, check connectivity with the robot\n");
}
else
{
     /* use encoders */
}

```


### Control de la posición 
Primero debemos hacer :

```
int i;
for (i = 0; i < jnts; i++) {
    tmp[i] = 40.0;
}
pos->setRefSpeeds(tmp.data());

```
Que settea la velocidad e referencia para todos los ejes a $40[{degree\over seg}]$ . Este referencia la usamos para interpolar la trayectoria entre la posicion actual y la esperada. 

Ahora podemos hacer :
```
bool ok = pos->positionMove(command_position.data());
```
donde "command_position" es la posicion esperada. Esto comienza un movimiento de todos los ejes a la posicion esperada. 

### Control de velocidad
Para enviar comandos de velocidad debemos :
```
bool ok = vel->velocityMove(command_velocity.data());
```
que acelera todos los ejes a la velocidad descrita por el vector "command_velocity" (en $[{degree\over seg}]$)

### Cerrando el dispositivo
Cuando ya esta listo todo lo que queriamos hacer (los movimientos o acciones llegan a su fin) no debemos olvidar cerrar el dispositivo con la siguiente linea de comando :
```
robotDevice.close();
```

#### Informacion que no esta en el tutorial
En este tutorial y en los siguientes se asume que ya se sabe como compilar los programas, por lo que una vez descrito el codigo, no se muestra ni lo que deberia pasar ni como echar a andar todo. 
Para poder correr el programa debemos aplicar el tutorial 1 (1.HelloWorld), de forma que debemos crear el archivo `CMakeLists.txt` donde debemos escribir el siguiente codigo :

```
project(arm_control)

find_package(YARP REQUIRED)

add_executable(armControl tutorial_arm.cpp)

# we now add the YARP libraries to our project.
target_link_libraries(armControl ${YARP_LIBRARIES})
```
Site fijas solo cambiamos los nombres de los archivos que vamos a correr y el nombre del proyecto. 
Guardamos y corremos el programa con `cmake ./` y luego `make` asi generaremos todos los archivos necesarios. 
finalmente debemos correr el programa con `./armControl` y agregar `--robot icubSim` ya que este es el nombre del robot que vamos a mover (el robot del simulador).










