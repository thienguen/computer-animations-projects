#include "screen.h"
#include "math.h"
#include <random>

class Point
{
    int id = 0;
    float velocity;
    SDL_FPoint position;
    SDL_FPoint direction;
    Point()
    {
        position.x = static_cast<float>(rand() % 320);
        position.y = static_cast<float>(rand() % 240);
        velocity = 1;
    }
};

class Sand
{
public:
    int id = 0;
    int alivefor;
    float velocity;
    float position_x;
    float position_y;
    float direction_x;
    float direction_y;

    Sand(float x, float y, int life, float dx, float dy, float vel, int uid)
    {
        id          = uid;
        velocity    = vel;
        alivefor    = life;
        position_x  = x;
        position_y  = y;
        direction_x = dx;
        direction_y = dy;
    }

    void update()
    {
        this->position_x += this->direction_x * this->velocity;
        this->position_y += this->direction_y * this->velocity;

        if (this->position_x > 320 || this->position_x < 0)
            this->direction_x *= -1;

        if (this->position_y > 240 || this->position_y < 0)
            this->direction_y *= -1;

        if (this->alivefor > 0)
            this->alivefor--;
    }

    bool isalive()
    {
        return this->alivefor > 0;
    }
};

class Explosion
{
public:
    int id;
    int life;
    int size;
    float position_x;
    float position_y;

    G *screen;
    Explosion(float x, float y, int lifetime, int uid, G *scr) : position_x(x),
                                                                 position_y(y),
                                                                 life(lifetime),
                                                                 id(uid),
                                                                 screen(scr) {}

    void update()
    {
        if (this->life > 0)
        {
            float d = 0.0;
            for (int i = 0; i < 63; i++)
            {
                screen->drawpixel(
                    static_cast<int>(sin(d)) * size + static_cast<int>(position_x),
                    static_cast<int>(cos(d)) * size + static_cast<int>(position_y));
                d += 0.1f;
            }
            size += 1;
            life -= 1;
        }
    }
};

bool areColiding(Sand &rhs, Sand &lhs)
{
    if ((int)rhs.position_y == (int)lhs.position_y &&
        (int)rhs.position_x == (int)lhs.position_x && lhs.id != rhs.id)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main(int argv, char **args)
{
    G screen;
    std::vector<Sand> world;
    std::vector<Explosion> world_explosions;

    // Create all particles with random values
    //
    std::mt19937 generator(time(nullptr));
    std::uniform_real_distribution<float> rand_float(0.0f, 1.0f);
    std::uniform_int_distribution<int> rand_int(0, 319);

    for (int i = 0; i < 100; i++)
    {
        float sine   = std::sin(rand_float(generator));
        float cosine = std::cos(rand_float(generator));
        float size   = rand_float(generator) * 100.0f / 55.0f;
        float x      = static_cast<float>(rand_int(generator));
        float y      = static_cast<float>(rand_int(generator));

        world.push_back(Sand(x, y, -1, sine, cosine, size, i));
    }
    // Main loop
    while (true)
    {
        // Update and draw all sand particles
        for (auto &w : world)
        {
            if (w.velocity > 0)
            {
                w.update();
            }
            screen.drawpixel(static_cast<int>(w.position_x), static_cast<int>(w.position_y));
        }

        // Update and draw all explosions
        for (auto &w : world_explosions)
            w.update();

        // Check for collisions between sand particles
        for (auto &w : world)
        {
            for (auto &m : world)
            {
                if ((m.velocity > 0 || w.velocity > 0) && areColiding(m, w))
                {
                    world_explosions.push_back(Explosion(w.position_x, w.position_y, 60, rand(), &screen));
                    m.velocity = 0;
                    w.velocity = 0;
                }
            }
        }

        // Update the screen and wait for the next frame
        screen.update();
        SDL_Delay(20);
        screen.input();
        screen.clearpixels();
    }

    
    return EXIT_SUCCESS;
}
