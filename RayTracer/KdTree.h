#pragma once
#include <vector>
#include "vec3.h"
#include "Object.h"
#include "AABB.h"

const int MAX_TREE_DEPTH = 16;
const int MIN_OBJECTS = 64;

struct Plane {
	double val;
	int axis; // 0, 1, 2 for x, y, z respectively
};

class KdNode {
private:
	Plane split;
	AABB* bounds;
	KdNode* left;
	KdNode* right;
	vector<shared_ptr<Object>> objects;

public:
	KdNode();
	KdNode(double _val, int _axis, AABB* _bounds, vector<shared_ptr<Object>> _objects);
	~KdNode();

	void SetLeftRight(KdNode* _left, KdNode* _right) {
		left = _left;
		right = _right;
	}
	bool IsLeaf() { return left == NULL && right == NULL; }

	Intersection BoundsIntersect(Ray ray, double w_min, double w_max);
	Intersection ObjectIntersect(Ray ray, double w_min, double w_max);
};

class KdTree
{
private:
	KdNode* root;

public:
	KdTree(vector<shared_ptr<Object>> _objects);
	~KdTree();

	KdNode* MakeTree(int axis, vector<shared_ptr<Object>> objects, int depth);

	int ToggleAxis(int axis) {
		return (axis + 1) % 3;
	}

	Intersection getClosestIntersect(Ray ray, double w_min, double w_max) {
		return root->BoundsIntersect(ray, w_min, w_max);
	}
};

