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

	log_info(logger, "Hola! Soy un log");

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP"); // Para mostrarlo con el log deberia tener un char* siempre
	puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de config

	log_info(logger, ip);

	/* ---------------- LEER DE CONSOLA ---------------- */
	t_list* leidos = list_create();

	leidos = leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion, leidos);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/

	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log", "TP0", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("cliente.config");

	return nuevo_config;
}

t_list* leer_consola(t_log* logger)
{
	char* leido;
	t_list* itemsLeidos = list_create();

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido, "") != 0) { //Me puso algo en la consola.
		log_info(logger, leido);
		list_add(itemsLeidos, leido);
		leido = readline("> ");
	}
	free(leido);

	// ¡No te olvides de liberar las lineas antes de regresar!
	return itemsLeidos;
}

void paquete(int conexion, t_list* leidos)
{
	// Ahora toca lo divertido!
	t_paquete* paquete = crear_paquete(); // Creo el paquete.
	char* linea;

	// Leemos y esta vez agregamos las lineas al paquete

	for (int i = 0; i < list_size(leidos); i++)
	{
		linea = list_get(leidos, i);
		agregar_a_paquete(paquete, linea, strlen(linea)+1);
	}
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	list_destroy_and_destroy_elements(leidos, free); // Libero la lista con sus elementos.
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
