/*
  Cabezera de configuracion
*/
// Parametros
const int sensibility = 100; // Valor minimo de señal para no descargar la muestra
//const float scale = 1500; // Factor de error relativo entre el peso medido y el peso real
const float patternWeight = 258; // Peso real del equipo [gr]
const byte stabilizer = 2; // Cantidad de muestras antes de devolver una medicion
const int between = 10000; // Tiempo entre mensajes
