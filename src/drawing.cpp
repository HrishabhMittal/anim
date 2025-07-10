#ifndef DRAWING
#define DRAWING
#include "textures.cpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <cstddef>
#include <cstdlib>
#include <ostream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
namespace Window {
    namespace {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Color curcolor;
        std::vector<Texture*> textures;
        std::vector<TTF_Font*> fonts;
        int width,height;
    }
    int mouseX,mouseY; Uint32 buttons;
    std::unordered_map<int,int> keyspressed;
    int running=1;
    int frames=0;
    int offsetx=0,offsety=0;
    void create(const std::string& title,int w,int h) {
        height=h;
        width=w;
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return;
        }
        if (TTF_Init() == -1) {
            std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
            return;
        }
        if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
            std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
            return;
        }
        window = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height,SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            return;
        }
        renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            return;
        }
    }
    SDL_Rect windowDimensions() {
        return {offsetx,offsety,width,height};
    }
    void pollEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type==SDL_QUIT) {
                running=false;
            }
            if (e.type==SDL_KEYDOWN) {
                keyspressed[e.key.keysym.sym]=1;
            } else if (e.type==SDL_KEYUP) {
                keyspressed[e.key.keysym.sym]=0;
            }
        }
        buttons=SDL_GetMouseState(&mouseX, &mouseY);
    }
    void destroy() {
        for (int i=0;i<textures.size();i++) {
            delete textures[i];
        }
        for (int i=0;i<fonts.size();i++) {
            TTF_CloseFont(fonts[i]);
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
    }
    int loadFont(std::string fontPath,int fontSize) {
        fonts.push_back(TTF_OpenFont(fontPath.c_str(),fontSize));
        return fonts.size()-1;
    }
    void QueryTexture(int i,int& w,int& h) {
        w=textures[i]->Width();
        h=textures[i]->Height();
    }
    void drawTexture(int i,SDL_Rect img,SDL_Rect win,bool mirrored = false) {
        SDL_Texture* texture = textures[i]->getTexture();
        win.x+=offsetx;
        win.y+=offsety;
        SDL_RendererFlip flip = mirrored ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer,texture,&img,&win,0,NULL,flip);
    }
    void drawTexture(int i, SDL_Rect win, bool mirrored = false) {
        SDL_Texture* texture = textures[i]->getTexture();
        win.x += offsetx;
        win.y += offsety;
        SDL_Rect img{0, 0, 0, 0};
        SDL_QueryTexture(texture, NULL, NULL, &img.w, &img.h);
        SDL_RendererFlip flip = mirrored ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer, texture, &img, &win, 0, NULL, flip);
    }
    void animate(int i,SDL_Rect loc,int totalframes,bool mirrored,int usingframes=-1,int frameoffset=0) {
        if (usingframes==-1) usingframes=totalframes;
        int f=(frames/15)%usingframes+frameoffset;
        SDL_Texture* texture=textures[i]->getTexture();
        SDL_Rect img{0,0,0,0};
        SDL_QueryTexture(texture,NULL,NULL,&img.w,&img.h);
        drawTexture(i,{f*img.w/totalframes,0,img.w/totalframes,img.h},loc,mirrored);
    }
    void drawBg(int i) {
        SDL_Texture* texture = textures[i]->getTexture();
        SDL_Rect img{0,0,0,0};
        SDL_Rect win{0,0,width,height};
        SDL_QueryTexture(texture,NULL,NULL,&img.w,&img.h);
        SDL_RenderCopy(renderer,texture,&img,&win);
    }
    int Width() {
        return width;
    }
    int Height() {
        return height;
    }
    void setOffsets(int x,int y) {
        offsetx=x;
        offsety=y;
    }
    void setColor(SDL_Color c) {
        SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
        curcolor=c;
    }
    void setColor(Uint8 r,Uint8 g,Uint8 b,Uint8 a) {
        SDL_SetRenderDrawColor(renderer,r,g,b,a);
        curcolor={r,g,b,a};
    }
    void drawPoint(int x,int y) {
        x+=offsetx;y+=offsety;
        SDL_RenderDrawPoint(renderer,x+offsetx,y+offsety);
    }
    void drawLine(int x1,int y1,int x2,int y2) {
        aalineRGBA(renderer,x1+offsetx,y1+offsety,x2+offsetx,y2+offsety,curcolor.r,curcolor.g,curcolor.b,curcolor.a);
    }
    void drawRect(int x,int y,int w,int h) {
        x+=offsetx;y+=offsety;
        SDL_Rect rect = {x,y,w,h};
        SDL_RenderDrawRect(renderer,&rect);
    }
    void fillRect(SDL_Rect r) {
        r.x+=offsetx;r.y+=offsety;
        SDL_RenderFillRect(renderer,&r);
    }
    void fillRect(int x,int y,int w,int h) {
        x+=offsetx;y+=offsety;
        SDL_Rect rect = {x,y,w,h};
        SDL_RenderFillRect(renderer,&rect);
    }
    void fillCircle(int centerX,int centerY,int radius) {
        filledCircleRGBA(renderer,offsetx+centerX,offsety+centerY,radius,curcolor.r,curcolor.g,curcolor.b,curcolor.a);
    }
    void fillRoundedRect(int x, int y, int w, int h, int r) {
        x+=offsetx;y+=offsety;
        SDL_Rect rect = {x + r, y, w - 2 * r, h};
        SDL_RenderFillRect(renderer, &rect);
        SDL_Rect rect2 = {x, y + r, r, h - 2 * r}; 
        SDL_RenderFillRect(renderer, &rect2);
        SDL_Rect rect3 = {x + w - r, y + r, r, h - 2 * r}; 
        SDL_RenderFillRect(renderer, &rect3);
        fillCircle(x + r, y + r, r); 
        fillCircle(x + w - r - 1, y + r, r); 
        fillCircle(x + r, y + h - r - 1, r); 
        fillCircle(x + w - r - 1, y + h - r - 1, r); 
    }
    void drawCircle(int centerX,int centerY,int radius) {
        circleRGBA(renderer,offsetx+centerX,offsety+centerY,radius,curcolor.r,curcolor.g,curcolor.b,curcolor.a);
    }
    void drawArc(int x,int y,int radius,float startAngle,float endAngle) {
        x+=offsetx;y+=offsety;
        float startRad = startAngle * (M_PI / 180.0f);
        float endRad = endAngle * (M_PI / 180.0f);
        buttons=SDL_GetMouseState(&mouseX, &mouseY);
        arcRGBA(renderer,x,y,radius,startRad,endRad,curcolor.r,curcolor.g,curcolor.b,curcolor.a);
    }
    void drawPoly(const std::vector<Sint16>& px, const std::vector<Sint16>& py) {
        int num_points = std::min(px.size(), py.size());
        std::vector<Sint16> x(num_points);
        std::vector<Sint16> y(num_points);
        for (int i = 0; i < num_points; i++) {
            x[i] = px[i] + offsetx;
            y[i] = py[i] + offsety;
        }
        polygonRGBA(renderer, x.data(), y.data(), num_points, curcolor.r, curcolor.g, curcolor.b, curcolor.a);
    }
    void fillPoly(const std::vector<Sint16>& px, const std::vector<Sint16>& py) {
        int num_points = std::min(px.size(), py.size());
        std::vector<Sint16> x(num_points);
        std::vector<Sint16> y(num_points);
        for (int i = 0; i < num_points; i++) {
            x[i] = px[i] + offsetx;
            y[i] = py[i] + offsety;
        }
        filledPolygonRGBA(renderer, x.data(), y.data(), num_points, curcolor.r, curcolor.g, curcolor.b, curcolor.a);
    }
    int loadImageOntoTexture(const std::string& path) {
        Texture* texture = new Texture(IMG_LoadTexture(renderer,path.c_str()),renderer);
        if (!texture->getTexture()) {
            std::cerr << "IMG_LoadTexture Error: " << IMG_GetError() << std::endl;
            return -1;
        }
        textures.push_back(texture);
        return textures.size()-1;
    }
    void drawText(std::string text,int x,int y,int f,int maxWidth,SDL_Color color) {
        x+=offsetx;y+=offsety;
        TTF_Font* font = fonts[f];
        if (!font) {
            std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
            return;
        }
        int textWidth, textHeight;
        if (TTF_SizeText(font, text.c_str(), &textWidth, &textHeight) != 0) {
            std::cerr << "TTF_SizeText Error: " << TTF_GetError() << std::endl;
            return;
        }
        while (textWidth > maxWidth && !text.empty()) {
            text.pop_back();
            TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);
        }
        SDL_Surface* surface = TTF_RenderText_Blended(font,text.c_str(),color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
        SDL_FreeSurface(surface);
        SDL_Rect destRect = {x,y,0,0};
        SDL_QueryTexture(texture,NULL,NULL,&destRect.w,&destRect.h);
        SDL_RenderCopy(renderer,texture,NULL,&destRect);
        SDL_DestroyTexture(texture);
    }
    void drawText(std::string text, int x, int y, const std::string& fontPath, int fontSize, int maxWidth, int maxHeight, SDL_Color color) {
        x += offsetx;
        y += offsety;
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!font) {
            std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
            return;
        }
        std::vector<std::string> lines;
        std::string currentLine;
        int lineHeight = 0;
        int totalHeight = 0;
        for (size_t i = 0; i < text.size(); ++i) {
            currentLine += text[i];
            int textWidth, textHeight;
            if (TTF_SizeText(font, currentLine.c_str(), &textWidth, &textHeight) != 0) {
                std::cerr << "TTF_SizeText Error: " << TTF_GetError() << std::endl;
                TTF_CloseFont(font);
                return;
            }
            lineHeight = textHeight;  
            if (textWidth > maxWidth || text[i] == '\n') {
                if (textWidth > maxWidth && !currentLine.empty()) {
                    currentLine.pop_back();
                    --i;  
                }
                lines.push_back(currentLine);  
                totalHeight += lineHeight;
                if (totalHeight + lineHeight > maxHeight) break;  
                currentLine.clear();  
            }
        }
        if (!currentLine.empty() && totalHeight + lineHeight <= maxHeight) {
            lines.push_back(currentLine);
        }
        int yOffset = 0;
        for (const std::string& line : lines) {
            SDL_Surface* surface = TTF_RenderText_Blended(font, line.c_str(), color);
            if (!surface) continue;
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            int textWidth, textHeight;
            SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect destRect = {x, y + yOffset, textWidth, textHeight};
            SDL_RenderCopy(renderer, texture, NULL, &destRect);
            yOffset += lineHeight;
            SDL_DestroyTexture(texture);
        }
        TTF_CloseFont(font);
    }
    void drawText(std::string text, int x, int y,int f, int maxWidth, int maxHeight, SDL_Color color) {
        x += offsetx;
        y += offsety;
        TTF_Font* font = fonts[f];
        if (!font) {
            std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
            return;
        }
        std::vector<std::string> lines;
        std::string currentLine;
        int lineHeight = 0;
        int totalHeight = 0;
        for (size_t i = 0; i < text.size(); ++i) {
            currentLine += text[i];
            int textWidth, textHeight;
            if (TTF_SizeText(font, currentLine.c_str(), &textWidth, &textHeight) != 0) {
                std::cerr << "TTF_SizeText Error: " << TTF_GetError() << std::endl;
                return;
            }
            lineHeight = textHeight;  
            if (textWidth > maxWidth || text[i] == '\n') {
                if (textWidth > maxWidth && !currentLine.empty()) {
                    currentLine.pop_back();
                    --i;  
                }
                lines.push_back(currentLine);  
                totalHeight += lineHeight;
                if (totalHeight + lineHeight > maxHeight) break;  
                currentLine.clear();  
            }
        }
        if (!currentLine.empty() && totalHeight + lineHeight <= maxHeight) {
            lines.push_back(currentLine);
        }
        int yOffset = 0;
        for (const std::string& line : lines) {
            SDL_Surface* surface = TTF_RenderText_Blended(font, line.c_str(), color);
            if (!surface) continue;
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            int textWidth, textHeight;
            SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect destRect = {x, y + yOffset, textWidth, textHeight};
            SDL_RenderCopy(renderer, texture, NULL, &destRect);
            yOffset += lineHeight;
            SDL_DestroyTexture(texture);
        }
    }
    void drawText(std::string text,int x,int y,const std::string& fontPath,int fontSize,int maxWidth,SDL_Color color) {
        x+=offsetx;y+=offsety;
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(),fontSize);
        if (!font) {
            std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
            return;
        }
        int textWidth, textHeight;
        if (TTF_SizeText(font, text.c_str(), &textWidth, &textHeight) != 0) {
            std::cerr << "TTF_SizeText Error: " << TTF_GetError() << std::endl;
            return;
        }
        while (textWidth > maxWidth && !text.empty()) {
            text.pop_back();
            TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);
        }
        SDL_Surface* surface = TTF_RenderText_Blended(font,text.c_str(),color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
        SDL_FreeSurface(surface);
        SDL_Rect destRect = {x,y,0,0};
        SDL_QueryTexture(texture,NULL,NULL,&destRect.w,&destRect.h);
        SDL_RenderCopy(renderer,texture,NULL,&destRect);
        SDL_DestroyTexture(texture);
        TTF_CloseFont(font);
    }
    void present() {
        SDL_RenderPresent(renderer);
        frames++;
    }
    void clear() {
        SDL_RenderClear(renderer);
    }
    void clear(SDL_Color c) {
        SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
        SDL_RenderClear(renderer);
    }
};
#endif
