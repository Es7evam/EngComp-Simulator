#include "biblioteca.h"

volatile int sair = FALSE;
void fecha_programa(){ sair = TRUE; }
END_OF_FUNCTION(fecha_programa)

volatile int timer;
void increment_timer() { timer++; }
END_OF_FUNCTION(increment_timer)

volatile int ticks;
void tick_counter(){ ticks++; }
END_OF_FUNCTION(tick_counter)

void keyboard_input(){
    int i;

    for(i=0; i < KEY_MAX; i++)
        teclas_anteriores[i] = key[i];

    poll_keyboard();
}
int keyboard_apertou(int TECLA){
    return(key[TECLA] == TRUE && teclas_anteriores[TECLA] == FALSE);
}

int keyboard_segurou(int TECLA){
    return(key[TECLA] == TRUE && teclas_anteriores[TECLA] == TRUE);
}

int keyboard_soltou(int TECLA){
    return(key[TECLA] == FALSE && teclas_anteriores[TECLA] == TRUE);
}

//------------------------------Jogo em si ----------------------------//
const int nrodiscp = 66;
typedef long long ll;

char name[100];
int playing = 1;
ll prog1 = 0,prog2 =0, prog3 =0, prog4 = 0;


typedef struct player {
    int vida;
    int dp;
    int resets;
    ll dinheiro;
    ll pontos;
    ll conhecimento;
} player;

player jog; //declarando a struct

typedef struct disciplina{
    char nome[25];
    int feita; //booleano, única coisa que fica no save.
    int dinheiro;
    int pontos;
    int conhecimento;
    int refatual;
    ll tempo;

}disciplina;

int cox;

void save(disciplina *disp){
    FILE * fp;
    char aux[10];
    strcpy(aux,"./save/");
    strcat(aux, name);
    fp = fopen(aux, "w");
    fprintf(fp,"%lld %lld %lld %lld %d %d %d %d %lld %lld %lld ", prog1, prog2, prog3, prog4, cox, jog.vida, jog.dp ,jog.resets, jog.dinheiro, jog.pontos, jog.conhecimento);
    int i;
    for(i = 0;i<nrodiscp;i++){
        fprintf(fp, "%d ", disp[i].feita);
    }
    fclose(fp);
}//salvando no arquivo

void reset(disciplina *disp){

    if (jog.pontos == 0){
        jog.pontos = 1;
    }
    int i;
    for(i=0;i<nrodiscp;i++){
        disp[i].feita = 0;
    }
}

void load(disciplina *disp){
    int i;
    FILE * fp;
    fp = fopen("./balanceamento/disciplinas.txt", "r");
    for(i=0;i<nrodiscp;i++){
        fscanf(fp, "%s", disp[i].nome);
    }
    fclose(fp);

    fp = fopen("./balanceamento/balancodisp.txt", "r");
    for(i=0;i<nrodiscp;i++){
        fscanf(fp, "%d %d %d %lld\n", &disp[i].dinheiro, &disp[i].pontos ,&disp[i].conhecimento, &disp[i].tempo);
    }
    fclose(fp);
}


void INITIATION(){
    //---------------------Iniciando o Allegro -------------------------//
    allegro_init();
    install_timer();
    install_keyboard();
    install_mouse();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
    set_window_title("Graduacao Simulator");


    LOCK_VARIABLE(sair);
    LOCK_FUNCTION(fecha_programa);
    set_close_button_callback(fecha_programa);

    timer = 0; //usado pra contar o tempo
    LOCK_FUNCTION(increment_timer);
    LOCK_VARIABLE(timer);
    install_int_ex(increment_timer, MSEC_TO_TIMER(1));

    ticks = 0;
    LOCK_FUNCTION(tick_counter);
    LOCK_VARIABLE(ticks);
    install_int_ex(tick_counter, BPS_TO_TIMER(60));

    screen_state = TITLESCREEN;
    srand((unsigned)time(NULL));
}


void coxeroxScreen(){
    int exit_screen = FALSE;
    ///BITMAP
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* background = load_bitmap("images/cox.bmp", NULL);
    BITMAP* mouse = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_normal = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_clique = load_bitmap("images/mouseclique.bmp", NULL);

    ///GAMELOOP
    while(!sair && !exit_screen){
        while(ticks>0 && !exit_screen && !sair){
            ///INPUT
            mouse = mouse_normal;
            //-----------------BOTAO COXINHA--------------------------------
            if (mouse_x > 129 && mouse_y > 299 && mouse_x < 379 && mouse_y < 379 ){
                mouse = mouse_clique;
                if(mouse_b == 1){
                    while(mouse_b);
                    exit_screen = TRUE;
                    screen_state = MAINMENU;
                    cox = 1;
                }
            }
            //-----------------BOTAO XEROX-------------------------------------
            if (mouse_x > 407 && mouse_y > 299 && mouse_x < 659 && mouse_y < 379 ){
                mouse = mouse_clique;
                if(mouse_b == 1){
                    while(mouse_b);
                    exit_screen = TRUE;
                    screen_state = MAINMENU;
                    cox = 0;
                }
            }
            ///DRAW
            draw_sprite(buffer, background, 0, 0);
            draw_sprite(buffer, mouse, mouse_x, mouse_y);
            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));

            ticks --;
        }
    }

    ///FINALIZAÇÃO
    destroy_bitmap(buffer);
    destroy_bitmap(background);
    destroy_bitmap(mouse_clique);
    destroy_bitmap(mouse_normal);
}

void winScreen(){
    int exit_screen = FALSE;
    ///BITMAP
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* background = load_bitmap("images/win.bmp", NULL);
    BITMAP* mouse = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_normal = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_clique = load_bitmap("images/mouseclique.bmp", NULL);
    ///FONTE
    FONT* arial16 = load_font("fonts/arial16.pcx", NULL, NULL);
    ///GAMELOOP
    while(!sair && !exit_screen){
        while(ticks>0 && !exit_screen && !sair){
            ///INPUT
            mouse = mouse_normal;
            //-----------------BOTAO CONTINUAR--------------------------------
            if (mouse_x > 268 && mouse_y > 343 && mouse_x < 528 && mouse_y < 405 ){
                mouse = mouse_clique;
                if(mouse_b == 1){
                    while(mouse_b);
                    exit_screen = TRUE;
                    screen_state = TITLESCREEN;
                }
            }
            ///DRAW
            draw_sprite(buffer, background, 0, 0);
            textout_centre_ex(buffer, arial16, "O Segredo do Jogo eh o Cinema", SCREEN_W/2, (SCREEN_H/2)+150, makecol(255,255,255), -1);
            textout_centre_ex(buffer, arial16, "Quanto menos Vida, mais Pontos Ganha", SCREEN_W/2, (SCREEN_H/2)+170, makecol(255,255,255), -1);
            draw_sprite(buffer, mouse, mouse_x, mouse_y);
            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));

            ticks --;
        }
    }

    ///FINALIZAÇÃO
    destroy_bitmap(buffer);
    destroy_bitmap(background);
    destroy_bitmap(mouse_clique);
    destroy_bitmap(mouse_normal);
    destroy_font(arial16);
}

void ptScreen(){
    int exit_screen = FALSE;
    ///BITMAP
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* background = load_bitmap("images/pt.bmp", NULL);
    BITMAP* mouse = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_normal = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_clique = load_bitmap("images/mouseclique.bmp", NULL);
    ///GAMELOOP
    while(!sair && !exit_screen){
        while(ticks>0 && !exit_screen && !sair){
            ///INPUT
            mouse = mouse_normal;
            //-----------------BOTAO CONTINUAR--------------------------------
            if (mouse_x > 268 && mouse_y > 343 && mouse_x < 528 && mouse_y < 405 ){
                mouse = mouse_clique;
                if(mouse_b == 1){
                    while(mouse_b);
                    exit_screen = TRUE;
                    screen_state = MAINMENU;
                }
            }
            ///DRAW
            draw_sprite(buffer, background, 0, 0);
            draw_sprite(buffer, mouse, mouse_x, mouse_y);
            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));

            ticks --;
        }
    }

    ///FINALIZAÇÃO
    destroy_bitmap(buffer);
    destroy_bitmap(background);
    destroy_bitmap(mouse_clique);
    destroy_bitmap(mouse_normal);
}

void resetScreen(){
    int exit_screen = FALSE;
    ///BITMAP
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* background = load_bitmap("images/reset.bmp", NULL);
    BITMAP* mouse = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_normal = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_clique = load_bitmap("images/mouseclique.bmp", NULL);
    ///GAMELOOP
    while(!sair && !exit_screen){
        while(ticks>0 && !exit_screen && !sair){
            ///INPUT
            mouse = mouse_normal;
            //-----------------BOTAO CONTINUAR--------------------------------
            if (mouse_x > 268 && mouse_y > 343 && mouse_x < 528 && mouse_y < 405 ){
                mouse = mouse_clique;
                if(mouse_b == 1){
                    while(mouse_b);
                    exit_screen = TRUE;
                    screen_state = MAINMENU;
                }
            }
            ///DRAW
            draw_sprite(buffer, background, 0, 0);
            draw_sprite(buffer, mouse, mouse_x, mouse_y);
            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));

            ticks --;
        }
    }

    ///FINALIZAÇÃO
    destroy_bitmap(buffer);
    destroy_bitmap(background);
    destroy_bitmap(mouse_clique);
    destroy_bitmap(mouse_normal);
}

void jubilaScreen(disciplina*disp){
    int exit_screen = FALSE;
    ///BITMAP
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* background = load_bitmap("images/jubila.bmp", NULL);
    BITMAP* mouse = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_normal = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_clique = load_bitmap("images/mouseclique.bmp", NULL);

    ///GAMELOOP
    while(!sair && !exit_screen){
        while(ticks>0 && !exit_screen && !sair){
            ///INPUT
            mouse = mouse_normal;
            //-----------------BOTAO CONTINUAR--------------------------------
            if (mouse_x > 268 && mouse_y > 343 && mouse_x < 528 && mouse_y < 405 ){
                mouse = mouse_clique;
                if(mouse_b == 1){
                    while(mouse_b);
                    exit_screen = TRUE;
                    screen_state = TITLESCREEN;
                    jog.conhecimento = jog.dp = jog.dinheiro = 0;
                    jog.pontos = 1;
                    jog.vida = 1000;
                    reset(disp);
                }
            }
            ///DRAW
            draw_sprite(buffer, background, 0, 0);
            draw_sprite(buffer, mouse, mouse_x, mouse_y);
            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));

            ticks --;
        }
    }

    ///FINALIZAÇÃO
    destroy_bitmap(buffer);
    destroy_bitmap(background);
    destroy_bitmap(mouse_clique);
    destroy_bitmap(mouse_normal);

}

void dpScreen(){
    int exit_screen = FALSE;
    ///BITMAP
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* dp1 = load_bitmap("images/dp/dp1.bmp", NULL);
    BITMAP* dp2 = load_bitmap("images/dp/dp2.bmp", NULL);
    BITMAP* dp3 = load_bitmap("images/dp/dp3.bmp", NULL);
    BITMAP* dp4 = load_bitmap("images/dp/dp4.bmp", NULL);
    BITMAP* dp5 = load_bitmap("images/dp/dp5.bmp", NULL);
    BITMAP* dp6 = load_bitmap("images/dp/dp6.bmp", NULL);
    BITMAP* dp7 = load_bitmap("images/dp/dp7.bmp", NULL);
    BITMAP* dp8 = load_bitmap("images/dp/dp8.bmp", NULL);
    BITMAP* dp9 = load_bitmap("images/dp/dp9.bmp", NULL);
    BITMAP* dp10 = load_bitmap("images/dp/dp10.bmp", NULL);
    BITMAP* dpSelected;
    BITMAP* mouse = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_normal = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_clique = load_bitmap("images/mouseclique.bmp", NULL);

    ///VARIAVEIS
    int numeroDp;
    numeroDp = rand()%10;
    switch (numeroDp){
        case 0:
            dpSelected = dp1;
            break;
        case 1:
            dpSelected = dp2;
            break;
        case 2:
            dpSelected = dp3;
            break;
        case 3:
            dpSelected = dp4;
            break;
        case 4:
            dpSelected = dp5;
            break;
        case 5:
            dpSelected = dp6;
            break;
        case 6:
            dpSelected = dp7;
            break;
        case 7:
            dpSelected = dp8;
            break;
        case 8:
            dpSelected = dp9;
            break;
        case 9:
            dpSelected = dp10;
            break;
    }


    ///GAMELOOP
    while(!sair && !exit_screen){
        while(ticks>0 && !exit_screen && !sair){
            ///INPUT
            mouse = mouse_normal;
            //-----------------BOTAO CONTINUAR--------------------------------
            if (mouse_x > 268 && mouse_y > 343 && mouse_x < 528 && mouse_y < 405 ){
                mouse = mouse_clique;
                if(mouse_b == 1){
                    while(mouse_b);
                    screen_state = MAINMENU;
                    exit_screen = TRUE;
                }
            }
            ///DRAW
            draw_sprite(buffer, dpSelected, 0, 0);
            draw_sprite(buffer, mouse, mouse_x, mouse_y);
            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));

            ticks --;
        }
    }

    ///FINALIZAÇÃO
    destroy_bitmap(dp1);
    destroy_bitmap(dp2);
    destroy_bitmap(dp3);
    destroy_bitmap(dp4);
    destroy_bitmap(dp5);
    destroy_bitmap(dp6);
    destroy_bitmap(dp7);
    destroy_bitmap(dp8);
    destroy_bitmap(dp9);
    destroy_bitmap(dp10);
    destroy_bitmap(buffer);
    destroy_bitmap(mouse_clique);
    destroy_bitmap(mouse_normal);
}

void discpStore(disciplina *disp){

    int exit_screen = FALSE;

    ///BITMAP
    BITMAP* background = load_bitmap("images/help.bmp", NULL);
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* mouse = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_normal = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_clique = load_bitmap("images/mouseclique.bmp", NULL);

    ///FONTS
    FONT* arial16 = load_font("fonts/arial16.pcx" ,NULL, NULL);
    FONT* arialBB8 = load_font("fonts/arialBB8.pcx", NULL, NULL);

    ///VARIAVEIS
    int mouse_bt = 0;

    ///GAMELOOP
    while(!sair && !exit_screen){
        while(ticks>0 && !exit_screen && !sair){

        ///INPUT
            keyboard_input();
            mouse = mouse_normal;
                if (mouse_b == 0){
                    mouse_bt = 0;
                }

            //-----------------BOTAO VOLTAR--------------------------------
                if (mouse_x > 0 && mouse_y > 543 && mouse_x < 211 && mouse_y < 600 ){
                    mouse = mouse_clique;
                    if(mouse_b == 1 && mouse_bt == 0){
                        mouse_bt = 1;
                        while(mouse_b);
                        screen_state = MAINMENU;
                        exit_screen = TRUE;
                    }
                }
            ///DRAW

            draw_sprite(buffer, background, 0, 0);
            draw_sprite(buffer, mouse, mouse_x, mouse_y);
            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));

            ticks --;
        }
    }
    ///FINALIZAÇÃO
    destroy_bitmap(background);
    destroy_bitmap(buffer);
    destroy_bitmap(mouse_normal);
    destroy_bitmap(mouse_clique);
    destroy_font(arial16);
    destroy_font(arialBB8);
}

void store(disciplina *disp){

    int exit_screen = FALSE;

    ///BITMAP
    BITMAP* background = load_bitmap("images/store.bmp", NULL);
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* mouse = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_normal = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_clique = load_bitmap("images/mouseclique.bmp", NULL);

    ///FONTS
    FONT* arial16 = load_font("fonts/arial16.pcx" ,NULL, NULL);
    FONT* arialBB8 = load_font("fonts/arialBB8.pcx", NULL, NULL);

    ///SOUNDS

    ///VARIAVEIS
    int mouse_bt = 0;

     ///GAMELOOP
    while(!sair && !exit_screen){
        while(ticks>0 && !exit_screen && !sair){

        ///INPUT
        keyboard_input();
        mouse = mouse_normal;
            if (mouse_b == 0){
                mouse_bt = 0;
            }

        //-----------------BOTAO COXINHA--------------------------------
            if (mouse_x > 329 && mouse_y > 90 && mouse_x < 478 && mouse_y < 139 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    if(jog.dinheiro - 4 >= 0){
                        jog.dinheiro -= 4;
                        if(cox == 1){
                            jog.vida += 100;
                        }else{
                            jog.vida += 30;
                        }
                    }
                }
            }

            //-----------------BOTAO PODRÃO--------------------------------
            if (mouse_x > 329 && mouse_y > 153 && mouse_x < 478 && mouse_y < 202 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    if(jog.dinheiro - 5 >= 0){
                        jog.dinheiro -= 5;
                        jog.vida += 40;

                    }

                }
            }

            //-----------------BOTAO AQUELA FESTA--------------------------------
            if (mouse_x > 329 && mouse_y > 214 && mouse_x < 478 && mouse_y < 263 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    if(jog.dinheiro - 40 >= 0){
                        jog.dinheiro -= 40;
                        jog.vida += 360;
                    }

                }
            }

            //-----------------BOTAO CINEMA--------------------------------
            if (mouse_x > 329 && mouse_y > 276 && mouse_x < 478 && mouse_y < 325 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    if(jog.dinheiro - 15 >= 0){
                        jog.dinheiro -= 15;
                        jog.pontos += 1 + (jog.pontos/jog.vida);
                        jog.vida += 60;
                    }

                }
            }

            //-------------------BOTAO XEROX----------------------------------
            if (mouse_x > 329 && mouse_y > 338 && mouse_x < 478 && mouse_y < 387 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    if(jog.dinheiro - 5 >= 0){
                        jog.dinheiro -= 5;
                        if (cox == 0){
                            jog.conhecimento += (jog.vida+jog.pontos)/(jog.conhecimento+1) * (jog.pontos/(jog.conhecimento+1));
                        }else{
                            jog.conhecimento += jog.conhecimento/1000*((rand()%5)+1);
                        }
                    }

                }
            }
            //-------------------BOTAO ÁLCOOL------------------------------------
            if (mouse_x > 329 && mouse_y > 400 && mouse_x < 478 && mouse_y < 449 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    if(jog.dinheiro - 20 >= 0){
                        jog.dinheiro -= 20;
                        jog.vida += 250;
                        if(rand()%7 == 0){
                            jog.vida -= 300;
                            screen_state = PTSCREEN;
                            exit_screen = TRUE;
                        }

                    }

                }
            }
            //------------------BOTAO PSICÓLOGO-----------------------------------
            if (mouse_x > 329 && mouse_y > 463 && mouse_x < 478 && mouse_y < 512 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    if(jog.dinheiro - 100 >= 0){
                        jog.dinheiro -= 100;
                        jog.vida = 1000;
                    }

                }
            }
            //------------------BOTAO VOLTAR-----------------------------------
            if (mouse_x > 407 && mouse_y > 532 && mouse_x < 773 && mouse_y < 587 ){
                mouse = mouse_clique;
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    while(mouse_b);
                    exit_screen = TRUE;
                    screen_state = MAINMENU;
                }
            }
            ///DRAW

            draw_sprite(buffer, background, 0, 0);
            textprintf_ex(buffer, arial16, 228, 548, makecol(0, 0, 0), -1, " %lld dinheiros", jog.dinheiro);//TEXTO DINHEIRO
            textprintf_ex(buffer, arial16, 580, 90, makecol(21,184,16), makecol(0,0,0), "Vida: %d", jog.vida);//TEXTO VIDA
            textprintf_ex(buffer, arial16, 580, 110, makecol(21,184,16), makecol(0,0,0), "Conhecimento: %lld", jog.conhecimento);
            textprintf_ex(buffer, arial16, 580, 130, makecol(21,184, 16), makecol(0,0,0), "Pontos: %lld", jog.pontos);
            draw_sprite(buffer, mouse, mouse_x, mouse_y);
            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));
            ticks --;
        }
    }
    ///FINALIZAÇÃO

    destroy_bitmap(background);
    destroy_bitmap(buffer);
    //destroy_bitmap(mouse);
    destroy_bitmap(mouse_normal);
    destroy_bitmap(mouse_clique);
    destroy_font(arial16);
    destroy_font(arialBB8);

}

void titlescreen(){

    int exit_screen = FALSE;

    int selection = -1;
    ///BITMAP
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* arrow = load_bitmap("images/pointer.bmp", NULL); //bitmap pra imagem

    ///FONTS
    FONT* arial16 = load_font("fonts/arial16.pcx" ,NULL, NULL);
    FONT* arialBB8 = load_font("fonts/arialBB8.pcx", NULL, NULL);

    ///SOUNDS

    ///GAME LOOP
    while(!sair && !exit_screen){
        while(ticks > 0 && !exit_screen && !sair){
            //INPUT

            keyboard_input();

            if(keyboard_apertou(KEY_ENTER)){ //apertar enter
                if (selection == -1){
                exit_screen = TRUE;
                screen_state = MAINMENU;
                }else if (selection == 0){
                exit_screen = TRUE;
                sair = TRUE;
                }
            }

            if(keyboard_apertou(KEY_DOWN)){
                selection ++;
            }else if (keyboard_apertou(KEY_UP)){
                selection --;
            }



            //UPDATE
            if(selection > 0){
                selection = -1;
            }
            if(selection < -1){
                selection= 0;
            }
            selection = selection *50;

            //DRAW
            draw_sprite(buffer,arrow, SCREEN_W/2-50, SCREEN_H/2+selection-5);
            textprintf_centre_ex(buffer, arial16,  SCREEN_W/2, 100, makecol(0, 0, 0), -1 ,"Eng Comp Simulator");
            textout_centre_ex(buffer, arialBB8,"Play",  SCREEN_W/2, SCREEN_H/2-50, makecol(0, 0, 0), -1);
            textout_centre_ex(buffer, arialBB8,"Exit",  SCREEN_W/2, SCREEN_H/2, makecol(0, 0, 0), -1);
            draw_sprite(screen, buffer, 0, 0);

            clear_to_color(buffer, makecol(255, 255, 255)); //cor do fundo

            ticks --;
            selection = selection /50;
            timer = 0;
        }
    }
    ///FINALIZAÇÂO

    destroy_bitmap(buffer);
    destroy_bitmap(arrow);
    destroy_font(arial16);
    destroy_font(arialBB8);

}

void completediscp(disciplina *disp, disciplina *atualdisp){
    int i = 0;
    int j = 0;


    // -------------- Atualizar as disciplinas ------------------//
    for(i=0;i<4;i++){
        if(disp[j].feita == 0){
            atualdisp[i].dinheiro = disp[j].dinheiro;
            atualdisp[i].pontos = disp[j].pontos;
            atualdisp[i].conhecimento = disp[j].conhecimento;
            atualdisp[i].tempo = disp[j].tempo;
            atualdisp[i].refatual = j;
            strcpy(atualdisp[i].nome, disp[j].nome);
        }else{
            if (i == 5){
                break;
            }
            i--;
        }
        j++;

    }

    // ----------- Fim da atualização das disciplinas-----------//
}

void mainmenu(disciplina *disp, disciplina *atualdisp){
    int exit_screen = FALSE;

    //COXINHA OU XEROX
    if(cox == 2){
        coxeroxScreen();
    }

    ///BITMAPS
    BITMAP* background = load_bitmap("images/background.bmp", NULL);
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* mouse = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_normal = load_bitmap("images/mouse.bmp", NULL);
    BITMAP* mouse_clique = load_bitmap("images/mouseclique.bmp", NULL);
    BITMAP* transparent = load_bitmap("images/transparente.bmp", NULL);

    ///FONTS
    FONT* arial16 = load_font("fonts/arial16.pcx" ,NULL, NULL);
    FONT* arialBB8 = load_font("fonts/arialBB8.pcx", NULL, NULL);


    ///TIMER

    int segundos = 0;
    int minutos = 0;
    int horas = 0;
    int mouse_bt = 0;
    int marcador = timer;

    int i = 0, j = 0;
    for(i=0;i<4;i++){
        if(disp[j].feita == 0){
            atualdisp[i].dinheiro = disp[j].dinheiro;
            atualdisp[i].pontos = disp[j].pontos;
            atualdisp[i].conhecimento = disp[j].conhecimento;
            atualdisp[i].tempo = disp[j].tempo;
            atualdisp[i].refatual = j;
            strcpy(atualdisp[i].nome, disp[j].nome);
        }else{
            i--;
        }
        j++;
    }

    i = 0; j = 0;


    ///GAMELOOP
    while(!sair && !exit_screen){
        while(ticks>0 && !exit_screen && !sair){

            ///INPUT
            keyboard_input();

            // CHEAT
            if(keyboard_apertou(KEY_UP)){
                jog.vida += 10;
            }else if(keyboard_apertou(KEY_DOWN)){
                jog.vida -=  100;
            }
            if(keyboard_apertou(KEY_SPACE)){
                jog.vida +=1000;
            }
            if(keyboard_apertou(KEY_8_PAD)){
                jog.pontos += jog.pontos*10;
            }
            if(keyboard_apertou(KEY_6_PAD)){
                jog.conhecimento += jog.conhecimento*10;
            }
            // FIM DO CHEAT
            mouse = mouse_normal;
            if (mouse_b == 0){
                mouse_bt = 0;
            }


            //-----------------BOTAO ESTUDAR 1--------------------------------
            if (mouse_x > 347 && mouse_y > 112 && mouse_x < 483 && mouse_y < 153 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    prog1 += jog.pontos;
                    if(prog1>=atualdisp[0].tempo){
                        jog.pontos += atualdisp[0].pontos;
                        jog.conhecimento += atualdisp[0].conhecimento;
                        jog.dinheiro += atualdisp[0].dinheiro;
                        disp[atualdisp[0].refatual].feita = 1;
                        completediscp(disp, atualdisp);
                        prog1 = prog2;
                        prog2 = prog3;
                        prog3 = prog4;
                        prog4 = 0;
                    }

                    if(prog1<atualdisp[0].tempo){
                        jog.vida -=  10;
                    }
                }
            }

            //-----------------BOTAO ESTUDAR 2--------------------------------
            if (mouse_x > 346 && mouse_y > 213 && mouse_x < 482 && mouse_y < 254 && disp[64].feita != 1 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    prog2 += jog.pontos;
                    if(prog2>=atualdisp[1].tempo){
                        jog.pontos += atualdisp[1].pontos;
                        jog.conhecimento += atualdisp[1].conhecimento;
                        jog.dinheiro += atualdisp[1].dinheiro;
                        disp[atualdisp[1].refatual].feita = 1;
                        completediscp(disp, atualdisp);
                        prog2 = prog3;
                        prog3 = prog4;
                        prog4 = 0;
                    }

                    if(prog2<atualdisp[1].tempo){
                        jog.vida -=  10;
                    }
                }
            }

            //-----------------BOTAO ESTUDAR 3--------------------------------
            if (mouse_x > 347 && mouse_y > 315 && mouse_x < 483 && mouse_y < 355 && disp[63].feita != 1){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    prog3 += jog.pontos;
                    if(prog3>=atualdisp[2].tempo){
                        jog.pontos += atualdisp[2].pontos;
                        jog.conhecimento += atualdisp[2].conhecimento;
                        jog.dinheiro += atualdisp[2].dinheiro;
                        disp[atualdisp[2].refatual].feita = 1;
                        completediscp(disp, atualdisp);
                        prog3 = prog4;
                        prog4 = 0;
                    }

                    if(prog3<atualdisp[2].tempo){
                        jog.vida -= 10;
                    }
                }
            }

            //-----------------BOTAO ESTUDAR 4--------------------------------
            if (mouse_x > 345 && mouse_y > 415 && mouse_x < 481 && mouse_y < 456 && disp[62].feita != 1 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    prog4 += jog.pontos;
                    if(prog4>=atualdisp[3].tempo){
                        jog.pontos += atualdisp[3].pontos; //mudar para cada disciplina
                        jog.conhecimento += atualdisp[3].conhecimento;
                        jog.dinheiro += atualdisp[3].dinheiro;
                        disp[atualdisp[3].refatual].feita = 1;
                        completediscp(disp, atualdisp);
                        prog4 = 0;
                    }

                    if(prog4<atualdisp[3].tempo){
                        jog.vida -= 10;
                    }
                }
            }

            //-------------------BOTAO RESET----------------------------------
            if (mouse_x > 552 && mouse_y > 556 && mouse_x < 800 && mouse_y < 600 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    jog.vida = 1000;
                    reset(disp);
                    completediscp(disp, atualdisp);
                    prog1 = 0;
                    prog2 = 0;
                    prog3 = 0;
                    prog4 = 0;
                    jog.conhecimento = jog.conhecimento * 0.8;
                    jog.pontos = jog.pontos * 0.8;
                    jog.dp = jog.dp * 0.25;
                    if (jog.pontos == 0) jog.pontos = 1;
                    screen_state = RESETSCREEN;
                    exit_screen = TRUE;
                }
            }
            //-------------------BOTAO LOJA------------------------------------
            if (mouse_x > 0 && mouse_y > 556 && mouse_x < 250 && mouse_y < 600 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    screen_state = STORESCREEN;
                    exit_screen = TRUE;
                    save(disp);
                }
            }
            //------------------BOTAO DISCP-----------------------------------
            if (mouse_x > 276 && mouse_y > 556 && mouse_x < 526 && mouse_y < 600 ){
                mouse = mouse_clique;
                if(mouse_b == 1 && mouse_bt == 0){
                    mouse_bt = 1;
                    screen_state = DISCPSCREEN;
                    exit_screen = TRUE;
                    save(disp);
                }
            }


            ///UPDATE
            segundos = (timer/1000)%60;
            minutos = (timer/(60*1000))%60;
            horas = (timer/(60*60*1000))%60;

            if (timer - marcador >= 1000){
                prog1 += jog.conhecimento;
                prog2 += jog.conhecimento;
                prog3 += jog.conhecimento;
                prog4 += jog.conhecimento;
                jog.vida --;
                marcador = timer;
            }
            if(atualdisp[0].refatual > 66){
                prog1 = 0;
                atualdisp[0].tempo = 1;
            }
            if(atualdisp[1].refatual > 66){
                prog2 = 0;
                atualdisp[1].tempo = 1;
            }
            if(atualdisp[2].refatual > 66){
                prog3 = 0;
                atualdisp[2].tempo = 1;
            }
            if(atualdisp[3].refatual > 66){
                prog4 = 0;
                atualdisp[3].tempo = 1;
            }
            if(disp[65].feita == 1){
                exit_screen = TRUE;
                screen_state = WINSCREEN;
            }
            if (disp[64].feita == 1){
                strcpy(atualdisp[1].nome, "");
                atualdisp[1].tempo = 0;
                prog2 = 0;
            }
            if (disp[63].feita == 1){
                strcpy(atualdisp[2].nome, "");
                atualdisp[2].tempo = 0;
                prog3 = 0;
            }
            if (disp[62].feita == 1){
                strcpy(atualdisp[3].nome, "");
                atualdisp[3].tempo = 0;
                prog4 = 0;
            }


            if (jog.vida>1000){
                jog.vida=1000;
            }else if(jog.vida<=0){
                jog.vida = 500;
                jog.dp +=1;
                exit_screen = TRUE;
                screen_state = DPSCREEN;
            }
            if (jog.dp >= 15){
                screen_state = JUBILASCREEN;
                exit_screen = TRUE;
            }




            ///DRAW

            draw_sprite(buffer, background, 0, 0);

            textprintf_centre_ex(buffer, arial16, 138, 120, makecol(21,184,16), -1, "%s", atualdisp[0].nome);//TEXTO DA CAIXA 1 DE DISCIPLINAS
            textprintf_centre_ex(buffer, arial16, 138, 221, makecol(21,184,16), -1, "%s", atualdisp[1].nome);//TEXTO DA CAIXA 2 DE DISCIPLINAS
            textprintf_centre_ex(buffer, arial16, 138, 323, makecol(21,184,16), -1, "%s", atualdisp[2].nome);//TEXTO DA CAIXA 3 DE DISCIPLINAS
            textprintf_centre_ex(buffer, arial16, 138, 424, makecol(21,184,16), -1, "%s", atualdisp[3].nome);//TEXTO DA CAIXA 4 DE DISCIPLINAS
            textprintf_ex(buffer, arialBB8, 438, 495, makecol(0, 0, 0), -1, "%d/1000", jog.vida); //TEXTO DA VIDA
            textprintf_ex(buffer, arialBB8, 190, 520, makecol(0, 0, 0), -1, "%lld ", jog.dinheiro);//TEXTO DINHEIRO
            textprintf_ex(buffer, arialBB8, 351, 520, makecol(0, 0, 0), -1, "%lld", jog.pontos);//TEXTO PONTOS
            textprintf_ex(buffer, arialBB8, 595, 520, makecol(0, 0, 0), -1, "%lld", jog.conhecimento);//TEXTO CONHECIMENTO
            textprintf_ex(buffer, arialBB8, 722, 520, makecol(0, 0, 0), -1, "%d", jog.dp);//TEXTO DPS
            textprintf_centre_ex(buffer, arialBB8, 655, 50, makecol(21,184,16), -1, "Tempo: %d : %d : %d",horas, minutos,segundos);
            textprintf_right_ex(buffer, arial16, 780, 112, makecol(21, 184, 16), -1, "%lld/%lld", prog1, atualdisp[0].tempo);//TEXTO PROGRESSO 1
            textprintf_right_ex(buffer, arial16, 780, 213, makecol(21, 184, 16), -1, "%lld/%lld", prog2, atualdisp[1].tempo);//TEXTO PROGRESSO 2
            textprintf_right_ex(buffer, arial16, 780, 315, makecol(21, 184, 16), -1, "%lld/%lld", prog3, atualdisp[2].tempo);//TEXTO PROGRESSO 3
            textprintf_right_ex(buffer, arial16, 780, 415, makecol(21, 184, 16), -1, "%lld/%lld", prog4, atualdisp[3].tempo);//TEXTO PROGRESSO 4
            draw_sprite(buffer, mouse, mouse_x, mouse_y);

            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));
            ticks --;

        }
    }
    ///FINALIZAÇÃO

    save(disp); //salvando o jogo
    destroy_bitmap(background);
    destroy_bitmap(buffer);
    destroy_bitmap(mouse_normal);
    destroy_bitmap(mouse_clique); 
    destroy_bitmap(transparent);
    destroy_font(arial16);
    destroy_font(arialBB8);

}

int main(){
    //------------------Declaração de variáveis--------------------------//
    disciplina *disp; //criando o vetor de structs de nome player
    disp = (disciplina*)calloc((nrodiscp), sizeof(disciplina)); //declara setando como 0 as habilidades

    disciplina *atualdisp; //criando o vetor de structs de nome player
    atualdisp = (disciplina*)calloc((4), sizeof(disciplina)); //declara setando como 0 as habilidades

    jog.vida = 1000;
    jog.dp = jog.resets = jog.dinheiro = jog.conhecimento = 0;
    jog.pontos = 1;
    cox = 2;
    int i;
    for(i=0;i<nrodiscp;i++){
        disp[i].feita = 0;
    }
    // ---------------------------Leitura do arquivo----------------------//

    printf("Digite o nome do usuario do jogo:\n");
    scanf("%s", name);
    char aux[10];
    strcpy(aux,"./save/");
    strcat(aux, name);
    FILE * fp;
    fp = fopen(aux, "a");
    rewind(fp);

    if(feof(fp)){
        save(disp);
    }

    fclose(fp);
    strcpy(aux,"./save/");
    strcat(aux, name);
    fp = fopen(aux, "r");
    if(fp){
        fscanf(fp,"%lld %lld %lld %lld %d %d %d %d %lld %lld %lld",&prog1, &prog2, &prog3, &prog4, &cox, &jog.vida, &jog.dp ,&jog.resets, &jog.dinheiro, &jog.pontos, &jog.conhecimento);
        for(i=0;i<nrodiscp;i++){
            fscanf(fp, "%d", &disp[i].feita);
        }
    }else{
        printf("Erro, nao foi capaz de abrir o arquivo\n");
    }

    fclose(fp);


    fp = fopen("./balanceamento/disciplinas.txt", "r");

    for(i=0;i<nrodiscp;i++){
        fscanf(fp, "%s", &disp[i].nome);
    }//dando load no arquivo das disciplinas
    fclose(fp);


    fp = fopen("./balanceamento/balancodisp.txt", "r");

    for(i=0;i<nrodiscp;i++){
        fscanf(fp, "%d %d %d %lld", &disp[i].dinheiro, &disp[i].pontos, &disp[i].conhecimento, &disp[i].tempo);
    }//dando load no arquivo de balanço das disciplinas

    fclose(fp);

    INITIATION();
    ///Musica
    MIDI*musica = load_midi("./sounds/musica.mid");
    play_midi(musica, TRUE);
    while(!sair){
        if (screen_state == MAINMENU){
            mainmenu(disp, atualdisp);
        }
        else if(screen_state == TITLESCREEN){
            titlescreen();
        }
        else if(screen_state == STORESCREEN){
            save(disp);
            store(disp);
        }
        else if(screen_state == DISCPSCREEN){
            save(disp);
            discpStore(disp);
        }
        else if(screen_state == DPSCREEN){
            save(disp);
            dpScreen();
        }
        else if(screen_state == JUBILASCREEN){
            save(disp);
            jubilaScreen(disp);
        }
        else if(screen_state == RESETSCREEN){
            save(disp);
            resetScreen();
        }
        else if(screen_state == PTSCREEN){
            save(disp);
            ptScreen();
        }else if(screen_state == WINSCREEN){
            save(disp);
            winScreen();
        }

    }

    destroy_midi(musica);
    save(disp);
    allegro_exit();
    free(disp);
    return 0;
}
END_OF_MAIN()
