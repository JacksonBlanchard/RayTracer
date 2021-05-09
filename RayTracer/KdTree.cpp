#include "KdTree.h"

/* KdNode */

KdNode::KdNode()
{
	split = { 0, 'x' };
	bounds = nullptr;
	left = nullptr;
	right = nullptr;
	objects = {};
}

KdNode::KdNode(double _val, int _axis, AABB* _bounds, vector<shared_ptr<Object>> _objects)
{
	split = { _val, _axis };
	bounds = _bounds;
	objects = _objects;
}

KdNode::~KdNode()
{
	delete bounds;
	delete left;
	delete right;
}

Intersection KdNode::BoundsIntersect(Ray ray, double w_min, double w_max)
{
	double s = split.val;
	int axis = split.axis;
	double tmin, tmax;

	// check for ray intersection with boundaries
	if (!bounds->Intersect(ray, tmin, tmax)) {
		return Intersection();
	}

	if (IsLeaf()) {
		// if leaf node check for object intersection
		return ObjectIntersect(ray, w_min, w_max);
	}

	double a = ray.at(tmin)[axis]; // split coordinate of close intersection
	double b = ray.at(tmax)[axis]; // split coordinate of far intersection

	if (a <= s) {
		if (b < s) {		// N1, N2, N3, P5, Z3
			return left->BoundsIntersect(ray, w_min, w_max);
		}
		else {
			if (b == s) {	// Z2
				return left->BoundsIntersect(ray, w_min, w_max); // arbitrary
			}
			else {			// N4
				// compute and store location of "s"
				// visit left and right?
				Intersection leftIntersect = left->BoundsIntersect(ray, w_min, w_max);
				Intersection rightIntersect = right->BoundsIntersect(ray, w_min, w_max);

				// if both hit return the closer intersection
				if (leftIntersect.hit() && rightIntersect.hit()) {
					return leftIntersect.W() < rightIntersect.W() ? leftIntersect : rightIntersect;
				}
				else {
					// otherwise return whichever hit
					return leftIntersect.hit() ? leftIntersect : rightIntersect;
				}
			}
		}
	}
	else {
		if (b > s) {		// P1, P2, P3, N5, Z1
			return right->BoundsIntersect(ray, w_min, w_max);
		}
		else {				// P4
			// compute and store location of "s"
			// visit right and left
			Intersection rightIntersect = right->BoundsIntersect(ray, w_min, w_max);
			Intersection leftIntersect = left->BoundsIntersect(ray, w_min, w_max);

			// if both hit return the closer intersection
			if (leftIntersect.hit() && rightIntersect.hit()) {
				return leftIntersect.W() < rightIntersect.W() ? leftIntersect : rightIntersect;
			}
			else {
				// otherwise return whichever hit
				return leftIntersect.hit() ? leftIntersect : rightIntersect;
			}
		}
	}


	/*
	// check for ray intersection with child boundaries
	if (!IsLeaf()) {
		Intersection leftIntersect = left->BoundsIntersect(ray, w_min, w_max);
		Intersection rightIntersect = right->BoundsIntersect(ray, w_min, w_max);
		// return the closer intersection
		return leftIntersect.W() < rightIntersect.W() ? leftIntersect : rightIntersect;
	}
	else {
		// if leaf node check for object intersection
		return ObjectIntersect(ray, w_min, w_max);
	}
	*/
}

Intersection KdNode::ObjectIntersect(Ray ray, double w_min, double w_max)
{
	Intersection hit;
	double closest_so_far = w_max;

	// check for an intersection with every object in this node
	for (shared_ptr<Object> obj : objects) {

		// check for an intersection
		Intersection i = obj->intersect(ray, w_min, w_max);

		// if the ray intersected with an object
		if (i.hit()) {
			// store the closest intersection
			if (i.W() < closest_so_far) {
				closest_so_far = i.W();
				hit = i;
			}
		}
	}

	return hit;
}

/* KdTree */

KdTree::KdTree(vector<shared_ptr<Object>> _objects)
{
	root = MakeTree(0, _objects, 1);
}

KdTree::~KdTree()
{
	delete root;
}

KdNode* KdTree::MakeTree(int axis, vector<shared_ptr<Object>> objects, int depth)
{
	AABB* bounds = new AABB();
	KdNode* left = nullptr;
	KdNode* right = nullptr;

	// get encompassing bounding box for the list of objects
	for (shared_ptr<Object> obj : objects) {
		bounds->Encompass(obj->getMin(), obj->getMax());
	}

	// two or less objects then return leaf node OR we hit max depth
	if (objects.size() <= MIN_OBJECTS || depth >= MAX_TREE_DEPTH) {
		// split doesn't matter for leaf node
		return new KdNode(0, axis, bounds, objects);
	}

	// try to split on each axis
	for (int retries = 0; retries < 3; retries++) {

		// value of partitioning plane along current axis
		double splitVal = bounds->SplitValue(axis);

		// place objects in either left or right child nodes
		vector<shared_ptr<Object>> leftObjects;	
		vector<shared_ptr<Object>> rightObjects;

		// for each object place it in the left or right
		for (shared_ptr<Object> obj : objects) {
			double obj_min = obj->getMin()[axis];
			double obj_max = obj->getMax()[axis];

			// object is on left side of partition
			if (obj_min <= splitVal) {
				leftObjects.push_back(obj);
			}
			// object is on right side of partition
			if (obj_max > splitVal) {
				rightObjects.push_back(obj);
			}
		}

		// change the axis for the child nodes
		int newAxis = ToggleAxis(axis);

		// if this split partitioned some objects in left and some in right
		if (leftObjects.size() != objects.size() && rightObjects.size() != objects.size()) {
			// create left and right child nodes
			left = MakeTree(newAxis, leftObjects, depth+1); // depth?
			right = MakeTree(newAxis, rightObjects, depth+1); // depth?

			KdNode* n = new KdNode(splitVal, axis, bounds, objects);
			n->SetLeftRight(left, right);
			return n;
		}

		axis = newAxis;
	}

	return new KdNode();
}

