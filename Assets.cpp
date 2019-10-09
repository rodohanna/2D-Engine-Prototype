#include "Assets.h"
#include <unordered_map>
#include "Texture.h"

std::unordered_map<std::string, Texture *> textureMap;

bool loadImageTexture(std::string texturePath, std::string textureKey, SDL_Renderer *renderer)
{
    Texture *texture = loadTextureFromFile(texturePath, renderer);

    if (texture != NULL)
    {
        textureMap[textureKey] = texture;
        return true;
    }
    printf("Error loading texture %s from %s.\n", textureKey.c_str(), texturePath.c_str());
    return false;
}

Texture *getTexture(std::string textureKey)
{
    if (textureMap.find(textureKey) != textureMap.end())
    {
        return textureMap[textureKey];
    }
    printf("Error finding texture %s.\n", textureKey.c_str());
    return NULL;
}