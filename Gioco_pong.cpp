// Gioco_pong.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
// 16/03/2023

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;

void resetRetA(int w, int h, int& xrA1, int& xrA2, int& yrA1, int& yrA2);   //reseta valori inizialiretangolo automatico
void resetRet(int w, int h, int& xr1, int& xr2, int& yr1, int& yr2);        //reseta valorei iniziali retangolo controlato da giocatore
void resetPala(int w, int h, int& xc, int& yc, bool& rimV, bool& rimL);     //reseta i valori iniziali della pala
void resetScoreSaveRecord(int& s, int& r);                                  //reseta il score  se salva il record in caso che sia maggiore al precedente

#define KEY_SEEN     1
#define KEY_RELEASED 2



int main()
{
    int w = 480, h = 640;  //variabili che indicano la dimensione della finestra

    //variabili posizione rettangolo
    int xr1 = w / 2 - 50, yr1 = h - 10;
    int xr2 = w /2 + 50, yr2 = h - 20;

    //variabili posizione rettangolo automatico
    int xrA1 = w / 2 - 50, yrA1 = 10;
    int xrA2 = w / 2 + 50, yrA2 = 20;
    double centroRet = (xrA1 + xrA2) / 2;


    //variabili palina
    int xc = w / 2, yc = h/ 2;
    int raggio = 10;

    bool rimV = false;      //rimbalzo verticale
    bool rimL = false;      //rimbalzo laterale
    
    //variabile per il punteggio
    int score = 0;
    int record = 0;

    bool start = false;

    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_acodec_addon();

    al_install_keyboard();
    al_install_audio();

    al_reserve_samples(16);

    //variabili Allegro5
    ALLEGRO_DISPLAY* display = al_create_display(w, h);             //dimensioni finestra
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();           //coda eventi
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 244.0);            //timer
    ALLEGRO_FONT* font = al_load_ttf_font("YARDSALE.TTF", 30, 0);   //font
    ALLEGRO_FONT* font2 = al_load_ttf_font("YARDSALE.TTF", 10, 0);   //font
    ALLEGRO_SAMPLE* suono = al_load_sample("ding2.mp3");            //suono
    ALLEGRO_SAMPLE* suono2 = al_load_sample("ding.mp3");            //suono

    ALLEGRO_EVENT event;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    //variabili per la chiusura del programma
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
            start = true;
            break;

        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= KEY_RELEASED;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        // controlla se si vuole chiedere il programma tramite tasto 'ESC' o la 'X' che chiede i programmi
        if (done)
            break;
        //inizio del gioco
        if (start) {
            //movimento del rettangolo automatico

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
            //destra
            if (xc < w - raggio && rimL == true) {
                xc++;
            }
            else {
                //suona
                if (xc == w - raggio) {
                    al_play_sample(suono, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                }
                rimL = false;
            }

            //sinistra
            if (xc > 0 + raggio && rimL == false) {
                xc--;
            }
            else {
                //suona
                if (xc == 0 + raggio) {
                    al_play_sample(suono, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                rimL = true;
            }
            //Rimbalzo verticale

            //giu
            if (yc < h - 20 - raggio && rimV == false) {
                yc++;
            }

            else if (xc >= xr1 && xc <= xr2) {
                //suona
                if (yc == h - 20 - raggio) {
                    al_play_sample(suono2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                rimV = true;
            }
            //su
            if (yc > 0 + 20 + raggio && rimV == true) {
                yc--;
            }
            else if (xc >= xrA1 && xc <= xrA2) {
                //suona
                if (yc == 0 + 20 + raggio) {
                    al_play_sample(suono2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                rimV = false;
            }



            // conteggio del puntegio
            if (yc == h - 20 - raggio && xc >= xr1 && xc <= xr2)
                score++;

            // reseta tutto alle impostazioni iniziali trane il Record che viene agiornato
            if (yc == h - 20 - raggio && (xc <= xr1 || xc >= xr2)) {
                resetRetA(w, h, xrA1, xrA2, yrA1, yrA2);
                resetRet(w, h, xr1, xr2, yr1, yr2);
                resetPala(w, h, xc, yc, rimV, rimL);
                resetScoreSaveRecord(score, record);
                start = false;
            }
        }
       
           

        al_clear_to_color(al_map_rgb(220, 220, 220));

        if (start) {
            al_draw_textf(font2, al_map_rgb(50, 50, 50), 0, 20, 0, "Score: %d    Record: %d", score, record);
        }
        else {
            al_draw_textf(font, al_map_rgb(50, 50, 50), w / 2, h / 2 + 50, ALLEGRO_ALIGN_CENTER, "Record: %d", record);
        }

        al_draw_filled_rectangle(xrA1, yrA1, xrA2, yrA2, al_map_rgb(0, 0, 0));    //rettangolo computer

        al_draw_filled_rectangle(xr1, yr1, xr2, yr2, al_map_rgb(0, 0, 0));  //rettangolo giocatore
        al_draw_filled_circle(xc, yc, raggio, al_map_rgb(0, 0, 0));         //palina

        al_flip_display();
        
        
    }

    return 0;
}

void resetRetA(int w, int h, int &xrA1, int& xrA2, int& yrA1, int& yrA2) {
    //variabili posizione rettangolo automatico
    xrA1 = w / 2 - 50, yrA1 = 10;
    xrA2 = w / 2 + 50, yrA2 = 20;
    return;
}

void resetRet(int w, int h, int& xr1, int& xr2, int& yr1, int& yr2) {

    //variabili posizione rettangolo
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
    rimL = false;      //rimbalzo laterale
    
    return;
}

void resetScoreSaveRecord(int& s, int& r) {
    if (s > r)
        r = s;
    s = 0;
    return;
}


