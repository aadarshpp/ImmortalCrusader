#include "raylib.h"
#include <string.h>

#include "scene_setup.c"
#include "scene_builder.c"
#include "character_setup.c"
#include "imaginate.c"
#include "intle.c"
#include "maze.c"

int main(void) {
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Immortal Crusade");
    ToggleFullscreen();
    SetTargetFPS(60);
    
    setUpWarrior(940, 520, START);
    setUpGoblin(850, 550, ENEMY_BASE);
    
    setUpBgImages();
    setUpItemImages();
    setUpMainMenu();
    setUpStartScene();
    setUpCI1Scene();
    setUpCI2Scene();
    setUpJN1Scene();
    setUpJN2Scene();
    setUpCD1Scene();
    setUpCD2Scene();
    setUpENEMY_BASE();
    setUpCASTLE();
    setUpQueenRoom();
    
    unsigned int frameNo = 0;
    
    int gameoverFrame = 0;
    int rebirthFrame = 0;
    int ressurectionFrame = 0;
    
    int starting_point_death_initialised = 0;
        
    int endSceneInitialised = 0;
    int deadQueenFrame = 0;
    int queenRessurectionMessageFrame = 0;
    int queenRessurectionFrame = 0;
    int finFrame = 0;
    int inFin = 0;
    int dead = 0;
    
    InitAudioDevice();              // Initialize audio device
    Music man_screamMusic = LoadMusicStream(RESOURCE_PATH "/Audio/man_scream.mp3");
    Music Medival_MusicMusic = LoadMusicStream(RESOURCE_PATH "/Audio/Medival_Music.mp3");
    Music end_scoreMusic = LoadMusicStream(RESOURCE_PATH "/Audio/end_score.mp3");
    int Medival_MusicMusicInitialised = 0;
   
    warrior.sceneName = MAINMENU;
    MainMenuOptions currentMainMenuOption = START_GAME;
    int ressurections = 5;
    int run = 1;
    
    int miniGameDeath = 0;
    int goToMainMenu = 0;
    
    while(run) {
        
        if (IsKeyPressed(KEY_B) || WindowShouldClose() || goToMainMenu) {
            warrior.sceneName = MAINMENU;
            warrior.x = 940; warrior.y = 520;
            warrior.intelSolved = 0;
            warrior.mazeSolved = 0;
            
            miniGameDeath = 0;
            
            goblin.sceneName = ENEMY_BASE;
            goblin.x = 850; goblin.y  =550;
            
            frameNo = 0;
            gameoverFrame = 0;
            
            starting_point_death_initialised = 0;
            
            endSceneInitialised = 0;
            inFin = 0;
            dead = 0;
            
            ressurections = 5;
        }
        
        if(Medival_MusicMusicInitialised==1) {
            if (warrior.sceneName==MAINMENU || warrior.sceneName==RULES || (warrior.sceneName==END && !inFin)) UpdateMusicStream(Medival_MusicMusic);
            else {
                Medival_MusicMusicInitialised = 0;
                StopMusicStream(Medival_MusicMusic);
            }
        }
        
        BeginDrawing();
        
        switch (warrior.sceneName) {
            
            case MAINMENU:
                if(!Medival_MusicMusicInitialised) PlayMusicStream(Medival_MusicMusic);
                Medival_MusicMusicInitialised = 1;
                
                DrawTexture(MAIN_MENU_BG_IMAGES[currentMainMenuOption], 0, 0, WHITE);
                if (IsKeyPressed(KEY_DOWN)) currentMainMenuOption = (currentMainMenuOption+1)%3;
                else if (IsKeyPressed(KEY_UP)) currentMainMenuOption = (currentMainMenuOption+2)%3;
                
                if (IsKeyPressed(KEY_ENTER)) {
                    if (currentMainMenuOption==START_GAME) warrior.sceneName = START;
                    else if (currentMainMenuOption==QUIT_GAME) run = 0;
                    else warrior.sceneName = RULES;
                }
                break;
            case RULES:
                Medival_MusicMusicInitialised = 1;
                DrawTexture(rulesImage, 0, 0, WHITE);
                break;
            
            case START:
                buildScene(&StartScene, &warrior, frameNo, ressurections);
                break;
            case CI1:
                buildScene(&CI1Scene, &warrior, frameNo, ressurections);
                break;
            case CI2:
                buildScene(&CI2Scene, &warrior, frameNo, ressurections);
                break;
            case JN1:
                buildScene(&JN1Scene, &warrior, frameNo, ressurections);
                break;
            case JN2:
                buildScene(&JN2Scene, &warrior, frameNo, ressurections);
                break;
            case CD1:
                buildScene(&CD1Scene, &warrior, frameNo, ressurections);
                break;
            case CD2:
                buildScene(&CD2Scene, &warrior, frameNo, ressurections);
                break;
            case ENEMY_BASE:
                buildScene(&ENEMY_BASEScene, &warrior, frameNo, ressurections);
                break;
            case CASTLE:
                buildScene(&CASTLEScene, &warrior, frameNo, ressurections);
                break;
            case QUEEN_ROOM:
                buildScene(&QueenRoomScene, &warrior, frameNo, ressurections);
                break;
                
            case INTEL:
                int wx_i = warrior.x;
                int wy_i = warrior.y;
                IntelGameState intelGameState = intelMain();
                switch(intelGameState) {
                    case EXIT_GAME_INTEL:
                        goToMainMenu = 1;
                        continue;
                        break;
                    case PREV_SCENE_INTEL:
                        warrior.sceneName = warrior.prevSceneName;
                        warrior.x = wx_i;
                        warrior.y = wy_i;
                        break;
                    case SAWED:
                        miniGameDeath = 1;
                        warrior.sceneName = DEATH;
                        break;
                    case SOLVED:
                        warrior.intelSolved = 1;
                        if (warrior.prevSceneName==JN2) {
                            warrior.x = 1030;
                            warrior.y = 240;
                        } else {
                            warrior.x = 300;
                            warrior.y = 620;
                        }
                        warrior.sceneName = ENEMY_BASE;
                        break;
                    case NO_CHANGE_INTEL:
                        break;
                }
                break;
                
            case MAZE:
                int wx_m = warrior.x;
                int wy_m = warrior.y;
                MazeGameState mazeGameState = mazeMain();
                switch (mazeGameState) {
                    case EXIT_GAME_MAZE:
                        goToMainMenu = 1;
                        continue;
                        break;
                    case BOMBED:
                        miniGameDeath = 1;
                        warrior.sceneName = DEATH;
                        break;
                    case OUT_OF_MAZE:
                        warrior.mazeSolved = 1;
                        warrior.sceneName = CASTLE;
                        warrior.x = 1030;
                        warrior.y = 240;
                        break;
                    case PREV_SCENE_MAZE:
                        warrior.sceneName = warrior.prevSceneName;
                        warrior.x = wx_m;
                        warrior.y = wy_m;
                        break;
                    case NO_CHANGE_MAZE:
                        break;
                }
                break;
                
             case DEATH:
             
                int death_initialised = (!dead);
             
                if (!dead) {
                    ressurections--;
                    dead = 1;
                }
            
                if (death_initialised) {
                    frameNo = 0;
                    gameoverFrame = 0;
                    if (!miniGameDeath){
                        gameoverFrame = frameNo + 18*10;
                        PlayMusicStream(man_screamMusic);
                    }
                    rebirthFrame= gameoverFrame + 18*5;
                    ressurectionFrame= rebirthFrame+18*15;
                } else if ((frameNo<gameoverFrame) && !miniGameDeath) {
                    UpdateMusicStream(man_screamMusic);
                    DrawTexture(BgImages[goblin.sceneName], 0, 0, WHITE);
                    for (int i=0; i<=ressurections; i++) DrawTexture(phoenixFeather, 60 + phoenixFeather.width*i, 10, WHITE);
                    animate(warrior.animationFramesArray[warrior.currentStill], 7, 21, frameNo, warrior.x, warrior.y);
                    animate(goblin.animationFramesArray[ATTACK], 18, 18, frameNo, goblin.x, goblin.y);
                } else if (frameNo<rebirthFrame){
                    StopMusicStream(man_screamMusic);
                    drawTextBox((Rectangle){0, 0, GetMonitorWidth(0), GetMonitorHeight(0)}, "[ YOU DIED ]", 30, BLACK, BLACK, RED);
                } else if (frameNo<ressurectionFrame) {
                    char message[100];
                    Color textColor = BLUE;
                    if (ressurections >= 0) sprintf(message, "[ USING ITEM: PHOENIX FEATHER (%d/5) ]\n\nreconstructing your physical body", ressurections+1);
                    else {
                        sprintf(message, "UNABLE TO USE PHOENIX FEATHER (0/5)");
                        textColor = RED;
                    }
                    drawTextBox((Rectangle){0, 0, GetMonitorWidth(0), GetMonitorHeight(0)}, message, 30, BLACK, BLACK, textColor);
                } else {
                    warrior.sceneName = START;
                    warrior.x = 940; warrior.y = 520;
                    gameoverFrame = 0;
                    frameNo = 0;
                    dead = 0;
                    miniGameDeath = 0;
                    
                    if (ressurections<0 || (abs(goblin.x-warrior.x)<=10 && abs(goblin.y-warrior.y)<=10)) {
                        warrior.sceneName = STARTING_POINT_DEATH_END;
                    }
                }
                
                break;
                
            case STARTING_POINT_DEATH_END:
                if (!starting_point_death_initialised) {
                    frameNo = 0;
                    starting_point_death_initialised = 1;
                }
            
                if (frameNo>120) warrior.sceneName = GAMEOVER;
                else {
                    drawTextBox((Rectangle){0, 0, GetMonitorWidth(0), GetMonitorHeight(0)}, "", 30, BLACK, BLACK, RED);
                    
                    
                    char message[1024] = "";  
                    for (int i = 0; i <= ressurections; i++) {
                        strcat(message, "[ YOU DIED ]\n\n\n");
                    }
                    
                    DrawText(message, GetMonitorWidth(0)/2-60, 540/(ressurections), 30, RED);
                }
                break;
            
            case GAMEOVER:
                drawTextBox((Rectangle){0, 0, GetMonitorWidth(0), GetMonitorHeight(0)}, "[ GAMEOVER ]", 30, BLACK, BLACK, (Color){139, 0, 0, 255});
                break;
                
            case END:
            
                if (!inFin) {
                    if (!Medival_MusicMusicInitialised) PlayMusicStream(Medival_MusicMusic);
                    Medival_MusicMusicInitialised = 1;
                }
            
                if(!endSceneInitialised) {
                    endSceneInitialised = 1;
              
                    frameNo = 0;
                    deadQueenFrame = frameNo + 4*60;
                    queenRessurectionMessageFrame = deadQueenFrame + 4*60;
                    queenRessurectionFrame = queenRessurectionMessageFrame + 4*60;
                    finFrame = queenRessurectionFrame+ 5*60;
                } else if (frameNo<deadQueenFrame) {
                    drawTextBox((Rectangle){0, 0, GetMonitorWidth(0), GetMonitorHeight(0)}, "[ Beheld Her Majesty ]", 30, BLACK, BLACK, (Color){153, 50, 204, 255});
                } else if (frameNo<queenRessurectionMessageFrame) {
                    DrawTexture(deadQueenImage, 0, 0, WHITE);
                } else if (frameNo<queenRessurectionFrame) {
                    char message[100];
                    if (ressurections > 0) sprintf(message, "[ USING ITEM: PHOENIX FEATHER (%d/5) ]\n\nressurecting the queen", ressurections);
                    else sprintf(message, "[ UNABLE TO RESSURECT THE QUEEN ]\n\nUNABLE TO USE PHOENIX FEATHER (0/5)");
                    drawTextBox((Rectangle){0, 0, GetMonitorWidth(0), GetMonitorHeight(0)}, message, 30, BLACK, BLACK, BLUE);
                } else if (frameNo<finFrame) {
                    if (ressurections==0) {
                        warrior.sceneName = GAMEOVER;
                    } else {
                        DrawTexture(ressurectionQueenImage, 0, 0, WHITE);
                    }
                } else {
                    
                    if (!inFin) PlayMusicStream(end_scoreMusic);
                    else UpdateMusicStream(end_scoreMusic);
                    inFin = 1; 
                    DrawTexture(finImage, 0, 0, WHITE);
                }
                
                break;
        }
        
        frameNo++;
        goToMainMenu = 0;
        
        EndDrawing();
    }
    
    unloadBgImages();
    unloadItemImages();
    
    unloadCharacterTextures(&warrior);
    unloadCharacterTextures(&goblin);
    freeWarrior();
    freeGoblin();
    
    CloseWindow();
    return 0;
}
