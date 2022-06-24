## 3. Interfaces de torque/impedancia
En este tutorial se nos muestra como utilizar las siguientes interfaces :
*  yarp::dev::IControlMode
* yarp::dev::ITorqueControl
* yarp::dev::IImpedanceControl
* yarp::dev::IInteractionControl

Este tutorial esta basado en los 2 anteriores, por lo que se obvian algunos conocimientos vistos con aterioridad (manejo de motores y compilacion de los programas).

En este caso utilizaremos un modulo llamado `wholeBodyDynamics`, el cual actua como un servidor y responde al calculo de joint torques. Para acceder al valor del torque, nuestro programa debe declarar y abrir la interfaz de `ITorqueControl`, de la misma forma que abrimos `IPositionControl` en el tutorial anterior. Ademas podemos abrir el `IControlMode` y `IImpedanceControl` con las siguientes lineas de codigo :

```
IPositionControl *pos;
IEncoders *encs;
IControlMode *ictrl;
IInteractionMode *iint;
IImpedanceControl *iimp;
ITorqueControl *itrq;

```
y 

```
bool ok;
ok = robotDevice.view(pos);
ok = ok && robotDevice.view(encs);
ok = ok && robotDevice.view(ictrl);
ok = ok && robotDevice.view(iint);
ok = ok && robotDevice.view(iimp);
ok = ok && robotDevice.view(itrq);
 
if (!ok) {
    printf("Problems acquiring interfaces\n");
    return 0;
}

```
Al igual que en los casos anteriores podemos revisar cuantos ejes podemos controlar :

```
int jnts = 0;
pos->getAxes(&jnts);
```
Luego seteamos algunos vectores que utilizaremos luego.
Inicializamos los controladores e iniciamos los amplificadores (al igual que en codigo anterior)
si se necesita tambien podemos verificar la posicion de los ejes con el mismo codigo visto anteriormente.
#### Control de posicion 
Primero debemos .
```
int i;
for (i = 0; i < jnts; i++) {
    tmp[i] = 40.0;
}
pos->setRefSpeeds(tmp.data());
```
Dondo setteamos las referencias de $40[{degree\over seg}]$. 

Ahora podemos hacer :

```
bool ok = pos->positionMove(command_position.data());

```
comandos ya explicados anteriormente .

#### Usando diferentes modos de control: Control de posicion 

Podemos cambiar los parametros de impedancia cada vez que queramos con codigo como este :

```
double stiffness = 0.111;          // stiffness coefficient, units are Nm/deg
double damping =  0.014;           // damping coefficient,   units are Nm/(deg/s)
bool ok = iimp->setImpedance(joint, stiffness, damping);

```

Podemos usar `IInteractionMode` para cambiar el modo de interaccion de una articulacion especifica a un modo de interaccion compatible :
```
ictrl->setPositionMode(3);
iint->setInteractionMode(3,VOCAB_IM_COMPLIANT);
```
El torque (Td) es analizado como :
$$T_d = {K_{spring} * (q_d -q)-K_{domp}*q + F_{offset}} $$
Que es controlado con el regulador PID interno .
Podemos especificar un nuevo punto de equilibrio usando la usual interface de posicion, ya que el modo de control de la articulacion sigue siendo `VOCAB_CM_POSITION` incluso si el modo de interaccion ha cambiado :
```
bool ok = pos->positionMove(command_position.data());
``` 
si se necesitar volver a la version anterior `` stiff mode `` debemos :
```
iint->setInteractionMode(3,VOCAB_IM_COMPLIANT);
```

