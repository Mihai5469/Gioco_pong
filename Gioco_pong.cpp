// Gioco_pong.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
// 16/03/2023

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

using namespace std;

void resetRetA(int w, int h, int& xrA1, int& xrA2, int& yrA1, int& yrA2);
void resetRet(int w, int h, int& xr1, int& xr2, int& yr1, int& yr2);
void resetPala(int w, int h, int& xc, int& yc, bool& rimV, bool& rimL);

#define KEY_SEEN     1
#define KEY_RELEASED 2



int main()
{
    int w = 480, h = 640;  //variabili che indicano la dimensione della finestra

    //variabili posizione retangolo
    int xr1 = w / 2 - 50, yr1 = h - 10;
    int xr2 = w /2 + 50, yr2 = h - 20;

    //variabili posizione retangolo automatico
    int xrA1 = w / 2 - 50, yrA1 = 10;
    int xrA2 = w / 2 + 50, yrA2 = 20;
    double centroRet = (xrA1 + xrA2) / 2;


    //variabili palina
    int xc = w / 2, yc = h/ 2;
    int raggio = 10;

    bool rimV = false;      //rimbalzo verticale
    bool rimL = false;      //rimbazo laterale
    
    //variabile per il puntegio
    int score = 0;


    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    al_install_keyboard();

    //variabili Allegro5
    ALLEGRO_DISPLAY* display = al_create_display(w, h);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 244.0);
    ALLEGRO_FONT* font = al_load_ttf_font("YARDSALE.TTF", 30, 0);

    ALLEGRO_EVENT event;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    //variabili per la chisura dell programma
    bool done = false;
    bool redraw = true;

    //per l input da tastiera
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    al_start_timer(timer);
    while (true) {

        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            
            if (key[ALLEGRO_KEY_LEFT] && xr1 != 2) {
                xr1--;
                xr2--;
            }
               
            if (key[ALLEGRO_KEY_RIGHT] && xr2 != w - 2) {
                xr1++;
                xr2++;
            }  

            if (key[ALLEGRO_KEY_A] && xr1 != 2) {
                xr1--;
                xr2--;
            }

            if (key[ALLEGRO_KEY_D] && xr2 != w - 2) {
                xr1++;
                xr2++;
            }
           
            if (key[ALLEGRO_KEY_ESCAPE])
                done = true;

            for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;
          
            redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;

        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= KEY_RELEASED;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        // controla se si vuole chiderre il programma tramite tasto 'ESC' o la 'X' che chide i programmi
        if (done)
            break;

        //muovimento dell retangolo automatico

        centroRet = (xrA1 + xrA2) / 2;

        if (centroRet < xc && xrA2 < w - 2) {
            xrA1++;
            xrA2++;
        }
        else if (xrA1 > 2) {
            xrA1--;
            xrA2--;
        }


        //Rimbalzo laterale
        if (xc < w - raggio && rimL == true) {
            xc++;
        }
        else
            rimL = false;

        if (xc > 0 + raggio && rimL == false) {
            xc--;
        }
        else
            rimL = true;

        //Rimbalzo verticale
        if (yc < h - 20 - raggio && rimV == false) {
            yc++;
        }
        else if (xc >= xr1 && xc <= xr2)
            rimV = true;
        else{
            score++;
            
            //da impostare
            resetRetA(w, h, xrA1, xrA2, yrA1, yrA2);
            resetRet(w, h, xr1, xr2, yr1, yr2);
            resetPala(w, h, xc, yc, rimV, rimL);
        }
        
            
           
        
        if (yc > 0 + 20 + raggio && rimV == true) {
            yc--;
        }
        else
            rimV = false;
            

        al_clear_to_color(al_map_rgb(220, 220, 220));

        al_draw_textf(font, al_map_rgb(50, 50, 50), w/2, h/2, 0, "Score: %d", score);


        al_draw_filled_rectangle(xrA1, yrA1, xrA2, yrA2, al_map_rgb(0, 0, 0));    //retangolo computer

        al_draw_filled_rectangle(xr1, yr1, xr2, yr2, al_map_rgb(0, 0, 0));  //retangolo giocatore
        al_draw_filled_circle(xc, yc, raggio, al_map_rgb(0, 0, 0));         //palina

        
        

        al_flip_display();

        //al_rest(0.01);

    }

    return 0;
}

void resetRetA(int w, int h, int &xrA1, int& xrA2, int& yrA1, int& yrA2) {
    //variabili posizione retangolo automatico
    xrA1 = w / 2 - 50, yrA1 = 10;
    xrA2 = w / 2 + 50, yrA2 = 20;
    return;
}

void resetRet(int w, int h, int& xr1, int& xr2, int& yr1, int& yr2) {

    //variabili posizione retangolo
    xr1 = w / 2 - 50; 
    yr1 = h - 10;
    xr2 = w / 2 + 50;
    yr2 = h - 20;
    return;
}


void resetPala(int w, int h, int& xc, int& yc, bool& rimV, bool& rimL) {
    //variabili palina
    xc = w / 2;
    yc = h / 2;
    
    rimV = false;      //rimbalzo verticale
    rimL = false;      //rimbazo laterale
    
    return;
}


