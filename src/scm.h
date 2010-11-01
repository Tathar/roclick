#ifndef __SCM_H
#define __SCM_H
#include <libguile.h>
#include <X11/Xlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MALLOC_DEBUG
#include "rmalloc/rmalloc.h"

extern Display * display;
extern Window fenetre_client;

extern int get_rc_guile_file (void);

extern int verbose;
extern int debug;
extern int test;

struct s_scheme {
	char * scheme;
	SCM fonction;
	pthread_t thread;
};
typedef struct s_scheme t_scheme;

extern t_scheme * scheme_KP;
extern unsigned int nb_scheme_KP;

extern t_scheme * scheme_KR;
extern unsigned int nb_scheme_KR;


struct s_grab_button {
	int nombre;
	int * boutons;
};
typedef struct s_grab_button t_grab_button;

extern t_grab_button grab_button;


struct s_use_key {
	int nombre;
	int* key;
};
typedef struct s_use_key t_use_key;

extern t_use_key use_key;

struct s_struct_key {
	int key;
	bool pressed;
	int* grab;
	int nbgrab;
};
typedef struct s_struct_key t_struct_key;

//extern t_struct_key struct_key;

struct s_mod_key {
	int nombre;
	t_struct_key* key;
};
typedef struct s_mod_key t_mod_key;

extern t_mod_key mod_key;

#endif /* __SCM_H */
