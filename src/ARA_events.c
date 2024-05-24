#include "ARA_events.h"


/**
 * @brief Displays the control instructions in the bottom window of the game.
 * @param game Pointer to the current game.
 */
void print_bottom_window(Game *game){
    mvwprintw(game->window.bottom,1,3,"    +---+");
    mvwprintw(game->window.bottom,2,3,"    | z |");
    mvwprintw(game->window.bottom,3,3,"+---+---+---+");
    mvwprintw(game->window.bottom,4,3,"| q | s | d |");
    mvwprintw(game->window.bottom,5,3,"+---+---+---+");
    mvwprintw(game->window.bottom,7,2,"m : Menu");
    game->timer.update(&game->timer);
    cprintf(game->window.bottom, 2, 6, BASE_CRS_COLOR_BRIGHT_RED, "Temps : %ld",game->timer.get(&game->timer));
}

/**
 * @brief Displays a task with a border made of emoji characters.
 *
 * This function draws a rectangular border using emoji characters.
 * 
 * @param game Pointer to the current game.
 */
void task_pop_up(Game *game, char * mission, char * emoji, int* x1, int* y1, int* x2, int* y2){
    int y=0;
    int x=0;
    *x1=(NB_COLS / 3)-22;
    *y1=(NB_LINES / 3)-7;
    *x2=(NB_COLS / 3)+23;
    *y2=(NB_LINES / 3)+7;
    //Il faut que x1 soit egale a (NB_COLS / 3)-k avec k pair comme ca ca ne bug pas avec l'affichage d une emote sur une autre
    char message[100];
    for(y=*y1; y<*y2; y++){
        for(x=*x1; x<*x2; x+=2){
            if(y==*y1 || y==*y2-1 || x==*x1 || x==*x2-1){
                snprintf(message, sizeof(message), emoji);
            }
            else{
                snprintf(message, sizeof(message), "  ");
            }
            mvwaddstr(game->window.top,y,x,message);
        }
    }
    //Comme mvwaddstr ne prend que 4 parametres je ne peux pas faire (....,"%s",mission) c est pour ca que snprintf formate ma chaine pour etre utilisee sans "%s"
    snprintf(message, sizeof(message), "%s",mission);
    mvwaddstr(game->window.top,*y1+1,*x1+(*x2-*x1)/2 - strlen(message)/2 +1,message);
}

void re_print_all(Game *game, int timeout){
    game->window.clear_all(&game->window);
    game->window.create(&game->window);
    wtimeout(game->window.main_window,timeout);
    print_bottom_window(game);
    print_right_window(game);
    print_map(game);
}

//🕹
int task_recalibrate(Game *game){
    int x1, x2, y1, y2;
    char buffer[100];
    int bar_x;
    int input = 0;
    int fail = -1;
    Direction dir = D_WEST;
    re_print_all(game, TASK_TIMOUT);
    task_pop_up(game, "Recalibrer le vaisssaux", "⚙ ", &x1, &y1, &x2, &y2);
    bar_x = x1+5;
    do
    {   
        re_print_all(game, TASK_TIMOUT/4);
        task_pop_up(game, "Recalibrer le vaisssaux", "⚙ ", &x1, &y1, &x2, &y2);
        if (dir == D_WEST)
        {
            bar_x++;
        } else {
            bar_x --;
        }
        if (bar_x >= x2-1)
        {
            dir = D_EAST;
        } 
        if (bar_x <= x1+1) {
            dir = D_WEST;
        }
    
        for (int y = y1+4; y < y2-3; y++)
        {
            for (int x = x1+2; x < x2-1; x++)
            {   
                if ( is_in(x, x1 + (x2-x1)/2 - 2, x1 + (x2-x1)/2 + 2))
                {   
                    if (bar_x == x)
                    {
                        cprintadd(game->window.top, x ,y ,FONT_CRS_COLOR_BRIGHT_RED,"█");
                    } else {
                        cprintadd(game->window.top, x ,y ,FONT_CRS_COLOR_BRIGHT_RED," ");
                    }
                    
                    
                } else if ( is_in(x, x1 + (x2-x1)/2 - 6, x1 + (x2-x1)/2 + 6 )){
                    if (bar_x == x)
                    {
                        cprintadd(game->window.top, x ,y ,FONT_CRS_COLOR_BRIGHT_YELLOW,"█");
                    } else {
                        cprintadd(game->window.top, x ,y ,FONT_CRS_COLOR_BRIGHT_YELLOW," ");
                    }
        
                } else {
                    if (bar_x == x)
                    {
                        cprintadd(game->window.top, x ,y ,FONT_CRS_COLOR_BRIGHT_GREEN,"█");
                    } else {
                        cprintadd(game->window.top, x ,y ,FONT_CRS_COLOR_BRIGHT_GREEN," ");
                    }
                }
                
                
            }
        }
        game->window.update_key(&game->window);
        input = game->window.get_key(&game->window);
        if (is_in(bar_x, x1 + (x2-x1)/2 - 2, x1 + (x2-x1)/2 + 2) && input == ' ')
        {
            fail = 0;
        } else if (input == ' ') {
            re_print_all(game, TASK_TIMOUT);
            task_pop_up(game, "Recalibrer le vaisssaux", "⚙ ", &x1, &y1, &x2, &y2);
            bar_x = x1+1;
            input = 0;
            dir = D_WEST;
            for (int y = y1+4; y < y2-3; y++)
        {
            for (int x = x1+2; x < x2-1; x++){   
                if ( is_in(x, x1 + (x2-x1)/2 - 2, x1 + (x2-x1)/2 + 2)){   
                    cprintadd(game->window.top, x ,y ,FONT_CRS_COLOR_BRIGHT_RED," ");
                } else if ( is_in(x, x1 + (x2-x1)/2 - 6, x1 + (x2-x1)/2 + 6 )){
                    cprintadd(game->window.top, x ,y ,FONT_CRS_COLOR_BRIGHT_YELLOW," ");
                } else {
                    cprintadd(game->window.top, x ,y ,FONT_CRS_COLOR_BRIGHT_GREEN," ");
                    }
                }
            }
        
            snprintf(buffer, sizeof(buffer), "█▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█");
            mvwaddstr(game->window.top,y1+5,x1+11,buffer);
            snprintf(buffer, sizeof(buffer), "█  █▀▀▀  █▀▀█ ▀█▀  █    █");
            mvwaddstr(game->window.top,y1+6,x1+11,buffer);
            snprintf(buffer, sizeof(buffer), "█  █▀▀▀  █▄▄█  █   █    █");
            mvwaddstr(game->window.top,y1+7,x1+11,buffer);
            snprintf(buffer, sizeof(buffer), "█  █     █  █ ▄█▄  █▄▄█ █");
            mvwaddstr(game->window.top,y1+8,x1+11,buffer);
            snprintf(buffer, sizeof(buffer), "█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█");
            mvwaddstr(game->window.top,y1+9,x1+11,buffer);
            wtimeout(game->window.main_window, MAIN_TIMEOUT);
            game->window.update_key(&game->window);
            input = game->window.get_key(&game->window);
        }   
    } while (fail != 0);


}

/**
 * @brief Task of filling a tank
 * Spam the SPACE button to fill the tank
 * @param game The current game
 */
void task_fill(Game *game){
    int x, y, x1, x2, y1, y2, posx1, posx2, posy1, posy2, count=0, k=0;
    struct timespec current;
    //C est pour avoir un temps en millisecondes
    long actual=0,final=0;
    //Pour stocker le tres grand nombre de millisecondes
    char carvalue=0;
    char buffer[100];
    do{
        if(count<1){
            count=0;
            //Pour eviter des valeurs negatives
        }
        re_print_all(game, TASK_TIMOUT);
        task_pop_up(game, "Remplir le reservoir", "🕸 ", &x1, &y1, &x2, &y2);
        posy1=y1+3;
        posy2=y2-2;
        posx1=x1+17;
        posx2=x2-18;
        //Ici je dessine le conteneur
        snprintf(buffer, sizeof(buffer), "🟥🟥🟥🟥");
        mvwaddstr(game->window.top,posy1,posx1-8,buffer);
        mvwaddstr(game->window.top,posy1,posx2,buffer);
        for(y=posy1+1;y<posy2;y++){
            snprintf(buffer, sizeof(buffer), "🟥");
            mvwaddstr(game->window.top,y,posx1-2,buffer);
            mvwaddstr(game->window.top,y,posx2,buffer);
        }
        snprintf(buffer, sizeof(buffer), "🟥🟥🟥🟥🟥🟥🟥");
        mvwaddstr(game->window.top,posy2,posx1-2,buffer);

        //Ici je remplis le conteneur par ligne
        k=count-count%5;
        //Comme ca on a que des valeurs divisibles par 5 pour montrer que les lignes
        for(x=0;x<2*k;x+=2){
            /*snprintf(buffer, sizeof(buffer), "💧");
            mvwaddstr(game->window.top,posy2-1,x % (posx2-posx1) + posx1,buffer);*/
            cprintadd(game->window.top, x % (posx2-posx1) + posx1, posy2-1, FONT_CRS_COLOR_BRIGHT_YELLOW,"  ");
            if(x % (posx2-posx1) + posx1==posx2-2){
                posy2--;
            }
        }
        game->window.update(&game->window);
        clock_gettime(CLOCK_REALTIME ,&current);
        //Il faut include <unistd.h> et include <time.h> 
        actual=current.tv_sec*1000+current.tv_nsec/1000000;
        //Ca transforme tout en millisecondes
        game->window.update_key(&game->window);
        carvalue=game->window.get_key(&game->window);
        clock_gettime(CLOCK_REALTIME,&current);
        final=current.tv_sec*1000+current.tv_nsec/1000000;
        //Comme ca avec la difference on regarde en combien de temps le joueur a tape une touche pour eviter que le joueur reste appuye sur une touche
        if(carvalue!=' ' || final-actual<50){
            count--;
        }
        else{
            count++;
        }
    }while(count<41);
}

/**
 * @brief Print the arena of the QTE game between the player and the monster
 * 
 * @param game Pointer to the Game structure containing game state and window information.
 * @param car The player input caracter
 */
void print_arena(Game *game, char car){
    int y=0,x=0,y1=(NB_LINES / 3)-5,y2=(NB_LINES / 3)+5,x1=(NB_COLS / 3)-22,x2=(NB_COLS / 3)+23;
    //Il faut que x1 soit egale a (NB_COLS / 3)-k avec k pair comme ca ca ne bug pas avec l'affichage d une emote sur une autre
    char fight[]="C'est l'heure du du-du-du-du-duel";
    char message[100];
    for(y=y1;y<y2;y++){
        for(x=x1;x<x2;x+=2){
            if(y==y1 || y==y2-1 || x==x1 || x==x2-1){
                snprintf(message, sizeof(message), "⚔ ");
            }
            else{
                snprintf(message, sizeof(message), "  ");
            }
            mvwaddstr(game->window.top,y,x,message);
        }
    }
    snprintf(message, sizeof(message), "%s",fight);
    //Comme mvwaddstr ne prend que 4 parametres je ne peux pas faire (....,"%s",mission) c est pour ca que snprintf formate ma chaine pour etre utilisee sans "%s"
    mvwaddstr(game->window.top,y1+1,x1+6,message);
    snprintf(message, sizeof(message), "🤖");
    mvwaddstr(game->window.top,y2-3,x1+3,message);
    snprintf(message, sizeof(message), "👾");
    mvwaddstr(game->window.top,y1+3,x2-4,message);
    if(car>=0){
        print_alphabet(game,car,(y1+y2)/2,(x1+x2)/2);
    }
    else{
        //Si car<0 c est un signal qui montre que je suis dans l'etape ou j attends que l'utilisateur tape espace
        print_alphabet(game,' ',(y1+y2)/2,(x1+x2)/2);
    }
    //A regarder le premier printArena dans le premier do while dans QTE pour comprendre pq je fais if car>0 
}

/**
 * @brief Executes a Quick Time Event (QTE) in the game.
 *
 * This function runs a QTE where the player must type random characters displayed on the screen 
 * within a certain time limit. If the player types the correct characters in the time limit, 
 * they pass the QTE; otherwise, they fail.
 *
 * @param game Pointer to the Game structure containing game state and window information.
 * @return true if the QTE is successfully completed.
 * @return false if not.
 */
int QTE(Game *game){
    char carvalue=0,car=0;
    int i=0;
    //Le premier do while c est en attendant que le joueur tape espace
    do{
        re_print_all(game, MAIN_TIMEOUT);
        print_arena(game,-1);
        game->window.update(&game->window);
        game->window.update_key(&game->window);
    }while(game->window.get_key(&game->window)!=' ');
    //Là le joueur a tape espace et il commence a vraiment jouer le QTE
    for(i=0;i<6;i++){
        carvalue=randint('a','z'+1);
        re_print_all(game, MAIN_TIMEOUT *2);
        print_arena(game,carvalue);          
        game->window.update(&game->window);
        game->window.update_key(&game->window);
        car=game->window.get_key(&game->window);
        if((car!=carvalue)||(car==ERR)){
            //Si le joueur perd on detruit tout
            game->window.destroy();
            game->map.destroy(&game->map);
            return 0;
        }
    }
    //Si le programme arrive ici ca veut dire que le joueur a reussi de gagner au QTE
    game->map.set(&game->map,game->player.get_x(&game->player),game->player.get_y(&game->player),MAP_ROOM);
    //Je mets du vide a la place du monstre
    return 1;
}

/**
 * @brief Gives the direction of the monster to get next to the player
 * 
 * @param game The current game
 * @param xmonster The x of the monster to move
 * @param ymonster the y of the monster to move
 */
void movementMonster(Game *game, int xmonster, int ymonster){
    int x = game->player.get_x(&game->player), y = game->player.get_y(&game->player);
    //A ne pas enlever la repetition du game->map.set(....,MAP_ROOM) sinon ca enleve le monstre meme si il ne peut pas bouger 
    if(x==xmonster){
        if(y<ymonster && game->map.get(&game->map,xmonster,ymonster-1)==MAP_ROOM){
            game->map.set(&game->map,xmonster,ymonster-1,MAP_MONSTER);
            game->map.set(&game->map,xmonster,ymonster,MAP_ROOM);
        }
        else if(y>ymonster && game->map.get(&game->map,xmonster,ymonster+1)==MAP_ROOM){
            game->map.set(&game->map,xmonster,ymonster+1,MAP_MONSTER);
            game->map.set(&game->map,xmonster,ymonster,MAP_ROOM);
        }
    }
    else{
        if(x<xmonster && game->map.get(&game->map,xmonster-1,ymonster)==MAP_ROOM){
            game->map.set(&game->map,xmonster-1,ymonster,MAP_MONSTER);
            game->map.set(&game->map,xmonster,ymonster,MAP_ROOM);
        }
        else if(x>xmonster && game->map.get(&game->map,xmonster+1,ymonster)==MAP_ROOM){
            game->map.set(&game->map,xmonster+1,ymonster,MAP_MONSTER);
            game->map.set(&game->map,xmonster,ymonster,MAP_ROOM);
        }
    }
}

/**
 * @brief Moves the monster next to the player
 * 
 * @param game The current game
 */
void monster(Game *game){
    int x=0,y=0,x1=0,y1=0,x2=0,y2=0,posx=game->player.get_x(&game->player),posy=game->player.get_y(&game->player);
    get_actual_room(game,posx,posy,&x1,&x2,&y1,&y2);
    for(x=x1;x<x2;x++){
        for(y=y1;y<y2;y++){
            if(game->map.get(&game->map,x,y)==MAP_MONSTER){
                movementMonster(game,x,y);
                return ;
                //Je return ; comme ca je ne retombe pas sur le meme monstre une autre fois
            }
        }
    }
}
