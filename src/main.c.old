#define NOB_IMPLEMENTATION
#include "../thirdparty/nob.h"
#include "distance.h"

#define WIDTH  800
#define HEIGHT 600

// NOTE: Color Storage
// 0xAABBGGRR - As we write
// 0xRRGGBBAA - In Memory
#define COLOR_RED                    0xFF0000FF
#define COLOR_GREEN                  0xFF00FF00
#define COLOR_BLUE                   0xFFFF0000
#define COLOR_WHITE                  0xFFFFFFFF
#define COLOR_BACKGROUND             0xFF181818
#define COLOR_GRUVBOX_BRIGHT_RED     0xFF3449FB
#define COLOR_GRUVBOX_BRIGHT_GREEN   0xFF26BBB8
#define COLOR_GRUVBOX_BRIGHT_YELLOW  0xFF2FBDFA
#define COLOR_GRUVBOX_BRIGHT_BLUE    0xFF98A583
#define COLOR_GRUVBOX_BRIGHT_PURPLE  0xFF9B86D3
#define COLOR_GRUVBOX_BRIGHT_AQUA    0xFF7CC08E
#define COLOR_GRUVBOX_BRIGHT_ORANGE  0xFF1980FE

#define COMPONENT_MASK_RED   0x000000FF 
#define COMPONENT_MASK_GREEN 0x0000FF00
#define COMPONENT_MASK_BLUE  0x00FF0000

#define SEED_COUNT 10
#define SEED_MARKER_RADIUS 5
#define SEED_MARKER_COLOR COLOR_BACKGROUND

typedef uint32_t Color32;

typedef struct {
    int x, y;
} Point;

Color32 point_to_color(Point p) 
{
    assert(p.x >= 0);
    assert(p.y >= 0);
    assert(p.x < UINT16_MAX);
    assert(p.y < UINT16_MAX);
    uint16_t x = p.x;
    uint16_t y = p.y;
    Color32 ret = (y << 16) | x; 
    return ret;
}

Point color_to_point(Color32 color)
{
    Point ret = {0};
    ret.x = (color & 0x0000FFFF) >> 0;
    ret.y = (color & 0xFFFF0000) >> 16;
    return ret;
}

static Color32 image[HEIGHT][WIDTH] = {0};
static Point seeds[SEED_COUNT] = {0};
static Color32 palette[] = {
    COLOR_GRUVBOX_BRIGHT_RED,
    COLOR_GRUVBOX_BRIGHT_GREEN,
    COLOR_GRUVBOX_BRIGHT_YELLOW,
    COLOR_GRUVBOX_BRIGHT_BLUE,
    COLOR_GRUVBOX_BRIGHT_PURPLE,
    COLOR_GRUVBOX_BRIGHT_AQUA,
    COLOR_GRUVBOX_BRIGHT_ORANGE,
};

#define palette_count (sizeof(palette)/sizeof(palette[0]))

// NOTE: forward declarations
void save_image_as_ppm(const char *path);

void fill_image(Color32 color) 
{
    for (size_t y = 0; y < HEIGHT; ++y) {
        for (size_t x = 0; x < WIDTH; ++x) {
            image[y][x] = color;
        }
    }
}

void fill_circle(int cx, int cy, int radius, uint32_t color, distance_fn distance) 
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
                        image[y][x] = color;
                    }
                }
            }
        }
    }
}

void generate_seeds_random(void) 
{
    for (size_t i = 0; i < SEED_COUNT; ++i) {
        seeds[i].x = rand() % WIDTH;
        seeds[i].y = rand() % HEIGHT;
    }
}

void render_seeds_marker(distance_fn distance) 
{
    for (size_t i = 0; i < SEED_COUNT; ++i) {
        Point seed = seeds[i];
        fill_circle(seed.x, seed.y, SEED_MARKER_RADIUS, SEED_MARKER_COLOR, distance);
    }
}

void apply_next_seed_color(Color32 next_seed_color, distance_fn distance)
{
    Point next_seed = color_to_point(next_seed_color);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            Point curr_seed = color_to_point(image[y][x]);
            int curr_dist = distance(curr_seed.x, curr_seed.y, x, y);
            int next_dist = distance(next_seed.x, next_seed.y, x, y);
            if (next_dist < curr_dist) {
                image[y][x] = next_seed_color;
            }
        }
    }
}

void render_voronoi_v0(distance_fn distance)
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int j = 0;
            for (int i = 1; i < SEED_COUNT; ++i) {
                Point prev = seeds[j];
                Point curr = seeds[i];
                int curr_dist = distance(curr.x, curr.y, x, y);
                int prev_dist = distance(prev.x, prev.y, x, y);
                if (curr_dist < prev_dist) {
                    j = i;
                }
            }
            image[y][x] = palette[j % palette_count];
        }
    }
}


void render_voronoi_v1(distance_fn distance) 
{
    fill_image(point_to_color(seeds[0]));
    for (size_t i = 1; i < SEED_COUNT; ++i) {
        apply_next_seed_color(point_to_color(seeds[i]), distance);
    }
}

int main(void) {
    srand(time(0));

    fill_image(COLOR_BACKGROUND);

    generate_seeds_random();

    distance_fn distance = &euclidean;
    render_voronoi_v0(distance);
    // render_voronoi_v1(distance);

    render_seeds_marker(distance);

    save_image_as_ppm("output.ppm");
    return 0;
}

void save_image_as_ppm(const char *path)
{
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        nob_log(NOB_ERROR, "Could not open file %s for writing: %s\n", path, strerror(errno));
        exit(1);
    }

    fprintf(f, "P6\n%d %d 255\n", WIDTH, HEIGHT);
    for (size_t y = 0; y < HEIGHT; ++y) {
        for (size_t x = 0; x < WIDTH; ++x) {
            uint32_t pixel = image[y][x];
            uint8_t bytes[3] = {
                (pixel & COMPONENT_MASK_RED)   >> 8*0, 
                (pixel & COMPONENT_MASK_GREEN) >> 8*1,
                (pixel & COMPONENT_MASK_BLUE)  >> 8*2,
            };
            fwrite(bytes, sizeof(bytes), 1, f);
        }
    }

    fclose(f);
}

