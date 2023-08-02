#include "GridLogic.hpp"

/*
  Constructor initialiazing grid and shape grid  
*/
GridLogic::GridLogic(const int R, const int C)
{

    ROWS = R;
    COLS = C;

    grid = new int[R * C];
    shape_grid = new int[SHAPE_COLS * SHAPE_ROWS];

    initGrid();
    initShapeGrid();
    makeShapeI();

    // the required rectangles for printing the grid
    srcRect = {25, 0, 33, 33};
    moverRect = {0, 0, 25, 25};

    shapeBaseRect = {baseX + int(C / 2) * 25, 0, 25, 25};

    groundY = baseY + (R * 25) - 125; // subtraction 125 because shape size is 125*125
}

/*
  Function initializing shape grid - with values of zero.  
*/
void GridLogic::initShapeGrid()
{
    for (int i = 0; i < SHAPE_ROWS; i++)
    {
        for (int j = 0; j < SHAPE_COLS; j++)
        {
            *(shape_grid + i * SHAPE_COLS + j) = 0;
        }
    }
    shapeBaseRect = {baseX + int(COLS / 2) * 25, 0, 25, 25};
}

/*
    Function initialiazing grid with values of zero
*/
void GridLogic::initGrid()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            *(grid + i * COLS + j) = 0;
        }
    }
}

/*
    Drawing the grid on the main screen. where value is 1 a block is formed.
*/
void GridLogic::drawGrid(SDL_Renderer *gRenderer, SDL_Texture *assets)
{

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            int value = *(grid + i * COLS + j);

            if (value == 1)
            {
                moverRect.y = baseY + (25 * i);
                moverRect.x = baseX + (25 * j);
                SDL_RenderCopy(gRenderer, assets, &srcRect, &moverRect);
            }
        }
    }
}

/*
    Function drawing the single block that is falling on the main screen
*/

void GridLogic::drawShapeGrid(SDL_Renderer *gRenderer, SDL_Texture *assets)
{
    for (int i = 0; i < SHAPE_ROWS; i++)
    {
        for (int j = 0; j < SHAPE_COLS; j++)
        {
            int value = *(shape_grid + i * SHAPE_COLS + j);

            if (value == 1)
            {
                moverRect.y = shapeBaseRect.y + (25 * i); // baseY + (25 * i);
                moverRect.x = shapeBaseRect.x + (25 * j);
                SDL_RenderCopy(gRenderer, assets, &srcRect, &moverRect);
            }
        }
    }
}

/*
  Function moving the shape down in the matrix by adjusting the zeros and ones.   
*/
bool GridLogic::moveShapeDown()
{
    int shapeBottomIndex = 0; // the index of the first one from the bottom side

    for (int i = SHAPE_ROWS * SHAPE_COLS - 1; i >= 0; i--)
    {
        if (*(shape_grid + i) == 1)
        {
            shapeBottomIndex = i;
            break;
        }
    }

    int shapeBottomPixels = 25 * (4 - int(shapeBottomIndex / 5));

    if ((shapeBaseRect.y + 25) <= (groundY + shapeBottomPixels) && !isCollidingDown())
    {
        shapeBaseRect.y += 25;
        return true;
    }
    else
    {
        // updating the grid
        updateGrid();
        initShapeGrid();
        // can make different objects here
        selectShape();
        return false;
    }
}

/*
    Function randomly selects different shapes which are falling on the main screen. 
*/
void GridLogic::selectShape()
{
    srand((unsigned)time(0));
    int secretNumber = rand() % 7;

    switch (secretNumber)
    {
    case 0:
        makeShapeI();
        break;
    case 1:
        makeShapeT();
        break;
    case 2:
        makeShapeJ();
        break;
    case 3:
        makeShapeL();
        break;
    case 4:
        makeShapeO();
        break;
    case 5:
        makeShapeS();
        break;
    case 6:
        makeShapeZ();
        break;
    }
}

/*
  Grid is continously upgrading grid-- checking collision and making it part of the grid itself.  
*/

void GridLogic::updateGrid()
{
    // writing the shape to grid
    int shapeCoord = 0;
    for (int i = ((shapeBaseRect.y - baseY) / 25); i < ((shapeBaseRect.y - baseY) / 25) + SHAPE_ROWS; i++)
    {
        for (int j = ((shapeBaseRect.x - baseX) / 25); j < ((shapeBaseRect.x - baseX) / 25) + SHAPE_COLS; j++)
        {
            // int gridValue = *(grid + i * COLS + j);

            int shapeValue = *(shape_grid + shapeCoord);
            shapeCoord++;

            if (shapeValue == 1)
                *(grid + i * COLS + j) = 1;
        }
    }

    // checing if any row in the grid is full
    bool flag = false;
    for (int i = 0; i < ROWS; i++)
    {
        flag = true;
        for (int j = 0; j < COLS; j++)
        {
            flag = *(grid + i * COLS + j);
            if (!flag)
            {
                break;
            }
        }
        if (flag)
        {
            removeGridRow(i);
            i--;
        }
    }
}

/*
  Function used to eliminate a row incase the entire row is filled with blocks.   
*/
void GridLogic::removeGridRow(int rowIndex)
{
    static int prevJindex;
    for (int i = rowIndex; i >= 0; i--)
    {
        for (int j = 0; j < COLS; j++)
        {
            prevJindex = (i - 1) * COLS + j;
            *(grid + i * COLS + j) = (prevJindex >= 0) * *(grid + (i - 1) * COLS + j);
        }
    }
}

/*
  Checks whether block is colliding with any other block as it is falling.  
*/

bool GridLogic::isCollidingDown()
{
    int shapeCoord = 0;
    for (int i = ((shapeBaseRect.y - baseY) / 25); i < ((shapeBaseRect.y - baseY) / 25) + SHAPE_ROWS; i++)
    {
        for (int j = ((shapeBaseRect.x - baseX) / 25); j < ((shapeBaseRect.x - baseX) / 25) + SHAPE_COLS; j++)
        {
            int shapeValue = *(shape_grid + shapeCoord);
            shapeCoord++;

            int gridValue = *(grid + (i + 1) * COLS + j);

            if (shapeValue == 1 && shapeValue == gridValue)
            {
                return true;
            }
        }
    }
    return false;
}

/*
  Function checks collision with blocks while moving to the right based on 1's in that direction.   
*/
bool GridLogic::isCollidingRight()
{
    int shapeCoord = 0;
    for (int i = ((shapeBaseRect.y - baseY) / 25); i < ((shapeBaseRect.y - baseY) / 25) + SHAPE_ROWS; i++)
    {
        for (int j = ((shapeBaseRect.x - baseX) / 25); j < ((shapeBaseRect.x - baseX) / 25) + SHAPE_COLS; j++)
        {
            int shapeValue = *(shape_grid + shapeCoord);
            shapeCoord++;

            if (shapeValue == 1 && j == COLS - 1) // if the shape is already at the right most corner
            {
                return true;
            }
            // cout << *(grid + (i * COLS) + j) << " ";
            int gridValue = *(grid + i * COLS + (j + 1));

            if (shapeValue == 1 && shapeValue == gridValue)
            {
                return true;
            }
        }
    }
    return false;
}
/*
  Function checks collision with blocks while moving to the right based on 1's in that direction.   
*/
bool GridLogic::isCollidingLeft()
{
    int shapeCoord = 0;
    for (int i = ((shapeBaseRect.y - baseY) / 25); i < ((shapeBaseRect.y - baseY) / 25) + SHAPE_ROWS; i++)
    {
        for (int j = ((shapeBaseRect.x - baseX) / 25); j < ((shapeBaseRect.x - baseX) / 25) + SHAPE_COLS; j++)
        {
            int shapeValue = *(shape_grid + shapeCoord);
            shapeCoord++;

            if (shapeValue == 1 && j == 0) // if the shape is already at the left most corner
            {
                return true;
            }

            int gridValue = *(grid + i * COLS + (j - 1));

            if (shapeValue == 1 && shapeValue == gridValue)
            {
                return true;
            }
        }
    }
    return false;
}
/*
  Function makes shape I in a 5*5 matrix by plaing in a single line
*/
void GridLogic::makeShapeI()
{
    for (int i = 0; i < SHAPE_ROWS; i++)
    {
        for (int j = 0; j < SHAPE_COLS; j++)
        {
            *(shape_grid + i * SHAPE_COLS + j) = (j == 2);
        }
    }
}
/*
  Function makes shape T in a 5*5 matrix by plaing in a single line and a 3 blocks on the top
*/
void GridLogic::makeShapeT()
{
    for (int i = 0; i < SHAPE_ROWS; i++)
    {
        for (int j = 0; j < SHAPE_COLS; j++)
        {
            *(shape_grid + i * SHAPE_COLS + j) = ((i == 1 && j == 2) || (i == 2 && (j == 1 || j == 2 || j == 3)));
        }
    }
}

/*
  Function makes shape J by reflecting shape L
*/
void GridLogic::makeShapeJ()
{
    for (int i = 0; i < SHAPE_ROWS; i++)
    {
        for (int j = 0; j < SHAPE_COLS; j++)
        {
            *(shape_grid + i * SHAPE_COLS + j) = ((i == 1 && j == 1) || (i == 2 && (j == 1 || j == 2 || j == 3)));
        }
    }
}
/*
  Function makes shape L in a 5*5 matrix by plaing 1s in a vertically and horizontally
*/
void GridLogic::makeShapeL()
{
    for (int i = 0; i < SHAPE_ROWS; i++)
    {
        for (int j = 0; j < SHAPE_COLS; j++)
        {
            *(shape_grid + i * SHAPE_COLS + j) = ((i == 1 && j == 3) || (i == 2 && (j == 1 || j == 2 || j == 3)));
        }
    }
}

/*
  Function makes shape O in a 5*5 matrix by 1 placing in a box
*/
void GridLogic::makeShapeO()
{
    for (int i = 0; i < SHAPE_ROWS; i++)
    {
        for (int j = 0; j < SHAPE_COLS; j++)
        {
            *(shape_grid + i * SHAPE_COLS + j) = ((i == 1 || i == 2) && (j == 1 || j == 2));
        }
    }
}

/*
  Function makes shape S in a 5*5 matrix.
*/
void GridLogic::makeShapeS()
{
    for (int i = 0; i < SHAPE_ROWS; i++)
    {
        for (int j = 0; j < SHAPE_COLS; j++)
        {
            *(shape_grid + i * SHAPE_COLS + j) = (((i == 1 || i == 2) && j == 2) || (i == 1 && j == 3) || (i == 2 && j == 1));
        }
    }
}
/*
  Function makes shape Z in a 5*5 matrix.
*/
void GridLogic::makeShapeZ()
{
    for (int i = 0; i < SHAPE_ROWS; i++)
    {
        for (int j = 0; j < SHAPE_COLS; j++)
        {
            *(shape_grid + i * SHAPE_COLS + j) = (((i == 1 || i == 2) && j == 2) || (i == 1 && j == 1) || (i == 2 && j == 3));
        }
    }
}

/*
  Function roates all shapes 90 degrees by adjusting ones and zeros in the matrix
*/
void GridLogic::rotateShape()
{
    int *temp = new int[SHAPE_COLS * SHAPE_ROWS];
    for (int blockRowArrow = 0; blockRowArrow < SHAPE_ROWS; blockRowArrow++)
    {
        for (int blockColArrow = 0; blockColArrow < SHAPE_COLS; blockColArrow++)
        {
            temp[(blockColArrow * SHAPE_COLS) + 4 - blockRowArrow] = shape_grid[blockRowArrow * SHAPE_COLS + blockColArrow];
        }
    }
    delete shape_grid;
    shape_grid = temp;
}
/*
  Function moves shape towards the left when left key is pressed on the keyboard. 
*/
void GridLogic::moveShapeLeft()
{
    // here check first if it collides to the left side
    if (!isCollidingLeft())
    {
        shapeBaseRect.x -= 25;
    }
}
/*
  Function moves shape towards the right when right key is pressed on the keyboard. 
*/
void GridLogic::moveShapeRight()
{
    // check first if it collide to the right side
    if (!isCollidingRight())
    {
        shapeBaseRect.x += 25;
    }
}
