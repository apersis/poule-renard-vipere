// gcc -o main main.c function.c -lSDL2
// ./main

#include <SDL2/SDL.h>
#include <unistd.h>
#include "function.h"
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 920
#define FPS 60
#define NBR_EMOJI 15 //le nombre doit etre divisible par 3
#define TAILLE_EMOJI 30

void drawEmoji();
void initEmoji();
void movementEmoji();
void rebondBord();
void rebondAutreEmoji();

//position du carre dessiné dans drawGame()
int x = 0;
int y = 0;

struct Emoji{
    int x;
    int y;
    int speed_x;
    int speed_y;
    int type;
};

struct Emoji emoji[NBR_EMOJI];

void drawGame(){
    /* Ici je dessine mon jeu
     * exemple position x, y modifiés dans KeyPressed() et utilisés pour
     * pouvoir deplacer la figure à chaque boucle de gameLoop()
     */
    clear();
    movementEmoji();
    rebondAutreEmoji();
    drawEmoji();
    actualize();
    usleep(1000000 / FPS); // 60 images par seconde | 1000000 = 1 seconde
}



void KeyPressed(SDL_Keycode touche){
    /** @brief event.key.keysym.sym renvoi la touche appuyé
     *
     */
    switch (touche) {
        // Voir doc SDL_Keycode pour plus de touches https://wiki.libsdl.org/SDL_Keycode
        // ici exemple ou appuyer sur les touches Q | D modifie x (position du carré dans drawGame())
        case SDLK_q:
            //touche q appuyé
            printf("Q\n");
            x--;
            break;
        case SDLK_d:
            //touche d appuyé
            printf("D\n");
            x++;
            break;
        case SDLK_ESCAPE:
            freeAndTerminate();
            break;
        default:
            break;
    }
}
void joyButtonPressed(){
}

void gameLoop() {
    int programLaunched = 1;
    while (programLaunched == 1) {
        // Boucle pour garder le programme ouvert
        // lorsque programLaunched est different de 1
        // on sort de la boucle, donc de la fonction
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // boucle pour la gestion d'évenement
            switch (event.type) {
                /** event.type renvoi le type d'evenement qui se passe
                 * (si on appuie sur une touche, clique de souris...)
                 * en fonction du type d'évènement on à alors
                 * différentes information accessibles
                 * voir doc pour plus d'event https://wiki.libsdl.org/SDL_EventType
                 */
                case SDL_QUIT:
                    // quand on clique sur fermer la fénêtre en haut à droite
                    programLaunched = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    /* clique de la souris
                     * event.motion.y | event.motion.x pour les positions de la souris
                     */
                    printf("position de la souris x : %d , y : %d\n", event.motion.x, event.motion.y);
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break;
                case SDL_JOYBUTTONDOWN:
                    break;
                default:
                    break;
            }
        }
        /* coder ici pour que le code s'execute après chaque évenement
         * exemple dessiner un carré avec position int x, int y qu'on peut
         * deplacer lorsqu'on appuie sur une touche
         */
        drawGame();
    }
}

int main(){
    /** @description 3 fonctions dans le main qui permettent de créer l'application et la maintenir ouverte :
     *  init(...) : initialiser la SDL/ fenêtre
     *  gameLoop() : boucle de jeu dans laquelle l'application reste ouverte
     *  freeAndTerminate() : quitte le programme proprement
     */
    srand(time(NULL));
    init(WINDOW_WIDTH, WINDOW_HEIGHT);
    initEmoji();
    gameLoop();
    printf("Fin du programme\n");
    freeAndTerminate();
}

void drawEmoji(){
    for(int i=0;i<NBR_EMOJI;i++){
        if(emoji[i].type==0){
            sprite(emoji[i].x,emoji[i].y,"./renard.bmp");
        }else if(emoji[i].type==1){
            sprite(emoji[i].x,emoji[i].y,"./poule.bmp");
        }else if (emoji[i].type==2){
            sprite(emoji[i].x,emoji[i].y,"./vipere.bmp");
        }
        //drawSquare(emoji[i].x,emoji[i].y,TAILLE_EMOJI);
        
    }
}

void initEmoji(){
    for (int i=0;i<NBR_EMOJI;i++){
        emoji[i].x = rand()%(WINDOW_WIDTH-TAILLE_EMOJI-1)+1;
        emoji[i].y = rand()%(WINDOW_HEIGHT-TAILLE_EMOJI-1)+1;
        emoji[i].speed_x = (rand()%2)*2-1;
        emoji[i].speed_y = (rand()%2)*2-1;
        emoji[i].type = i%3;
        
    }
}

void movementEmoji(){
    rebondBord();
    for (int i=0;i<NBR_EMOJI;i++){
        emoji[i].x = emoji[i].x + emoji[i].speed_x;
        emoji[i].y = emoji[i].y + emoji[i].speed_y; 
    }
}

void rebondBord(){
    for (int i=0;i<NBR_EMOJI;i++){
        if (emoji[i].x <= 0){
            emoji[i].speed_x = -emoji[i].speed_x;
        }
        if (emoji[i].x >= WINDOW_WIDTH-TAILLE_EMOJI){
            emoji[i].speed_x = -emoji[i].speed_x;
        }
        if (emoji[i].y <= 0){
            emoji[i].speed_y = -emoji[i].speed_y;
        }
        if (emoji[i].y >= WINDOW_HEIGHT-TAILLE_EMOJI){
            emoji[i].speed_y = -emoji[i].speed_y;
        }
    }
}

void rebondAutreEmoji(){
    for (int i=0;i<NBR_EMOJI;i++){
        int rebondi=0;
        for (int j=0;j<NBR_EMOJI;j++){
            //if (rebondi==0){
            if (i!=j){
                if (emoji[i].x <= emoji[j].x && emoji[i].x+TAILLE_EMOJI >= emoji[j].x){
                    int diff_x = emoji[i].x - emoji[j].x;
                    if (emoji[i].y <= emoji[j].y && emoji[i].y+TAILLE_EMOJI >= emoji[j].y || emoji[j].y <= emoji[i].y && emoji[j].y+TAILLE_EMOJI >= emoji[i].y){
                        rebondi = 1;
                        int diff_y = emoji[i].y-emoji[j].y;
                        if (diff_x == -TAILLE_EMOJI || diff_x == -TAILLE_EMOJI+1 || diff_x == TAILLE_EMOJI-1 || diff_x == TAILLE_EMOJI){           
                            int temp = emoji[i].speed_x;
                            emoji[i].speed_x = emoji[j].speed_x;
                            emoji[j].speed_x = temp;
                        }
                        if (diff_y == -TAILLE_EMOJI || diff_y == -TAILLE_EMOJI+1 || diff_y == TAILLE_EMOJI-1 || diff_y == TAILLE_EMOJI){
                            int temp = emoji[i].speed_y;
                            emoji[i].speed_y = emoji[j].speed_y;
                            emoji[j].speed_y = temp;
                        }
                        //Le type 1 mange le 2 qui mange le 0 qui mange le 1
                        if (emoji[i].type==emoji[j].type){
                            //aucun type ne change
                        }else if (emoji[i].type == 1 && emoji[j].type==2 || emoji[i].type == 2 && emoji[j].type==0 || emoji[i].type == 0 && emoji[j].type==1){
                            emoji[j].type = emoji[i].type;
                        }else if (emoji[i].type == 1 && emoji[j].type==0 || emoji[i].type == 2 && emoji[j].type==1 || emoji[i].type == 0 && emoji[j].type==2){
                            emoji[i].type = emoji[j].type;
                        }
                    }
                }
            }
        }
    }
}