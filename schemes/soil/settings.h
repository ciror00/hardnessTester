// Modo debbuging
#define LOGS 0

// Declaraciones de la configuracion
const long sensibility = 5; // Valor minimo de fuerza para tomar un registro [kg]
const long tolerance = 0; // Corrimiento minimo de profundidad [cm]
const byte stabilizer = 2; // Cantidad de muestras antes de devolver una medicion
const float scale = 62000; // Escala para configuracion por defecto
const int baudrateGPS = 9600; // Velocidad de comunicacion con el GPS

// Tiempos parametrizables
const int waitConfiguration = 7000; // Tiempo de espera para entrar y salir de modo configuracion
const unsigned long updateTime = 5; // Lapsos de actualizacion de estado del equipo (SD y GPS) [minutos]
const int dates[] = {8, 9, 2020}; // Configuracion de fecha DD/MM/AAAA
const int times[] = {23, 30}; // Configuracion de hora HH:MM
