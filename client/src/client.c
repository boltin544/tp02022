#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un Log");


	/* ------- --------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	ip = config_get_string_value(config , "IP");
	puerto = config_get_string_value(config , "PUERTO" );
	valor = config_get_string_value(config , "VALOR");

	// Loggeamos el valor de config
	log_info(logger , valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor , conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	if((nuevo_logger = log_create("tp0.log", "tp0" , 1 , LOG_LEVEL_INFO)) == NULL)
	{
		printf("Error: No se pudo crear el logger\n");
		exit(1);
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	if((nuevo_config = config_create("./client.config")) == NULL){

		printf("Error: no se puede crear la config");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	leido = readline("> ");

	while(!string_is_empty(leido))
	{
		log_info(logger , leido);
		leido = readline("> ");
	}

	free(leido);

}

void paquete(int conexion)
{
	char* leido = readline("> ");
	t_paquete* paquete;
	int tamanio = strlen(leido) + 1;

	paquete = crear_paquete();

	while(!string_is_empty(leido))
	{
		agregar_a_paquete(paquete , leido , tamanio);
		leido = readline("> ");
	}

	enviar_paquete(paquete , conexion);

	eliminar_paquete(paquete);

	liberar_conexion(conexion);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if(logger != NULL){
		log_destroy(logger);
	}

	if(config != NULL){
	config_destroy(config);
	}

	liberar_conexion(conexion);
}
