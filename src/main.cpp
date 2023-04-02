#include <iostream>
#include <random> // for random number generation
#include <cmath> // for math functions

#include "stb_image_write.h" // for saving image files

class PerlinNoise {
public:
    PerlinNoise(); // constructor
    double noise(double x, double y, double z); // calculate Perlin noise at given coordinates
private:
    int p[512]; // permutation table used to generate random gradient vectors
    double fade(double t); // smoothing function
    double lerp(double t, double a, double b); // linear interpolation function
    double grad(int hash, double x, double y, double z); // dot product of gradient vector and distance vector
};

PerlinNoise::PerlinNoise() {
    // initialize permutation table with random values between 0 and 255
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);
    for (int i = 0; i < 256; i++) {
        p[i] = p[256 + i] = dist(gen);
    }
}

// smoothing function
double PerlinNoise::fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// linear interpolation function
double PerlinNoise::lerp(double t, double a, double b) {
    return a + t * (b - a);
}

// dot product of gradient vector and distance vector
double PerlinNoise::grad(int hash, double x, double y, double z) {
    int h = hash & 15; // convert hash to 4-bit value (0-15)
    double u = h < 8 ? x : y; // if h is less than 8, set u to x, otherwise set it to y
    double v = h < 4 ? y : h == 12 || h == 14 ? x : z; // if h is less than 4, set v to y, otherwise check if h is 12 or 14 and set v to x or z accordingly
    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v); // calculate dot product using bitwise operations
}

// calculate Perlin noise at given coordinates
double PerlinNoise::noise(double x, double y, double z) {
    int X = (int)std::floor(x) & 255; // find grid cell coordinates of point
    int Y = (int)std::floor(y) & 255;
    int Z = (int)std::floor(z) & 255;
    x -= std::floor(x); // find fractional coordinates within grid cell
    y -= std::floor(y);
    z -= std::floor(z);
    double u = fade(x); // apply smoothing function to fractional coordinates
    double v = fade(y);
    double w = fade(z);
    int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z; // find indices of gradient vectors for each corner of the grid cell
    int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;
    return 	lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), // calculate dot products and interpolate between them
        	grad(p[BA], x - 1, y, z)),
            lerp(u, grad(p[AB], x, y - 1, z),
            grad(p[BB], x - 1, y - 1, z))),
            lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
            grad(p[BA + 1], x - 1, y, z - 1)),
			lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
			grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

int main() {
    const int width = 256; // set width and height of texture
    const int height = 256;
    std::vector<unsigned char> texture(width * height); // create vector to store texture
    PerlinNoise noise; // create PerlinNoise object
    double zoom = 25.0; // set zoom level
    for (int y = 0; y < height; y++) { // iterate over pixels in texture
        for (int x = 0; x < width; x++) {
            double nx = (double)x / width * zoom - 0.5 * zoom; // calculate x and y coordinates of point in space
            double ny = (double)y / height * zoom - 0.5 * zoom;
            double value = noise.noise(nx, ny, 0.0); // calculate Perlin noise value at point
            texture[y * width + x] = (unsigned char)(value * 255.0); // scale noise value and store in texture vector
        }
    }

	// Apply a simple smoothing filter to the texture
    const int filterSize = 3; // Change the filter size here
    std::vector<unsigned char> smoothedTexture(width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sum = 0;
            for (int fy = -filterSize/2; fy <= filterSize/2; fy++) {
                for (int fx = -filterSize/2; fx <= filterSize/2; fx++) {
                    int nx = std::min(std::max(x + fx, 0), width - 1);
                    int ny = std::min(std::max(y + fy, 0), height - 1);
                    sum += texture[ny * width + nx];
                }
            }
            smoothedTexture[y * width + x] = sum / (filterSize * filterSize);
        }
    }
	
    stbi_write_png("texture.png", width, height, 1, texture.data(), 0); // save texture to file using stb_image_write
    return 0;
}