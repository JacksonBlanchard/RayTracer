#pragma once
#include <vector>
#include "KdTree.h"
#include "LightSource.h"
#include "IlluminationModel.h"
#include "Air.h"
using namespace std;

class World
{
private:
	double w_min;
	double w_max;
	vector<shared_ptr<Object>> objects; // list of objects
	vector<shared_ptr<LightSource>> lights; // list of lights
	IlluminationModel* model;
	KdTree* root;

public:
	color3 bgColor;
	Material* defaultMaterial;

	World(IlluminationModel* _model); // parameterized constructor
	~World(); // destructor

	void addObject(shared_ptr<Object> object);
	void addLight(shared_ptr<LightSource> light);
	//void addBunny(Material* bunny_mat);

	Intersection spawn(Ray &ray);
	color3 spawnShadow(Intersection& hit);

	void buildTree();
};

