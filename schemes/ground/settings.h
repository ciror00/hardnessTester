// Declaraciones de la configuracion
const long sensibility = 10; // Valor minimo de fuerza para tomar un registro [kg]
const byte stabilizer = 4; // Cantidad de muestras antes de devolver una medicion
const int showMeasure = 1000; // Tiempo que se muestra la medicion por pantalla
const int waitConfiguration = 7000; // Tiempo de espera para entrar y salir de modo configuracion
const float scale = 62000; // Escala para configuracion por defecto
const long updateTime = 1000*60*45; // Lapsos de tiempo para actualizar el GPS [milisegundos]
