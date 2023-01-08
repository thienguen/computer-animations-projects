#include <SDL2/SDL.h>
#include <random>
#include <algorithm>
#include <iostream>

/**
 * Runs a single iteration of the simulation.
 * 
 * @param renderer The renderer to use for drawing.
 * @param vp A vector of points representing the triangle.
 * @param random_point A reference to the current point.
 * @param collection A reference to the collection of plotted points.
 * @param delay A reference to the delay between iterations.
 */
void run_simulation(SDL_Renderer* renderer, std::vector<SDL_Point>& vp, SDL_Point& random_point, std::vector<SDL_Point>& collection, int& delay)
{
    // Clear the screen and select a random point in the triangle
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    auto second = vp[rand() % vp.size()];

    // Draw a line between the current point and the selected triangle point
    SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
    SDL_RenderDrawLine(renderer, random_point.x, random_point.y, second.x, second.y);
    SDL_Delay(delay);

    // Plot the current point
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, random_point.x, random_point.y);
    SDL_Delay(delay);

    // Add the midpoint between the current point and the selected triangle point to the collection
    // and set the current point to the midpoint
    collection.push_back({(random_point.x + second.x) / 2, (random_point.y + second.y) / 2});
    random_point = collection.back();

    // Draw the points in the collection
    SDL_RenderDrawPoints(renderer, collection.data(), static_cast<int>(collection.size()));
    SDL_RenderPresent(renderer);

    // Decrease the delay between iterations
    if (delay > 0)
    {
        delay--;
        if (delay > 166)
        {
            delay -= 250;
        }
    }
}

int main(int argc, char *argv[])
{
    // Initialize SDL and create window and renderer
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_CreateWindowAndRenderer(360 * 4, 166 * 4, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, 4, 4);
    /* SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); */

    /* Create a random number generator and a vector of points to represent the triangle */
    std::random_device rd;
    std::vector<SDL_Point> vp;
    std::uniform_int_distribution<int> d(0, 360);
    
    /*  */
    vp.push_back({360   / 2, 0});
    vp.push_back({0, 166});
    vp.push_back({360  , 166});

    // Set the render draw color and randomly select a point inside the triangle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    auto random_point = SDL_Point{d(rd), d(rd)};

    int delay = 10;

    // Create a collection of points to store the plotted points
    std::vector<SDL_Point> collection;

    // Main loop
    while (true)
        run_simulation(renderer, vp, random_point, collection, delay);

    return EXIT_SUCCESS;
}
