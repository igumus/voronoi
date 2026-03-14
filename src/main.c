#define NOB_IMPLEMENTATION
#include "../thirdparty/nob.h"
#include "distance.h"

#include "raylib.h"

#define WIDTH  800
#define HEIGHT 600

// NOTE: Color Storage
#define COLOR_RED                    (Color) {0x00, 0x00, 0xFF, 0xFF}
#define COLOR_GREEN                  (Color) {0x00, 0xFF, 0x00, 0xFF}
#define COLOR_BLUE                   (Color) {0xFF, 0x00, 0x00, 0xFF}
#define COLOR_WHITE                  (Color) {0xFF, 0xFF, 0xFF, 0xFF}
#define COLOR_BACKGROUND             (Color) {0x18, 0x18, 0x18, 0xFF}
#define COLOR_GRUVBOX_BRIGHT_RED     (Color) {0x34, 0x49, 0xFB, 0xFF}
#define COLOR_GRUVBOX_BRIGHT_GREEN   (Color) {0x26, 0xBB, 0xB8, 0xFF}
#define COLOR_GRUVBOX_BRIGHT_YELLOW  (Color) {0x2F, 0xBD, 0xFA, 0xFF}
#define COLOR_GRUVBOX_BRIGHT_BLUE    (Color) {0x98, 0xA5, 0x83, 0xFF}
#define COLOR_GRUVBOX_BRIGHT_PURPLE  (Color) {0x9B, 0x86, 0xD3, 0xFF}
#define COLOR_GRUVBOX_BRIGHT_AQUA    (Color) {0x7C, 0xC0, 0x8E, 0xFF}
#define COLOR_GRUVBOX_BRIGHT_ORANGE  (Color) {0x19, 0x80, 0xFE, 0xFF}

#define SEED_COUNT_MAX 10
#define SEED_MARKER_RADIUS 5
#define SEED_MARKER_COLOR COLOR_BACKGROUND

typedef struct {
    int x, y;
} Point;


static Color pixels[HEIGHT * WIDTH] = {0};
static Point seeds[SEED_COUNT_MAX] = {0};
static Color palette[] = {
    COLOR_GRUVBOX_BRIGHT_RED,
    COLOR_GRUVBOX_BRIGHT_GREEN,
    COLOR_GRUVBOX_BRIGHT_YELLOW,
    COLOR_GRUVBOX_BRIGHT_BLUE,
    COLOR_GRUVBOX_BRIGHT_PURPLE,
    COLOR_GRUVBOX_BRIGHT_AQUA,
    COLOR_GRUVBOX_BRIGHT_ORANGE,
};
static size_t seed_count = 0;

#define palette_count (sizeof(palette)/sizeof(palette[0]))

void fill_circle(int cx, int cy, int radius, Color color, distance_fn distance) 
{
    int x0 = cx-radius;
    int y0 = cy-radius;
    int x1 = cx+radius;
    int y1 = cy+radius;

    for (int y = y0; y <= y1; ++y) {
        if (y >= 0 && y < HEIGHT) {
            for (int x = x0; x <= x1; ++x) {
                if (x >= 0 && x < WIDTH) {
                    int dist = distance(cx, cy, x, y);
                    if (dist < radius) {
                        pixels[y* WIDTH + x] = color;
                    }
                }
            }
        }
    }
}

void render_seeds_marker(distance_fn distance) 
{
    for (size_t i = 0; i < seed_count; ++i) {
        Point seed = seeds[i];
        fill_circle(seed.x, seed.y, SEED_MARKER_RADIUS, SEED_MARKER_COLOR, distance);
    }
}

void render_voronoi_v0(distance_fn distance)
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int j = 0;
            for (size_t i = 1; i < seed_count; ++i) {
                Point prev = seeds[j];
                Point curr = seeds[i];
                int curr_dist = distance(curr.x, curr.y, x, y);
                int prev_dist = distance(prev.x, prev.y, x, y);
                if (curr_dist < prev_dist) {
                    j = i;
                }
            }
            pixels[y*WIDTH + x] = palette[j % palette_count];
        }
    }
}

static void update_image() {
    distance_fn distance = &euclidean;
    render_voronoi_v0(distance);
    render_seeds_marker(distance);
}

void reset() {
    seed_count = 0;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            pixels[y*WIDTH + x] = COLOR_BACKGROUND;
        }
    }
}

int main(void) {
    Image image = {
        .data = pixels,
        .width = WIDTH,
        .height = HEIGHT,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    };

    SetTargetFPS(60);
    InitWindow(WIDTH, HEIGHT, "framebuffer texture");

    Texture2D texture = LoadTextureFromImage(image);
    reset();

    while (!WindowShouldClose()) {


        if (IsKeyPressed(KEY_R)) {
            reset();
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (seed_count < SEED_COUNT_MAX) {
                Vector2 mouse = GetMousePosition();
                seeds[seed_count].x = mouse.x;
                seeds[seed_count].y = mouse.y;
                seed_count++;

                update_image();
            }
        }

        UpdateTexture(texture, pixels);

        BeginDrawing();
        ClearBackground(COLOR_BACKGROUND);
        DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
