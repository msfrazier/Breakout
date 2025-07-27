#include <SDL3/SDL.h>
#include <iostream>

enum KeyPressSurface {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;



//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The images that correspond to a keypress
SDL_Surface* gKeyPressSurface[KEY_PRESS_SURFACE_TOTAL];

//Current displayed image
SDL_Surface* gCurrentSurface = NULL;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_FULLSCREEN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

SDL_Surface* loadSurface(std::string path) {
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    SDL_Surface* optimizedSurface = NULL;

    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    else {
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format);
        if (optimizedSurface == NULL) {
			printf("Unable to optimize image %s! SDL Error: %s\n!", path.c_str(), SDL_GetError());
        }

		SDL_DestroySurface(loadedSurface);
    }

    return optimizedSurface;
}

bool loadMedia() {
    bool success = true;

    gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("images/default.bmp");
    if (gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        printf("Failed to load default image!\n");
        success = false;
    }

    gKeyPressSurface[KEY_PRESS_SURFACE_UP] = loadSurface("images/up.bmp");
    if (gKeyPressSurface[KEY_PRESS_SURFACE_UP] == NULL) {
        printf("Failed to load up image!\n");
        success = false;
    }

    gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] = loadSurface("images/down.bmp");
    if (gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] == NULL) {
        printf("Failed to load down image!\n");
        success = false;
    }

    gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] = loadSurface("images/left.bmp");
    if (gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] == NULL) {
        printf("Failed to load left image!\n");
        success = false;
    }

    gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] = loadSurface("images/right.bmp");
    if (gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        printf("Failed to load right image!\n");
        success = false;
    }

    return success;
}

void close() {
    //Deallocate surfaces
    for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i)
    {
        SDL_DestroySurface(gKeyPressSurface[i]);
        gKeyPressSurface[i] = NULL;
    }

    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char* argv[]) {

    if (!init()) {
		printf("Failed to initialize!\n");
        return 1;
	}

    else {
        if (!loadMedia()) {
            printf("Failed to load media!\n");
            return 1;

        }
        else {
            SDL_Event e;
            bool quit = false;
            gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];

            while (!quit) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_EVENT_QUIT) {
                        quit = true;
                    }
                    else if (e.type == SDL_EVENT_KEY_DOWN) {
                        switch (e.key.key) {
                        case SDLK_UP:
                        case SDLK_W:
                            gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_UP];
                            break;
                        case SDLK_DOWN:
						case SDLK_S:
                            gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DOWN];
                            break;
                        case SDLK_LEFT:
						case SDLK_A:
                            gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_LEFT];
                            break;
                        case SDLK_RIGHT:
						case SDLK_D:
                            gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT];
                            break;
                        case SDLK_SPACE:
							gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
                            break;

                        case SDLK_ESCAPE:
                            close();
                            break;
                        }
                    }
                }

                SDL_Rect stretchRect;
                stretchRect.x = 0;
                stretchRect.y = 0;
                stretchRect.w = gScreenSurface->w;
				stretchRect.h = gScreenSurface->h;
				SDL_BlitSurfaceScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect, SDL_SCALEMODE_NEAREST);
                /*SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);*/

                SDL_UpdateWindowSurface(gWindow);
            }

            return 0;
        }
    }
    close();
    return 0;
}