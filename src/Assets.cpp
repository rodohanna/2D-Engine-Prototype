#include "Assets.h"
#include "Texture.h"
#include <unordered_map>

std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;
std::unordered_map<std::string, TTF_Font *> fontMap;

bool loadImageTexture(std::string texturePath, std::string textureKey, SDL_Renderer *renderer)
{
    std::shared_ptr<Texture> texture = Texture::makeTextureFromFile(texturePath, renderer);
    if (texture != nullptr)
    {
        textureMap[textureKey] = texture;
        return true;
    }
    printf("Error loading texture %s from %s.\n", textureKey.c_str(), texturePath.c_str());
    return false;
}

bool loadFont(std::string fontPath, std::string fontKey, Uint32 fontSize)
{
    TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (font != NULL)
    {
        fontMap[fontKey] = font;
        return true;
    }
    printf("Error loading font %s from %s.\n", fontKey.c_str(), fontPath.c_str());
    return false;
}

std::shared_ptr<Texture> getTexture(std::string textureKey)
{
    if (textureMap.find(textureKey) != textureMap.end())
    {
        return textureMap[textureKey];
    }
    printf("Error finding texture %s.\n", textureKey.c_str());
    return NULL;
}

TTF_Font *getFont(std::string fontKey)
{
    if (fontMap.find(fontKey) != fontMap.end())
    {
        return fontMap[fontKey];
    }
    printf("Error finding font %s.\n", fontKey.c_str());
    return NULL;
}