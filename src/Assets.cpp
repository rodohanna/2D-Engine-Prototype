#include "Assets.h"
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <unordered_map>
#include <assert.h>

std::unordered_map<std::string, size_t> texture_index_map;
std::vector<std::unique_ptr<Texture>> texture_table;
std::unique_ptr<Texture> create_texture_from_file(std::string path, SDL_Renderer *renderer, size_t index);

void Assets::load_assets_from_manifest(SDL_Renderer *renderer, std::string path)
{
    if (renderer == nullptr)
    {
        printf("load_assets_from_manifest received null renderer\n");
    }
    std::ifstream manifest(path);
    if (manifest.fail())
    {
        manifest.close();
        printf("Error: Unable to load manifest file at %s\n", path.c_str());
        return;
    }
    std::string key, type, asset_path;
    size_t index = 0;
    while (manifest >> key >> type >> asset_path)
    {
        if (manifest.fail())
        {
            printf("Reading manifest %s failed, quiting early\n", path.c_str());
            break;
        }
        printf("Loading key: %s, type: %s, path: %s\n", key.c_str(), type.c_str(), asset_path.c_str());
        if (type == "sprite")
        {
            std::unique_ptr<Texture> texture = create_texture_from_file(asset_path, renderer, index);
            texture_index_map[key] = index;
            texture_table.push_back(std::move(texture));
        }
        ++index;
    }
}

size_t Assets::get_texture_index(std::string texture_key)
{
    if (texture_index_map.find(texture_key) != texture_index_map.end())
    {
        return texture_index_map[texture_key];
    }
    printf("Error finding texture %s.\n", texture_key.c_str());
    return -1;
}

V2 Assets::get_texture_dimensions(std::string texture_key)
{
    if (texture_index_map.find(texture_key) != texture_index_map.end())
    {
        size_t texture_index = texture_index_map[texture_key];
        assert(texture_index >= 0 && texture_index < texture_table.size());
        return texture_table[texture_index]->dimensions;
    }
    printf("get_texture_dimensions: Error finding texture %s.\n", texture_key.c_str());
    return {0, 0};
}

std::unique_ptr<Texture> create_texture_from_file(std::string path, SDL_Renderer *renderer, size_t index)
{
    SDL_Surface *loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == nullptr)
    {
        printf("Unable to load asset %s. SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return nullptr;
    }

    SDL_Texture *new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    if (new_texture == nullptr)
    {
        printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
        return nullptr;
    }

    Texture *texture = new Texture(new_texture, {loaded_surface->w, loaded_surface->h}, index);
    SDL_FreeSurface(loaded_surface);
    return std::unique_ptr<Texture>(texture);
}

std::vector<std::unique_ptr<Texture>> *Assets::get_texture_table()
{
    return &texture_table;
}

// Texture

Texture::Texture(SDL_Texture *texture, const V2 &dimensions, size_t index) : texture(texture), dimensions(dimensions), index(index) {}

Texture::~Texture()
{
    if (this->texture != nullptr)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
}

void Texture::render(SDL_Renderer *renderer, const V2 &position, SDL_Rect *clip, int scale, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    if (this->texture == nullptr)
    {
        printf("Warning! A null texture render was attempted.\n");
        return;
    }
    SDL_Rect render_quad = {position.x, position.y, this->dimensions.x * scale, this->dimensions.y * scale};
    if (clip != NULL)
    {
        render_quad.w = clip->w * scale;
        render_quad.h = clip->h * scale;
    }
    SDL_RenderCopyEx(renderer, this->texture, clip, &render_quad, angle, center, flip);
}