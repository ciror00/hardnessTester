// Modo debbuging
#define LOGS 0

// Declaraciones de la configuracion
const long sensibility = 2; // Valor minimo de fuerza para tomar un registro [kg]
const long tolerance = 0; // Corrimiento minimo de profundidad [cm]
//onst int zeroHorizon = 3; // Profundidad minima, aceptable, para guardar una medición
const byte stabilizer = 2; // Cantidad de muestras antes de devolver una medicion
const float scale = 62000; // Escala para configuracion por defecto

// Tiempos parametrizables
const int waitConfiguration = 7000; // Tiempo de espera para entrar y salir de modo configuracion
const unsigned long updateTime = 2; // Lapsos de tiempo para actualizar el GPS [minutos]
const unsigned long updateSD = 1; // Lapsos de tiempo para actualizar la SD [minutos]
