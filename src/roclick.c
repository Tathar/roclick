/*
   Simple Xlib application drawing a box in a window.
 */

// gcc -g -I/usr/include/ -L/usr/lib/ -lX11 -lXtst -lpthread -lguile -o roclick roclick.c scm.h scm.c rmalloc/rmalloc.c rmalloc/rmalloc.h

//#include <X11/Xutil.h>

#include <X11/Xlib.h>
//#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include <X11/Intrinsic.h>

#include "../libXRecPlay/liblibXRecPlay.h"

#include <libguile.h>
#include "scm.h"

#include <signal.h>


//#define MALLOC_DEBUG
#include "rmalloc/rmalloc.h"

#define PRINTF printf
//#  define PRINTF

//variable globale
Display *display = NULL;
Window fenetre_client;

char *suite = NULL;
char *cpy = NULL;

int nbchar;
int lensuite;

//int verbose = False;
bool infinie = True;

t_grab_button grab_button_sup;

/*
 * Hierarchie
 */
/*
static void Hierarchie ( Window racine )
{
  Window w1, child, *children=(Window *)NULL ;
  int    nchildren, i;

    XQueryTree ( display, racine, &w1, &child, &children, &nchildren );
	if ( nchildren > 0 )
	{
		for ( i = 0 ; i < nchildren ; i++ ) {
				Hierarchie ( children[i] );
      	}
	}
  XFree(children);
  XSelectInput (display, racine, KeyPressMask | KeyReleaseMask | SubstructureNotifyMask);
}
*/
/*
 * resize
 */
bool resize()
{

    if (debug)
        printf("DEBUT resize\n  if nbchar = %i lensuite = %i \n",nbchar, lensuite);

    if ( ( nbchar + 1 ) > lensuite )
    {
        if (verbose)
            printf("realloc suite & cpy %i octets\n",nbchar + 1);
        suite = realloc(suite, sizeof(char) * (nbchar + 1 ) );
        if(suite == NULL) exit(1);
        cpy = realloc(cpy, sizeof(char) * (nbchar + 1 ) );
        if (cpy == NULL) exit(1);
        lensuite = nbchar + 1;
        if (debug)
            printf("FIN resize\n");
        return True;
    }
    if (debug)
        printf("FIN resize\n");
    return False;
}

/*
 * f_suite
 */
void f_suite ( bool keypress )
{

    if (debug)
        printf("\nDEBUT f_suite\n");

    if (display == NULL) exit(1);
    unsigned int i = 0;
    if (cpy == NULL) exit(1);
    strcpy(suite,cpy);
    cpy[0] = '\0';
//	suite[strlen(suite)]='\0';

    for(i=0; i < mod_key.nombre; i++)
    {
        if (mod_key.key[i].pressed)
        {
            if (debug)
                printf("  MODE\n");
            nbchar = sizeof(char) * (strlen(suite) + strlen(cpy) + strlen( XKeysymToString( XKeycodeToKeysym( display, mod_key.key[i].key, 1))) + 1 );
            resize();
            strcat(cpy, XKeysymToString( XKeycodeToKeysym( display, mod_key.key[i].key, 1)) );
            strcat(cpy," ");
        }
    }

    strcat(cpy, suite);
    if (debug)
        printf("\tsuite = |%s|\nCPY = |%s|\n",suite,cpy);

//on suprime le charactère espace de fin
    if (strlen(suite) > 0)
        suite[strlen(suite) -1 ] = '\0';
    if (strlen(cpy) > 0)
        cpy[strlen(cpy) -1 ] = '\0';

    if ( keypress )
    {
        if ( (verbose) && (strcmp(cpy,"") != 0) )
            printf("(roclick_KP \"%s\")\n",cpy);

        for(i=0; i<nb_scheme_KP; i++)
        {
// si la combinaison actuelle a été défini sur KeyPress
            if ( (strlen(cpy) > 0) && (strcmp( cpy, scheme_KP[i].scheme)== 0) )
            {
                scm_call_0 (scheme_KP[i].fonction);
            }
// si la combinaison actuelle a été défini sur KeyPress avec le mod "all"
            if ( (strlen(scheme_KP[i].scheme) > 3) && (strlen(suite) > 0) && (strncmp( scheme_KP[i].scheme, "all", 3) == 0) && (strcmp( suite, &scheme_KP[i].scheme[4]) == 0) )
            {
                scm_call_0 (scheme_KP[i].fonction);
            }
        }
    }
    else
    {
        if ( (verbose) && (strcmp(cpy,"") != 0) )
            printf("(roclick_KR \"%s\")\n",cpy);

        for(i=0; i<nb_scheme_KR; i++)
        {
// si la combinaison actuelle a été défini sur KeyRelease
            if ( (strlen(cpy) > 0) && (strcmp( cpy, scheme_KR[i].scheme) == 0) )
            {
                scm_call_0 (scheme_KR[i].fonction);
            }
// si la combinaison actuelle a été défini sur KeyRelease avec le mod "all"
            if ( (strlen(scheme_KR[i].scheme) > 3) && (strlen(suite) > 0) && (strncmp( scheme_KR[i].scheme, "all", 3) == 0) && (strcmp( suite, &scheme_KR[i].scheme[4]) == 0) )
            {
                scm_call_0 (scheme_KR[i].fonction);
            }
        }
    }

    if (debug)
        printf("  strlen(suite) = %i\n",(int)strlen(suite));
    if (strlen(suite) > 0)
        suite[strlen(suite)] = ' ';
    if (debug)
        printf("FIN f_suite\n\n");
}

/*
 * inner_main
 *
 */
void inner_main()
{
    if (debug)
        printf("DEBUT inner_main\n");
    int i;
    int j;
    int nbpressed = 0;
    int queue =0 ;
    Window focus;
    int revert;

    libXRecPlay_t_EventReturn retour;

    XEvent e;
    nbchar = 0;
    lensuite = 3;

    suite = malloc(sizeof(char) * lensuite );
    if (suite == NULL) exit(1);
    suite[0]='\0';

    cpy = malloc(sizeof(char) * lensuite );
    if (cpy == NULL) exit(1);
    cpy[0]='\0';

    //open connection with the server
    display=XOpenDisplay(NULL);
    if(display==NULL)
    {
        printf("Cannot open display\n");
        exit(1);
    }

    if (!libXRecPlay_init(display) )
    {
        fprintf(stderr,"Cannot init libXRecPlay\n");
        exit(1);
    }

//GUILE

    get_rc_guile_file();
    /*
    	if ( (use_key.key != NULL) || (mod_key.key != NULL) )
    	{
    // Désactivation de l'autorepetition du clavier
    		XAutoRepeatOff(display);
    		sleep(1);

    		XGetInputFocus(display, &focus, &revert);
    		if (revert == RevertToParent)
    			XSelectInput (display, focus, KeyPressMask | KeyReleaseMask | FocusChangeMask);

    			Hierarchie(DefaultRootWindow(display));
    		}
    //	}
    */

    /*
        for(i=0; grab_button.nombre > i ; i++)
        {
            //XGrabButton (display, grab_button.boutons[i], AnyModifier, DefaultRootWindow(display), False, ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None);
            XEvent send;
            send.xbutton.type = ButtonPress;
            send.xbutton.button = grab_button.boutons[i];
            if (i == grab_button.nombre - 1)
            {
                libXRecPlay_AddEvent( display, e.xany.window, send, True, True );
            }
            else
            {
                libXRecPlay_AddEvent( display, e.xany.window, send, True, False );
            }

        }
    */

    grab_button_sup.nombre = 12;
    grab_button_sup.boutons = malloc( 12 *sizeof(int));
    if (grab_button_sup.boutons == NULL) exit(1);
    int cpt=1;
    int grab_test;
    for (i=1; grab_button_sup.nombre > i - cpt ; ++i)
    {
        grab_test=True;
        for (j=0; grab_button.nombre > j; ++j)
        {
            if ( i == grab_button.boutons[j] )
            {
                ++cpt;
                grab_test = False;
                break;
            }
        }
        if (grab_test)
        {
            grab_button_sup.boutons[i - cpt]=i;
        }
    }

    grab_button_sup.nombre = 13 - cpt ;

    if (debug)
    {
        printf("grab_button_sup = ");
        for(i=0; grab_button_sup.nombre > i ;++i)
        {
            printf("%i ",grab_button_sup.boutons[i]);
        }
        printf("|\n");
    }


    if (test)
        printf("mode test\n");

    if (verbose)
    {
        for(i=0; grab_button.nombre > i ; i++)
        {
            printf("(grab-button %i)\n",grab_button.boutons[i]);
        }

        for(i=0; i < use_key.nombre; i++)
        {
            printf("(use-key \"%s\")\n",XKeysymToString( XKeycodeToKeysym( display, use_key.key[i], 0)) );
        }

        for(i=0; i < mod_key.nombre; i++)
        {
            printf("(mod-key \"%s\")\n",XKeysymToString( XKeycodeToKeysym( display, mod_key.key[i].key, 0)) );
            for(j=0; mod_key.key[i].nbgrab > j ; j++)
            {
                printf("  (mod-grab-button \"%s\" %i)\n", XKeysymToString( XKeycodeToKeysym( display, mod_key.key[i].key, 0)), mod_key.key[i].grab[j]);
            }
        }
    }


// début de la boucle evenementiel infinie
    while(infinie)
    {
        retour = libXRecPlay_GetEvent (display,&e);

        if ( retour == Event )
        {
            if (debug)
            {
                printf("fenetre = %i, event == %i ,detail = %i\n",e.xkey.window, e.xkey.type, e.xkey.keycode);
            }

            fenetre_client = e.xany.window;

            if(e.xany.type == KeyPress)
            {
                if (debug)
                    printf("\nDEBUT Event KeyPress\n");
//			nbpressed++;
                for(i=0; i < mod_key.nombre; i++)
                {
                    if ( e.xkey.keycode == mod_key.key[i].key )
                    {
//					nbpressed++;
//					nbpressed--;
                        strcpy(cpy,suite);
                        mod_key.key[i].pressed=True;
                        f_suite ( True );

                        for(j=0; mod_key.key[i].nbgrab > j ; j++)
                        {
                            //XGrabButton (display, mod_key.key[i].grab[j], AnyModifier, DefaultRootWindow(display), False, ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None);
                            printf("ici\n");
                            XEvent send;
                            send.xbutton.type = ButtonPress;
                            send.xbutton.button = mod_key.key[i].grab[j];
                            if (j == mod_key.key[i].nbgrab - 1)
                            {
                                printf("ici true\n");
                                libXRecPlay_AddEvent( display, e.xany.window, send, True, True );
                            }
                            else
                            {
                                printf("ici false\n");
                                libXRecPlay_AddEvent( display, e.xany.window, send, True, False );
                            }
                        }
                    }
                }

                for(i=0; i < use_key.nombre; i++)
                {
                    if ( e.xkey.keycode == use_key.key[i] )
                    {
                        nbchar = snprintf(cpy, lensuite, "%s%s ", suite, XKeysymToString( XKeycodeToKeysym( display,  e.xkey.keycode, 0)));
                        if ( resize() )
                            nbchar = snprintf(cpy, lensuite, "%s%s ", suite, XKeysymToString( XKeycodeToKeysym( display,  e.xkey.keycode, 0)));
                        f_suite ( True );
                    }
                }

                if (debug)
                    printf("FIN Event KeyPress\n\n");
            }

            if(e.xany.type == KeyRelease)
            {
                if (debug)
                    printf("DEBUT Event KeyRelease\n");
                for(i=0; i < mod_key.nombre; i++)
                {
                    if ( e.xkey.keycode == mod_key.key[i].key )
                    {
                        strcpy(cpy,suite);
                        f_suite ( False );
                        mod_key.key[i].pressed=False;
                        suite[0]='\0';
                        cpy[0]='\0';
                        for(j=0; mod_key.key[i].nbgrab > j ; j++)
                        {
//						XUngrabButton (display, mod_key.key[i].grab[j], AnyModifier, DefaultRootWindow(display));
                            XEvent send;
                            send.xbutton.type = ButtonPress;
                            send.xbutton.button = mod_key.key[i].grab[j];
                            if (j == mod_key.key[i].nbgrab - 1)
                            {
                                libXRecPlay_RemEvent( display, e.xany.window, send, True, True );
                            }
                            else
                            {
                                libXRecPlay_RemEvent( display, e.xany.window, send, True, False );
                            }
                        }
                    }
                }

                for(i=0; i < use_key.nombre; i++)
                {
                    if ( e.xkey.keycode == use_key.key[i] )
                    {
                        strcpy(cpy,suite);
                        if (debug)
                            printf("suite = %s\n",suite);
                        f_suite ( False );
                        suite[0]='\0';
                        cpy[0]='\0';
                    }
                }

                if (debug)
                    printf("FIN Event KeyRelease\n\n");

            }

            if(e.xany.type==ButtonPress)
            {
                if (debug)
                    printf("\nDEBUT Event ButtonPress\n");

                for(i=0; grab_button.nombre > i ; i++)
                {
                    if (grab_button.boutons[i] == e.xbutton.button )
                    {
                        for(j=0; grab_button_sup.nombre > j ; ++j)
                        {
                            XEvent send;
                            send.xbutton.type = ButtonPress;
                            send.xbutton.button = grab_button_sup.boutons[j];
                            if ( grab_button_sup.nombre == (j + 1) )
                            {
                                libXRecPlay_AddEvent( display, fenetre_client, send, True, True );
                            }
                            else
                            {
                                libXRecPlay_AddEvent( display, fenetre_client, send, True, False );
                            }
                        }
                        break;
                    }

                }

                nbchar = snprintf(cpy,lensuite,"%s%i ",suite,e.xbutton.button);
                if ( resize() )
                    nbchar = snprintf(cpy,lensuite,"%s%i ",suite,e.xbutton.button);
                if (debug)
                    printf("suite = |%s|\n",cpy);
                f_suite ( True );
                if (debug)
                    printf("FIN Event ButtonPress\n\n");

            }

            if(e.xany.type==ButtonRelease)
            {
                if (debug)
                    printf("\nDEBUT Event ButtonPress\n");

                for(i=0; grab_button.nombre > i ; ++i)
                {
                    if (grab_button.boutons[i] == e.xbutton.button )
                    {
                        for(j=0; grab_button_sup.nombre > j ; ++j)
                        {
                            XEvent send;
                            send.xbutton.type = ButtonPress;
                            send.xbutton.button = grab_button_sup.boutons[j];
                            if ( grab_button_sup.nombre == (j + 1) )
                            {
                                libXRecPlay_RemEvent( display, fenetre_client, send, True, True );
                            }
                            else
                            {
                                libXRecPlay_RemEvent( display, fenetre_client, send, True, False );
                            }
                        }
                        break;
                    }

                }

                strcpy(cpy,suite);
                if (debug)
                    printf("suite = |%s|\n",suite);
                f_suite ( False );
                suite[0]='\0';
                cpy[0]='\0';
                if (debug)
                    printf("FIN Event ButtonPress\n\n");
            }

        }
        else if ( retour == Client );
        {
            if (e.xany.type == CreateNotify )
            {
                fenetre_client = e.xany.window;
                for(i=0; grab_button.nombre > i ; i++)
                {
                    XEvent send;
                    send.xbutton.type = ButtonPress;
                    send.xbutton.button = grab_button.boutons[i];
                    if (i == grab_button.nombre - 1)
                    {
                        libXRecPlay_AddEvent( display, e.xany.window, send, True, True );
                    }
                    else
                    {
                        libXRecPlay_AddEvent( display, e.xany.window, send, True, False );
                    }
                }

                for(i=0; use_key.nombre > i ; i++)
                {
                    XEvent send;
                    send.xbutton.type = KeyPress;
                    send.xbutton.button = use_key.key[i];
                    if (i == use_key.nombre - 1)
                    {
                        libXRecPlay_AddEvent( display, e.xany.window, send, False, True );
                    }
                    else
                    {
                        libXRecPlay_AddEvent( display, e.xany.window, send, False, False );
                    }
                }

                for(i=0; mod_key.nombre > i ; i++)
                {
                    XEvent send;
                    send.xbutton.type = KeyPress;
                    send.xbutton.button = mod_key.key[i].key;
                    if (i == mod_key.nombre - 1)
                    {
                        libXRecPlay_AddEvent( display, e.xany.window, send, False, True );
                    }
                    else
                    {
                        libXRecPlay_AddEvent( display, e.xany.window, send, False, False );
                    }
                }


            }
            else if (e.xany.type == DestroyNotify )
            {
                fenetre_client = NULL;
            }
        }
//fin de la boucle infini
    }

    for(i=0; i < nb_scheme_KR; i++)
    {
        free( scheme_KR[i].scheme);

    }
    free(scheme_KR);

    for(i=0; i < nb_scheme_KP; i++)
    {
        free( scheme_KP[i].scheme);

    }
    free(scheme_KP);

// Réactivation de l'autorepetition du clavier
//	XAutoRepeatOn(display);

//close connection to server
    XCloseDisplay(display);

}

void traite_signal( int s )
{
    printf( "SIG %d\n", s );
    infinie = False;
    int i;
    /*
    	if ( (use_key.key != NULL) || (mod_key.key != NULL) )
    		{
    // Réactivation de l'autorepetition du clavier
    		XAutoRepeatOn(display);
    		XFlush(display);
    		usleep(100000);
    		}
    */
    for(i=0; i < nb_scheme_KR; i++)
    {

        free( scheme_KR[i].scheme);
    }
    if (scheme_KR != NULL)
        free(scheme_KR);

    for(i=0; i < nb_scheme_KP; i++)
    {

        free( scheme_KP[i].scheme);
    }
    if (scheme_KP != NULL)
        free(scheme_KP);

    for(i=0; grab_button.nombre > i ; i++)
    {
//		XUngrabButton (display, grab_button.boutons[i], AnyModifier, DefaultRootWindow(display));
    }
    if (grab_button.boutons != NULL)
        free(grab_button.boutons);

    if (grab_button_sup.boutons != NULL)
        free(grab_button_sup.boutons);

    if (use_key.key != NULL)
        free(use_key.key);
    for(i=0; mod_key.nombre > i ; i++)
    {
        if (mod_key.key[i].grab != NULL)
            free(mod_key.key[i].grab);
    }
    if (mod_key.key != NULL)
        free(mod_key.key);
    if (suite != NULL)
        free(suite);
    if (cpy != NULL)
        free(cpy);

//close connection to server
//	XCloseDisplay(display);
    if (debug)
        printf("FIN inner_main\n");
    exit(1);
}


int
main (void)
{
    extern void Rmalloc_reinit(void);
    Rmalloc_reinit();

    struct sigaction action;
    action.sa_handler = traite_signal;


    sigaction( SIGHUP, &action, NULL);
    sigaction( SIGINT, &action, NULL);
    sigaction( SIGQUIT, &action, NULL);
    sigaction( SIGALRM, &action, NULL);
    sigaction( SIGTERM, &action, NULL);
    sigaction( SIGTSTP, &action, NULL);
    sigaction( SIGHUP, &action, NULL);

    //guile shouldn't steal our arguments! we already parse them!
    //so we put them in temporary variables.
//	argv_t = argv;
//	argc_t = argc;
//	printf("Starting in guile mode...\n"); //debugery!
    scm_boot_guile(0,(char**)NULL,(void *)inner_main,NULL);
    return 0; /* not reached ...*/
}

