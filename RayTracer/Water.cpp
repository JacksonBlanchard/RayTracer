#include "Water.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
Water::Water() : Material()
{
	direction = vec3(0, 0, 1); // toward the camera
	wavelength = 4;
	speed = 1;
	steepness = 1;
	amplitude = 1;
}
*/

color3 Water::DiffuseColor(double u, double v)
{
    return color3();

    // TODO: for project
    /*
	//i.point = i.point + vec3(0, cos(v), 0);
	i.object_color = color3(0, 128/255.0, 128/255.0);
	i.specular_color = color3(1, 1, 1);

    // ... x = width, y = height, n = # 8-bit components per pixel ...
    // ... replace '0' with '1'..'4' to force that many components per pixel
    // ... but 'n' will always be the number that it would have been if you said 0
    string filename = "../Textures/water normal map2.png";
    int x, y, n;
    unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 0);

    // ... process data if not NULL ..
    if (data != nullptr && x > 0 && y > 0)
    {
        if (n == 3 || n == 4)
        {
            // std::cout << "First pixel: RGB "
            //     << static_cast<int>(data[0]) << " "
            //     << static_cast<int>(data[1]) << " "
            //     << static_cast<int>(data[2]);
            double x = (static_cast<int>(data[0])) / 255.0;
            double y = (static_cast<int>(data[1])) / 255.0;
            double z = (static_cast<int>(data[2])) / 255.0;
            i.normal = vec3(x, y, z);
        }
    }
    else
    {
        std::cout << "Some error\n";
    }

    stbi_image_free(data);
    */
}
