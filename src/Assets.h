#ifndef ASSETS_h_
#define ASSETS_h_

#include "SDLWrapper.h"
#include "GameTypes.h"
#include <string>
#include <memory>
#include <vector>

struct Texture
{
    Texture(SDL_Texture *, const V2 &dimensions, size_t index);
    ~Texture();
    void render(SDL_Renderer *renderer, const V2 &position, SDL_Rect *clip = NULL, int scale = 1, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    SDL_Texture *texture;
    V2 dimensions;
    size_t index;
};

namespace Assets
{
void load_assets_from_manifest(SDL_Renderer *, std::string);
size_t get_texture_index(std::string texture_key);
std::vector<std::unique_ptr<Texture>> *get_texture_table();
V2 get_texture_dimensions(std::string texture_key);
} // namespace Assets

#endif