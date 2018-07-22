#include <polygonalmesh.h>
#include "naca4digit.h"
#include "math.h"
#include <vector>

std::vector <YsVec3> MakeNACA4DigitAirfoil(int naca,const double dz)
{
	printf("naca:%d\n",naca );
	float nacaf = (float)naca;
	std::vector <YsVec3> vtx;
	float t = naca % 100;
	t/=100.0;
	float m = (float)(naca / 1000);
	m/= 100.0;
	float p = (float)((naca % 1000) / 100);
	p = p/10.0;
	float c=1.0;
	double y_t;
	double y_c;
	printf("%f\n",nacaf);
	printf("%f\n",t);
	printf("%f\n",m);
	printf("%f\n",p);

	printf("Starting to print\n");
	for (float z=0; z<=1.0; z+=dz)
	{
		y_t = 5*t*(0.2969*sqrt(z)-0.1260*z-0.3516*pow(z,2)+0.2484*pow(z,3)-0.1015*pow(z,4));
		if(0<=z && z<=p*c)
		{
			y_c = (m/pow(p,2))*(2*p*(z/c)-pow((z/c),2));
		}
		else if(p*c<=z && z<=c)
		{
			y_c = (m/pow((1-p),2))*(1-2*p+2*p*(z/c)-pow((z/c),2));
		}
		// printf("%f,%f,%f\n",y_t,y_c,z);
		vtx.push_back(YsVec3(0, y_t + y_c, z));

		vtx.push_back(YsVec3(0, y_t - y_c, z));
	}
	
	return vtx;
}

void MakeNACA4DigitAirfoilWing(class PolygonalMesh &mesh,int naca,float span,float sweepBack,float dihedral,float taper,float dz)
{
	auto cross_section_zero = MakeNACA4DigitAirfoil(naca,dz);
	auto cross_section_pos = cross_section_zero;
	for (int i=0; i<cross_section_pos.size(); i++)
	{
		cross_section_pos[i][0]+=span;
		cross_section_pos[i][1]+=dihedral;
		cross_section_pos[i][2]=sweepBack+cross_section_pos[i][2]*taper;
	}
	auto cross_section_neg = cross_section_zero;
	for (int i=0; i<cross_section_neg.size(); i++)
	{
		cross_section_neg[i][0]-=span;
		cross_section_neg[i][1]+=dihedral;
		cross_section_neg[i][2]=sweepBack+cross_section_neg[i][2]*taper;
	}
	
	for (int i=0; i<cross_section_zero.size()-1; i+=2)
	{
		
		std::vector <PolygonalMesh::VertexHandle> VHdVector;

		// First Triangle
		auto pt1 = cross_section_zero[i];
		auto VertexHandle1 = mesh.AddVertex(pt1);
		auto pt2 = cross_section_zero[i+1];
		auto VertexHandle2 = mesh.AddVertex(pt2);
		auto pt3 = cross_section_pos[i];
		auto VertexHandle3 = mesh.AddVertex(pt3);

		VHdVector.push_back(VertexHandle1);
		VHdVector.push_back(VertexHandle2);
		VHdVector.push_back(VertexHandle3);

		auto plHd = mesh.AddPolygon(VHdVector);
		auto v1 = pt2 - pt1;
		auto v2 = pt3 - pt2;
		auto norm = YsUnitVector(v1^v2);
		mesh.SetPolygonNormal(plHd,norm);

		VHdVector.clear();

		// Second Triangle
		pt1 = cross_section_zero[i+1];
		VertexHandle1 = mesh.AddVertex(pt1);
		pt2 = cross_section_pos[i+1];
		VertexHandle2 = mesh.AddVertex(pt2);
		pt3 = cross_section_pos[i];
		VertexHandle3 = mesh.AddVertex(pt3);

		VHdVector.push_back(VertexHandle1);
		VHdVector.push_back(VertexHandle2);
		VHdVector.push_back(VertexHandle3);

		plHd = mesh.AddPolygon(VHdVector);
		v1 = pt2 - pt1;
		v2 = pt3 - pt2;
		norm = YsUnitVector(v1^v2) ;
		mesh.SetPolygonNormal(plHd,norm);

		VHdVector.clear();

		// Third Triangle
		pt1 = cross_section_zero[i+1];
		VertexHandle1 = mesh.AddVertex(pt1);
		pt2 = cross_section_zero[i];
		VertexHandle2 = mesh.AddVertex(pt2);
		pt3 = cross_section_neg[i];
		VertexHandle3 = mesh.AddVertex(pt3);

		VHdVector.push_back(VertexHandle1);
		VHdVector.push_back(VertexHandle2);
		VHdVector.push_back(VertexHandle3);

		plHd = mesh.AddPolygon(VHdVector);
		v1 = pt2 - pt1;
		v2 = pt3 - pt2;
		norm = YsUnitVector(v1^v2) ;
		mesh.SetPolygonNormal(plHd,norm);

		VHdVector.clear();

		// Fourth Triangle
		pt1 = cross_section_zero[i+1];
		VertexHandle1 = mesh.AddVertex(pt1);
		pt2 = cross_section_neg[i];
		VertexHandle2 = mesh.AddVertex(pt2);
		pt3 = cross_section_neg[i+1];
		VertexHandle3 = mesh.AddVertex(pt3);

		VHdVector.push_back(VertexHandle1);
		VHdVector.push_back(VertexHandle2);
		VHdVector.push_back(VertexHandle3);

		plHd = mesh.AddPolygon(VHdVector);
		v1 = pt2 - pt1;
		v2 = pt3 - pt2;
		norm = YsUnitVector(v1^v2) ;
		mesh.SetPolygonNormal(plHd,norm);

		VHdVector.clear();
	}

	printf("Number of triangles:%lld\n", mesh.GetNumPolygon()	);

}


// void MakeNACA4DigitAirfoilWing(class PolygonalMesh &mesh,int naca,float span,float sweepBack,float dihedral,float taper,float dz)
// {
// 	auto cross_section_zero = MakeNACA4DigitAirfoil(naca,dz);
// 	auto cross_section_pos = cross_section_zero;
// 	for (int i=0; i<cross_section_pos.size(); i++)
// 	{
// 		cross_section_pos[i][0]+=span;
// 		cross_section_pos[i][1]+=dihedral;
// 		cross_section_pos[i][2]=sweepBack+cross_section_pos[i][2]*taper;
// 	}
// 	auto cross_section_neg = cross_section_zero;
// 	for (int i=0; i<cross_section_neg.size(); i++)
// 	{
// 		cross_section_neg[i][0]-=span;
// 		cross_section_neg[i][1]+=dihedral;
// 		cross_section_neg[i][2]=sweepBack+cross_section_neg[i][2]*taper;
// 	}

	
// 	for (int i=0; i<cross_section_zero.size()-1; i+=2)
// 	{
		
// 		std::vector <PolygonalMesh::VertexHandle> VHdVector1;
// 		std::vector <PolygonalMesh::VertexHandle> VHdVector2;

// 		// Points at 0
// 		auto pt1 = cross_section_zero[i];
// 		auto VertexHandle1 = mesh.AddVertex(pt1);
// 		VHdVector1.push_back(VertexHandle1);
// 		VHdVector2.push_back(VertexHandle1);
// 		auto pt2 = cross_section_zero[i+1];
// 		auto VertexHandle2 = mesh.AddVertex(pt2);
// 		VHdVector1.push_back(VertexHandle2);
// 		VHdVector2.push_back(VertexHandle2);


// 		// Points at +s
// 		auto pt3 = cross_section_pos[i];
// 		auto VertexHandle3 = mesh.AddVertex(pt3);
// 		VHdVector1.push_back(VertexHandle3);
// 		auto pt4 = cross_section_pos[i+1];
// 		auto VertexHandle4 = mesh.AddVertex(pt4);
// 		VHdVector1.push_back(VertexHandle4);

// 		mesh.AddPolygon(VHdVector1);

// 		// Points at -s
// 		auto pt5 = cross_section_neg[i];
// 		auto VertexHandle5 = mesh.AddVertex(pt5);
// 		VHdVector2.push_back(VertexHandle5);
// 		auto pt6 = cross_section_neg[i+1];
// 		auto VertexHandle6 = mesh.AddVertex(pt6);
// 		VHdVector2.push_back(VertexHandle4);

// 		mesh.AddPolygon(VHdVector2);
// 	}

// }
