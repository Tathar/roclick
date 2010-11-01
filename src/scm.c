#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <X11/keysym.h>
//#include <X11/extensions/XTest.h>
#include <ctype.h>
#include <libguile.h>
#include <stdbool.h>

#include "scm.h"

#define CMD_STRING_MAXLEN 256

/*
rc_guile_file -> scm_file
*/

//variable globale
t_scheme * scheme_KR = NULL;
unsigned int nb_scheme_KR = 0;
t_scheme * scheme_KP = NULL;
unsigned int nb_scheme_KP = 0;

t_grab_button grab_button;
t_use_key use_key;
t_mod_key mod_key;

int verbose = False;
int debug = False;
int test = False;


//definition de fonction
SCM roclick_KR_wrapper (SCM key, SCM fun);
SCM roclick_KP_wrapper (SCM key, SCM fun);
SCM key_press_wrapper ( SCM fake_key);
SCM string_wrapper ( SCM string);
SCM button_grab_wrapper (SCM bouton);
SCM key_use_wrapper (SCM key);
SCM key_mod_wrapper (SCM key);
SCM mod_button_grab_wrapper (SCM key, SCM bouton);
SCM verbose_wrapper (void);
SCM debug_wrapper (void);
SCM test_wrapper (void);

int
init_xbk_guile_fns (void)
{
	if (debug)
		printf("initializing guile fns...\n");
	scm_c_define_gsubr("roclick", 2, 0, 0, roclick_KR_wrapper);
	scm_c_define_gsubr("roclick-KR", 2, 0, 0, roclick_KR_wrapper);
	scm_c_define_gsubr("roclick-KP", 2, 0, 0, roclick_KP_wrapper);
	scm_c_define_gsubr("key-press", 1, 0, 0, key_press_wrapper);
	scm_c_define_gsubr("string", 1, 0, 0, string_wrapper);
	scm_c_define_gsubr("button-grab", 1, 0, 0, button_grab_wrapper);
	scm_c_define_gsubr("key-use", 1, 0, 0, key_use_wrapper);
	scm_c_define_gsubr("key-mod", 1, 0, 0, key_mod_wrapper);
	scm_c_define_gsubr("mod-button-grab", 2, 0, 0, mod_button_grab_wrapper);
	scm_c_define_gsubr("verbose", 0, 0, 0, verbose_wrapper);
	scm_c_define_gsubr("debug", 0, 0, 0, debug_wrapper);
	scm_c_define_gsubr("test", 0, 0, 0, test_wrapper);
	return 0;
}




int get_rc_guile_file (void)
{
	FILE *stream;
	char *scm_file = "./roclick.scm";
	if (debug)
		printf("getting rc guile file %s.\n", scm_file);

  /* Open RC File */
	if ((stream = fopen (scm_file, "r")) == NULL)
	{
		if (debug)
		fprintf (stderr, "WARNING : %s not found or reading not allowed.\n",
			scm_file);
		return (-1);
	}
	fclose (stream);

	init_xbk_guile_fns();
	scm_primitive_load(scm_take0str(scm_file));
	return 0;
}


SCM roclick_KR_wrapper (SCM key, SCM fun)
{
	char *keystr;

	//Guile strings are not \0 terminated. hence we must copy.
	keystr = malloc(sizeof(char) * SCM_LENGTH(key) + 1);
	strncpy(keystr, SCM_CHARS(key), SCM_LENGTH(key));
	keystr[SCM_LENGTH(key)] = '\0';

	if (debug)
		printf("roclick = %s\n", keystr);

	t_scheme * scheme1 = malloc( sizeof(t_scheme) );
	if (scheme1 == NULL) exit(1);
	(*scheme1).scheme = keystr;
	(*scheme1).fonction = fun;
	(*scheme1).thread = False;

//attention prévoir quelque chose pour libérer la memoire
	if (scheme_KR == NULL) {
		nb_scheme_KR++;
		scheme_KR = malloc( nb_scheme_KR*sizeof(t_scheme));
		if (scheme_KR == NULL) exit(1);
		scheme_KR[nb_scheme_KR - 1] = *scheme1;
	}else{
		nb_scheme_KR++;
		scheme_KR = realloc(scheme_KR, nb_scheme_KR*sizeof(t_scheme));
		if (scheme_KR == NULL) exit(1);
		scheme_KR[nb_scheme_KR - 1] = *scheme1;
	}


	free (scheme1);


	return SCM_UNSPECIFIED;
}


SCM roclick_KP_wrapper (SCM key, SCM fun)
{
	char *keystr;

	//Guile strings are not \0 terminated. hence we must copy.
	keystr = malloc(sizeof(char) * SCM_LENGTH(key) + 1);
	strncpy(keystr, SCM_CHARS(key), SCM_LENGTH(key));
	keystr[SCM_LENGTH(key)] = '\0';

	if (debug)
		printf("roclick = %s\n", keystr);

	t_scheme * scheme1 = malloc( sizeof(t_scheme) );
	if (scheme1 == NULL) exit(1);
	(*scheme1).scheme = keystr;
	(*scheme1).fonction = fun;
	(*scheme1).thread = False;

//attention prévoir quelque chose pour libérer la memoire
	if (scheme_KP == NULL) {
		nb_scheme_KP++;
		scheme_KP = malloc( nb_scheme_KP*sizeof(t_scheme));
		if (scheme_KP == NULL) exit(1);
		scheme_KP[nb_scheme_KP - 1] = *scheme1;
	}else{
		nb_scheme_KP++;
		scheme_KP = realloc(scheme_KP, nb_scheme_KP*sizeof(t_scheme));
		if (scheme_KP == NULL) exit(1);
		scheme_KP[nb_scheme_KP - 1] = *scheme1;
	}


	free (scheme1);

	return SCM_UNSPECIFIED;
}


SCM key_press_wrapper ( SCM fake_key) {


	char *keystr;
  //Guile strings are not \0 terminated. hence we must copy.
	keystr = malloc(sizeof(char) * SCM_LENGTH(fake_key) + 1);
	strncpy(keystr, SCM_CHARS(fake_key), SCM_LENGTH(fake_key));
	keystr[SCM_LENGTH(fake_key)] = '\0';
	if (debug)
		printf("key-press fake_key=%s\n", keystr);

	if(display==NULL) {
		printf("display is NULL\n");
		exit(1);
		}

	if (debug)
		printf("key %i\n",(int) XStringToKeysym(keystr));

	//appuie sur une touche
//	XTestFakeKeyEvent(display, XKeysymToKeycode( display, XStringToKeysym(keystr)), True, CurrentTime);
//	XTestFakeKeyEvent(display, XKeysymToKeycode( display, XStringToKeysym(keystr)), False, CurrentTime);
    XEvent send;
    send.xkey.keycode = XKeysymToKeycode( display, XStringToKeysym(keystr));
    send.xkey.type = KeyPress;
    libXRecPlay_SendEvent( display, fenetre_client, send );
    send.xkey.type = KeyRelease;
    libXRecPlay_SendEvent( display, fenetre_client, send );
	XFlush(display);


	free(keystr);
	return SCM_UNSPECIFIED;
}


SCM string_wrapper ( SCM string) {

//	char *key_modifiers[] = { NULL, "Shift_L", "ISO_Level3_Shift" };

	wchar_t *keystr;
	wchar_t key[2];
	key[ 1 ] = L'\0';
	int i,j;

  //Guile strings are not \0 terminated. hence we must copy.
	keystr = malloc(sizeof(wchar_t) * ( SCM_LENGTH(string) + 1) );
	wmemset( keystr, L'\0', SCM_LENGTH(string) + 1 );
	mbstowcs( keystr, SCM_CHARS(string), SCM_LENGTH(string) + 1 );
	if (debug)
		printf("string_wrapper string=%ls\n", keystr);

	if(display==NULL) {
		printf("display is NULL\n");
		exit(1);
		}

	int keysyms_per_keycode;
	KeySym *keysyms, keysym;
	KeyCode keycode;


	for(i=0; i < wcslen(keystr); i++) {
		key[ 0 ] = keystr[ i ];
		keysym = key[ 0 ];
		keycode = XKeysymToKeycode( display, keysym);
		keysyms = XGetKeyboardMapping( display,keycode,1,&keysyms_per_keycode );

		for (j=0;keysyms[j] != keysym;j++);

		if (debug) {
			printf("string_wrapper char=%ls keycode=%i keysym=0x%x keysym 0x%x shift 0x%x alt_gr 0x%x\n"
			,key
			,keycode
			,(unsigned int)keysym
			,(unsigned int)keysyms[ 0 ]
			,(unsigned int)keysyms[ 1 ]
			,(unsigned int)keysyms[ 2 ] );
			printf("mode = %i\n",j);
		}


		if (j > 2 ) {
			printf("error\nstring_wrapper char=%ls keycode=%i keysym=0x%x keysym 0x%x shift 0x%x alt_gr 0x%x\n"
				,key
				,keycode
				,(unsigned int)keysym
				,(unsigned int)keysyms[ 0 ]
				,(unsigned int)keysyms[ 1 ]
				,(unsigned int)keysyms[ 2 ] );
				free(keystr);
				exit(1);
		}

		if (j == 1)
		{
//			XTestFakeKeyEvent(display, 50, True, CurrentTime);
			XEvent send;
            send.xkey.keycode = 50;
            send.xkey.type = KeyPress;
            libXRecPlay_SendEvent( display, fenetre_client, send );
		}

		if (j == 2)
		{
//		    XTestFakeKeyEvent(display, 113, True, CurrentTime);
		    XEvent send;
            send.xkey.keycode = 113;
            send.xkey.type = KeyPress;
            libXRecPlay_SendEvent( display, fenetre_client, send );
		}

        XEvent send;
        send.xkey.keycode = keycode;
        send.xkey.type = KeyPress;
        libXRecPlay_SendEvent( display, fenetre_client, send );

        send.xkey.type = KeyRelease;
        libXRecPlay_SendEvent( display, fenetre_client, send );

//		XTestFakeKeyEvent(display, keycode, True, CurrentTime);
//		XTestFakeKeyEvent(display, keycode, False, CurrentTime);

		if (j == 1)
		{
//			XTestFakeKeyEvent(display, 50, False, CurrentTime);
		    XEvent send;
            send.xkey.keycode = 50;
            send.xkey.type = KeyRelease;
            libXRecPlay_SendEvent( display, fenetre_client, send );
		}

		if (j == 2)
		{
//		    XTestFakeKeyEvent(display, 113, False, CurrentTime);
            XEvent send;
            send.xkey.keycode = 113;
            send.xkey.type = KeyRelease;
            libXRecPlay_SendEvent( display, fenetre_client, send );
		}

		XFlush(display);
		XFree(keysyms);
	}

	free(keystr);
	return SCM_UNSPECIFIED;
}




SCM button_grab_wrapper (SCM bouton)
{
	int i;
	int j;
	if (scm_integer_p(bouton) == SCM_BOOL_F ) {
		printf("bouton-grab n'est pas de type int\n");
		exit(1);
	}

	if (debug)
		printf("bouton-grab = %i\n",(int)SCM_INUM(bouton));


//attention prévoir quelque chose pour libéré la memoire
//si premier appel a la fonction on résèrve de la memoire
	if (grab_button.boutons == NULL) {
		grab_button.nombre++;
		grab_button.boutons = malloc( grab_button.nombre*sizeof(int));
		if (grab_button.boutons == NULL) exit(1);
		grab_button.boutons[grab_button.nombre - 1] = (int)SCM_INUM(bouton);
	}else{
//on test si ce bouton n'a pas été déjà selectionné la fonction "mod-button-grab"
		for(i=0;mod_key.nombre > i ;i++) {
			for(j=0;mod_key.key[i].nbgrab > j ;j++) {
				if (mod_key.key[i].grab[j] == (int)SCM_INUM(bouton))
						return SCM_UNSPECIFIED;
			}
		}
//on test si le bouton n'a pas été déjà selectionné
		for(i=0;grab_button.nombre > i ;i++){
			if ( grab_button.boutons[i] == (int)SCM_INUM(bouton) )
				return SCM_UNSPECIFIED;
		}
//sinon on le rajoute dans la liste des bouton selectionné
		grab_button.nombre++;
		grab_button.boutons = realloc(grab_button.boutons, grab_button.nombre*sizeof(int));
		if (grab_button.boutons == NULL) exit(1);
		grab_button.boutons[grab_button.nombre - 1] = (int)SCM_INUM(bouton);
	}
	return SCM_UNSPECIFIED;
}


SCM key_use_wrapper (SCM key)
{
	char *keystr;
	int i;
  //Guile strings are not \0 terminated. hence we must copy.
	keystr = malloc(sizeof(char) * SCM_LENGTH(key) + 1);
	strncpy(keystr, SCM_CHARS(key), SCM_LENGTH(key));
	keystr[SCM_LENGTH(key)] = '\0';

	if (debug)
		printf("key_use_wrapper = %s\n",keystr);

//attention prévoir quelque chose pour libéré la memoire
//si premier appel à la fonction on résèrve de la memoire
	if (use_key.key == NULL) {
		use_key.nombre++;
		use_key.key = malloc( use_key.nombre*sizeof(int));
		if (use_key.key == NULL) exit(1);
		use_key.key[use_key.nombre - 1] = (int)XKeysymToKeycode(display, XStringToKeysym(keystr));
	}else{
//on test si la touche n'a pas été déjà selectionné
		for(i=0;use_key.nombre > i ;i++){
			if (use_key.key[i] == (int)XKeysymToKeycode(display, XStringToKeysym(keystr))) {
				free(keystr);
				return SCM_UNSPECIFIED;
			}
		}
//on test si la touche n'a pas été déjà selectionné	dans la fonction mod_key
		for(i=0;mod_key.nombre > i ;i++){
			if (mod_key.key[i].key == (int)XKeysymToKeycode(display, XStringToKeysym(keystr))) {
				free(keystr);
				return SCM_UNSPECIFIED;
			}
		}
//sinon on la rajoute dans la liste des touche selectionne
		use_key.nombre++;
		use_key.key = realloc(use_key.key, use_key.nombre*sizeof(int));
		if (use_key.key == NULL) exit(1);
		use_key.key[use_key.nombre - 1] = (int)XKeysymToKeycode(display, XStringToKeysym(keystr));
	}

	if (debug)
		printf("FIN key_use_wrapper = %i\n", use_key.key[use_key.nombre - 1]);

	free(keystr);
	return SCM_UNSPECIFIED;
}

SCM key_mod_wrapper (SCM key)
{
	char *keystr;
	int i;
  //Guile strings are not \0 terminated. hence we must copy.
	keystr = malloc(sizeof(char) * SCM_LENGTH(key) + 1);
	strncpy(keystr, SCM_CHARS(key), SCM_LENGTH(key));
	keystr[SCM_LENGTH(key)] = '\0';

	if (debug)
		printf("key_mod_wrapper = %s\n",keystr);

//attention prévoir quelque chose pour libéré la memoire
//si premier appel à la fonction on résèrve de la memoire
	if (mod_key.key == NULL) {
		mod_key.nombre++;
		mod_key.key = malloc( mod_key.nombre*sizeof(t_struct_key));
		if (mod_key.key == NULL) exit(1);

		mod_key.key[mod_key.nombre - 1].key = (int)XKeysymToKeycode(display, XStringToKeysym(keystr));
		mod_key.key[mod_key.nombre - 1].pressed = False;
		mod_key.key[mod_key.nombre - 1].grab = NULL;
		mod_key.key[mod_key.nombre - 1].nbgrab = 0;

	} else {
//on test si la touche n'a pas été déjà selectionné
		for(i=0;mod_key.nombre > i ;i++){
			if (mod_key.key[i].key == (int)XKeysymToKeycode(display, XStringToKeysym(keystr))) {
				free(keystr);
				return SCM_UNSPECIFIED;
			}
		}
//on test si la touche n'a pas été déjà selectionné dans la fonction use_key
		for(i=0;use_key.nombre > i ;i++){
			if (use_key.key[i] == (int)XKeysymToKeycode(display, XStringToKeysym(keystr))) {
				free(keystr);
				return SCM_UNSPECIFIED;
			}
		}
//sinon on la rajoute dans la liste des touche selectionne
		mod_key.nombre++;
		mod_key.key = realloc(mod_key.key, mod_key.nombre*sizeof(t_struct_key));
		if (mod_key.key == NULL) exit(1);
//		struct_key = mod_key.key[mod_key.nombre - 1] ;

		mod_key.key[mod_key.nombre - 1].key = (int)XKeysymToKeycode(display, XStringToKeysym(keystr));
		mod_key.key[mod_key.nombre - 1].pressed = False;
		mod_key.key[mod_key.nombre - 1].grab = NULL;
		mod_key.key[mod_key.nombre - 1].nbgrab = 0;

	}
	if (debug)
		printf("FIN key_mod_wrapper %i = %i\n", mod_key.nombre, mod_key.key[mod_key.nombre - 1].key);

	free(keystr);
	return SCM_UNSPECIFIED;
}

SCM mod_button_grab_wrapper (SCM key, SCM bouton)
{
	char *keystr;
	int i;
	int j;
  //Guile strings are not \0 terminated. hence we must copy.
	keystr = malloc(sizeof(char) * SCM_LENGTH(key) + 1);
	strncpy(keystr, SCM_CHARS(key), SCM_LENGTH(key));
	keystr[SCM_LENGTH(key)] = '\0';


	if (scm_integer_p(bouton) == SCM_BOOL_F ) {
		printf("le 2eme argument de mod-bouton-grab n'est pas de type int\n");
		exit(1);
	}

	if (debug)
		printf("mod_button_grab_wrapper = %s %i\n", keystr, (int)SCM_INUM(bouton));

	for(i=0;mod_key.nombre > i ;i++) {
		if ( (mod_key.key[i].key == (int)XKeysymToKeycode(display, XStringToKeysym(keystr)) ) || ( strcmp(keystr,"all") == 0 ) )
		{
//attention prévoir quelque chose pour libéré la memoire
//si premier bouton dans ce "mod_grab" on résèrve de la memoire
			if (mod_key.key[i].grab == NULL) {
				mod_key.key[i].nbgrab++;
				mod_key.key[i].grab = malloc( mod_key.key[i].nbgrab*sizeof(int));
				if (mod_key.key[i].grab == NULL) exit(1);
				mod_key.key[i].grab[mod_key.key[i].nbgrab - 1] = (int)SCM_INUM(bouton);
			} else {
//on test si ce bouton n'a pas été déjà selectionné la fonction "button-grab"
				for(j=0;grab_button.nombre > j ;j++){
					if ( grab_button.boutons[j] == (int)SCM_INUM(bouton) ) {
						free(keystr);
						return SCM_UNSPECIFIED;
					}
				}

//on test si ce bouton n'a pas été déjà selectionné	dans la fonction "mod-button-grab" pour cette touche
				for(j=0;mod_key.key[i].nbgrab > j ;j++){
					if (mod_key.key[i].grab[j] == (int)SCM_INUM(bouton)) {
						free(keystr);
						return SCM_UNSPECIFIED;
					}
				}
//sinon on le rajoute dans la liste des boutons selectionne
				mod_key.key[i].nbgrab++;
				mod_key.key[i].grab = realloc(mod_key.key[i].grab, mod_key.key[i].nbgrab*sizeof(int));
				if (mod_key.key[i].grab == NULL) exit(1);
				mod_key.key[i].grab[mod_key.key[i].nbgrab - 1] = (int)SCM_INUM(bouton);
			}
		}
	}

	free(keystr);
	return SCM_UNSPECIFIED;
}

SCM verbose_wrapper (void) {
	verbose = True;
	return SCM_UNSPECIFIED;
}

SCM debug_wrapper (void) {
	debug = True;
	verbose = True;
	return SCM_UNSPECIFIED;
}

SCM test_wrapper (void) {
	test = True;
	return SCM_UNSPECIFIED;
}

