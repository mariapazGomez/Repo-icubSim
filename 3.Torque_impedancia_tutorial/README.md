## 3. Interfaces de torque/impedancia
En este tutorial se nos muestra como utilizar las siguientes interfaces :
*  yarp::dev::IControlMode
* yarp::dev::ITorqueControl
* yarp::dev::IImpedanceControl
* yarp::dev::IInteractionControl

Este tutorial esta basado en los 2 anteriores, por lo que se obvian algunos conocimientos vistos con aterioridad (manejo de motores y compilacion de los programas).

En este caso utilizaremos un modulo llamado `wholeBodyDynamics`, el cual actua como un servidor y responde al calculo de joint torques. Para acceder al valor del torque, nuestro programa debe declarar y abrir la interfaz de `ITorqueControl`, de la misma forma que abrimos `IPositionControl` en el tutorial anterior. Ademas podemos abrir el `IControlMode` y `IImpedanceControl` 

