#include "iGraphics.h"
#include "iSound.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string> // Required for std::string
#include <vector> // Required for std::vector (optional, but good for dynamic arrays)

// To use std::string and std::vector without std:: prefix
using namespace std;

double x1=0,y11=0;
char hab[10][13]={"hab001_1.png","hab002_1.png","hab003_1.png","hab004_1.png","hab005_1.png","hab006_1.png","hab007_1.png","hab008_1.png","hab009_1.png","hab010_1.png"};
char obs[3][10]={"obs1.png","obs2.png","obs3.png"};
char cloud[7][13]={"cloud1.png","cloud2.png","cloud3.png","cloud4.png","cloud5.png","cloud6.png","cloud7.png"};
char menu[5][15]={"menu_1.png","menu_2.png","menu_3.png","menu_4.png","menu_5.png"};
char menuBig[5][20] = {"menu_1_big.png","menu_2_big.png","menu_3_big.png","menu_4_big.png","menu_5_big.png"};
bool hoverMenu[5] = {false};
int menuX[5] = {100, 300, 20, 185, 350};
int menuY[5] = {300, 300, 50, 50, 50};
int menuW = 120, menuH = 160;
Image menuBigImg[5];
Image cloudImg[7];
Image skyImg;
Image aboutUsBgImg; // Declare a new Image variable for the About Us background
Image heartbreak;
Image menuImg[5];
Image menuImghab;
bool showHeartbreak = false;
int heartbreakCounter = 0;
Sprite habimg[10];
Sprite obsimg[3];
Image habimgfrm[10];
Image obsimgfrm[3];
int gamestate=0;
int score=0,scoretick=0;
int health = 3;
int bi=0;
int obsx[3],obsy[3],obsi[3];
int x=200,y=70;
int easy,wrong;
int finalScore = 0;

float obstacleSpeed = 2.0;

int aboutUsSlideY = -800; // Panel vertical position
bool slideInAboutUs = false; // Controls panel sliding in
bool slideOutAboutUs = false; // Controls panel sliding out
int slideSpeed = 5; // Speed of the panel slide animation

// --- NEW VARIABLES for About Us Text Scrolling ---
string aboutUsTextLines[] = {
    "~ ABOUT THE GAME ~",
    "",
    "This game is created by:",
    "- Dibya Saha",
    "- Himel Chowdhury",
    "",
    "The theme of the game is to save the balloon from obstacles",
    "to carry the game on.",
    "Thus the people sitting in the balloon get saved from the",
    "destruction of the world in 2169.",
    "",
    "Keep trying to save the people from the danger!",
    "",
    "",
    "                           ~ END OF MESSAGE ~"
};
int aboutUsTotalLines = sizeof(aboutUsTextLines) / sizeof(aboutUsTextLines[0]);
float aboutUsScrollY = -30 * aboutUsTotalLines; // Initial Y for the text to start off-screen below
float aboutUsScrollSpeed = 0.5; // Speed of text scrolling
bool aboutUsScrolling = false; // Flag to start/stop text scrolling
// --- END NEW VARIABLES ---

void scoreupdate()
{
    scoretick++;
    if(scoretick%9==0) {
      score++;
      // The speed will now increase much more rapidly with the score.
      obstacleSpeed = 2.0 + (score * 0.025); 
    }
}

void move()
{
    const int BALLOON_WIDTH = 80;
    const int MIN_X = 0;
    const int MAX_X = 500 - BALLOON_WIDTH;

    if(isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
        x += 10;
        if (x > MAX_X) {
            x = MAX_X;
        }
    }
    if(isSpecialKeyPressed(GLUT_KEY_LEFT)) {
        x -= 10;
        if (x < MIN_X) {
            x = MIN_X;
        }
    }

    iSetSpritePosition(&habimg[bi], x, y);
}

double c1x=100,c2x=150,c3x=200,c4x=400,c5x=250,c6x=320,c7x=425;
double c1y=114,c2y=229,c3y=343,c4y=457,c5y=500,c6y=686,c7y=800;
double yy=800;

bool isObsActive[3] = {true, true, true};

void obstacle()
{
    for(int i=0;i<3;i++)
    {
        if(isObsActive[i]){
            obsy[i]-=obstacleSpeed;
            if(obsy[i]<-50)
            {
                obsy[i]=800;
                obsx[i]=rand()%300;
                obsi[i]=rand()%3;
                iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);
            }
            iSetSpritePosition(&obsimg[i], obsx[i], obsy[i]);
        } else {
            obsy[i] -= obstacleSpeed;
            if (obsy[i] < -50)
            {
                isObsActive[i] = true;
                obsy[i] = 800;
                obsx[i] = rand() % 300;
                obsi[i] = rand() % 3;
                iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);
            }
            iSetSpritePosition(&obsimg[i], obsx[i], obsy[i]);
        }
    }
}

void incmnt()
{
    bi++;
    if(bi==10)
      bi=0;
    iChangeSpriteFrames(&habimg[bi], &habimgfrm[bi], 1);
    iSetSpritePosition(&habimg[bi], x, y);
}

void cldx()
{
    c1x+=.2;
    if(c1x>490)c1x=-150;
    c2x-=.1;
    if(c2x<-100)c2x=550;
    c3x-=.3;
    if(c3x<-100)c3x=550;
    c4x+=.5;
    if(c4x>490)c4x=-150;
    c5x+=.25;
    if(c5x>490)c5x=-150;
    c6x-=.3;
    if(c6x<-100)c6x=550;
    c7x+=.8;
    if(c7x>490)c7x=-150;
}

void cldy()
{
    c1y-=2;
    if(c1y<-50)c1y=800;
    c2y-=2;
    if(c2y<-50)c2y=800;
    c3y-=2;
    if(c3y<-50)c3y=800;
    c4y-=2;
    if(c4y<-50)c4y=800;
    c5y-=2;
    if(c5y<-50)c5y=800;
    c6y-=2;
    if(c6y<-50)c6y=800;
    c7y-=2;
    if(c7y<-50)c7y=800;
}

void animateAboutUsSlide() {
    if (gamestate == 7) {
        if (slideInAboutUs) {
            aboutUsSlideY += slideSpeed;
            if (aboutUsSlideY >= 0) {
                aboutUsSlideY = 0;
                slideInAboutUs = false; // Stop panel slide-in
                aboutUsScrolling = true; // Start text scrolling
                aboutUsScrollY = -30 * aboutUsTotalLines; // Reset text scroll position
            }
        } else if (slideOutAboutUs) {
            aboutUsSlideY -= slideSpeed;
            if (aboutUsSlideY <= -800) {
                aboutUsSlideY = -800;
                slideOutAboutUs = false;
                gamestate = 0; // Return to main menu AFTER panel slides out
                aboutUsScrolling = false; // Stop text scrolling
            }
        } else if (aboutUsScrolling) { // Only scroll text if panel is in and scrolling is enabled
            aboutUsScrollY += aboutUsScrollSpeed;
            // You can add a condition here to stop scrolling if the text goes too far up
        }
    }
}

void iDraw()
{
    iClear();

    iShowLoadedImage(0,0,&skyImg);
    iShowLoadedImage(c1x,c1y,&cloudImg[0]);
    iShowLoadedImage(c2x,c2y,&cloudImg[1]);
    iShowLoadedImage(c3x,c3y,&cloudImg[2]);
    iShowLoadedImage(c4x,c4y,&cloudImg[3]);
    iShowLoadedImage(c5x,c5y,&cloudImg[4]);
    iShowLoadedImage(c6x,c6y,&cloudImg[5]);
    iShowLoadedImage(c7x,c7y,&cloudImg[6]);

    if(gamestate==0)
    {
        iShowLoadedImage(50, 530, &menuImghab);

        for (int i = 0; i < 5; i++)
        {
            if (hoverMenu[i])
            {
                iShowLoadedImage(menuX[i] - 10, menuY[i] - 10, &menuBigImg[i]);
            }
            else
            {
                iShowLoadedImage(menuX[i], menuY[i], &menuImg[i]);
            }
        }
    }
    else if(gamestate==1)
    {
        iShowSprite(&habimg[bi]);
        for(int i=0;i<3;i++)
        {
            if(isObsActive[i])
            {
                iShowSprite(&obsimg[i]);
                if(iCheckCollision(&habimg[bi], &obsimg[i]))
                {
                    showHeartbreak = true;
                    heartbreakCounter = 60;

                    iPauseSound(easy);
                    iPlaySound("wrong.wav", false, 80);
                    iResumeSound(easy);

                    isObsActive[i] = false;
                    health--;

                    if (health <= 0) {
                        gamestate = 6;
                        iPauseSound(easy);
                        finalScore = score;
                    }
                }
            }
        }

        if(showHeartbreak)
        {
            iShowLoadedImage(200, 400, &heartbreak);
            heartbreakCounter--;
            if(heartbreakCounter <= 0)
            {
                showHeartbreak = false;
            }
        }

        char strScore[30];
        sprintf(strScore,"Score: %d",score);
        iText(10,760,strScore,GLUT_BITMAP_HELVETICA_18);

        char strHealth[30];
        sprintf(strHealth,"Health: %d",health);
        iText(10,740,strHealth,GLUT_BITMAP_HELVETICA_18);

        char strSpeed[50];
        sprintf(strSpeed, "Speed: %.2f", obstacleSpeed);
        iText(10, 720, strSpeed, GLUT_BITMAP_HELVETICA_18);
    }
    else if (gamestate == 2)
    {
        iText(150, 600, "INSTRUCTIONS", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(50, 500, "Use LEFT and RIGHT arrow keys to move the balloon.", GLUT_BITMAP_HELVETICA_18);
        iText(50, 470, "Avoid obstacles to gain score.", GLUT_BITMAP_HELVETICA_18);
        iText(50, 440, "You have 3 health. Colliding with obstacles reduces health.", GLUT_BITMAP_HELVETICA_18);
        iText(50, 410, "The obstacle speed continuously increases as your score rises!", GLUT_BITMAP_HELVETICA_18);
        iText(50, 380, "Game ends when health reaches 0.", GLUT_BITMAP_HELVETICA_18);
        iText(50, 300, "Press 'M' to return to Main Menu", GLUT_BITMAP_HELVETICA_18);
    }
    else if (gamestate == 5)
    {
        iText(180, 600, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(50, 500, "Game Design & Development: [Your Name/Team Name]", GLUT_BITMAP_HELVETICA_18);
        iText(50, 470, "Sounds: iSound Library default/custom sounds", GLUT_BITMAP_HELVETICA_18);
        iText(50, 440, "Images: Provided assets", GLUT_BITMAP_HELVETICA_18);
        iText(50, 300, "Press 'M' to return to Main Menu", GLUT_BITMAP_HELVETICA_18);
    }
    else if (gamestate == 6)
    {
        iText(180, 450, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
        char finalScoreStr[50];
        sprintf(finalScoreStr, "Final Score: %d", finalScore);
        iText(180, 400, finalScoreStr, GLUT_BITMAP_HELVETICA_18);
        iText(100, 300, "Press 'R' to Restart or 'Q' to Quit", GLUT_BITMAP_HELVETICA_18);
    }
    else if (gamestate == 7)
    {
        iShowLoadedImage(0, 0, &aboutUsBgImg);

        iSetColor(100, 100, 100);
        iFilledRectangle(50, aboutUsSlideY + 100, 400, 600);

        iSetColor(255, 255, 255);

        int textStartX = 70;
        int textBaseY = aboutUsSlideY + 150;
        int lineHeight = 25;

        for (int i = 0; i < aboutUsTotalLines; i++) {
            float currentLineY = textBaseY + aboutUsScrollY + (aboutUsTotalLines - 1 - i) * lineHeight;
            if (currentLineY > aboutUsSlideY + 100 && currentLineY < aboutUsSlideY + 700) {
                 iText(textStartX, currentLineY, const_cast<char*>(aboutUsTextLines[i].c_str()), GLUT_BITMAP_HELVETICA_18);
            }
        }
        
        iText(100, aboutUsSlideY + 120, "Press 'M' to return to Main Menu", GLUT_BITMAP_HELVETICA_18);
    }
}


void iMouseMove(int mx, int my)
{
    if (gamestate == 0) {
        for (int i = 0; i < 5; i++)
        {
            hoverMenu[i] = (mx >= menuX[i] && mx <= menuX[i] + menuW &&
                              my >= menuY[i] && my <= menuY[i] + menuH);
        }
    }
}


void iMouseDrag(int mx, int my)
{
    // place your codes here
}


void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(gamestate == 0)
        {
            if(mx >= menuX[0] && mx <= menuX[0] + menuW && my >= menuY[0] && my <= menuY[0] + menuH)
            {
                gamestate = 1;
                easy = iPlaySound("Easy.wav", true, 80);
            }
            else if(mx >= menuX[1] && mx <= menuX[1] + menuW && my >= menuY[1] && my <= menuY[1] + menuH)
            {
                gamestate = 2;
            }
            else if(mx >= menuX[2] && mx <= menuX[2] + menuW && my >= menuY[2] && my <= menuY[2] + menuH)
            {
                exit(0);
            }
            else if(mx >= menuX[3] && mx <= menuX[3] + menuW && my >= menuY[3] && my <= menuY[3] + menuH)
            {
                gamestate = 7;
                slideInAboutUs = true;
                slideOutAboutUs = false;
                aboutUsSlideY = -800;
                aboutUsScrolling = false; 
                aboutUsScrollY = -30 * aboutUsTotalLines;
            }
            else if(mx >= menuX[4] && mx <= menuX[4] + menuW && my >= menuY[4] && my <= menuY[4] + menuH)
            {
                gamestate = 5;
            }
        }
    }
}


void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

void iKeyboard(unsigned char key)
{
    switch (key)
    {
    case 'q':
        if (gamestate == 6) {
            exit(0);
        }
        break;
    case 'r':
        if (gamestate == 6) {
            gamestate = 0;
            score = 0;
            scoretick = 0;
            health = 3;
            x = 200;
            y = 70;
            bi = 0;
            iSetSpritePosition(&habimg[bi], x, y);

            obstacleSpeed = 2.0;

            for (int i = 0; i < 3; i++) {
                obsy[i] = 800 + i * 267;
                obsx[i] = rand() % 300;
                obsi[i] = rand() % 3;
                isObsActive[i] = true;
                iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);
                iSetSpritePosition(&obsimg[i], obsx[i], obsy[i]);
            }
            iPauseSound(easy);
        }
        break;
    case 'm':
        if (gamestate == 2 || gamestate == 5) {
            gamestate = 0;
        } else if (gamestate == 7) {
            slideInAboutUs = false;
            slideOutAboutUs = true;
            aboutUsScrolling = false; 
        }
        break;
    default:
        break;
    }
}

void iSpecialKeyboard(unsigned char key)
{
    // The `move()` function already handles left/right arrow keys
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    srand(time(0));

    for(int i = 0; i < 3; i++) {
        iLoadImage(&obsimgfrm[i], obs[i]);
    }

    iInitSprite(&habimg[0]);
    iChangeSpriteFrames(&habimg[0], &habimgfrm[0], 1);
    iSetSpritePosition(&habimg[0], x, y);

    for (int i = 0; i < 3; i++) {
        iInitSprite(&obsimg[i]);
        obsi[i] = rand() % 3;
        iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);
        obsx[i] = rand() % 300;
        obsy[i] = 800 + i * 267;
        iSetSpritePosition(&obsimg[i], obsx[i], obsy[i]);
    }

    for (int i = 0; i < 7; i++) {
        iLoadImage(&cloudImg[i], cloud[i]);
    }

    for(int j=0;j<10;j++)
    {
        iLoadImage(&habimgfrm[j],hab[j]);
    }

    for(int i=0;i<5;i++)
    {
        iLoadImage(&menuImg[i],menu[i]);
        iLoadImage(&menuBigImg[i], menuBig[i]);
    }

    iLoadImage(&menuImghab, "menuhab.png");
    iLoadImage(&skyImg, "sky.jpg");
    iLoadImage(&heartbreak, "heartbreak.png");
    iLoadImage(&aboutUsBgImg, "aboutusbg.jpg");

    iSetTimer(500,incmnt);
    iSetTimer(16,cldx);
    iSetTimer(16,cldy);
    iSetTimer(10, move);
    iSetTimer(16,obstacle);
    iSetTimer(33,scoreupdate);
    iSetTimer(16, animateAboutUsSlide);

    iInitializeSound();

    iInitialize(500, 800, "Hot Air Balloon");

    return 0;
}