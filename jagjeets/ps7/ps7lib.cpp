#include <fssimplewindow.h> // For FsSubSecondTimer
#include <meshlattice.h>
#include <math.h>
#include <unordered_set>
#include "ps7lib.h"
#include <stdio.h>

std::vector <float> MakeSliceVertexArray(const YsShellExt &mesh)
{
	auto t0=FsSubSecondTimer();
	std::vector <float> sliceVtx;

	// You fill this part.
	YsVec3 minBB;
	YsVec3 maxBB;

	mesh.GetBoundingBox(minBB, maxBB);

	auto miny = minBB[1];
	auto maxy = maxBB[1];

	std::vector <float> vtx, vty, vtz;
	// Looping through all the polygons
	for(auto plHd=mesh.NullPolygon(); true==mesh.MoveToNextPolygon(plHd); )
	{
		vtx.clear();
		vty.clear();
		vtz.clear();
		auto plVtHd=mesh.GetPolygonVertex(plHd);
		auto plCol=mesh.GetColor(plHd);
		auto plNom=mesh.GetNormal(plHd);
		float Plminy = 100;
		float Plmaxy = -100; 
		if(3==plVtHd.size())
		{
			// Find min and max y for the polygon
			for(int i=0; i<3; ++i)
			{
				auto vtPos=mesh.GetVertexPosition(plVtHd[i]);
				vtx.push_back(vtPos.xf());
				vty.push_back(vtPos.yf());
				vtz.push_back(vtPos.zf());
				if (vtPos.yf()>Plmaxy)
				{
					Plmaxy = vtPos.yf();
				}

				if (vtPos.yf()<Plminy)
				{
					Plminy = vtPos.yf();
				}
			}
		}
		// Looping through planes which intersect the polygon
		auto m = int(Plminy*100/(maxy-miny));
		m++;
		auto start = (maxy-miny)*m/100.0;
		for (float y = start; y<=Plmaxy; y+=(maxy-miny)/100.0)
		{
			// Creating plane
			auto pointOnPLane = YsVec3(0.0, y, 0.0);
			auto normal = YsVec3(0.0, 1.0, 0.0);
			auto plane = YsPlane(pointOnPLane, normal);
			// Finding intersection points of the plane and polygon
			for (int i=0; i<3; i++)
			{

				auto i2 = (i+1)%3;
				YsVec3 inters;
				if ((vty[i]<y && vty[i2]>y) || (vty[i]>y && vty[i2]<y))
				{
					auto pt1 = YsVec3(vtx[i], vty[i], vtz[i]);
					auto pt2 = YsVec3(vtx[i2], vty[i2], vtz[i2]);
					plane.GetPenetration(inters, pt1, pt2);
					sliceVtx.push_back(inters.xf());
					sliceVtx.push_back(inters.yf());
					sliceVtx.push_back(inters.zf());
				}
			}

		}
	}
	printf("%d milli seconds\n",(int)FsSubSecondTimer()-t0);
	return sliceVtx;
}


#define USE_LATTICE

double getTriangleArea(YsVec3 v1, YsVec3 v2, YsVec3 v3)
{
	auto edge1 = v2 - v1;
	auto edge2 = v3 - v1;
	auto normal = edge1^edge2;
	auto normalLength = normal.GetLength();
	return 0.5*normalLength;
}


std::vector <float> MakeRayIntersectionVertexArray(const YsShellExt &mesh)
{
	auto t0=FsSubSecondTimer();
	std::vector <float> rayItscVtx;
	
	YsVec3 minBB;
	YsVec3 maxBB;

	// Get min and max coordinates of the aeroplane
	mesh.GetBoundingBox(minBB, maxBB);

	auto minx = minBB[0];
	auto miny = minBB[1];
	auto minz = minBB[2];
	auto maxx = maxBB[0];
	auto maxy = maxBB[1];
	auto maxz = maxBB[2];


	std::vector <float> vtx, vty, vtz;

	// Looping through all the polygons
	for(auto plHd=mesh.NullPolygon(); true==mesh.MoveToNextPolygon(plHd); )
	{
		vtx.clear();
		vty.clear();
		vtz.clear();
		auto plVtHd=mesh.GetPolygonVertex(plHd);
		float Plminy = maxy;
		float Plmaxy = miny; 
		float Plminx = maxx;
		float Plmaxx = minx; 
		if(3==plVtHd.size())
		{
			// Find min and max y for the polygon
			for(int i=0; i<3; ++i)
			{
				auto vtPos=mesh.GetVertexPosition(plVtHd[i]);
				vtx.push_back(vtPos.xf());
				vty.push_back(vtPos.yf());
				vtz.push_back(vtPos.zf());
				if (vtPos.yf()>Plmaxy)
				{
					Plmaxy = vtPos.yf();
				}

				if (vtPos.yf()<Plminy)
				{
					Plminy = vtPos.yf();
				}
				if (vtPos.xf()>Plmaxx)
				{
					Plmaxx = vtPos.xf();
				}

				if (vtPos.xf()<Plminx)
				{
					Plminx = vtPos.xf();
				}
			}
		}
		// Looping through planes which intersect the polygon
		auto deltax = (maxx-minx)/100.0;
		auto mx = int((Plminx-minx)/deltax);
		auto deltay = (maxy-miny)/100.0;
		auto my = int((Plminy-miny)/deltay);
		// auto mx = int((Plminx)*100.0/(maxx-minx));
		// auto my = int((Plminy)*100.0/(maxy-miny));
		mx++;
		my++;
		// printf("mx:%d, my:%d\n", mx, my);
		auto startx = minx + deltax*mx;
		auto starty = miny + deltay*my;
		// auto startx = (maxx-minx)*mx/100.0;
		// auto starty = (maxy-miny)*my/100.0;
		// printf("startx:%f, starty:%f\n", startx, starty);
		// printf("maxy:%f, Plmaxy:%f, deltay:%f\n", maxy, Plmaxy, (maxy-miny)/100.0);

		for (float x = startx; x<=Plmaxx; x+=deltax)
		{
			
			// fprintf(stderr, "info: %s:%d: \n", __FUNCTION__, __LINE__);
			for (float y = starty; y<=Plmaxy; y+=deltay) 
			{
				auto pt1 = YsVec3(x,y,minz);
				auto pt2 = YsVec3(x,y,maxz);
				
				// Creating plane from Polygon
				auto plane = YsPlane();
				auto vt1 = YsVec3(vtx[0], vty[0], vtz[0]);
				auto vt2 = YsVec3(vtx[1], vty[1], vtz[1]);
				auto vt3 = YsVec3(vtx[2], vty[2], vtz[2]);
				plane.MakePlaneFromTriangle(vt1, vt2, vt3);
				YsVec3 inters;
				plane.GetPenetration(inters, pt1, pt2);

				// Approach 1
				
				// std::vector <YsVec3> VtArray;
				// VtArray.push_back(vt1);
				// VtArray.push_back(vt2);
				// VtArray.push_back(vt3);
				// auto isInside = YsCheckInsidePolygon3(inters, VtArray);

				// if (isInside == YSINSIDE)
				// {
				// 	rayItscVtx.push_back(inters.xf());
				// 	rayItscVtx.push_back(inters.yf());
				// 	rayItscVtx.push_back(inters.zf());	
				// }

				
				// Approach 2

				// checking if intersection point is within the triangle

				auto Area = getTriangleArea(vt1, vt2, vt3);
				auto area1 = getTriangleArea(vt1, vt2, inters);
				auto area2 = getTriangleArea(vt2, vt3, inters);
				auto area3 = getTriangleArea(vt3, vt1, inters);

				if (area1+area2+area3 == Area)
				{
					rayItscVtx.push_back(inters.xf());
					rayItscVtx.push_back(inters.yf());
					rayItscVtx.push_back(inters.zf());
				}
			}
		}
	}

	printf("%d milli seconds\n",(int)FsSubSecondTimer()-t0);
	return rayItscVtx;
}

double getAngle(YsVec3 v1, YsVec3 v2)
{
	auto dot = v1.xf()*v2.xf() + v1.yf()*v2.yf() + v1.zf()*v2.zf();   
	auto lenSq1 = v1.xf()*v1.xf() + v1.yf()*v1.yf() + v1.zf()*v1.zf();
	auto lenSq2 = v2.xf()*v2.xf() + v2.yf()*v2.yf() + v2.zf()*v2.zf();
	auto angle = acos(dot/sqrt(lenSq1 * lenSq2));
	return fabs(angle*180.0/YsPi);
}

void Paint(YsShellExt &mesh,YsShell::PolygonHandle from,const double angleTolerance)
{
	std::vector <YsShellExt::PolygonHandle> todo;
	std::unordered_set <unsigned int> visited;

	todo.push_back(from);
	auto fromAngle = mesh.GetNormal(from);
	visited.insert(mesh.GetSearchKey(from));

	for(int i=0; i<todo.size(); ++i)
	{
		auto plHd=todo[i];
		mesh.SetPolygonColor(plHd,YsRed());
		for(int edIdx=0; edIdx<mesh.GetPolygonNumVertex(plHd); ++edIdx)
		{
			auto neiPlHd=mesh.GetNeighborPolygon(plHd,edIdx);
			auto neiAngle = mesh.GetNormal(neiPlHd);
			if(visited.end()==visited.find(mesh.GetSearchKey(neiPlHd)) && getAngle(fromAngle, neiAngle)<angleTolerance)
			{
				todo.push_back(neiPlHd);
				visited.insert(mesh.GetSearchKey(neiPlHd));
			}
		}
	}

}
