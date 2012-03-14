

#include "Surface3D.h"

Surface3D::Surface3D() {
	// TODO Auto-generated constructor stub
}

Surface3D::Surface3D(Vertex3D a, Vertex3D b, Vertex3D c, Vertex3D d) {
	this->topleft = a;
	this->topright = b;
	this->bottomright = c;
	this->bottomleft = d;

	hasTexture = false;
}

Surface3D::~Surface3D() {
	// TODO Auto-generated destructor stub
}

void Surface3D::setColor(float r, float g, float b){
	this->colorR = r;
	this->colorG = g;
	this->colorB = b;
	hasTexture = false;
}

void Surface3D::draw(){

	if(hasTexture){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureName);

		glColor3f(1,1,1);
		float mcolor[] = { 1, 1, 1, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 0.0);		topleft.draw();
			glTexCoord2d(0.0, 1.0);		topright.draw();
			glTexCoord2d(1.0, 1.0);		bottomright.draw();
			glTexCoord2d(1.0, 0.0);		bottomleft.draw();
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}else{
		glColor3f(colorR,colorG,colorB);
		float mcolor[] = { colorR, colorG, colorB, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
		glBegin(GL_POLYGON);
			topleft.draw();
			topright.draw();
			bottomright.draw();
			bottomleft.draw();
		glEnd();
	}

}


void Surface3D::setTexture(Texture texture) {

	glGenTextures(1, &textureName);
	glBindTexture(GL_TEXTURE_2D, textureName);

	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.imagewidth, texture.imageheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.xData );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.imagewidth, texture.imageheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.xData );

	hasTexture = true;
}


void Surface3D::deleteTexture() {
	glDeleteTextures(1, &textureName);

	hasTexture = false;
}
