// XXX: http://poj.org/problem?id=2635
// XXX: https://www.spoj.com/problems/CRYPTON/
// XXX: http://xoptutorials.com/index.php/2017/01/01/spojcrypton/
// XXX: https://github.com/fookwood/zoj-solutions-fish_ball/blob/master/Vol.19(2800-2899)/%232806%20The%20Embarrassed%20Cryptographer/p2806.cpp
// XXX: https://www.spoj.com/problems/NORDICPD/

/*
 the thing is that you need to reduce the constant of the algorithm by storing more than just a digit in every array slot of the big number representation, so that the number of operations needed to calculate the mod is also reduced. For example:

 123456789 can be stored as
 1|2|3|4|5|6|7|8|9
 where each slot is a char, or as
 123|456|789
 where each slot is a int.

 This does not reduce the complexity of the algorithm, but speeds up the mod operation.

 Now, you can also test your algorithm in  https://www.spoj.com/problems/CRYPTON/ where the judge is a bit less strict. There is not needed to do this "compression" of the large number
 */

#if 1
#define _GNU_SOURCE
//#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#ifndef ONLINE_JUDGE
#include <unistd.h>
#include <sys/time.h>
#endif
#include <math.h>
#include <stdint.h>
#include <ctype.h>

#ifdef CACA_COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#if 1
#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX LONG_MAX
#endif

#ifndef ULONG_LONG_MAX
#define ULONG_LONG_MAX ULONG_MAX
#endif

#define CACA_COMUN_TAM_MAX_LINEA (16*200000)
#define CACA_LOG_MAX_TAM_CADENA 2000

#define CACA_COMUN_BUF_STATICO_TAM 1000
#define CACA_COMUN_BUF_STATICO (char[CACA_COMUN_BUF_STATICO_TAM] ) { '\0' }

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define CACA_COMUN_ASSERT_DUROTE 0
#define CACA_COMUN_ASSERT_SUAVECITO 1
#define CACA_COMUN_ASSERT_NIMADRES 2

#define CACA_COMUN_VALOR_INVALIDO ((tipo_dato)UINT_MAX)
#define CACA_COMUN_IDX_INVALIDO ((natural)CACA_COMUN_VALOR_INVALIDO)
//#define CACA_COMUN_LOG
#define CACA_COMUN_FUNC_STATICA static inline

typedef char byteme;
typedef unsigned int natural;
typedef byteme tipo_dato;
typedef long long entero_largo;
typedef unsigned long long entero_largo_sin_signo;
typedef long long bitch_vector;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

/*
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_SUAVECITO
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_NIMADRES
 */
#define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_DUROTE

#define assert_timeout_dummy(condition) 0;

#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){while(1){printf("");};abort();}
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef CACA_COMUN_LOG
#define caca_log_debug(formato, args...) \
		do \
		{ \
			size_t profundidad = 0; \
			void *array[CACA_LOG_MAX_TAM_CADENA]; \
 			profundidad = backtrace(array, CACA_LOG_MAX_TAM_CADENA); \
			caca_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
		} \
		while(0);
#else
#define caca_log_debug(formato, args...) 0;
#endif

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

#define caca_comun_calloc_local(tipo) (&(tipo){0})

void caca_log_debug_func(const char *format, ...);

#ifndef ONLINE_JUDGE
CACA_COMUN_FUNC_STATICA void caca_comun_current_utc_time(struct timespec *ts) {

#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
#ifdef CACA_COMUN_LOG
	clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

CACA_COMUN_FUNC_STATICA void caca_comun_timestamp(char *stime) {
	time_t ltime;
	long ms;
#ifndef ONLINE_JUDGE
	struct tm result;
	struct timespec spec;
#endif
	char parte_milisecundos[50];

	ltime = time(NULL);

#ifndef ONLINE_JUDGE
	localtime_r(&ltime, &result);
	asctime_r(&result, stime);
#endif

	*(stime + strlen(stime) - 1) = ' ';

#ifndef ONLINE_JUDGE
	caca_comun_current_utc_time(&spec);
	ms = round(spec.tv_nsec / 1.0e3);
#endif
	sprintf(parte_milisecundos, "%ld", ms);
	strcat(stime, parte_milisecundos);
}

#endif // ONLINE_JUDGE
#ifdef CACA_COMUN_LOG
void caca_log_debug_func(const char *format, ...) {

	va_list arg;
	va_list arg2;
	const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
	const char *HEADER =
	"archivo: %s; funcion: %s; linea %d; nivel: %zd caca 8====D ";
	char formato[CACA_LOG_MAX_TAM_CADENA + sizeof(HEADER)
	+ sizeof(PEDAZO_TIMESTAMP_HEADER)] = {'\0'};
	char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = {'\0'};
	char cadena_timestamp[100] = {'\0'};

	caca_comun_timestamp(cadena_timestamp);
	sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);

	strcpy(formato, pedazo_timestamp);
	strcat(formato, HEADER);
	strcat(formato, format);
	strcat(formato, "\n");

	va_start(arg, format);
	va_copy(arg2, arg);
	vprintf(formato, arg2);
	va_end(arg2);
	va_end(arg);
	setbuf(stdout, NULL);
}
#endif

#ifdef CACA_COMUN_LOG
CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, natural tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%1d", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	int i, j;
	natural inicio_buffer_act = 0;
	for (int i = 0; i < filas_tam; i++) {
		caca_comun_arreglo_a_cadena(matrix + i * columas_tam, columas_tam,
				buffer + inicio_buffer_act);
		inicio_buffer_act += strlen(buffer + inicio_buffer_act);
		buffer[inicio_buffer_act++] = '\n';
		/*		caca_log_debug("pero q mierda inicio buffer act %u %s",inicio_buffer_act,buffer);*/
	}
	return buffer;
}

CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena_entero_largo_sin_signo(
		entero_largo_sin_signo *arreglo, entero_largo_sin_signo tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%llu", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
#else
CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo,
		natural tam_arreglo, char *buffer) {
	return NULL;
}
CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena_natural(
		natural *arreglo, natural tam_arreglo, char *buffer) {
	return NULL;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	return NULL;
}

CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena_entero_largo_sin_signo(
		entero_largo_sin_signo *arreglo, entero_largo_sin_signo tam_arreglo,
		char *buffer) {
	return NULL;
}
#endif

#define caca_comun_arreglo_a_cadena_entero_largo_sin_signo_buf_local(a,a_tam) caca_comun_arreglo_a_cadena_entero_largo_sin_signo(a,a_tam,CACA_COMUN_BUF_STATICO)
#define caca_comun_arreglo_a_cadena_buf_local(a,a_tam) caca_comun_arreglo_a_cadena(a,a_tam,CACA_COMUN_BUF_STATICO)
#define caca_comun_arreglo_a_cadena_natural_buf_local(a,a_tam) caca_comun_arreglo_a_cadena_natural(a,a_tam,CACA_COMUN_BUF_STATICO)

CACA_COMUN_FUNC_STATICA void caca_comun_strreplace(char s[], char chr,
		char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

CACA_COMUN_FUNC_STATICA int caca_comun_lee_matrix_long_stdin(tipo_dato *matrix,
		int *num_filas, int *num_columnas, int num_max_filas,
		int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	tipo_dato numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(CACA_COMUN_TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas
			&& fgets(linea, CACA_COMUN_TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
			indice_columnas++;
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

CACA_COMUN_FUNC_STATICA natural caca_comun_cuenta_bitchs(tipo_dato num) {
	natural bitch_cnt = 0;
	tipo_dato num_mod = 0;
	num_mod = num;
	while (num_mod) {
		num_mod &= ~(num_mod & (-num_mod));
		bitch_cnt++;
	}
	return bitch_cnt;
}

CACA_COMUN_FUNC_STATICA char caca_comun_letra_a_valor_minuscula(char letra) {
	return letra - 'a';
}

CACA_COMUN_FUNC_STATICA natural caca_comun_max_natural(natural *nums,
		natural nums_tam) {
	natural max = 0;

	for (int i = 0; i < nums_tam; i++) {
		natural num_act = nums[i];
		if (num_act > max) {
			max = num_act;
		}
	}

	return max;
}

// XXX: https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
CACA_COMUN_FUNC_STATICA char *caca_comun_trimea(char *cad, natural cad_tam) {
	char c = cad[0];
	char tmp = '\0';
	natural i = 0;
	natural j = 0;

	caca_log_debug("entrada %s cad_tam %u", cad, cad_tam);
	while (j < cad_tam && cad[j] != '\0') {
		caca_log_debug("en j %u car %c", j, cad[j]);
		while (j < cad_tam && !isalpha(cad[j])) {
			caca_log_debug("brincando j %u car %c", j, cad[j]);
			j++;
		}
		caca_log_debug("aora j %u car %c", j, cad[j]);
		if (j == cad_tam) {
			caca_log_debug("q ped");
			break;
		}
		tmp = cad[i];
		cad[i] = cad[j];
		cad[j] = tmp;
		i++;
		j++;
	}
	caca_log_debug("mierda '%c'", cad[j]);

	i = 0;
	while (isalpha(cad[i++]))
		;
	caca_log_debug("salida %s", cad);
	cad[i - 1] = '\0';

	return cad;
}

#endif

#define CACA_COMUN_LIMPIA_MEM(m,s) (memset(m,0,s))
#define CACA_COMUN_LIMPIA_MEM_STATICA(m) (memset(m,0,sizeof(*(m))))

CACA_COMUN_FUNC_STATICA bool caca_comun_es_digito(char c) {
	return c >= '0' && c <= '9';
}

CACA_COMUN_FUNC_STATICA byteme caca_comun_caracter_a_num(char c) {
	return c - '0';
}

CACA_COMUN_FUNC_STATICA void caca_comun_invierte_arreglo_byteme(byteme *a,
		natural a_tam) {
	natural i = 0;
	natural j = a_tam - 1;
	while (i < j) {
		byteme t = a[i];
		a[i] = a[j];
		a[j] = t;
		i++;
		j--;
	}
}

CACA_COMUN_FUNC_STATICA void caca_comun_invierte_arreglo_natural(natural *a,
		natural a_tam) {
	natural i = 0;
	natural j = a_tam - 1;
	while (i < j) {
		natural t = a[i];
		a[i] = a[j];
		a[j] = t;
		i++;
		j--;
	}
}

#endif

#if 1

entero_largo numero_largote_anade_digito(entero_largo l, byteme d) {
	return l * 10 + d;
}

CACA_COMUN_FUNC_STATICA entero_largo numero_largote_divis_divis(
		byteme *dividendo, natural dividendo_tam, entero_largo divis,
		byteme *cociente, natural cociente_tam) {
	entero_largo resto = 0;
	int i = dividendo_tam - 1;

	caca_log_debug("dividen %s divis %llu",
			caca_comun_arreglo_a_cadena_buf_local(dividendo,dividendo_tam),
			divis);

	assert_timeout(!cociente || cociente_tam >= dividendo_tam);

	if (cociente) {
		CACA_COMUN_LIMPIA_MEM(cociente, cociente_tam);
	}

	while (i >= 0) {
		entero_largo c = 0;
		while (resto < divis && i >= 0) {
			resto = numero_largote_anade_digito(resto, dividendo[i]);
			i--;
		}
		if (resto >= divis) {
			c = resto / divis;
			resto = resto % divis;
			if (cociente) {
				cociente[i + 1] = c;
			}
		}
	}

	if (cociente) {
		caca_log_debug("coci %s resto %llu",
				caca_comun_arreglo_a_cadena_buf_local(dividendo,dividendo_tam),
				resto);
	} else {
		caca_log_debug("resto nomas %llu", resto);
	}

	return resto;
}

CACA_COMUN_FUNC_STATICA entero_largo numero_largote_mcd(entero_largo a,
		entero_largo b) {
	entero_largo r = 0;
	if (!a) {
		r = 0;
	} else {
		if (!b) {
			r = a;
		} else {
			r = numero_largote_mcd(b, a % b);
		}
	}
	return r;
}

CACA_COMUN_FUNC_STATICA entero_largo numero_largote_mcd_entero_largo(byteme *a,
		natural a_tam, entero_largo b) {
	return numero_largote_mcd(b,
			numero_largote_divis_divis(a, a_tam, b, NULL, 0));
}

CACA_COMUN_FUNC_STATICA void numero_largote_multiplica(byteme *a, natural a_tam,
		entero_largo b, byteme *r, natural r_tam) {
	entero_largo carrie = 0;
	natural i = 0;
	assert_timeout(r_tam >= a_tam);
	caca_log_debug("dividienti %s entre %llu",
			caca_comun_arreglo_a_cadena_buf_local(a, a_tam), b);

	for (i = 0; i < a_tam; i++) {
		entero_largo r_tmp = a[i] * b + carrie;
		caca_log_debug("a[%u] es %u mult %llu carrie %llu res %llu", i, a[i], b,
				carrie, r_tmp);
		r[i] = r_tmp % 10;
		carrie = r_tmp / 10;
	}
	if (r_tam > a_tam) {
		caca_log_debug("procesnado carrie %llu", carrie);
		while (carrie && i < r_tam) {
			entero_largo r_tmp = carrie;
			r[i] = r_tmp % 10;
			carrie = r_tmp / 10;
		}
	}
	caca_log_debug("coc %s rest %llu",
			caca_comun_arreglo_a_cadena_buf_local(r,r_tam), carrie);
	assert_timeout(!carrie);
}
CACA_COMUN_FUNC_STATICA natural numero_largote_cuenta_digitos(byteme *a,
		natural a_tam) {
	int i = a_tam - 1;
	while (i >= 0 && !a[i]) {
		i--;
	}
	return i + 1;
}

CACA_COMUN_FUNC_STATICA char *numero_largote_a_cadena(natural *a, natural a_tam,
		char *buffer, natural buffer_tam) {
	int i = 0;
	natural j = 0;
	assert_timeout_dummy(a_tam<=buffer_tam);
	CACA_COMUN_LIMPIA_MEM(buffer, buffer_tam);
	i = a_tam - 1;
	while (i >= 0 && !a[i]) {
		i++;
	}
	for (j = 0; i >= 0; i--, j++) {
		buffer[j] = a[i] + '0';
	}
	buffer[j] = '\0';
	return buffer;
}

CACA_COMUN_FUNC_STATICA natural numero_largote_mod(natural *n, natural n_tam,
		natural d, natural tu) {
	entero_largo mod = 0;
	natural f = pow(10, tu);
	for (int i = n_tam - 1; i >= 0; i--) {
		caca_log_debug("modulando %u contra %u, mod %u", n[i], d, mod);
		mod = (mod * f + n[i]) % d;
	}
	caca_log_debug("%s mod %u es %u",
			numero_largote_a_cadena(n, n_tam, CACA_COMUN_BUF_STATICO, CACA_COMUN_BUF_STATICO_TAM),
			d, mod);
	return mod;
}

CACA_COMUN_FUNC_STATICA natural numero_largote_comprime(natural *a,
		natural a_tam, natural tam_unidad) {
	assert_timeout(tam_unidad > 1);
	natural j = 0;
	natural k = 0;
	natural acum = 0;
	for (natural i = 0; i < a_tam; i++) {
		caca_log_debug("acum %u a[i] %u k %u", acum, a[i], k);
		if (i && !(i % tam_unidad)) {
			a[j] = acum;
			j++;
			k = 0;
			acum = 0;
		}
		acum = acum + a[i] * pow(10, k);
		k++;
	}
	if (acum) {
		a[j] = acum;
		j++;
	}
	return j;
}

#endif

#if 1

#define PRIMOS_CACA_MAX ((int)1E6)

natural primos_caca[PRIMOS_CACA_MAX + 1] = { 0 };
natural primos_caca_tam = 0;
bool primos_caca_es_primo[PRIMOS_CACA_MAX + 1] = { [0 ... 1]=falso,
		[2 ... PRIMOS_CACA_MAX]=verdadero };

CACA_COMUN_FUNC_STATICA natural primos_caca_criba(natural limite) {
	caca_log_debug("primos asta %u", limite);
	assert_timeout(limite<=PRIMOS_CACA_MAX);
	for (natural i = 0; i < limite; i++) {
		if (primos_caca_es_primo[i]) {
//			caca_log_debug("el %u es primo", i);
			primos_caca[primos_caca_tam++] = i;
		}
		for (natural j = 0; j < primos_caca_tam && primos_caca[j] * i <= limite;
				j++) {
			primos_caca_es_primo[primos_caca[j] * i] = falso;
			if (!(i % primos_caca[j])) {
				break;
			}
		}
	}
	return primos_caca_tam;
}

#endif

#define CRIPTO_CACA_BUFFER_MAX ((PRIMOS_CACA_MAX + 1) * 8)
char buffer[CRIPTO_CACA_BUFFER_MAX] = { 0 };
natural k[CRIPTO_CACA_BUFFER_MAX] = { 0 };

CACA_COMUN_FUNC_STATICA natural cripto_caca_menor_factor_primo(natural *a,
		natural a_tam, natural tam_unidad) {
	natural i = 0;
	natural r = 0;
	natural resi = 0;
	while (i < primos_caca_tam
			&& (resi = numero_largote_mod(a, a_tam, primos_caca[i], tam_unidad))) {
//		printf("resi %u\n", resi);
		i++;
	}
	if (i < primos_caca_tam) {
		r = primos_caca[i];
	}
	caca_log_debug("r %u", r);
	return r;
}

CACA_COMUN_FUNC_STATICA natural cripto_caca_core(natural *a, natural a_tam,
		natural limite, natural tam_unidad) {
	natural r = 0;
	natural factor_primo_minimo = cripto_caca_menor_factor_primo(a, a_tam,
			tam_unidad);
	if (factor_primo_minimo < limite) {
		r = factor_primo_minimo;
	}
	caca_log_debug("factor min %u", r);
	return r;
}

CACA_COMUN_FUNC_STATICA void cripto_caca_main() {
	char *b = buffer;
	size_t s = CRIPTO_CACA_BUFFER_MAX;
	natural tam_unidad = 3;
	primos_caca_criba(PRIMOS_CACA_MAX);
//	caca_log_debug("%s\n", caca_comun_arreglo_a_cadena_natural(primos_caca, primos_caca_tam, buffer));
//	while (getline(&k, &s, stdin) != -1) {
	while (fgets(b, s, stdin)) {
		natural l = 0;
		natural i = 0;
		natural k_tam = 0;
		while (i < s && !caca_comun_es_digito(b[i])) {
			i++;
		}
		while (i < s && caca_comun_es_digito(b[i])) {
			k[i] = caca_comun_caracter_a_num(b[i]);
			i++;
			k_tam++;
		}
		if (!k_tam) {
			break;
		}
		caca_comun_invierte_arreglo_natural(k, k_tam);
		caca_log_debug("rpr decim %s",
				caca_comun_arreglo_a_cadena_natural_buf_local(k, k_tam));
		k_tam = numero_largote_comprime(k, k_tam, tam_unidad);
		while (i < s && !caca_comun_es_digito(b[i])) {
			i++;
		}
		while (i < s && caca_comun_es_digito(b[i])) {
			l = l * 10 + caca_comun_caracter_a_num(b[i]);
			i++;
		}

		if (!l) {
			break;
		}

		caca_log_debug("k %s y l %u",
				caca_comun_arreglo_a_cadena_natural_buf_local(k, k_tam), l);

		natural r = cripto_caca_core(k, k_tam, l, tam_unidad);
//		natural r = 0;
		if (r) {
			printf("BAD %u\n", r);
		} else {
			printf("GOOD\n");
		}

		s = CRIPTO_CACA_BUFFER_MAX;
		CACA_COMUN_LIMPIA_MEM_STATICA(buffer);
	}
}

int main() {
	cripto_caca_main();
	return EXIT_SUCCESS;
}
