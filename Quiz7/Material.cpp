#include "Material.h"

using namespace std;


vector<GLuint> loadMat() {

	int listIndex = 1;

	vector<GLuint> activeCreatedMats;


	GLuint defaultMat = glGenLists(listIndex);
	glNewList(defaultMat, GL_COMPILE);
	GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
	GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	glEndList();

	activeCreatedMats.push_back(defaultMat);

	listIndex++;

	GLuint chromeMat = glGenLists(listIndex);
	glNewList(chromeMat, GL_COMPILE);
	GLfloat mat_emission2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission2);
	GLfloat mat_ambient2[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient2);
	GLfloat mat_diffuse2[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse2);
	GLfloat mat_specular2[] = { 0.774597f,0.774597f, 0.774597f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 76.8);
	glEndList();

	activeCreatedMats.push_back(chromeMat);

	listIndex++;

	GLuint jadeMat = glGenLists(listIndex);
	glNewList(jadeMat, GL_COMPILE);
	GLfloat mat_emission3[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission3);
	GLfloat mat_ambient3[] = { 0.135f, 0.2225f, 0.1575f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient3);
	GLfloat mat_diffuse3[] = { 0.54f, 0.89f, 0.63f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse3);
	GLfloat mat_specular3[] = { 0.316228f, 0.316228f, 0.316228f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular3);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 12.8);
	glEndList();

	activeCreatedMats.push_back(jadeMat);

	listIndex++;

	GLuint obsMat = glGenLists(listIndex);
	glNewList(obsMat, GL_COMPILE);
	GLfloat mat_emission4[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission4);
	GLfloat mat_ambient4[] = { 0.05375f, 0.05f, 0.06625f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient4);
	GLfloat mat_diffuse4[] = { 0.18275f, .17f, .22525f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse4);
	GLfloat mat_specular4[] = { 0.332741f, 0.328634f, 0.346435f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular4);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 38.4);
	glEndList();

	activeCreatedMats.push_back(obsMat);

	listIndex++;

	GLuint pearlMat = glGenLists(listIndex);
	glNewList(pearlMat, GL_COMPILE);
	GLfloat mat_emission5[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission5);
	GLfloat mat_ambient5[] = { 0.25f, 0.20725f, 0.20725f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient5);
	GLfloat mat_diffuse5[] = { 1.0, 0.829f, 0.829f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse5);
	GLfloat mat_specular5[] = { 0.296648f, 0.296648f, 0.296648f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular5);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 11.264);
	glEndList();

	activeCreatedMats.push_back(pearlMat);

	listIndex++;

	


	return activeCreatedMats;
}