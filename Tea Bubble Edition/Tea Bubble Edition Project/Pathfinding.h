#pragma once
#include "j1Module.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "p2PQueue.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Textures.h"

//This class will be used by any entity in order to get a path to a certain destination. It will have a public attribute called "last_path" 
//and a public method called "create path", that when used will fill last path with a set of tiles, that will represent the path. It will be a part of App
class Pathfinding: public j1Module
{
public:
	//Constructor/Destructor
	Pathfinding();
	~Pathfinding();
private:
	//Private methods

	//Private attributes
	uint height, width;
	uint* walkability_values = nullptr;	
	p2DynArray<iPoint> last_path;


	p2PQueue<iPoint>	frontier;
	p2List<iPoint>		visited;
	p2List<iPoint>		breadcrumbs;
public:
	//Public methods
	void SetMap(); //Has to be called whenever map is changed
	void CreatePath(iPoint origin, iPoint destination); // This will fill "last path"
	void Path_BackTracking(iPoint goal);
	bool CleanUp();
	bool IsWalkable(const iPoint& pos)const; //Returns 0 if walkable and -1 if not walkable
	void DebugDraw();
	p2DynArray<iPoint>* GetLastPath();


	//Public attributes
	
};

