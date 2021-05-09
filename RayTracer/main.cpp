// Memory Management
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

// Header files
#include "vec3.h"
#include "Ray.h"
#include "RayTracer.h"
// Objects
#include "Object.h"
#include "Sphere.h"
#include "Triangle.h"
#include "World.h"
#include "Camera.h"
#include "LightSource.h"
// Illumination Models
#include "IlluminationModel.h"
#include "Phong.h"
#include "PhongBlinn.h"
// Materials
#include "Checkerboard.h"
#include "Glass.h"
#include "Water.h"

#include <chrono>
using namespace std::chrono;
using namespace std;

/* All coordinates are in World Space with +X right, +Y up, and -Z forward. */
void wrapper()
{
	// Image Size Constants
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	//const int image_height = 200;
	//const int image_width = 200;
	const char* filename = "image.ppm";

	/* OBJECTS */
	
	// Sphere 1
	Material sphere1_mat(0, 1, 1.333, color3(0.1,0.1,0.1), color3(1,1,1)); // transmissive water
	vec3 sphere1_pos(0, 4, 4); // opposite z
	Sphere sphere1(sphere1_pos, 2.2, &sphere1_mat);

	// Sphere 2
	Material sphere2_mat(1, color3(0.3,0.3,0.3)); // perfectly reflective
	vec3 sphere2_pos(4, 3, 0); // opposite z
	Sphere sphere2(sphere2_pos, 2.2, &sphere2_mat);

	// Floor
	Checkerboard floor_cb(4); // floor checkerboard material
	vec3 top_left_point(-4, 0, -10.5); // opposite z
	vec3 top_right_point(11.5, 0, -10.5); // opposite z
	vec3 bottom_left_point(-4, 0, 8); // opposite z
	vec3 bottom_right_point(11.5, 0, 8); // opposite z
	Triangle tri1(top_left_point, bottom_right_point, bottom_left_point, &floor_cb);
	Triangle tri2(top_left_point, top_right_point, bottom_right_point, &floor_cb);

	/* LIGHTS */

	// Point Light 1
	vec3 light1_pos(-1, 10, 12);
	vec3 light2_pos(1, 5, 6);
	color3 light1_color(1, 1, 1); // white
	double light1_power = 1.0;
	LightSource light1(light1_pos, light1_color, light1_power);

	/* CAMERA */

	// final render pos: (0, 4, 10) look at: (0, 3, 3)
	// bunny cam pos: (0, 1, 3) look at: (0, 1, 0);
	vec3 camera_pos = vec3(0, 4, 10);
	vec3 look_at = vec3(0, 3, 3);
	//vec3 camera_pos = vec3(0, 1, 3);
	//vec3 look_at = vec3(0, 1, 0);
	vec3 local_up = vec3(0, 1, 0);
	Camera camera(camera_pos, look_at, local_up, image_width, 0, 60, aspect_ratio);
	//Camera camera(camera_pos, look_at, local_up, image_width, image_height, 60, image_height / image_width);

	/* WORLD */

	// Illumination Models
	Phong phong(0.3, 0.7, 0.4, 3.0);
	PhongBlinn phong_blinn(0.3, 0.7, 0.4, 1.0);

	// World
	World world = World(&phong);
	/* hide objects for now, just render bunny
	*/
	world.addObject(make_shared<Sphere>(sphere1));
	world.addObject(make_shared<Sphere>(sphere2));
	world.addObject(make_shared<Triangle>(tri1));
	world.addObject(make_shared<Triangle>(tri2));

	world.addLight(make_shared<LightSource>(light1));

	/* RAY TRACE! */

	RayTracer rayTracer(&camera, &world);

	// Bunny

	// move bunny code? return vector of Tris
	//Material bunny_mat(color3(0.8, 0.8, 0.8), color3(1, 1, 1)); // light gray with white specular highlight
	//Material bunny_mat(0, 1, 1.333, color3(0.1, 0.1, 0.1), color3(1, 1, 1)); // light gray with white specular highlight
	//rayTracer.ReadBunny(&bunny_mat);

	// KdTree

	// time the building of the KdTree
	auto start = high_resolution_clock::now();

	// Build KdTree once all objects are added
	world.buildTree();

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("KdTree generation: %4.3f s\n", duration.count() / 1000.0);

	// Render

	// time the rendering
	start = high_resolution_clock::now();

	rayTracer.RenderPPM(filename);

	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);
	printf("Rendering duration: %4.3f s\n", duration.count() / 1000.0);


	return;
}

int main()
{
	wrapper();
	_CrtDumpMemoryLeaks();
} 