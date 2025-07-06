#include "iGraphics.h"
#include "iSound.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>    // Required for std::string
#include <vector>    // Required for std::vector
#include <algorithm> // Required for std::sort
#include <fstream>   // Required for file operations
#include <iostream>  // For debugging output (optional)

// To use std::string and std::vector without std:: prefix
using namespace std;

// --- High Score Structures and Variables ---
struct PlayerScore {
    string name;
    int score;

    // Operator for sorting (descending order of score)
    bool operator<(const PlayerScore& other) const {
        return score > other.score;
    }
};

vector<PlayerScore> highScores;
const string HIGHSCORE_FILE = "highscores.txt";
const int MAX_HIGHSCORES = 5; // To store top 5 scores

char playerName[50] = "";
int playerNameIndex = 0;
bool inputName = false; // Flag to indicate if name input is active

// --- END High Score Structures and Variables ---


double x1=0,y11=0;
char hab[10][13]={"hab001_1.png","hab002_1.png","hab003_1.png","hab004_1.png","hab005_1.png","hab006_1.png","hab007_1.png","hab008_1.png","hab009_1.png","hab010_1.png"};
char obs[3][10]={"obs1.png","obs2.png","obs3.png"};
char cloud[7][13]={"cloud1.png","cloud2.png","cloud3.png","cloud4.png","cloud5.png","cloud6.png","cloud7.png"};
char menu[5][15]={"menu_1.png","menu_2.png","menu_3.png","menu_4.png","menu_5.png"};
char menuBig[5][20] = {"menu_1_big.png","menu_2_big.png","menu_3_big.png","menu_4_big.png","menu_5_big.png"};
bool hoverMenu[5] = {false};
int menuX[5] = {100, 300, 20, 185, 350}; // Play, Instructions, Exit, About Us, High Score (formerly Credits)
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
int gamestate=0; // 0: Main Menu, 1: Game Play, 2: Instructions, 3: Input Name, 5: High Score, 6: Game Over, 7: About Us
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
    "            ~ END OF MESSAGE ~"
};
int aboutUsTotalLines = sizeof(aboutUsTextLines) / sizeof(aboutUsTextLines[0]);
float aboutUsScrollY = -30 * aboutUsTotalLines; // Initial Y for the text to start off-screen below
float aboutUsScrollSpeed = 0.5; // Speed of text scrolling
bool aboutUsScrolling = false; // Flag to start/stop text scrolling
// --- END NEW VARIABLES ---

// --- High Score Functions ---
void loadHighScores() {
    highScores.clear();
    ifstream file(HIGHSCORE_FILE);
    if (file.is_open()) {
        string name;
        int score;
        while (file >> name >> score) {
            highScores.push_back({name, score});
        }
        file.close();
        sort(highScores.begin(), highScores.end()); // Sort in descending order
        cout << "High scores loaded." << endl; // Debugging
    } else {
        cout << "High score file not found or could not be opened. A new one will be created." << endl; // Debugging
    }
}

void saveHighScores() {
    ofstream file(HIGHSCORE_FILE);
    if (file.is_open()) {
        // Ensure only MAX_HIGHSCORES are saved
        for (int i = 0; i < min((int)highScores.size(), MAX_HIGHSCORES); ++i) {
            file << highScores[i].name << " " << highScores[i].score << endl;
        }
        file.close();
        cout << "High scores saved." << endl; // Debugging
    } else {
        cout << "Could not open high score file for saving." << endl; // Debugging
    }
}

void addHighScore(string name, int score) {
    // Replace spaces in name with underscores for file saving
    for (char &c : name) {
        if (c == ' ') {
            c = '_';
        }
    }

    highScores.push_back({name, score});
    sort(highScores.begin(), highScores.end()); // Sort after adding new score
    // Keep only the top MAX_HIGHSCORES
    if (highScores.size() > MAX_HIGHSCORES) {
        highScores.resize(MAX_HIGHSCORES);
    }
    saveHighScores(); // Save the updated high scores immediately
    cout << "Added high score: " << name << " " << score << endl; // Debugging
}
// --- END High Score Functions ---


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

    if(gamestate==0) // Main Menu
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
    else if(gamestate==3) // Input Name
    {
        iSetColor(255, 255, 255);
        iText(100, 400, "Enter Your Name:", GLUT_BITMAP_HELVETICA_18);
        iText(100, 370, playerName, GLUT_BITMAP_HELVETICA_18);
        iText(100, 300, "Press ENTER to start the game.", GLUT_BITMAP_HELVETICA_18);
    }
    else if(gamestate==1) // Game Play
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
                        // Add score to high scores
                        addHighScore(playerName, finalScore);
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
    else if (gamestate == 2) // Instructions
    {
        iText(150, 600, "INSTRUCTIONS", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(50, 500, "Use LEFT and RIGHT arrow keys to move the balloon.", GLUT_BITMAP_HELVETICA_18);
        iText(50, 470, "Avoid obstacles to gain score.", GLUT_BITMAP_HELVETICA_18);
        iText(50, 440, "You have 3 health. Colliding with obstacles reduces health.", GLUT_BITMAP_HELVETICA_18);
        iText(50, 410, "The obstacle speed continuously increases as your score rises!", GLUT_BITMAP_HELVETICA_18);
        iText(50, 380, "Game ends when health reaches 0.", GLUT_BITMAP_HELVETICA_18);
        iText(50, 300, "Press 'M' to return to Main Menu", GLUT_BITMAP_HELVETICA_18);
    }
    else if (gamestate == 5) // High Scores (formerly Credits)
    {
        iText(180, 600, "HIGH SCORES", GLUT_BITMAP_TIMES_ROMAN_24);
        int startY = 500;
        int rank = 1;
        if (highScores.empty()) {
            iText(100, 500, "No high scores yet. Play a game!", GLUT_BITMAP_HELVETICA_18);
        } else {
            for (const auto& ps : highScores) {
                // Replace underscores in name with spaces for display
                string displayName = ps.name;
                for (char &c : displayName) {
                    if (c == '_') {
                        c = ' ';
                    }
                }
                char scoreText[100];
                sprintf(scoreText, "%d. %s: %d", rank++, displayName.c_str(), ps.score);
                iText(100, startY, scoreText, GLUT_BITMAP_HELVETICA_18);
                startY -= 30;
            }
        }
        iText(50, 300, "Press 'M' to return to Main Menu", GLUT_BITMAP_HELVETICA_18);
    }
    else if (gamestate == 6) // Game Over
    {
        iText(180, 450, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
        char finalScoreStr[50];
        sprintf(finalScoreStr, "Final Score: %d", finalScore);
        iText(180, 400, finalScoreStr, GLUT_BITMAP_HELVETICA_18);
        iText(100, 300, "Press 'R' to Restart or 'Q' to Quit", GLUT_BITMAP_HELVETICA_18);
    }
    else if (gamestate == 7) // About Us
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
            // Play Button
            if(mx >= menuX[0] && mx <= menuX[0] + menuW && my >= menuY[0] && my <= menuY[0] + menuH)
            {
                gamestate = 3; // Go to name input
                inputName = true;
                playerName[0] = '\0'; // Clear previous name
                playerNameIndex = 0;
            }
            // Instructions Button
            else if(mx >= menuX[1] && mx <= menuX[1] + menuW && my >= menuY[1] && my <= menuY[1] + menuH)
            {
                gamestate = 2;
            }
            // Exit Button
            else if(mx >= menuX[2] && mx <= menuX[2] + menuW && my >= menuY[2] && my <= menuY[2] + menuH)
            {
                saveHighScores(); // Save scores before exiting
                exit(0);
            }
            // About Us Button
            else if(mx >= menuX[3] && mx <= menuX[3] + menuW && my >= menuY[3] && my <= menuY[3] + menuH)
            {
                gamestate = 7;
                slideInAboutUs = true;
                slideOutAboutUs = false;
                aboutUsSlideY = -800;
                aboutUsScrolling = false; 
                aboutUsScrollY = -30 * aboutUsTotalLines;
            }
            // High Score Button (formerly Credits)
            else if(mx >= menuX[4] && mx <= menuX[4] + menuW && my >= menuY[4] && my <= menuY[4] + menuH)
            {
                loadHighScores(); // Load scores before displaying
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
    if (gamestate == 3 && inputName) { // Inputting player name
        if (key == '\b') { // Backspace
            if (playerNameIndex > 0) {
                playerNameIndex--;
                playerName[playerNameIndex] = '\0';
            }
        } else if (key == '\r' || key == '\n') { // Enter key
            inputName = false;
            gamestate = 1; // Start game after name input
            easy = iPlaySound("Easy.wav", true, 80);
            // Reset game variables for a new game
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
        } else if (playerNameIndex < sizeof(playerName) - 1) { // Regular character input
            // Allow only alphanumeric characters and spaces for names
            if (isalnum(key) || key == ' ') { 
                playerName[playerNameIndex] = key;
                playerNameIndex++;
                playerName[playerNameIndex] = '\0';
            }
        }
    } else { // Other game states
        switch (key)
        {
        case 'q':
            if (gamestate == 6) {
                saveHighScores(); // Save scores before exiting from Game Over screen
                exit(0);
            }
            break;
        case 'r':
            if (gamestate == 6) {
                gamestate = 0; // Go back to main menu
                // Reset all game variables to initial state
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
                iPauseSound(easy); // Stop game music if it was playing
            }
            break;
        case 'm':
            if (gamestate == 2 || gamestate == 5) {
                gamestate = 0; // Return to Main Menu from Instructions or High Scores
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
}

void iSpecialKeyboard(unsigned char key)
{
    // The `move()` function already handles left/right arrow keys
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    srand(time(0));

    // Load high scores at the start of the program
    loadHighScores(); // <-- IMPORTANT ADDITION

    // Load images for obstacles
    for(int i = 0; i < 3; i++) {
        iLoadImage(&obsimgfrm[i], obs[i]);
    }

    // Initialize the main balloon sprite
    iInitSprite(&habimg[0]);
    iChangeSpriteFrames(&habimg[0], &habimgfrm[0], 1);
    iSetSpritePosition(&habimg[0], x, y);

    // Initialize obstacle sprites and positions
    for (int i = 0; i < 3; i++) {
        iInitSprite(&obsimg[i]);
        obsi[i] = rand() % 3;
        iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);
        obsx[i] = rand() % 300;
        obsy[i] = 800 + i * 267; // Stagger initial obstacle positions
        iSetSpritePosition(&obsimg[i], obsx[i], obsy[i]);
    }

    // Load cloud images
    for (int i = 0; i < 7; i++) {
        iLoadImage(&cloudImg[i], cloud[i]);
    }

    // Load balloon animation frames
    for(int j=0;j<10;j++)
    {
        iLoadImage(&habimgfrm[j],hab[j]);
    }

    // Load menu images (normal and hover states)
    for(int i=0;i<5;i++)
    {
        iLoadImage(&menuImg[i],menu[i]);
        iLoadImage(&menuBigImg[i], menuBig[i]);
    }

    // Load other necessary images
    iLoadImage(&menuImghab, "menuhab.png");
    iLoadImage(&skyImg, "sky.jpg");
    iLoadImage(&heartbreak, "heartbreak.png");
    iLoadImage(&aboutUsBgImg, "aboutusbg.jpg");

    // Set up game timers
    iSetTimer(500,incmnt); // Balloon animation
    iSetTimer(16,cldx);    // Cloud horizontal movement
    iSetTimer(16,cldy);    // Cloud vertical movement
    iSetTimer(10, move);   // Balloon movement (handled by arrow keys and `move` function)
    iSetTimer(16,obstacle); // Obstacle movement and collision detection
    iSetTimer(33,scoreupdate); // Score update and speed increase
    iSetTimer(16, animateAboutUsSlide); // About Us panel animation

    iInitializeSound(); // Initialize sound engine

    iInitialize(500, 800, "Hot Air Balloon"); // Initialize the iGraphics window

    return 0;
}
