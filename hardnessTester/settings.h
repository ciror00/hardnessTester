// Declaraciones de la configuracion
const long sensibility = 40; // Valor minimo de fuerza para tomar un registro [gr]
const byte stabilizer = 2; // Cantidad de muestras antes de devolver una medicion
const int showMeasure = 1000; // Tiempo que se muestra la medicion por pantalla
const int waitConfiguration = 7000; // Tiempo de espera para entrar y salir de modo configuracion
const float scale = -110.18; // Escala para configuracion por defecto
const int trick = 0; // Cantidad de espacio que se corre para la coma

const bool dateTimeSetting = true;
const int dates[] = {8, 8, 2020}; // Configuracion de fecha DD/MM/AAAA
const int times[] = {00, 00}; // Configuracion de hora HH:MM
