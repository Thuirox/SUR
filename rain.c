#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include <unistd.h>

typedef struct Drop Drop;
struct Drop{
    double x;
    double y;
    double length;
    double width;
    double velocityY;
    double groundLimit;
    int hit;
};

typedef struct Splash Splash;
struct Splash{
    double x;
    double y;
    double radius;
    double maxRadius;
    Color colorExt;
    Color colorInt;
    double coefSize;
    double coefPerspective;
    double velocityDispertion;
    int run;
};

void newDrop(Drop *drops, int index);
void newSplash(Splash *Splashs, Drop drop, int index);

const double groundHeight = 100;
Color groundColor;

int main(int argc, char * argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Rain");

    SetTargetFPS(30);               // Set our game to run at 60 frames-per-second
    HideCursor();
    //--------------------------------------------------------------------------------------

    Drop *drops = (Drop *)malloc(sizeof(Drop));
    newDrop(drops, 0);

    int lengthDrops = 1;

    Drop drop;

    const int nbDrops = 100;

    int firstDropHitGround = 0;

    Splash *splashs = (Splash *)malloc(sizeof(Splash));
    Splash splash;

    // Initialize random
    srand(time(NULL));

    // Initialize Visual elemens
    const int sunX = screenWidth/2;
    const int sunY = screenHeight-groundHeight;
    const int sunHeight = 100;

    const int fontSize = 200;
    const int textY = 200;
    int textSize = 0;
    time_t rawtime;
    struct tm * timeinfo;
    char* clockText;

    rawtime = time(NULL);
    timeinfo = localtime(&rawtime);
    clockText = TextFormat("%02i:%02i", timeinfo->tm_hour, timeinfo->tm_min);

    Color skyColor = CLITERAL(Color){ 255, 148, 71, 255 };
    groundColor = skyColor;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(!firstDropHitGround || lengthDrops < nbDrops){
            drops = (Drop *)realloc(drops, (lengthDrops+1)*sizeof(Drop));
            newDrop(drops, lengthDrops);
            lengthDrops++;
            splashs = (Splash *)realloc(splashs, (lengthDrops)*sizeof(Splash));
        }

        // Compute current time
        rawtime = time(NULL);
        timeinfo = localtime(&rawtime);
        clockText = TextFormat("%02i:%02i", timeinfo->tm_hour, timeinfo->tm_min);
        textSize = MeasureText(clockText, fontSize);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(skyColor);

            // Draw sun
            DrawCircleGradient(sunX, sunY, screenHeight, CLITERAL(Color){ 255, 255, 150, 150 }, CLITERAL(Color){ 255, 255, 150, 0 });
            DrawCircleGradient(sunX, sunY, sunHeight+10, CLITERAL(Color){ 255, 255, 150, 150 }, CLITERAL(Color){ 255, 255, 150, 120 });
            DrawCircle(sunX, sunY, sunHeight, CLITERAL(Color){ 255, 255, 150, 150});

            // Draw ground
            DrawRectangle(0, screenHeight-groundHeight, screenWidth, groundHeight, groundColor);
            DrawRectangleGradientV(0, screenHeight-groundHeight, screenWidth, 5, CLITERAL(Color){ 0, 0, 0, 50 }, CLITERAL(Color){ 0, 0, 0, 0 });

            // Draw time
            DrawText(clockText, screenWidth/2 - textSize/2, textY, fontSize, CLITERAL(Color){ 0, 0, 0, 50 });

            for(int i=0; i < lengthDrops; i++){
                splash = splashs[i];

                if(splash.run){
                    DrawEllipse(
                        splash.x,
                        splash.y,
                        splash.radius,
                        splash.radius/splash.coefPerspective,
                        splash.colorExt);

                    DrawEllipse(
                        splash.x,
                        splash.y,
                        (int)(splash.radius / splash.coefSize),
                        (int)((splash.radius/splash.coefPerspective)/splash.coefSize),
                        splash.colorInt);

                    splashs[i].radius += splash.velocityDispertion;
                    if(splashs[i].radius > splash.maxRadius){
                        splashs[i].run = 0;
                    }
                }
            }

            for(int i=0; i < lengthDrops; i++){
                drop = drops[i];

                if(drop.length <= 0){
                    newDrop(drops, i);
                    firstDropHitGround = 1;
                } else {
                    DrawRectangle((int)drop.x, (int)drop.y, (int)drop.width, (int)(drop.length), CLITERAL(Color){ 0, 0, 0, 50 });
                    drops[i].y = drop.y + drop.velocityY;
                    if(drop.y + drop.length > (screenHeight-groundHeight)+drop.groundLimit){
                        drops[i].length = (screenHeight-groundHeight+drop.groundLimit)-drop.y;

                        if(!drops[i].hit){
                            drops[i].hit = 1;
                            newSplash(splashs, drops[i], i);
                        }
                    }
                }
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}



void newDrop(Drop *drops, int index){
    double dropZ = (rand()%(int)(groundHeight-50))+10;
    drops[index].x = rand()%1280;
    double coef = (((groundHeight+dropZ)/groundHeight)+0.6);

    drops[index].length = ((rand()%5))*coef+30;
    drops[index].y = -drops[index].length;
    drops[index].width = 1*coef;
    drops[index].velocityY = ((rand()%150)/100.0+2)*2*2;
    drops[index].groundLimit = dropZ;
    drops[index].hit = 0;
}



void newSplash(Splash *splashs, Drop drop, int index){
    splashs[index].x = drop.x + (drop.width/2);
    splashs[index].y = drop.y + drop.length;
    splashs[index].radius = (int)(drop.length*0.1);
    splashs[index].maxRadius = drop.length;
    splashs[index].colorExt = CLITERAL(Color){ 0, 0, 0, 20 };
    splashs[index].colorInt = groundColor;
    splashs[index].coefSize = 1.5;
    splashs[index].coefPerspective = 5;
    splashs[index].velocityDispertion = drop.velocityY/8;
    splashs[index].run = 1;
}