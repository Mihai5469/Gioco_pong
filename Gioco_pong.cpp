// Gioco_pong.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
// 16/03/2023

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

#define KEY_SEEN     1
#define KEY_RELEASED 2

int main()
{
    int w = 640, h = 860;  //variabili che indicano la dimensione della finestra

    //variabili posizione retangolo
    int xr1 = 400, yr1 = 850;
    int xr2 = 500, yr2 = 840;

    //variabili palina
    int xc = w / 2, yc = h / 2;
    int raggio = 10;


    al_init();
    al_init_primitives_addon();

    al_install_keyboard();


    //variabili Allegro5

    ALLEGRO_DISPLAY* display = al_create_display(w, h);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 244.0);

    ALLEGRO_EVENT event;


    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));



    bool done = false;
    bool redraw = true;

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    al_start_timer(timer);
    while (true) {

        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            
            if (key[ALLEGRO_KEY_LEFT]) {
                xr1--;
                xr2--;
            }
               
            if (key[ALLEGRO_KEY_RIGHT]) {
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

        al_clear_to_color(al_map_rgb(220, 220, 220));

      
        al_draw_filled_rectangle(xr1, yr1, xr2, yr2, al_map_rgb(0, 0, 0));
        al_draw_filled_circle(xc, yc, raggio, al_map_rgb(0, 0, 0));

        al_flip_display();

    }



    return 0;
}

