/*
 * Vertex3D.cpp
 *
 *  Created on: Feb 27, 2012
 *      Author: terix
 */

#include "Vertex3D.h"

Vertex3D::Vertex3D() {
	x = 0;
	y = 0;
	z = 0;
}

Vertex3D::Vertex3D(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;	
}

Vertex3D::~Vertex3D() {
	// TODO
}

void Vertex3D::setSize(int s){
	this->size = s;
}

#include <iostream>

void Vertex3D::draw(){
	std::cout << "vertex3d draw" << std::endl;
	// Warning: gl_Begin() has to come first
	glPointSize(size); // TODO does this work here?
	glVertex3f(x,y,z);
}

float Vertex3D::getX()
{
	return x;
}

float Vertex3D::getY()
{
	return y;
}

float Vertex3D::getZ()
{
	return z;
}

