#include <iostream>
#include <SDL.h> 
#include <SDL_image.h>   
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* main_window = NULL; // actual application window
SDL_Renderer* main_renderer = NULL; // renders textures ie puts them on window


// TODO figure out how to combine upgrade and menutexture
// probably by putting calls to load_from_file within the upgrade constructor


// To access textures array in Game by name instead of magic numbers
enum TEXTURES_NAME {
        CAT,
        FACE,
        TEXTURES_COUNT
};

class MenuTexture {
    public: 
        MenuTexture() {
            texture = NULL;
            width = 0;
            height = 0;
        }

        MenuTexture(int w, int h, std::string f) {
            texture = NULL;
            width = w;
            height = h;
            file_path = f;
        }

        ~MenuTexture() {
            free();
        }

        bool load_from_file() {
            // get rid of preexisting textures
            free();

            SDL_Texture* loaded_texture = IMG_LoadTexture(main_renderer, file_path.c_str());

            if (loaded_texture == NULL) {
                std::cout << "Unable to load image! SDL Error: " << SDL_GetError() << "\n";
            }
            else {
                // width = loaded_texture->w;
                // height = loaded_texture->h;
                // width = 100;
                // height = 100;
            }

            texture = loaded_texture;
            return texture != NULL;
        }

        void free() {
            if (texture != NULL) {
                SDL_DestroyTexture(texture);
                texture = NULL;
                width = 0;
                height = 0;
            }
        }

        void render(int xpos, int ypos) {
            SDL_Rect quad = {xpos, ypos, width, height};
            SDL_RenderCopy(main_renderer, texture, NULL, &quad);
        }

        int get_width() {
            return width;
        }
        
        int get_height() {
            return height;
        }

    private:
        SDL_Texture* texture;
        int width;
        int height;
        std::string file_path;
};

// MenuTexture cat(100, 400);
// MenuTexture face(50, 50); 

// class Logic: 
/*
class Upgrade{
    public:
        Upgrade() {
            // icon = NULL;
            // menu_pane = NULL;
        }

        ~Upgrade() {
            icon.free();
            menu_pane->free();
        }



        void render(int xpos, int ypos) {
            icon.render(xpos, ypos);
            SDL_Rect rect = {xpos, ypos, menu_pane->get_width(), menu_pane->get_height()};
            SDL_SetRenderDrawColor(main_renderer, 0x00, 0x00, 0x00, 0xff);
            SDL_RenderDrawRect(main_renderer, &rect);
        }        


    private:
    //     int count; 
    //     int price;
        MenuTexture icon;
        MenuTexture* menu_pane = new MenuTexture(100, 100);
    //     int how much gen per sec;
}; */

// class GiantCookie: private MenuTexture {
//     public:
//         void clicked() {
//             money += 1;
//         }
//     private:
//         MenuTexture sprite
    
// }


class Game { 
    public:

        bool init() {
            bool success = true;

            if (SDL_Init(SDL_INIT_VIDEO) == -1) { // SDL_Init() return -1 if it fails
                std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
                success = false;
            }
            else {
                // ensure we use linear texture filting
                // linear texture filtering is an image convolution 
                // where each pixel samples its neighbors and output pixel is a combination of samples
                // overall effect is output images look "smoother" even when manipulated (ex scaled)
                if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
                {
                    std::cout << "Warning: Linear texture filtering not enabled!\n";
                }

                main_window = SDL_CreateWindow("Sebi Sux", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
                if (main_window == NULL) {
                    std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
                    success = false;

                } 
                else {
                    // -1: "the index of the rendering driver to initialize, or -1 to initialize the first one supporting the requested flags"
                    // acclerated flag means renderer uses hardware ie graphics card
                    main_renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
                    if (main_renderer == NULL) {
                        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
                        success = false;
                    }
                    else {
                        // color for drawing operations in rgba
                        SDL_SetRenderDrawColor(main_renderer, 0xff, 0xff, 0xff, 0xff);

                        int img_flags = IMG_INIT_PNG;
                        if(!(IMG_Init(img_flags) & img_flags)) {
                            std::cout << "SDL_image could not initialize! SDL_image Error:" << SDL_GetError() << "\n";
                            success = false;
                        }

                    }
                }
            }

            return success;
        }

        bool load_media() {
            bool success = true;

            //initialize all menutextures here
            //could be moved elsewhere?
            textures[CAT] = MenuTexture(100, 400, "wait_im_goated.bmp");
            textures[FACE] = MenuTexture(50, 50, "trust_me_bro.png");  

            for (int i = 0; i < TEXTURES_COUNT; i++) {
                if (success) {
                    if (!textures[i].load_from_file()) {
                        std::cout << "Failed to load texture image!\n";
                        success = false;
                    }
                }
                else {
                    break;
                }
            }

            return success;
        }

        void draw_rect(int xpos, int ypos, int width, int height) {
            SDL_Rect rect = {xpos, ypos, width, height};
            SDL_SetRenderDrawColor(main_renderer, 0x00, 0x00, 0x00, 0xff);
            SDL_RenderDrawRect(main_renderer, &rect);
        }

        void close() {

            SDL_DestroyRenderer(main_renderer);
            main_renderer = NULL;

            SDL_DestroyWindow(main_window);
            main_window = NULL;

            IMG_Quit();
            SDL_Quit();
        }
        
        MenuTexture textures[TEXTURES_COUNT];
        
};


int main (int argc, char* args[]) {
    Game game;

    if (!game.init()) {
        std::cout << "Failed to initialize!\n";
    }
    else {
        if (!game.load_media()) {
            std::cout << "Failed to load media!\n";
        }
        else {
            SDL_Event event; // event listener

            bool quit = false;

            while (!quit) {
                
                while (SDL_PollEvent(&event) != 0) {

                    if (event.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                // clear renderer and clear screen and reset color to white
                SDL_SetRenderDrawColor(main_renderer, 0xff, 0xff, 0xff, 0xff);
                SDL_RenderClear(main_renderer);

                // render texture to screen 
                // Null mean render the entire texture
                // this is main_texture so its controlled by load_media()
                // SDL_RenderCopy(main_renderer, main_texture, NULL, NULL);
                const SDL_Rect rect1 = {150, 150, 300, 300}; 
                const SDL_Rect rect2 = {SCREEN_WIDTH-200, SCREEN_HEIGHT-200, 200, 200}; 
                // SDL_RenderCopy(main_renderer, cat., &rect1, &rect2);

                // main_texture = load_texture("trust_me_bro.png");
                // SDL_RenderCopy(main_renderer, main_texture, NULL, &rect1);

                

                game.textures[CAT].render(0, 0);
                game.textures[FACE].render(SCREEN_WIDTH-100, SCREEN_HEIGHT-100);
                game.textures[FACE].render(SCREEN_WIDTH-200, SCREEN_HEIGHT-100);
                game.textures[FACE].render(SCREEN_WIDTH-300, SCREEN_HEIGHT-100);
                

                // need to draw rects after because order matters I guess
                game.draw_rect(100, 100, 100, 100);

                // const SDL_Rect top_left_viewport = {0, 0, 400, 400};
                // SDL_RenderSetViewport(main_renderer, &top_left_viewport);
                // SDL_RenderCopy(main_renderer, main_texture, NULL, NULL);
                
                // Update screen 
                SDL_RenderPresent(main_renderer);
            }
        }
    }

    game.close();

    return 0;
}