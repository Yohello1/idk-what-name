#include <SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iomanip>

// --- Configuration ---
const int BUFFER_WIDTH = 720;
const int BUFFER_HEIGHT = 720;
const int BYTES_PER_PIXEL = 3; // R, G, B
const int SCREEN_SCALE = 1;

const int DISTANCE_BETWEEN_POINTS = 8;
constexpr int POINTS_WIDTH = (BUFFER_WIDTH/DISTANCE_BETWEEN_POINTS) + 1;
constexpr int POINTS_HEIGHT = (BUFFER_HEIGHT/DISTANCE_BETWEEN_POINTS) + 1;
constexpr int POINTS_AMT = (POINTS_HEIGHT)*(POINTS_HEIGHT);

const int FLOATER_AMT = 10000;
const int FLOATER_SPEED = 3;

const int CAPACITY = 1024;

const uint16_t THRESHOLD = 180;

// Query radius - only check floaters within this distance
const int QUERY_RADIUS = 128;

uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];

struct point
{
    uint16_t x;
    uint16_t y;

    uint16_t i_x;
    uint16_t i_y;

    uint16_t id;
    uint16_t strength;

};

struct floater {
    float x;
    float y;

    float v_x;
    float v_y;

    uint8_t r;
    uint8_t b;
};

struct quadNode
{
    int x, y; // top left
    int height, width;

    int particles_amt;

    std::vector<floater*> particles;

    // 1 2
    // 3 4
    quadNode* children[4];

    // Neighbor indices:
    // 0 = top
    // 1 = right
    // 2 = bottom
    // 3 = left
    quadNode* _neighbour[4];
    quadNode* parent;

    quadNode() : parent(nullptr), particles_amt(0)
    {
        particles.reserve(CAPACITY*2);
        std::memset(children, 0, sizeof(children));
        std::memset(_neighbour, 0, sizeof(_neighbour));
    }

    ~quadNode()
    {
        if(children[0] != 0)
        {
            delete children[0];
            delete children[1];
            delete children[2];
            delete children[3];
        }
    }

    size_t size()
    {
        return particles.size();
    }

    size_t area()
    {
        return height*width;
    }

    bool contains(int px, int py) const
    {
        return px >= x && px < x + width && py >= y && py < y + height;
    }

    // Check if a rectangular region intersects with this node
    bool intersects(int qx, int qy, int qw, int qh) const
    {
        return !(qx + qw < x || qx > x + width || qy + qh < y || qy > y + height);
    }

    // Calculate strength contribution from floaters within a rectangular region
    // This directly accumulates strength instead of building a list
    void calculateStrengthInRegion(int qx, int qy, int qw, int qh, int px, int py, float& strength)
    {
        // If this node doesn't intersect the query region, return
        if (!intersects(qx, qy, qw, qh)) {
            return;
        }

        // If this is a leaf node, process all particles
        if (children[0] == nullptr) {
            const float qx_max = qx + qw;
            const float qy_max = qy + qh;
            const float qx_f = static_cast<float>(qx);
            const float qy_f = static_cast<float>(qy);
            const float px_f = static_cast<float>(px);
            const float py_f = static_cast<float>(py);

            for (floater* f : particles) {
                // Check if floater is in query region
                if (f->x >= qx_f && f->x < qx_max && f->y >= qy_f && f->y < qy_max) {
                    float dx = f->x - px_f;
                    float dy = f->y - py_f;
                    float dist_sq = dx*dx + dy*dy;

                    if (dist_sq > 0.1f) { // Avoid division by zero
                        float val = (f->r + f->b) / dist_sq;
                        strength += val;
                    }
                }
            }
        } else {
            // Internal node - recurse to children
            for (int i = 0; i < 4; i++) {
                children[i]->calculateStrengthInRegion(qx, qy, qw, qh, px, py, strength);
            }
        }
    }

    // Check which neighbor (if any) should contain this particle
    // Returns: 0=top, 1=right, 2=bottom, 3=left, -1=stays here
    int getNeighborDirection(floater* p) const
    {
        // Check if particle moved outside bounds
        if (p->y < y) return 0; // moved up -> top neighbor
        if (p->x >= x + width) return 1; // moved right -> right neighbor
        if (p->y >= y + height) return 2; // moved down -> bottom neighbor
        if (p->x < x) return 3; // moved left -> left neighbor

        return -1; // still inside this node
    }

    int getChildIndex(floater* p) const
    {
        int mid_x = x + width / 2;
        int mid_y = y + height / 2;

        int idx = 0;

        // Check X position (Right half? Add 1)
        if (p->x >= mid_x) {
            idx += 1;
        }

        // Check Y position (Bottom half? Add 2)
        if (p->y >= mid_y) {
            idx += 2;
        }

        return idx;
    }

    // Move particle to the appropriate neighbor
    // Returns true if successfully moved, false if particle is out of tree bounds
    bool moveParticleToNeighbor(floater* p, int direction)
    {
        if (direction < 0 || direction > 3) return false;

        quadNode* neighbor = _neighbour[direction];
        if (neighbor == nullptr) {
            // No neighbor in this direction - particle is out of bounds
            return false;
        }

        // If neighbor has children, find the correct child
        if (neighbor->children[0] != nullptr) {
            int childIdx = neighbor->getChildIndex(p);
            neighbor->children[childIdx]->particles.push_back(p);
        } else {
            neighbor->particles.push_back(p);
        }

        return true;
    }

    // Process all particles and move those that have left this node
    void updateParticlePositions()
    {
        if (children[0] != nullptr) {
            // Internal node - recurse to children
            for (int i = 0; i < 4; i++) {
                children[i]->updateParticlePositions();
            }
            return;
        }

        // Leaf node - check all particles
        auto it = particles.begin();
        while (it != particles.end()) {
            floater* p = *it;

            int direction = getNeighborDirection(p);

            if (direction >= 0) {
                // Particle moved to a neighbor
                if (moveParticleToNeighbor(p, direction)) {
                    it = particles.erase(it);
                } else {
                    // Out of tree bounds - you may want to handle this differently
                    ++it;
                }
            } else {
                ++it;
            }
        }

        particles_amt = particles.size();
    }

    void setupNeighbors()
    {
        if (children[0] != nullptr) {
            // Setup parent pointers
            for (int i = 0; i < 4; i++) {
                children[i]->parent = this;
            }

            // Internal neighbor connections between siblings
            // 0 = top-left, 1 = top-right, 2 = bottom-left, 3 = bottom-right

            // Horizontal neighbors (left-right)
            children[0]->_neighbour[1] = children[1]; // 0's right is 1
            children[1]->_neighbour[3] = children[0]; // 1's left is 0
            children[2]->_neighbour[1] = children[3]; // 2's right is 3
            children[3]->_neighbour[3] = children[2]; // 3's left is 2

            // Vertical neighbors (top-bottom)
            children[0]->_neighbour[2] = children[2]; // 0's bottom is 2
            children[2]->_neighbour[0] = children[0]; // 2's top is 0
            children[1]->_neighbour[2] = children[3]; // 1's bottom is 3
            children[3]->_neighbour[0] = children[1]; // 3's top is 1

            // Propagate external neighbors to children
            // Top edge children inherit top neighbor
            if (_neighbour[0]) {
                children[0]->_neighbour[0] = _neighbour[0];
                children[1]->_neighbour[0] = _neighbour[0];
            }
            // Right edge children inherit right neighbor
            if (_neighbour[1]) {
                children[1]->_neighbour[1] = _neighbour[1];
                children[3]->_neighbour[1] = _neighbour[1];
            }
            // Bottom edge children inherit bottom neighbor
            if (_neighbour[2]) {
                children[2]->_neighbour[2] = _neighbour[2];
                children[3]->_neighbour[2] = _neighbour[2];
            }
            // Left edge children inherit left neighbor
            if (_neighbour[3]) {
                children[0]->_neighbour[3] = _neighbour[3];
                children[2]->_neighbour[3] = _neighbour[3];
            }

            // Recurse to setup deeper levels
            for (int i = 0; i < 4; i++) {
                children[i]->setupNeighbors();
            }
        }
    }

    void update()
    {
        if(particles.size() > CAPACITY && children[0] == nullptr)
        {
            // create 4 quad trees
            children[0] = new quadNode();
            children[1] = new quadNode();
            children[2] = new quadNode();
            children[3] = new quadNode();

            // Compute the 4 corners + lengths widths
            int mid_x = x + width / 2;
            int mid_y = y + height / 2;

            // Top-left
            children[0]->x = x;
            children[0]->y = y;
            children[0]->width = width / 2;
            children[0]->height = height / 2;

            // Top-right
            children[1]->x = mid_x;
            children[1]->y = y;
            children[1]->width = width - width / 2;
            children[1]->height = height / 2;

            // Bottom-left
            children[2]->x = x;
            children[2]->y = mid_y;
            children[2]->width = width / 2;
            children[2]->height = height - height / 2;

            // Bottom-right
            children[3]->x = mid_x;
            children[3]->y = mid_y;
            children[3]->width = width - width / 2;
            children[3]->height = height - height / 2;

            std::vector<floater*> temp_particles;
            temp_particles.swap(particles);

            for(floater* p : temp_particles)
            {
                int idx = getChildIndex(p);
                children[idx]->particles.push_back(p);
            }

            setupNeighbors();

            for(int i = 0; i < 4; i++)
            {
                children[i]->update();
            }

            particles.clear();
            particles.reserve(CAPACITY);
        }

        if(children[0] != nullptr)
        {
            size_t sum = 0;
            for(int i = 0; i < 4; i++)
            {
                sum += children[i]->particles_amt;
            }

            particles_amt = sum;

            if(sum < CAPACITY)
            {
                children[0]->merge(particles);
                children[1]->merge(particles);
                children[2]->merge(particles);
                children[3]->merge(particles);

                delete children[0];
                delete children[1];
                delete children[2];
                delete children[3];

                std::memset(children, 0, sizeof(children));
            }
        }
    }

    // Takes in parent's array, and adds its particles to the particles
    void merge(std::vector<floater*>& vec)
    {
        if(children[0] != nullptr)
        {
            children[0]->merge(vec);
            children[1]->merge(vec);
            children[2]->merge(vec);
            children[3]->merge(vec);

            delete children[0];
            delete children[1];
            delete children[2];
            delete children[3];

            memset(children, 0, sizeof(children));
        }

        for(auto &p : particles)
        {
            vec.push_back(p);
        }
    }

    // Recursively update particle counts from leaf to root
    void updateParticleCount()
    {
        if (children[0] != nullptr) {
            // Internal node - sum children counts
            particles_amt = 0;
            for (int i = 0; i < 4; i++) {
                children[i]->updateParticleCount();
                particles_amt += children[i]->particles_amt;
            }
        } else {
            // Leaf node - count is the size of particles vector
            particles_amt = particles.size();
        }
    }
};


struct point*   points = new struct point[POINTS_AMT];
struct floater* floaters = new struct floater[FLOATER_AMT];

int signBit(int a) { return a/abs(a); };
float fsignBit(float a) { return a/abs(a); };

// --- Drawing function that fills the static array ONCE ---
void InitializeStaticBuffer() {
    for (int y = 0; y < BUFFER_HEIGHT; ++y) {
        for (int x = 0; x < BUFFER_WIDTH; ++x) {

            // Calculate the starting index for the R, G, B components of the current pixel
            int buffer_index = (y * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;

            // --- Static Black-to-Blue Gradient Calculation ---

            // Calculate blue component based on X position (0 to 255)
            uint8_t blue = 0;

            uint8_t red = 0;   // Static Red component
            uint8_t green = 0; // Static Green component

            // Store the RGB components in the array
            // Index + 0: Red
            static_rgb_buffer[buffer_index + 0] = red;
            // Index + 1: Green
            static_rgb_buffer[buffer_index + 1] = green;
            // Index + 2: Blue
            static_rgb_buffer[buffer_index + 2] = blue;
        }
    }
}

std::pair<int, int> get_mid_point(const point& p0, const point& p1)
{
    return {(p0.i_x + p1.i_x) / 2, (p0.i_y + p1.i_y) / 2};
}

void draw_line_std_pair(uint8_t* buffer,
                        std::pair<int, int> p0,
                        std::pair<int, int> p1,
                        uint8_t r, uint8_t g, uint8_t b) {

    int x0 = p0.first;
    int y0 = p0.second;
    int x1 = p1.first;
    int y1 = p1.second;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    int x = x0;
    int y = y0;

    while (true) {
        if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT) {
            int index = (y * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;

            static_rgb_buffer[index + 0] = 255;
            static_rgb_buffer[index + 1] = 255;
            static_rgb_buffer[index + 2] = 255;
        }

        if (x == x1 && y == y1) break;

        e2 = err;

        if (e2 > -dx) {
            err -= dy;
            x += sx;
        }

        if (e2 < dy) {
            err += dx;
            y += sy;
        }
    }
}

void initFloaters()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        floaters[i].x = rand() % BUFFER_WIDTH-1;
        floaters[i].y = rand() % BUFFER_HEIGHT-1;

        floaters[i].v_x = ((rand() % 50) - 25)/10.0f;
        floaters[i].v_y = ((rand() % 50) - 25)/10.0f;

        floaters[i].r = (rand() % 127)+128;
        floaters[i].b = (rand() % 127)+128;
    }
}

void initGrid()
{
    for(int i = 0; i < POINTS_AMT; i++)
    {
        int x = (i%POINTS_WIDTH);
        int y = (i/POINTS_HEIGHT);

        points[i].x = x;
        points[i].y = y;

        points[i].i_x = x*DISTANCE_BETWEEN_POINTS;
        points[i].i_y = y*DISTANCE_BETWEEN_POINTS;

        points[i].id = i;

        points[i].strength = rand() % 256;

    }
}


void drawFloaters()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        int idx = (int)floaters[i].x*BYTES_PER_PIXEL + (int)floaters[i].y*BUFFER_WIDTH*BYTES_PER_PIXEL;
        if (idx >= 0 && idx < BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL - 2) {
            static_rgb_buffer[idx]   = floaters[i].r;
            static_rgb_buffer[idx+1] = floaters[i].b;
        }
    }
}

void simulateFloaters()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        floaters[i].x += floaters[i].v_x;
        floaters[i].y += floaters[i].v_y;


        if (floaters[i].x < 0.0f)
        {
            floaters[i].v_x *= -1.0f;
            floaters[i].x = 0.0f;
        }


        else if (floaters[i].x > (BUFFER_WIDTH-1))
        {
            floaters[i].v_x *= -1.0f;
            floaters[i].x = (BUFFER_WIDTH-1);
        }


        if (floaters[i].y < 0.0f)
        {
            floaters[i].v_y *= -1.0f;
            floaters[i].y = 0.0f;
        }

        else if (floaters[i].y > (BUFFER_HEIGHT-1))
        {
            floaters[i].v_y *= -1.0f;
            floaters[i].y = (BUFFER_HEIGHT-1);
        }
    }
}

 void strengthCalc(quadNode* qtree)
{
    int offset = DISTANCE_BETWEEN_POINTS/2;
    const int SQR_PER_ROW = POINTS_WIDTH - 1;
    const int SQR_PER_COL = POINTS_WIDTH - 1;

    // Calculate strengths using quadtree queries
    for(int i = 0; i < POINTS_WIDTH; i++)
    {
        for(int j = 0; j < POINTS_HEIGHT; j++)
        {
            int idx = POINTS_WIDTH*i+j;

            float strength = 0.0f;

            int x = points[idx].i_x;
            int y = points[idx].i_y;

            // Define query region centered on point
            int qx = std::max(0, x - QUERY_RADIUS);
            int qy = std::max(0, y - QUERY_RADIUS);
            int qw = std::min(BUFFER_WIDTH - qx, 2 * QUERY_RADIUS);
            int qh = std::min(BUFFER_HEIGHT - qy, 2 * QUERY_RADIUS);

            // Calculate strength directly without building a vector
            qtree->calculateStrengthInRegion(qx, qy, qw, qh, x, y, strength);

            points[idx].strength = strength;

            //std::cout << (strength > THRESHOLD) ? '1' : ' ';
        }
    }
}

void drawConnections(quadNode* qtree)
{
    int offset = DISTANCE_BETWEEN_POINTS/2;

    const int SQR_PER_ROW = POINTS_WIDTH - 1;
    const int SQR_PER_COL = POINTS_WIDTH - 1;

    strengthCalc(qtree);

    //std::cout << '\n';

    // Draw marching squares
    for(int i = 0; i < SQR_PER_ROW; i++)
    {
        for(int j = 0; j < SQR_PER_COL; j++)
        {
            // 0 - 1
            // - X -
            // 2 - 3

            // 0 = sqr_per_row*row         + col
            // 1 = sqr_per_row*row     + 1 + col
            // 2 = (sqr_per_row+1)*row     + col
            // 3 = (sqr_per_row+1)*row + 1 + col

            int idx_0 = POINTS_WIDTH*i + j;
            int idx_1 = POINTS_WIDTH*i + j + 1;
            int idx_2 = POINTS_WIDTH*(1+i) + j;
            int idx_3 = POINTS_WIDTH*(1+i) + j + 1;

            bool idx_0_b = (points[idx_0].strength >= THRESHOLD);
            bool idx_1_b = (points[idx_1].strength >= THRESHOLD);
            bool idx_2_b = (points[idx_2].strength >= THRESHOLD);
            bool idx_3_b = (points[idx_3].strength >= THRESHOLD);

            int config_index = 0;
            if (idx_0_b) config_index |= 1;
            if (idx_1_b) config_index |= 2;
            if (idx_3_b) config_index |= 4;
            if (idx_2_b) config_index |= 8;

            std::pair<int, int> pA = get_mid_point(points[idx_0], points[idx_1]);
            std::pair<int, int> pB = get_mid_point(points[idx_1], points[idx_3]);
            std::pair<int, int> pC = get_mid_point(points[idx_3], points[idx_2]);
            std::pair<int, int> pD = get_mid_point(points[idx_2], points[idx_0]);

            int R = 255, G = 255, B = 255;

            switch (config_index)
            {
                case 0:
                case 15:
                    break;

                case 1: draw_line_std_pair(static_rgb_buffer, pA, pD, R, G, B); break; // P0
                case 2: draw_line_std_pair(static_rgb_buffer, pA, pB, R, G, B); break; // P1
                case 4: draw_line_std_pair(static_rgb_buffer, pB, pC, R, G, B); break; // P3
                case 8: draw_line_std_pair(static_rgb_buffer, pC, pD, R, G, B); break; // P2

                case 14: draw_line_std_pair(static_rgb_buffer, pA, pD, R, G, B); break;
                case 13: draw_line_std_pair(static_rgb_buffer, pA, pB, R, G, B); break;
                case 11: draw_line_std_pair(static_rgb_buffer, pB, pC, R, G, B); break;
                case 7: draw_line_std_pair(static_rgb_buffer, pC, pD, R, G, B); break;

                case 3: draw_line_std_pair(static_rgb_buffer, pD, pB, R, G, B); break;
                case 12: draw_line_std_pair(static_rgb_buffer, pD, pB, R, G, B); break;

                case 6: draw_line_std_pair(static_rgb_buffer, pA, pC, R, G, B); break;
                case 9: draw_line_std_pair(static_rgb_buffer, pA, pC, R, G, B); break;

                case 5:
                    draw_line_std_pair(static_rgb_buffer, pA, pB, R, G, B);
                    draw_line_std_pair(static_rgb_buffer, pC, pD, R, G, B);
                    break;
                case 10:
                    draw_line_std_pair(static_rgb_buffer, pA, pD, R, G, B);
                    draw_line_std_pair(static_rgb_buffer, pB, pC, R, G, B);
                    break;
            }

        }
    }
}

void drawGrid()
{
    for(int i = 0; i < POINTS_AMT; i++)
    {
        int idx = points[i].i_x*BYTES_PER_PIXEL + points[i].i_y*BUFFER_WIDTH*BYTES_PER_PIXEL;

        uint16_t strength = points[i].strength;

        if(!(strength >= THRESHOLD))
            strength = 0;

        if (idx >= 0 && idx < BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL - 2) {
            static_rgb_buffer[idx] = strength;
            static_rgb_buffer[idx+1] = strength;
            static_rgb_buffer[idx+2] = strength;
        }
    }
}

int main(int argc, char* argv[]) {

    srand(100);
    initGrid();
    initFloaters();

    // Initialize quadtree ONCE
    quadNode* qtree = new quadNode();
    qtree->x = 0;
    qtree->y = 0;
    qtree->width = BUFFER_WIDTH;
    qtree->height = BUFFER_HEIGHT;

    // Insert all floaters into quadtree initially
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        qtree->particles.push_back(&floaters[i]);
    }
    qtree->update();

    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 2. Create and fill the static array ONCE
    InitializeStaticBuffer();

    // 3. Create a window
    SDL_Window* window = SDL_CreateWindow(
        "SDL Static C-Style Array Buffer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        BUFFER_WIDTH * SCREEN_SCALE,
        BUFFER_HEIGHT * SCREEN_SCALE,
        SDL_WINDOW_SHOWN
    );

    // 4. Create an SDL_Surface that points to our C-style array
    // We use the array name 'static_rgb_buffer' which decays to a pointer (uint8_t*)
    SDL_Surface* bufferSurface = SDL_CreateRGBSurfaceFrom(
        static_rgb_buffer, // C-style array name acts as pointer to data
        BUFFER_WIDTH,
        BUFFER_HEIGHT,
        24, // Bits per pixel (3 bytes)
        BUFFER_WIDTH * BYTES_PER_PIXEL, // Pitch (bytes per row)
        0x00FF0000, // R-mask (standard little-endian 24-bit RGB)
        0x0000FF00, // G-mask
        0x000000FF, // B-mask
        0x00000000  // No Alpha
    );

    if (bufferSurface == nullptr) {
        std::cerr << "Could not create buffer surface! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 5. Get the destination (window) surface
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    // Main loop variables
    bool quit = false;
    SDL_Event e;

    clock_t start, end;

    // 6. Main application loop
    while (!quit)
    {
        start = clock();


        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        memset(static_rgb_buffer, 0, sizeof(uint8_t)*BUFFER_HEIGHT*BUFFER_WIDTH*BYTES_PER_PIXEL);

        simulateFloaters();

        // Update quadtree incrementally instead of rebuilding
        qtree->updateParticlePositions();  // Move particles that changed cells
        qtree->updateParticleCount();       // Update particle counts
        qtree->update();                    // Split/merge nodes as needed

        drawFloaters();
        drawConnections(qtree);

        // --- COPY (BLIT) STATIC BUFFER TO SCREEN ---
        // Copies the fixed gradient image onto the window surface.
        SDL_BlitSurface(bufferSurface, nullptr, screenSurface, nullptr);

        // 7. Update the window to display the fixed changes
        SDL_UpdateWindowSurface(window);

        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        std::cout << "Time taken by program is : " << std::fixed
        << time_taken << std::setprecision(8) << std::endl;

    }

    // 8. Clean up resources
}
