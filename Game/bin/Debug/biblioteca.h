#ifndef BIBLIOTECA_H_INCLUDED
#define BIBLIOTECA_H_INCLUDED

#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


enum{ TITLESCREEN, MAINMENU, STORESCREEN, DISCPSCREEN, DPSCREEN, RESETSCREEN, COXEROXSCREEN, JUBILASCREEN, PTSCREEN, WINSCREEN};
int screen_state;



//void INITIATION();

///TECLADO
int teclas_anteriores[KEY_MAX];
void keyboard_input();
int keyboard_apertou(int TECLA);
int keyboard_segurou(int TECLA);
int keyboard_soltou(int TECLA);



#endif // BIBLIOTECA_H_INCLUDED
