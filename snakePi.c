
#include "snakePi.h"

int flags = 0;

// Declaracion del objeto teclado
TipoTeclado teclado = {
	.columnas = {
		// A completar por el alumno...
		// ...
	},
	.filas = {
		// A completar por el alumno...
		// ...
	},
	.rutinas_ISR = {
		// A completar por el alumno...
		// ...
	},

	// A completar por el alumno...
	// ...
};

// Declaracion del objeto display
TipoLedDisplay led_display = {
	.columnas = {
		// A completar por el alumno...
		// ...
	},
	.filas = {
		// A completar por el alumno...
		// ...
	},
	// A completar por el alumno...
	// ...
};


//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------

// int ConfiguracionSistema (TipoSistema *p_sistema): procedimiento de configuracion
// e inicializacion del sistema.
// Realizará, entra otras, todas las operaciones necesarias para:
// configurar el uso de posibles librerías (e.g. Wiring Pi),
// configurar las interrupciones externas asociadas a los pines GPIO,
// configurar las interrupciones periódicas y sus correspondientes temporizadores,
// la inicializacion de los diferentes elementos de los que consta nuestro sistema,
// crear, si fuese necesario, los threads adicionales que pueda requerir el sistema
// como el thread de exploración del teclado del PC


int ConfiguraInicializaSistema (TipoSistema *p_sistema) {
	int result = 0;
	// A completar por el alumno...
	// ...


	// Lanzamos thread para exploracion del teclado convencional del PC
	wiringPiSetupGpio();
	result = piThreadCreate (thread_explora_teclado_PC);

	if (result != 0) {
		printf ("Thread didn't start!!!\n");
		return -1;
	}

	return result;
}

//------------------------------------------------------
// FUNCIONES LIGADAS A THREADS ADICIONALES
//------------------------------------------------------

PI_THREAD (thread_explora_teclado_PC) {
	int teclaPulsada;

	while(1) {
		delay(10); // Wiring Pi function: pauses program execution for at least 10 ms

		piLock (STD_IO_BUFFER_KEY);

		if(kbhit()) {
			teclaPulsada = kbread();

			switch(teclaPulsada) {
				// A completar por el alumno...
				// ...

				case 'w':
				piLock(SYSTEM_FLAGS_KEY);
				flags |= FLAG_MOV_ARRIBA;
				piUnlock(SYSTEM_FLAGS_KEY);
				printf("Tecla W pulsada\n") ;
				break;

				/*case 's':
				piLock(SYSTEM_FLAGS_KEY);
				flags |= FLAG_TIMER_JUEGO;
				piUnlock(SYSTEM_FLAGS_KEY);
				fflush(stdout);
				printf("Tecla S pulsada!\n");
				break;*/

				case 'k':
				piLock(SYSTEM_FLAGS_KEY);
				flags |= FLAG_BOTON;
				piUnlock(SYSTEM_FLAGS_KEY);
				printf("Tecla W pulsada\n") ;
				break;

				case 'a':
				piLock(SYSTEM_FLAGS_KEY);
				flags |= FLAG_MOV_IZQUIERDA;
				piUnlock(SYSTEM_FLAGS_KEY);
				printf("Tecla A pulsada\n") ;
				break;

				case 'd':
				piLock(SYSTEM_FLAGS_KEY);
				flags |= FLAG_MOV_DERECHA;
				piUnlock(SYSTEM_FLAGS_KEY);
				printf("Tecla D pulsada\n");
				break;

				case 'x':
				piLock(SYSTEM_FLAGS_KEY);
		    	flags |= FLAG_MOV_ABAJO;
				piUnlock(SYSTEM_FLAGS_KEY);
				printf("Tecla X pulsada\n");
			    break;

				case 'q':
					exit(0);
					break;

				default:
					printf("INVALID KEY!!!\n");
					break;
			}
		}

		piUnlock (STD_IO_BUFFER_KEY);
	}
}

// wait until next_activation (absolute time)
void delay_until (unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay (next - now);
	}
}


int main () {
	TipoSistema sistema;
	unsigned int next;
	//tmr_t* tmr = tmr_new (timer_general);
	fsm_trans_t snakePi[] = {
		{ WAIT_START, CompruebaBotonPulsado, WAIT_PUSH, InicializaJuego },
		{ WAIT_PUSH, CompruebaTimeoutActualizacionJuego, WAIT_PUSH, ActualizarJuego },
		{ WAIT_PUSH, CompruebaMovimientoArriba, WAIT_PUSH, MueveSerpienteArriba },
		{ WAIT_PUSH, CompruebaMovimientoAbajo, WAIT_PUSH, MueveSerpienteAbajo },
		{ WAIT_PUSH, CompruebaMovimientoIzquierda, WAIT_PUSH, MueveSerpienteIzquierda },
		{ WAIT_PUSH, CompruebaMovimientoDerecha, WAIT_PUSH, MueveSerpienteDerecha },
		{ WAIT_PUSH, CompruebaFinalJuego, WAIT_END, FinalJuego },
		{ WAIT_END, CompruebaBotonPulsado, WAIT_START, ReseteaJuego },
		{-1, NULL, -1, NULL },
	};

	// Configuracion e inicializacion del sistema
	ConfiguraInicializaSistema (&sistema);

	fsm_t* snakePi_fsm = fsm_new (WAIT_START, snakePi, &(sistema.snakePi));

	// A completar por el alumno...
	// ...

	next = millis();
	while (1) {
		fsm_fire (snakePi_fsm);

		// A completar por el alumno...
		// ...

		next += CLK_MS;
		delay_until (next);
	}

	fsm_destroy (snakePi_fsm);

	return 0;
}
