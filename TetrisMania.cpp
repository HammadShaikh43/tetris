#include "TetrisMania.hpp"

/*
  Constructor of tettris mania.  
*/
TetrisMania::TetrisMania(SDL_Renderer *gr, SDL_Texture *asset, const int r, const int c) : gRenderer(gr), assets(asset)
{
    gridLogic = new GridLogic(r, c);
}

/*
  Function drawing the object(blocks on the screen)  
*/
bool TetrisMania::drawObjects()
{
    bool parity = gridLogic->moveShapeDown();
    // Drawing all objects here.
    gridLogic->drawGrid(gRenderer, assets);
    gridLogic->drawShapeGrid(gRenderer, assets);
    return parity;
}

/*
  Function drawing main screen buttons(start, exit, images of tetris on main page)  
*/
void TetrisMania::drawMainScreenButtons()
{
    // First drawing Img Tet
    SDL_Rect sRect = {187, 161, 177, 124};
    SDL_Rect mRect = {366, 290, 270, 200};

    SDL_RenderCopy(gRenderer, assets, &sRect, &mRect);

    // Second drawing start Game
    sRect = {16, 231, 109, 48};
    mRect = {400, 726, 200, 80};

    SDL_RenderCopy(gRenderer, assets, &sRect, &mRect);

    // Third drawing the Exit button
    sRect = {31,338, 93, 40};
    mRect = {15, 10, 125, 50};

    SDL_RenderCopy(gRenderer, assets, &sRect, &mRect);
}

void TetrisMania::drawback()
{
    // SDL_Surface *screen = SDL_GetWindowSurface(gWindow);
    // SDL_Surface *back = NULL;
	// back = IMG_Load("back.bmp");
    // SDL_Rect bak = {940, 790, 60, 60};
	// SDL_BlitSurface(back, NULL, screen, &bak);
    // SDL_RenderCopy(gRenderer, assets, &sRect, &mRect);
    // SDL_Rect sRect = {252, 303, 60, 60};
    // SDL_Rect mRect = {940, 790, 60, 60};

    // SDL_RenderCopy(gRenderer, assets, &sRect, &mRect);
}

/*
  Checks whether block has reached the ground.  
*/
bool TetrisMania::isReached()
{
    return false;
}

/*
    Function used to roate the individual blocks.
*/
void TetrisMania::rotateShape()
{
    gridLogic->rotateShape();
    SDL_Delay(25);
}

/*
    Function used to move the block to the left
*/
void TetrisMania::moveeShapeLeft()
{
    gridLogic->moveShapeLeft();
    SDL_Delay(25);
}

/*
    Function used to move the block to the right.
*/
void TetrisMania::moveShapeRight()
{
    gridLogic->moveShapeRight();
    SDL_Delay(25);
}

// Function used tomove block to the bottom and at the same time detect collision.
void TetrisMania::slideShapeDown()
{
    while (gridLogic->moveShapeDown())
    {
    }
}
