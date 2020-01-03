#include "Assets.h"
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <unordered_map>
#include <assert.h>

std::unordered_map<std::string, size_t> texture_index_map;
std::unordered_map<std::string, size_t> font_index_map;
std::vector<std::unique_ptr<Texture>> texture_table;
std::vector<Font *> font_table;
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
    size_t font_size;
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
            size_t index = texture_table.size();
            std::unique_ptr<Texture> texture = create_texture_from_file(asset_path, renderer, index);
            texture_index_map[key] = index;
            texture_table.push_back(std::move(texture));
        }
        else if (type == "font")
        {
            manifest >> font_size;
            Font *font = TTF_OpenFont(asset_path.c_str(), font_size);
            if (font != nullptr)
            {
                size_t font_index = font_table.size();
                font_index_map[key] = font_index;
                font_table.push_back(font);
            }
            else
            {
                printf("Error loading font %s from %s.\n", key.c_str(), asset_path.c_str());
            }
        }
    }
}

TextTextureInfo Assets::create_texture_from_text(SDL_Renderer *renderer, size_t font_index, std::string texture_key, std::string text, const Color &color)
{
    if (font_index < 0 || font_index >= font_table.size())
    {
        printf("Error: create_texture_from_text received a bad font_index %d\n", font_index);
        return {-1};
    }
    Font *some_font = font_table[font_index];
    SDL_Surface *text_surface = TTF_RenderText_Solid(some_font, text.c_str(), color);
    if (text_surface == nullptr)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", IMG_GetError());
        return {-1};
    }

    SDL_Texture *new_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (new_texture == nullptr)
    {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return {-1};
    }
    int texture_index = -1;
    int existing_texture_index = get_texture_index(texture_key);
    Texture *texture = nullptr;
    if (existing_texture_index == -1)
    {
        texture_index = texture_table.size();
        texture = new Texture(new_texture, {text_surface->w, text_surface->h}, texture_index);
        texture_table.push_back(std::unique_ptr<Texture>(texture));
        texture_index_map[texture_key] = texture_index;
    }
    else
    {
        texture_index = existing_texture_index;
        texture = new Texture(new_texture, {text_surface->w, text_surface->h}, texture_index);
        texture_table[texture_index] = std::unique_ptr<Texture>(texture);
    }
    SDL_FreeSurface(text_surface);
    return {texture_index, texture->dimensions};
}

int Assets::get_texture_index(std::string texture_key)
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

void Texture::render(SDL_Renderer *renderer, const V2 &position, SDL_Rect *clip, size_t scale, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    if (this->texture == nullptr)
    {
        printf("Warning! A null texture render was attempted.\n");
        return;
    }
    SDL_Rect render_quad = {position.x, position.y, this->dimensions.x * static_cast<int>(scale), this->dimensions.y * static_cast<int>(scale)};
    if (clip != NULL)
    {
        render_quad.w = clip->w * scale;
        render_quad.h = clip->h * scale;
    }
    SDL_RenderCopyEx(renderer, this->texture, clip, &render_quad, angle, center, flip);
}