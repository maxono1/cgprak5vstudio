//
//  TriangleBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "TriangleBoxModel.h"


TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth)
{
    VB.begin();
    //Die Werte im Koordinatensystem
    float CoordWidth = Width / 2;
    float CoordHeight = Height / 2;
    float CoordDepth = Depth / 2;

    //Die Punkte
    Vector A(CoordWidth, -CoordHeight, CoordDepth);
    Vector B(CoordWidth, -CoordHeight, -CoordDepth);
    Vector C(-CoordWidth, -CoordHeight, -CoordDepth);
    Vector D(-CoordWidth, -CoordHeight, CoordDepth);
    Vector E(CoordWidth, CoordHeight, CoordDepth);
    Vector F(CoordWidth, CoordHeight, -CoordDepth);
    Vector G(-CoordWidth, CoordHeight, -CoordDepth);
    Vector H(-CoordWidth, CoordHeight, CoordDepth);

    //Die Normalen
    //Vector K = -A.normal(A, B, C);
    Vector K = A.normal(C, B, A);
    Vector L = A.normal(A, B, E);
    Vector M = A.normal(B, C, F);
    Vector N = -A.normal(D, C, G);
    Vector O = -A.normal(F, E, H);
    Vector P = -A.normal(A, D, H);

    //Unten
    // 0
    VB.addNormal(K);
    VB.addTexcoord0(1, 0);
    VB.addVertex(A);
    // 1
    VB.addNormal(K);
    VB.addTexcoord0(1, 1);
    VB.addVertex(B);
    // 2
    VB.addNormal(K);
    VB.addTexcoord0(0, 1);
    VB.addVertex(C);
    // 3
    VB.addNormal(K);
    VB.addTexcoord0(0, 0);
    VB.addVertex(D);
    //Seite L
    // 4
    VB.addNormal(L);
    VB.addTexcoord0(0, 0);
    VB.addVertex(E);
    // 5
    VB.addNormal(L);
    VB.addTexcoord0(1, 0 );
    VB.addVertex(F);
    // 6
    VB.addNormal(L);
    VB.addTexcoord0(1, 1);
    VB.addVertex(B);
    // 7
    VB.addNormal(L);
    VB.addTexcoord0(0, 1);
    VB.addVertex(A);

    //Seite M
    // 8
    VB.addNormal(M);
    VB.addTexcoord0(0, 0);
    VB.addVertex(F);
    // 9
    VB.addNormal(M);
    VB.addTexcoord0(1, 0);
    VB.addVertex(G);
    // 10
    VB.addNormal(M);
    VB.addTexcoord0(1, 1);
    VB.addVertex(C);
    // 11
    VB.addNormal(M);
    VB.addTexcoord0(0, 1);
    VB.addVertex(B);

    // Seite n
    //12
    VB.addNormal(N);
    VB.addTexcoord0(0, 0);
    VB.addVertex(G);
    //13
    VB.addNormal(N);
    VB.addTexcoord0(1, 0);
    VB.addVertex(H);
    //14
    VB.addNormal(N);
    VB.addTexcoord0(0, 1);
    VB.addVertex(C);
    //15
    VB.addNormal(N);
    VB.addTexcoord0(1, 1);
    VB.addVertex(D);

    //Oben o
    //16
    VB.addNormal(O);
    VB.addTexcoord0(1, 1);
    VB.addVertex(E);
    //17
    VB.addNormal(O);
    VB.addTexcoord0(1, 0);
    VB.addVertex(F);
    //18
    VB.addNormal(O);
    VB.addTexcoord0(0, 0);
    VB.addVertex(G);
    //19
    VB.addNormal(O);
    VB.addTexcoord0(0, 1);
    VB.addVertex(H);

    //Vorne p
    // 20
    VB.addNormal(P);
    VB.addTexcoord0(1, 1);
    VB.addVertex(A);
    // 21
    VB.addNormal(P);
    VB.addTexcoord0(0, 1);
    VB.addVertex(D);
    // 22
    VB.addNormal(P);
    VB.addTexcoord0(0, 0);
    VB.addVertex(H);
    // 23
    VB.addNormal(P);
    VB.addTexcoord0(1, 0);
    VB.addVertex(E);

    VB.end();

    IB.begin();
    //Unten K
    IB.addIndex(0);
    IB.addIndex(3);
    IB.addIndex(2);

    IB.addIndex(2);
    IB.addIndex(1);
    IB.addIndex(0);

    //Seite L
    IB.addIndex(6);
    IB.addIndex(5);
    IB.addIndex(4);

    IB.addIndex(4);
    IB.addIndex(7);
    IB.addIndex(6);

    //Seite M
    IB.addIndex(10);
    IB.addIndex(9);
    IB.addIndex(8);

    IB.addIndex(8);
    IB.addIndex(11);
    IB.addIndex(10);

    //Seite N
    IB.addIndex(14);
    IB.addIndex(15);
    IB.addIndex(13);

    IB.addIndex(13);
    IB.addIndex(12);
    IB.addIndex(14);
     

    //Seite O
    IB.addIndex(18);
    IB.addIndex(19);
    IB.addIndex(16);

    IB.addIndex(16);
    IB.addIndex(17);
    IB.addIndex(18);

    //Seite P
    IB.addIndex(23);
    IB.addIndex(22);
    IB.addIndex(21);

    IB.addIndex(21);
    IB.addIndex(20);
    IB.addIndex(23);

    IB.end();
}


void TriangleBoxModel::draw(const BaseCamera& Cam)
{
	BaseModel::draw(Cam);

	// TODO: Add your code (Exercise 2)

	VB.activate();
	IB.activate();

	glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

	IB.deactivate();
	VB.deactivate();
}
