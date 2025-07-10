#ifndef TEXTURE 
#define TEXTURE
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
#include <vector>
bool operator==(const SDL_Rect& one,const SDL_Rect& other) {
    return one.x == other.x && one.y == other.y && one.w==other.w && one.h==other.h;
}
bool operator!=(const SDL_Rect& one,const SDL_Rect& other) {
    return one.x != other.x || one.y != other.y || one.w!=other.w || one.h!=other.h;
}
int integralSquareRoot(int num) {
    if (num < 0) return -1; 
    if (num == 0 || num == 1) return num;
    int low = 1, high = num, result = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (mid == num / mid) {
            return mid; 
        } else if (mid < num / mid) {
            result = mid; 
            low = mid + 1; 
        } else {
            high = mid - 1; 
        }
    }
    return result; 
}
double calcAngle(int x1, int y1, int x2, int y2) {
    return fmod(atan2(y2 - y1, x2 - x1) * (180.0 / M_PI) + 360.0, 360.0);
}
bool pointInside(SDL_Point p,SDL_Rect r) {
    if (p.x>r.x&&p.y>r.y&&p.x<r.x+r.w&&p.y<r.y+r.h) {
        return true;
    }
    return false;
}
bool collide(SDL_Rect r1,SDL_Rect r2) {
    if (r1.x+r1.w>r2.x && r2.x+r2.w>r1.x && r1.y+r1.h>r2.y && r2.y+r2.h>r1.y) return true;
    return false;
}
void setFrameRate(int targetFPS) {
    static Uint32 lastTime = 0;
    Uint32 frameTime = 1000 / targetFPS;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 deltaTime = currentTime - lastTime;
    if (deltaTime < frameTime) {
        SDL_Delay(frameTime - deltaTime);
    }
    lastTime = SDL_GetTicks();
}
class Texture {
    SDL_Texture* texture = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Color curcolor;
    std::vector<Texture*> textures;
    std::vector<TTF_Font*> fonts;
    int width,height;
public:
    int offsetx=0,offsety=0;
    Texture(SDL_Renderer* rend, int w, int h)
        : renderer(rend), width(w), height(h) {
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        if (!texture) {
            std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
        }
    }
    SDL_Texture* getTexture() {
        return texture;
    }
    Texture(SDL_Texture* text,SDL_Renderer* rend): renderer(rend),texture(text) {}
    SDL_Rect windowDimensions() {
        return {offsetx,offsety,width,height};
    }

    ~Texture() {
        SDL_DestroyTexture(texture);
        for (int i=0;i<textures.size();i++) {
            delete textures[i];
        }
        for (int i=0;i<fonts.size();i++) {
            TTF_CloseFont(fonts[i]);
        }
    }
    int loadFont(std::string fontPath,int fontSize) {
        fonts.push_back(TTF_OpenFont(fontPath.c_str(),fontSize));
        return fonts.size()-1;
    }
    void QueryTexture(int i,int& w,int& h) {
            w=textures[i]->width;
            h=textures[i]->height;
    }
    void drawTexture(int i,SDL_Rect img,SDL_Rect win,bool mirrored = false) {
        SDL_SetRenderTarget(renderer, texture);
        SDL_Texture* texture = textures[i]->texture;
        win.x+=offsetx;
        win.y+=offsety;
        SDL_RendererFlip flip = mirrored ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer,texture,&img,&win,0,NULL,flip);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void drawTexture(int i, SDL_Rect win, bool mirrored = false) {
        SDL_SetRenderTarget(renderer, texture);
        SDL_Texture* texture = textures[i]->texture;
        win.x += offsetx;
        win.y += offsety;
        SDL_Rect img{0, 0, 0, 0};
        SDL_QueryTexture(texture, NULL, NULL, &img.w, &img.h);
        SDL_RendererFlip flip = mirrored ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer, texture, &img, &win, 0, NULL, flip);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void drawBg(int i) {
        SDL_SetRenderTarget(renderer, texture);
        SDL_Texture* texture = textures[i]->texture;
        SDL_Rect img{0,0,0,0};
        SDL_Rect win{0,0,width,height};
        SDL_QueryTexture(texture,NULL,NULL,&img.w,&img.h);
        SDL_RenderCopy(renderer,texture,&img,&win);
        SDL_SetRenderTarget(renderer, nullptr);
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
        SDL_SetRenderTarget(renderer, texture);
        x+=offsetx;y+=offsety;
        SDL_RenderDrawPoint(renderer,x+offsetx,y+offsety);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void drawLine(int x1,int y1,int x2,int y2) {
        SDL_SetRenderTarget(renderer, texture);
        aalineRGBA(renderer,x1+offsetx,y1+offsety,x2+offsetx,y2+offsety,curcolor.r,curcolor.g,curcolor.b,curcolor.a);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void drawRect(int x,int y,int w,int h) {
        SDL_SetRenderTarget(renderer, texture);
        x+=offsetx;y+=offsety;
        SDL_Rect rect = {x,y,w,h};
        SDL_RenderDrawRect(renderer,&rect);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void fillRect(int x,int y,int w,int h) {
        SDL_SetRenderTarget(renderer, texture);
        x+=offsetx;y+=offsety;
        SDL_Rect rect = {x,y,w,h};
        SDL_RenderFillRect(renderer,&rect);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void fillCircle(int centerX,int centerY,int radius) {
        SDL_SetRenderTarget(renderer, texture);
        filledCircleRGBA(renderer,offsetx+centerX,offsety+centerY,radius,curcolor.r,curcolor.g,curcolor.b,curcolor.a);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void fillRoundedRect(int x, int y, int w, int h, int r) {
        SDL_SetRenderTarget(renderer, texture);
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
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void drawCircle(int centerX,int centerY,int radius) {
        SDL_SetRenderTarget(renderer, texture);
        circleRGBA(renderer,offsetx+centerX,offsety+centerY,radius,curcolor.r,curcolor.g,curcolor.b,curcolor.a);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void drawArc(int x,int y,int radius,float startAngle,float endAngle) {
        SDL_SetRenderTarget(renderer, texture);
        x+=offsetx;y+=offsety;
        float startRad = startAngle * (M_PI / 180.0f);
        float endRad = endAngle * (M_PI / 180.0f);
        arcRGBA(renderer,x,y,radius,startRad,endRad,curcolor.r,curcolor.g,curcolor.b,curcolor.a);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void drawPoly(const std::vector<Sint16>& px, const std::vector<Sint16>& py) {
        SDL_SetRenderTarget(renderer, texture);
        int num_points = std::min(px.size(), py.size());
        std::vector<Sint16> x(num_points);
        std::vector<Sint16> y(num_points);
        for (int i = 0; i < num_points; i++) {
            x[i] = px[i] + offsetx;
            y[i] = py[i] + offsety;
        }
        polygonRGBA(renderer, x.data(), y.data(), num_points, curcolor.r, curcolor.g, curcolor.b, curcolor.a);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void fillPoly(const std::vector<Sint16>& px, const std::vector<Sint16>& py) {
        SDL_SetRenderTarget(renderer, texture);
        int num_points = std::min(px.size(), py.size());
        std::vector<Sint16> x(num_points);
        std::vector<Sint16> y(num_points);
        for (int i = 0; i < num_points; i++) {
            x[i] = px[i] + offsetx;
            y[i] = py[i] + offsety;
        }
        filledPolygonRGBA(renderer, x.data(), y.data(), num_points, curcolor.r, curcolor.g, curcolor.b, curcolor.a);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    int loadImageOntoTexture(const std::string& path) {
        Texture* texture = new Texture(IMG_LoadTexture(renderer,path.c_str()),renderer);
        if (!texture->texture) {
            std::cerr << "IMG_LoadTexture Error: " << IMG_GetError() << std::endl;
            return -1;
        }
        textures.push_back(texture);
        return textures.size()-1;
    }
    void drawText(std::string text,int x,int y,int f,int maxWidth,SDL_Color color) {
        SDL_SetRenderTarget(renderer, texture);
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
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void drawText(std::string text, int x, int y, const std::string& fontPath, int fontSize, int maxWidth, int maxHeight, SDL_Color color) {
        SDL_SetRenderTarget(renderer, texture);
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
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void drawText(std::string text, int x, int y,int f, int maxWidth, int maxHeight, SDL_Color color) {
        SDL_SetRenderTarget(renderer, texture);
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
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void drawText(std::string text,int x,int y,const std::string& fontPath,int fontSize,int maxWidth,SDL_Color color) {
        SDL_SetRenderTarget(renderer, texture);
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
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void clear() {
        SDL_SetRenderTarget(renderer, texture);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void clear(SDL_Color c) {
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr);
    }
};
#endif
