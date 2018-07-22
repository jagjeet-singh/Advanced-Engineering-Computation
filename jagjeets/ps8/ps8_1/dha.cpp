#include "dha.h"

#include <unordered_set>
#include <stdio.h>

double getAngle(YsVec3 v1, YsVec3 v2)
{
	auto dot = v1*v2;
	double lenSq1 = double(pow(v1[0],2))+double(pow(v1[1],2))+double(pow(v1[2],2));
	double lenSq2 = double(pow(v2[0],2))+double(pow(v2[1],2))+double(pow(v2[2],2));
	auto angle = acos(float(dot/sqrt(lenSq1 * lenSq2)));
	return fabs(angle);

}


// Input parameters are a mesh and dihedral-angle threshold.
// The return value is a map from a polygon search key to a segment identifier.
std::unordered_map <YSHASHKEY,int> MakeDihedralAngleBasedSegmentation(const YsShellExt &mesh,const double dhaThr)
{
	std::unordered_map <YSHASHKEY,int> faceGrp;

	int segment_label = 0;
	
	// Each iteration of the loop will find all the polygons in one particular segmentation label
	while(true){

		// Find a polygon to start with
		auto from = mesh.NullPolygon();
		while(true){
			auto nextPolygon = mesh.MoveToNextPolygon(from);

			// If all the polygons are visited
			if (false == nextPolygon){
				printf("Number of groups:%d\n", segment_label);
				return faceGrp;
			}

			// If next Polygon exists and has not been added to faceGrp yet
			if (true == nextPolygon && faceGrp.count(mesh.GetSearchKey(from))==0){
				break;
			}
		}

		// Adding the new polygon to the current group
		faceGrp[mesh.GetSearchKey(from)] == segment_label;

		// Do BFS
		std::vector <YsShellExt::PolygonHandle> todo;
		std::unordered_set <unsigned int> visited;

		todo.push_back(from);
		visited.insert(mesh.GetSearchKey(from));

		for(int i=0; i<todo.size(); ++i)
		{
			auto plHd=todo[i];
			auto fromNormal = mesh.GetNormal(plHd);
			for(int edIdx=0; edIdx<mesh.GetPolygonNumVertex(plHd); ++edIdx)
			{
				auto neiPlHd=mesh.GetNeighborPolygon(plHd,edIdx);
				auto neiNormal = mesh.GetNormal(neiPlHd);
				// Neighbour Polygon should not be in visited, faceGrp and angle < dhaThr
				if(0 == faceGrp.count(mesh.GetSearchKey(neiPlHd)) 
					&& visited.end()==visited.find(mesh.GetSearchKey(neiPlHd)) 
					&& getAngle(fromNormal, neiNormal)<dhaThr)
				{
					todo.push_back(neiPlHd);
					visited.insert(mesh.GetSearchKey(neiPlHd));
					faceGrp[mesh.GetSearchKey(neiPlHd)] = segment_label;
				}
			}
		}

		segment_label++;
	}

	return faceGrp;
}

// Input parameters are a mesh and the segmentation (face grouping) obtained from MakeDihedralAngleBasedSegmentaion.
// Output is a vertex array that can be drawn as GL_LINES.
std::vector <float> MakeGroupBoundaryVertexArray(const YsShellExt &mesh,const std::unordered_map <YSHASHKEY,int> &faceGroupInfo)
{
	std::vector <float> vtxArray;

	for(auto plHd=mesh.NullPolygon(); true==mesh.MoveToNextPolygon(plHd); ){
		auto plHd_HK = mesh.GetSearchKey(plHd);
		auto currFG = faceGroupInfo.at(plHd_HK);
		auto plVtHd = mesh.GetPolygonVertex(plHd);

		if(3==plVtHd.size())
		{
			for(int i=0; i<3; ++i)
			{
				auto vt1 = plVtHd[i];
				auto vt2 = plVtHd[(i+1)%3];
				auto neiPlHd = mesh.GetNeighborPolygon(plHd, vt1, vt2);
				auto neighFG = faceGroupInfo.at(mesh.GetSearchKey(neiPlHd));
				if (currFG!=neighFG){
					vtxArray.push_back(mesh.GetVertexPosition(vt1).xf());
					vtxArray.push_back(mesh.GetVertexPosition(vt1).yf());
					vtxArray.push_back(mesh.GetVertexPosition(vt1).zf());
					vtxArray.push_back(mesh.GetVertexPosition(vt2).xf());
					vtxArray.push_back(mesh.GetVertexPosition(vt2).yf());
					vtxArray.push_back(mesh.GetVertexPosition(vt2).zf());
				}
			}
		}
	}

	return vtxArray;
}

// For bonus questions:
// Input parameters are a mesh and the segmentation (face grouping) obtained from MakeDihedralAngleBasedSegmentaion.
// Paint polygons so that no two neighboring face groups have a same color.
void MakeFaceGroupColorMap(YsShellExt &mesh,const std::unordered_map <YSHASHKEY,int> &faceGroupInfo)
{
}

