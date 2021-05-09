#pragma once
// IO and File streams
#include <iostream>
#include <fstream>
#include <sstream>
// SFML
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
// Other Includes
#include "Ray.h"
#include "Camera.h"
#include "World.h"
#include "Triangle.h"

const int MAX_DEPTH = 5;

class RayTracer
{
private:
	int raysCast = 0;
	int samples_per_pixel;
	Camera* camera;
	World* world;
	double ld_max;
	bool whichTR;

	color3 Illuminate(Ray ray);
	double ComputeReflectance(vec3 view, vec3 normal, double n1, double n2);
	double LogAverageLuminance(double** luminances, int rows, int cols);
	void Compression(double lwa, int rows, int cols, color3** radiances);

	void WriteColorPPM(ostream& out, color3 pixel_color);
	void WriteColorSFML(/*Image image,*/ color3 pixel_color, int x, int y);

public:
	RayTracer(); // Default constructor
	RayTracer(Camera* _camera, World* _world); // Parameterized constructor
	~RayTracer(); // Destructor

	void RenderPPM(const char* filename);
	void RenderSFML(const char* filename);
	void ReadBunny(Material* bunny_mat);
};

inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

inline std::string FormatWithCommas(int value) {
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << value;
	return ss.str();
}

inline void IgnoreLines(std::ifstream* inputFile, int numLines) {
	char* line = new char[64];
	for (int l = 0; l < numLines; l++) {
		inputFile->getline(line, 64); // ignore
	}
	delete[] line;
}
