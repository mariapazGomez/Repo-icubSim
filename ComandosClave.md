## Comandos Clave para el trabajo con iCub 

### YARP 
[Pagina de Tutoriales YARP](https://www.yarp.it/latest/tutorials.html) 

yarpserver
: Comando que inicialza el servidor para trabajar con yarp, siempre se debe inicializar antes de abrir el simulador (iCub_SIM) ya que si no no funcionara nada.

yarpmanager
: En este se muestran los programas disponibles para correr Ademas muestra los codigos y enlaces que s 

yarp run --server 3030 
: Comando que crea un servidor local para que el robot se muestre en el sistema de yarpmanager yarp 

### iCub_SIM 

#### Datos importantes 
* Nombre del robot : En muchos de los programas iniciales tuve que indicar cual era el nombre del robot, el cual por defecto es icubSim y debe ser indicado con el comando `--robot icubSim`  
#### Eye View 
Para tener el control de los ojos del icub en el simulador, tenemos dos ociones, crear nuestros propias instancias de ojos, o utilizar las que ya esten inicializadas, y conectarlas al simulador. Para esto utilizamos 2 comandos clave :

yarpview --name /example 
: Con este comando creamos un nuevo nodo que nos abrira la interfaz grafica de un ojo aqui podemos elgir el nombre de nuestro nodo, reemplazando example por el nombre del ojo elegido (Ejemplo : /RightEye ).

yarp connect [ Nodo que transmite la imagen ] [ Nodo de interfaz grafica de ojos ]
: En los casos en los que ya tenemos un nodo para la interfaz grafica que nos mostrara la imagen delos ojos, debemos conectarnos a una fuente que nos entrege la imagen que ven los ojos, es por esto que podemos utilizar este comando para poder conectar los ojos al nodo del simulador que ya esta inicializado. 

