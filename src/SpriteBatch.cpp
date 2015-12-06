#include "SpriteBatch.h"
#include "GameWindow.h"
#include "global_vars.h"

SpriteBatch::SpriteBatch(SDL_Renderer *renderer)
{
    __renderer = renderer;
    if(TTF_Init() != 0)
    {
        std::cout << "Couldn't init SDL_ttf! " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    const std::string resPath = GameWindow::getResourcePath("") + "gamefont.ttf";

    mainGameFont = TTF_OpenFont(resPath.c_str(), 8);
    if(mainGameFont == nullptr)
    {
        std::cout << "Error loading gamefont.ttf: " << SDL_GetError() << std::endl;
    }
}

SpriteBatch::~SpriteBatch()
{}

SDL_Texture* SpriteBatch::loadTexture(const std::string &file, SDL_Renderer **ren)
{
    //std::cout << "addr_of mainRenderer (loadTexture): " << ren << std::endl;
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(*ren, file.c_str());

    if(texture == NULL)
    {
        std::cout << "Error loading texture: " << SDL_GetError() << std::endl;
    }

    return texture;
}

SDL_Texture* SpriteBatch::loadTexture(const std::string &file)
{
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(__renderer, file.c_str());

    if(texture == NULL)
    {
        std::cout << "Error loading texture: " << SDL_GetError() << std::endl;
    }
    return texture;
}

void SpriteBatch::sbBegin()
{
    if(drawingInProgress)
        throw "sbBegin already called!";
    SDL_RenderClear(__renderer);
    drawingInProgress = true;
}

void SpriteBatch::sbSetRenderTarget(SDL_Texture *target)
{
    if(target == nullptr)
        SDL_SetRenderTarget(__renderer, nullptr);
    else
        SDL_SetRenderTarget(__renderer, target);
}

void SpriteBatch::sbSetMainGameCamera(Camera2d *cam)
{
    mainGameCamera = cam;
}

void SpriteBatch::sbDrawFont(std::string msg, int x, int y, SDL_Color color, float scale, bool _upper)
{
    std::transform(msg.begin(), msg.end(), msg.begin(), _upper == true ? ::toupper : ::tolower);

    SDL_Texture *textToRender = drawFontToTexture(msg, color);
    //sbDrawTexture(textToRender, x, y);
    sbDrawTextureScaledConstant(textToRender, x, y, scale);

    SDL_DestroyTexture(textToRender);
}

SDL_Texture *SpriteBatch::drawFontToTexture(std::string msg, SDL_Color color)
{
    if(msg == "")
        msg = " ";


    SDL_Surface *surface = TTF_RenderText_Blended(mainGameFont, msg.c_str(), color);

    if(surface == nullptr)
    {
        std::cout << "Error rendering font: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *txt = SDL_CreateTextureFromSurface(__renderer, surface);
    if(txt == nullptr)
    {
        std::cout << "Error creating texture from surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_FreeSurface(surface);
    return txt;
}

void SpriteBatch::sbDrawTexture(SDL_Texture *tex, int x, int y)
{
    if(!drawingInProgress)
        throw "sbBegin must be called.";

    SDL_Rect dst;
    if(mainGameCamera == nullptr)
    {
        dst.x = x;
        dst.y = y;
    }
    else
    {
        dst.x = x + mainGameCamera->getCameraX();
        dst.y = x + mainGameCamera->getCameraY();
    }

    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(__renderer, tex, NULL, &dst);
}

void SpriteBatch::sbDrawTextureConstant(SDL_Texture *tex, int x, int y)
{
    if(!drawingInProgress)
        throw "sbBegin must be called.";

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(__renderer, tex, NULL, &dst);
}

void SpriteBatch::sbDrawTextureArea(SDL_Texture *tex, int x, int y, SDL_Rect area)
{
    if(!drawingInProgress)
        throw "sbBegin must be called.";
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = area.w;
    dst.h = area.h;
    //SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(__renderer, tex, &area, &dst);
}

/**
These draw relative to the camera
*/
void SpriteBatch::sbDrawTextureScaled(SDL_Texture *tex, int x, int y, int w, int h)
{
    if(!drawingInProgress)
        throw "sbBegin must be called.";

    SDL_Rect dst;
    if(mainGameCamera == NULL)
    {
        dst.x = x;
        dst.y = y;
    }
    else
    {
        dst.x = x + mainGameCamera->getCameraX();
        dst.y = y + mainGameCamera->getCameraY();
    }

    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

    dst.w = w;
    dst.h = h;

    SDL_RenderCopy(__renderer, tex, NULL, &dst);
}

void SpriteBatch::sbDrawTextureAreaScaled(SDL_Texture *tex, int x, int y, SDL_Rect area, float scale)
{
    if(!drawingInProgress)
        throw "sbBegin must be called.";
    SDL_Rect dst;
    if(mainGameCamera == NULL)
    {
        dst.x = x;
        dst.y = y;
    }
    else
    {
        dst.x = x + mainGameCamera->getCameraX();
        dst.y = y + mainGameCamera->getCameraY();
    }

    dst.w = area.w * scale;
    dst.h = area.h * scale;
    //SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(__renderer, tex, &area, &dst);
}

void SpriteBatch::sbDrawTextureScaled(SDL_Texture *tex, int x, int y, float scale)
{
    if(!drawingInProgress)
        throw "sbBegin must be called.";

    SDL_Rect dst;
    if(mainGameCamera == NULL)
    {
        dst.x = x;
        dst.y = y;
    }
    else
    {
        dst.x = x + mainGameCamera->getCameraX();
        dst.y = y + mainGameCamera->getCameraY();
    }

    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

    dst.w = int(float(dst.w) * scale);
    dst.h = int(float(dst.h) * scale);

    SDL_RenderCopy(__renderer, tex, NULL, &dst);
}
/**
End Camera Dependent
*/

/**
These functions basically don't give a crap where the camera is and will draw based on the screen instead.
*/
void SpriteBatch::sbDrawTextureScaledConstant(SDL_Texture *tex, int x, int y, float scale)
{
     if(!drawingInProgress)
        throw "sbBegin must be called.";

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

    dst.w = int(float(dst.w) * scale);
    dst.h = int(float(dst.h) * scale);

    SDL_RenderCopy(__renderer, tex, NULL, &dst);
}

void SpriteBatch::sbDrawTextureScaledConstant(SDL_Texture *tex, int x, int y, int w, int h)
{
    if(!drawingInProgress)
        throw "sbBegin must be called.";

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

    dst.w = w;
    dst.h = h;

    SDL_RenderCopy(__renderer, tex, NULL, &dst);
}
/**
End Constant Function
*/
#include "global_vars.h"
void SpriteBatch::sbFillScreen(SDL_Color *color)
{
    if(!drawingInProgress)
        throw "sbBegin never called!";

    SDL_Color oldColor;

    SDL_GetRenderDrawColor(__renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
    SDL_SetRenderDrawColor(__renderer, color->r, color->g, color->g, color->b);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 800;
    rect.h = 600;
    SDL_RenderFillRect(__renderer, &rect);
    SDL_RenderDrawRect(__renderer, &rect);
    SDL_SetRenderDrawColor(__renderer, (oldColor.r), (oldColor.g), (oldColor.b), (oldColor.a));
}

void SpriteBatch::sbEnd()
{
    if(!drawingInProgress)
        throw "sbBegin never called!";
    SDL_RenderPresent(__renderer);
    drawingInProgress = false;
}
