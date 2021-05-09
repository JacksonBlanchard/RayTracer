#include "RayTracer.h"

RayTracer::RayTracer()
{
	samples_per_pixel = 16;
	camera = nullptr;
	world = nullptr;
	ld_max = 100;
	whichTR = true;
}

RayTracer::RayTracer(Camera* _camera, World* _world) : RayTracer()
{
	camera = _camera;
	world = _world;
}

RayTracer::~RayTracer()
{
	camera = nullptr;
	world = nullptr;
}

color3 RayTracer::Illuminate(Ray ray) {
	raysCast += 1;
	color3 pixel_color;

	// if we exceed the ray bounce limit, no more light gathered
	if (ray.Depth() > MAX_DEPTH)
		return color3();

	// check ray for intersection
	Intersection i = world->spawn(ray);

	// if no intersection, return background color
	if (!i.hit()) {
		return world->bgColor;
	}

	// add color from direct illumination
	if(i.Outside()) {
		pixel_color += world->spawnShadow(i);
	}

	// add color from indirect illumination
	double kr = i.EndMat()->Kr();
	double kt = i.EndMat()->Kt();

	// if not refractive or reflective don't proceed
	if (kr == 0 && kt == 0) {
		return pixel_color;
	}

	double n1 = i.RayMat()->RefractiveIndex();
	double n2 = i.EndMat()->RefractiveIndex();
	
	// compute reflection and transmission directions
	vec3 reflect_vec = reflectView(i.View(), i.Normal());
	vec3 refract_vec = refractView(i.View(), i.Normal(), n1 / n2);

	// compute reflactance coefficient
	if (kr == -1) {
		kr = ComputeReflectance(i.View(), i.Normal(), n1, n2);
	}

	// recurse with reflection ray
	if (kr > 0) {
		Ray reflection(i.Point(), reflect_vec, i.RayMat(), ray.Depth() + 1); // reflection ray maintains ray material
		pixel_color += kr * Illuminate(reflection);
	}

	// recurse with refraction ray
	if (kt > 0) {
		Ray refraction;
		if (reflect_vec == refract_vec) {
			// total internal reflection keeps object material
			refraction = Ray(i.Point(), reflect_vec, i.RayMat(), ray.Depth() + 1);
		}
		else { 
			// otherwise ray crosses into intersection material
			refraction = Ray(i.Point(), refract_vec, i.EndMat(), ray.Depth() + 1);
		}
		pixel_color += kt * Illuminate(refraction);
	}

	return pixel_color;
}

double RayTracer::ComputeReflectance(vec3 view, vec3 normal, double n1, double n2) {
	double eta = n1 / n2;
	double cos_t = dot(normal, view);
	double sin_t2 = pow(eta, 2) * (1 - pow(cos_t, 2));

	// total internal reflection
	if (sin_t2 > 1.0) {
		return 1;
	}

	double r0 = pow((n1 - n2) / (n1 + n2), 2);
	double rtheta = r0 + (1 - r0) * pow((1 - cos_t), 5);
	return rtheta; // reflectance coefficient as a function of incident angle theta

	//double cos_t = sqrt(1 - sin_t2);
	//double r0rth = (ki * cos_i - kt * cos_t) / (ki * cos_i + kt * cos_t);
	//double rPar = (kt * cos_i - ki * cos_t) / (kt * cos_i + ki * cos_t);
	//return (r0rth * r0rth + rPar * rPar) / 2.0;
}

void RayTracer::RenderPPM(const char* filename) {
	const int img_width = camera->GetImageWidth();
	const int img_height = camera->GetImageHeight();
	double max_lum = -1;
	double min_lum = 999999;

	// create 2D array of pixel irradiance values (color)
	color3** pixel_colors = new color3*[img_height];
	for (int i = 0; i < img_height; i++)
		pixel_colors[i] = new color3[img_width];

	// create 2D array of pixel luminance values
	double** pixel_luminances = new double*[img_height];
	for (int i = 0; i < img_height; i++)
		pixel_luminances[i] = new double[img_width];

	// create PPM image file using input filename
	std::ofstream outputFile;
	outputFile.open(filename);
	/* add the first 3 lines to the file:
	P3
	400 200
	255
	*/
	outputFile << "P3\n" << img_width << " " << img_height << "\n255\n";

	// loop over pixel rows from top to bottom
	for (int j = 0; j < img_height; j++) {
		std::cerr << "\rScanlines remaining: " << j << " " << std::flush; // print progress

		// loop over pixel columns from left to right
		for (int i = 0; i < img_width; i++) {

			// sample multiple times per pixel
			for (int s = 0; s < samples_per_pixel; s++) {

				double u, v;
				if (samples_per_pixel == 1) { // go to middle of pixel
					u = double(i + 0.5) / ((double)img_width - 1);
					v = double(j + 0.5) / ((double)img_height - 1);
				}
				else { // add a small random double to each pixel index
					u = double(i + random_double()) / ((double)img_width - 1);
					v = double(j + random_double()) / ((double)img_height - 1);
				}

				Ray ray = camera->getRay(u, v);
				ray.startMaterial = world->defaultMaterial;

				// add the color of each ray to the total pixel color
				pixel_colors[j][i] += Illuminate(ray);
			}

			// quick approximation to pixel illuminance
			pixel_luminances[j][i] = (0.27 * pixel_colors[j][i].x()) + (0.67 + pixel_colors[j][i].y()) + (0.06 * pixel_colors[j][i].z());
			//pixel_luminances[j][i] = (0.2126 * pixel_colors[j][i].x()) + (0.7152 + pixel_colors[j][i].y()) + (0.0722 * pixel_colors[j][i].z());

			if (pixel_luminances[j][i] > max_lum)
				max_lum = pixel_luminances[j][i];
			if (pixel_luminances[j][i] < min_lum)
				min_lum = pixel_luminances[j][i];
		}
	}

	// compute log average luminance
	double la = LogAverageLuminance(pixel_luminances, img_height, img_width);

	// TONE REPRODUCTION COMPRESSION (converts to range [0, Ld_max])
	//Compression(la, img_height, img_width, pixel_colors);

	// loop over pixel rows from top to bottom
	for (int j = 0; j < img_height; j++) {
		// loop over pixel columns from left to right
		for (int i = 0; i < img_width; i++) {
			//pixel_colors[j][i] /= pixel_luminances[j][i];
			// write the pixel color to the PPM file
			WriteColorPPM(outputFile, pixel_colors[j][i]);
		}
	}

	// delete 2D arrays
	for (int i = 0; i < img_height; i++) {
		delete[] pixel_colors[i];
		delete[] pixel_luminances[i];
	}
	delete[] pixel_colors;
	delete[] pixel_luminances;

	// done rendering
	std::cerr << "\nDone.\n";

	// print rays cast
	cout << "Rays cast: " << FormatWithCommas(raysCast) << endl;

	// print min and max luminance from scene
	//std::printf("Max Luminance: %f\n", max_lum);
	//std::printf("Min Luminance: %f\n", min_lum);

	outputFile.close();
}

void RayTracer::WriteColorPPM(ostream& out, color3 pixel_color) {
	// divide pixel color channel by the number of samples (average color)
	pixel_color /= samples_per_pixel;

	// TONE REPRODUCTION COLOR MANAGEMENT
	//pixel_color /= ld_max; // range is now [0,1]

	// write each [0,255] rgb color component to the out stream (PPM image file)
	out << (int)(256 * clamp(pixel_color.x(), 0.0, 0.999)) << " "
		<< (int)(256 * clamp(pixel_color.y(), 0.0, 0.999)) << " "
		<< (int)(256 * clamp(pixel_color.z(), 0.0, 0.999)) << "\n";
}

void RayTracer::RenderSFML(const char* filename) {
	// potentially render (display?) image using SFML RenderWindow
	/*
	sf::Texture texture;
	texture.loadFromImage(image);
	sf::Sprite sprite;
	sprite.setTexture(texture, true);
	*/
}

void RayTracer::WriteColorSFML(/*Image image,*/ color3 pixel_color, int x, int y) {
	// write each [0,255] rgb color component to 
	/*
	Color pix_color = Color(pixel_color[0], pixel_color[1], pixel_color[2]);
	image.setPixel(x, y, pix_color);
	*/
}

double RayTracer::LogAverageLuminance(double** luminances, int rows, int cols)
{
	double la = 0;
	double delta = 0.00001;
	for (int j = 0; j < rows; j++) {
		for (int i = 0; i < cols; i++) {
			la += log10(delta + luminances[j][i]);
		}
	}
	la /= double(rows) * double(cols);
	la = exp(la);
	return la;
}

void RayTracer::Compression(double lwa, int rows, int cols, color3** radiances)
{
	if (whichTR) {
		// Ward - heuritsic model based on perceptual tests
		double num = 1.219 + pow(ld_max / 2, 0.4);
		double den = 1.219 + pow(lwa, 0.4); // lwa is adaptive luminance
		double sf = pow(num / den, 2.5); // scale factor

		for (int j = 0; j < rows; j++) {
			for (int i = 0; i < cols; i++) {
				// apply scale factor to each pixel
				radiances[j][i] *= sf;
			}
		}
	}
	else {
		// Reinhard - heuristic model based on photographic systems
		for (int j = 0; j < rows; j++) {
			for (int i = 0; i < cols; i++) {
				radiances[j][i] *= (0.18 / lwa);
				color3 t = radiances[j][i];
				color3 temp = color3(t.x() / (1 + t.x()), t.y() / (1 + t.y()), t.z() / (1 + t.z()));
				radiances[j][i] = temp * ld_max;
			}
		}
	}
}

void RayTracer::ReadBunny(Material* bunny_mat)
{
	// bunny PLY file
	const char* filename = "../bunny/reconstruction/bun_zipper.ply";
	size_t max_line = 64; // shouldn't be more than 64

	// create image file stream using bunny filename
	std::ifstream inputFile;
	inputFile.open(filename);
	char* line = new char[max_line];


	/* Get number of vertices (from header line 4) */

	int num_vertices;
	IgnoreLines(&inputFile, 3); // ignore the first 3 lines
	// read in the header line and create a string stream
	inputFile.getline(line, max_line);
	istringstream ss1(line);
	// read and save the number of vertices
	string ignore;
	ss1 >> ignore >> ignore >> num_vertices;	

	/* Get number of faces (from header line 10) */

	int num_faces;
	IgnoreLines(&inputFile, 5); // ignore 5 more lines
	// read in the header line and create a string stream
	inputFile.getline(line, max_line);
	istringstream ss2(line);
	// read and save the number of faces
	ss2 >> ignore >> ignore >> num_faces;

	// skip remaining header lines until vertex lines
	IgnoreLines(&inputFile, 2);


	/* Read in all vertices */

	vec3* vertices = new vec3[num_vertices];
	
	for (int l = 0; l < num_vertices; l++) {
		// read in the vertex line and create a string stream
		inputFile.getline(line, max_line);
		istringstream ss(line);

		// read and save the points of the vertex
		double x, y, z;
		ss >> x >> y >> z;
		vertices[l] = vec3(x, y, z); // TODO: opposite Z???
	}

	/* Read and create all triangle face objects */

	// create faces from vertices
	while (inputFile.getline(line, max_line)) {
		istringstream ss(line);

		int ignore;
		int vLine1, vLine2, vLine3;
		ss >> ignore >> vLine1 >> vLine2 >> vLine3;

		// get vertices and scale by 10
		vec3 v1 = vertices[vLine1] * 10;
		vec3 v2 = vertices[vLine2] * 10;
		vec3 v3 = vertices[vLine3] * 10;
		
		// create Triangle object and add it to the world
		Triangle tri = Triangle(v1, v2, v3, bunny_mat);
		//faces.push_back(make_shared<Triangle>(tri));
		world->addObject(make_shared<Triangle>(tri));
	}

	cout << "Triangles: " << num_faces << endl;

	delete[] line;
}

