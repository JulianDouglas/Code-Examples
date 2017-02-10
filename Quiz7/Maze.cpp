#include "imathvec.h"
#include "glut.h"
#include "Material.h"
#include <windows.h>
#include <wingdi.h>
#include<iostream>
#include<fstream>
#include<string>
#include <vector>


#define PI 3.1415926

using namespace std;
using namespace Imath;

// mesh
typedef Vec3<float> Vec3f;
typedef Vec2<float> Vec2f;

struct Mesh {

	// vertex
	vector<Vec3f> dot_vertex;
	vector<Vec3f> dot_normal;
	//vector<Vec3f> dot_normalV;
	vector<Vec2f> dot_texture;

	// faces
	vector<int> face_index_vertex;
	vector<int> face_index_normal;
	//vector<int> face_index_normalV;
	vector<int> face_index_texture;

};


// global
int width = 1200;
int height = 600;
float ratio = 1.0;

GLuint textureArray[8];

GLuint displayPlaneFace, displayPlaneVer, displayCube, displayBorder, displayGoal;



vector<GLuint> activeMats;



// controling parameters
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 1.0;
float x_angle = 0.0;
float y_angle = 0.0;
float x_trans = 425.0;
float y_trans = -425.0;

int playerX = static_cast<int>(x_trans + 500);
int playerY = static_cast<int>(y_trans + 500);



// global moving
float angle = 0;
Vec3f moving_position = Vec3f(0, 0, 0);
Vec3f center_position = Vec3f(0, 0, 0);

// object rotating
float moveX = 0;
float moveZ = 0;
float tick = 1.6;
float radius = 9;
float angle_animated_object = 0;

bool normalBool = true;
bool lightBool = true;


int mazeArray[20][20] = {

	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 2, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 2 },
	{ 2, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 2 },
	{ 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 2 },
	{ 2, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 2 },
	{ 2, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 2 },
	{ 2, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 2 },
	{ 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 2 },
	{ 2, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 2 },
	{ 2, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 2 },
	{ 2, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 2 },
	{ 2, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 2 },
	{ 2, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 2 },
	{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 2 },
	{ 2, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 2 },
	{ 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 2 },
	{ 2, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 2 },
	{ 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2 },
	{ 2, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }

};
/* ---------------------------------------- BEGIN:MESH CODE  ---------------------------------------- */


// str to int
int StrToInt(const string &str) {

	int i;

	if (sscanf_s(str.c_str(), "%i", &i) == 1) return i;

	else return 0;
}



// split string

vector<string> split_string(const string& str, const string& split_str) {

	vector<string> stlv_string;

	string part_string("");

	string::size_type i;



	i = 0;

	while (i < str.size()) {



		if (split_str.find(str[i]) != string::npos) {

			stlv_string.push_back(part_string);

			part_string = "";



			while (split_str.find(str[i]) != string::npos) {

				++i;

			}

		}

		else {
			part_string += str[i];
			++i;
		}
	}

	if (!part_string.empty()) {

		stlv_string.push_back(part_string);

	}

	return stlv_string;
}

// load file
Mesh* loadFile(const char* file) {

	Mesh *m = new Mesh;

	m->dot_vertex.clear();

	m->face_index_vertex.clear();

	ifstream infile(file);



	if (infile.fail()) {
		cout << "Error opening file " << file;
		return NULL;

	}
	char current_line[1024];

	while (!infile.eof()) {
		infile.getline(current_line, 1024);

		switch (current_line[0]) {
		case'v':
			float x, y, z;
			switch (current_line[1]) {

			case 'n':
				sscanf_s(current_line, "vn %f %f %f", &x, &y, &z);
				m->dot_normal.push_back(Vec3f(x, y, z));
				break;

			case 't':
				sscanf_s(current_line, "vt %f %f", &x, &y);
				m->dot_texture.push_back(Vec2f(x, y));
				break;

			default:
				sscanf_s(current_line, "v %f %f %f", &x, &y, &z);
				m->dot_vertex.push_back(Vec3f(x, y, z));
				break;
			}
			break;

		case 'f': {
			vector<string> faces = split_string(current_line, " \t.\r\n");
			int vnt[3][3] = { { -1, -1, -1 },{ -1, -1, -1 },{ -1, -1, -1 } };

			string::size_type begin, end;

			for (int i = 0; i < 2; i++) {
				begin = 0;
				int j = 0;
				do {
					end = faces[i + 1].find_first_of("/", begin);
					if (begin < end) {
						vnt[i][j] = StrToInt(faces[i + 1].substr(begin, end - begin)) - 1;
					}
					else {
						vnt[i][j] = -1;
					}
					begin = end + 1;
					j++;

				} while (end != string::npos);
			}

			for (unsigned int i = 3; i < faces.size(); i++) {
				begin = 0;
				int j = 0;

				do {
					end = faces[i].find_first_of("/", begin);
					if (begin < end) {
						vnt[2][j] = StrToInt(faces[i].substr(begin, end - begin)) - 1;
					}
					else {
						vnt[2][j] = -1;
					}
					begin = end + 1;
					j++;

				} while (end != string::npos);

				for (int j = 0; j < 3; j++) {
					m->face_index_vertex.push_back(vnt[j][0]);
					if (vnt[j][1] != -1) m->face_index_texture.push_back(vnt[j][1]);
					if (vnt[j][2] != -1) m->face_index_normal.push_back(vnt[j][2]);
				}


				memcpy(&vnt[1], &vnt[2], sizeof(int) * 3);
			}

		}
				  break;

		default:
			break;

		}
	}

	return m;
}


void calculateNormalPerFace(Mesh* mesh){

	Vec3<float> v1, v2, v3, v4, v5;
	mesh->dot_normal.clear();
	mesh->face_index_normal.clear();

	for (float i = 0; i < mesh->face_index_vertex.size(); i += 3){

		v1 = mesh->dot_vertex[mesh->face_index_vertex[i]];
		v2 = mesh->dot_vertex[mesh->face_index_vertex[i + 1]];
		v3 = mesh->dot_vertex[mesh->face_index_vertex[i + 2]];
		v4 = (v2 - v1);
		v5 = (v3 - v1);
		v4 = v4.cross(v5);
		v4.normalize();

		mesh->dot_normal.push_back(v4);



		float pos = mesh->dot_normal.size() - 1;

		mesh->face_index_normal.push_back(pos);
		mesh->face_index_normal.push_back(pos);
		mesh->face_index_normal.push_back(pos);
	}

}


void calculateNormalPerVertex(Mesh* mesh){

	vector<int> face_index_normalPerFace = mesh->face_index_normal;
	vector<Vec3f> dot_normalPerFace = mesh->dot_normal;

	mesh->dot_normal.clear();
	mesh->face_index_normal.clear();

	Vec3<float> suma; suma.x = 0; suma.y = 0; suma.z = 0;



	for (unsigned int val = 0; val < mesh->dot_vertex.size(); val++){

		mesh->dot_normal.push_back(suma);

	}

	for (long pos = 0; pos < mesh->face_index_vertex.size(); pos++){

		mesh->dot_normal[mesh->face_index_vertex[pos]] += dot_normalPerFace[face_index_normalPerFace[pos]];

	}

	for (unsigned int val = 0; val < mesh->dot_normal.size(); val++){

		mesh->dot_normal[val] = mesh->dot_normal[val].normalize();
	}

	for (unsigned int pos = 0; pos < mesh->face_index_vertex.size(); pos++){

		mesh->face_index_normal.push_back(mesh->face_index_vertex[pos]);

	}

}


// creating a triangulated plane
Mesh* createPlane(int arena_width, int arena_depth, int arena_cell) {

	Mesh *me = new Mesh;

	int n = arena_width / arena_cell;
	int m = arena_depth / arena_cell;



	// vertices
	for (int i = 0; i<n; i++) {

		for (int j = 0; j < m; j++) {

			me->dot_vertex.push_back(Vec3<GLfloat>(i*arena_cell, 0.0, j*arena_cell));

		}

	}

	// faces
	for (int i = 0; i<(n*m) - m; i++) {

		if ((i + 1) % n == 0) continue;

		me->face_index_vertex.push_back(i); me->face_index_vertex.push_back(i + 1);
		me->face_index_vertex.push_back(i + n);
		me->face_index_vertex.push_back(i + 1); me->face_index_vertex.push_back(i + n + 1);
		me->face_index_vertex.push_back(i + n);

	}

	return me;

}



Mesh* createCube() {

	Mesh *mesh = new Mesh;

	// Vertexe
	int BOX_WIDTH = 50;
	int BOX_HEIGHT = 50;
	int BOX_DEPTH = 50;

	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, BOX_HEIGHT, BOX_DEPTH));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, 0.0, BOX_DEPTH));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, 0.0, BOX_DEPTH));

	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, BOX_HEIGHT, 0.0));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, BOX_HEIGHT, 0.0));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, 0.0, 0.0));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, 0.0, 0.0));



	// face 1
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(2);//1
	mesh->face_index_vertex.push_back(1);//2
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(3);//2
	mesh->face_index_vertex.push_back(2);//3



	// face 2
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(5);//1
	mesh->face_index_vertex.push_back(4);//5
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(1);//5
	mesh->face_index_vertex.push_back(5);//4


	// face 3
	mesh->face_index_vertex.push_back(1);//1
	mesh->face_index_vertex.push_back(6);//2
	mesh->face_index_vertex.push_back(5);//6
	mesh->face_index_vertex.push_back(1);//1
	mesh->face_index_vertex.push_back(2);//6
	mesh->face_index_vertex.push_back(6);//5
	mesh->face_index_vertex.push_back(2);//2
	mesh->face_index_vertex.push_back(7);//3
	mesh->face_index_vertex.push_back(6);//7
	mesh->face_index_vertex.push_back(2);//2
	mesh->face_index_vertex.push_back(3);//7
	mesh->face_index_vertex.push_back(7);//6



	mesh->face_index_vertex.push_back(4);//0
	mesh->face_index_vertex.push_back(3);//3
	mesh->face_index_vertex.push_back(0);//7
	mesh->face_index_vertex.push_back(4);//0
	mesh->face_index_vertex.push_back(7);//7
	mesh->face_index_vertex.push_back(3);//4



	mesh->face_index_vertex.push_back(5);//4 
	mesh->face_index_vertex.push_back(7);//5
	mesh->face_index_vertex.push_back(4);//6
	mesh->face_index_vertex.push_back(5);//4
	mesh->face_index_vertex.push_back(6);//6
	mesh->face_index_vertex.push_back(7);//7



	mesh->dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	mesh->dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));
	mesh->dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	mesh->dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));



	for (int t = 0; t<6; t++) {

		mesh->face_index_texture.push_back(0);//0
		mesh->face_index_texture.push_back(2);//1
		mesh->face_index_texture.push_back(1);//2
		mesh->face_index_texture.push_back(0);//0
		mesh->face_index_texture.push_back(3);//2
		mesh->face_index_texture.push_back(2);//3

	}

	return mesh;
}


// draw

GLuint meshToDisplayList(Mesh* m, int id, GLuint mat = NULL, int texture = NULL) {

	GLuint listID = glGenLists(id);

	glNewList(listID, GL_COMPILE);

	if (mat != NULL)
		glCallList(mat);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (texture != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, textureArray[texture]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
	}

	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < m->face_index_vertex.size(); i++) {

		if (!m->dot_normal.empty() && !m->face_index_normal.empty())
			glNormal3fv(&m->dot_normal[m->face_index_normal[i]].x);

		if (!m->dot_texture.empty() && !m->face_index_texture.empty())
			glTexCoord2fv(&m->dot_texture[m->face_index_texture[i]].x);



		// color

		Vec3f offset = (m->dot_vertex[m->face_index_vertex[i]]);

		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));

		glVertex3fv(&m->dot_vertex[m->face_index_vertex[i]].x);

	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEndList();

	return listID;
}


/* ----------------------------------------- END:MESH CODE  ----------------------------------------- */

GLubyte *LoadDIBitmap(const char *filename, BITMAPINFO **info) {
	FILE *fp;      // open file pointer
	GLubyte * bits; // bitmap pixel bits
	int bitsize;   // Size of bitmap
	int infosize;  // Size of header information
	BITMAPFILEHEADER header; // File header
							 // try opening the file; use "rb" mode to read this *binary* file.
	if ((fp = fopen(filename, "rb")) == NULL)
		return (NULL);
	// read the file header and any following bitmap information.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		// Couldn't read the file header - return NULL.
		fclose(fp);
		return (NULL);
	}
	// Check for BM reversed.
	if (header.bfType != 'MB') {
		// not a bitmap file - return NULL.
		fclose(fp);
		return (NULL);
	}
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		// couldn't allocate memory for bitmap info - return NULL.
		fclose(fp);
		return (NULL);
	}
	if (fread(*info, 1, infosize, fp) < infosize) {
		// Couldn't read the bitmap header - return NULL.
		free(*info);
		fclose(fp);
		return (NULL);
	}
	// Now that we have all the header info read in, allocate memory for the bitmap and read *it* in.
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth*(*info)->bmiHeader.biBitCount + 7) / 8 * abs((*info)->bmiHeader.biHeight);
	if ((bits = (GLubyte *)malloc(bitsize)) == NULL) {
		// Couldn't allocate memory - return NULL!
		free(*info);
		fclose(fp);
		return (NULL);
	}
	if (fread(bits, 1, bitsize, fp) < bitsize) {
		// couldn't read bitmap - free memory and return NULL!
		free(*info);
		free(bits);
		fclose(fp);
		return (NULL);
	}
	// OK, everything went fine - return the allocated bitmap.
	fclose(fp);
	return (bits);
}

void bmpTexture(UINT textureArray[], LPSTR file, int n) {
	BITMAPINFO *bitmapInfo; // Bitmap information
	GLubyte    *bitmapBits; // Bitmap data
	if (!file) {
		cout << "texture file not found!" << endl;
		return;
	}
	bitmapBits = LoadDIBitmap(file, &bitmapInfo);
	glGenTextures(1, &textureArray[n]);
	glBindTexture(GL_TEXTURE_2D, textureArray[n]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // must set to 1 for compact data
										   // glTexImage2D Whith size and minification
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bitmapInfo->bmiHeader.biWidth, bitmapInfo->bmiHeader.biHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmapBits);
}


Mesh* mesh1 = createPlane(1000, 1000, 40);
Mesh* mesh2 = createPlane(1000, 1000, 40);
Mesh* mesh3 = createCube();
Mesh* mesh4 = loadFile("lowpoyltree.obj");
Mesh* mesh5 = loadFile("deer-obj.obj");

// init
void init() {

	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);

	activeMats = loadMat();

	calculateNormalPerFace(mesh1);
	calculateNormalPerFace(mesh2);
	calculateNormalPerVertex(mesh2);
	calculateNormalPerFace(mesh3);
	calculateNormalPerVertex(mesh3);

	bmpTexture(textureArray, "grass.bmp", 0);
	bmpTexture(textureArray, "stones.bmp", 1);
	bmpTexture(textureArray, "treeLeaves.bmp", 2);
	bmpTexture(textureArray, "leopard.bmp", 3);

	displayPlaneFace = meshToDisplayList(mesh1, 6, activeMats[2], 0);
	displayPlaneVer = meshToDisplayList(mesh2, 7, activeMats[2], 0);
	displayCube = meshToDisplayList(mesh4, 8, activeMats[2], 2);
	displayBorder = meshToDisplayList(mesh3, 9, activeMats[1], 1);
	displayGoal = meshToDisplayList(mesh5, 10, activeMats[0], 3);

	ratio = (double)width / (double)height;
}



// reshape
void reshape(int w, int h) {

	width = w;
	height = h;

	if (h == 0) h = 1;
	ratio = 1.0f * w / h;

}

// mouse
void mouse(int button, int state, int x, int y) {
	
	mouse_x = x;
	mouse_y = y;
	mouse_button = button;

}


// motion
void motion(int x, int y) {

	if (mouse_button == GLUT_LEFT_BUTTON) {
		y_angle += (float(x - mouse_x) / width) *360.0;
		x_angle += (float(y - mouse_y) / height)*360.0;
	}

	if (mouse_button == GLUT_RIGHT_BUTTON) {

		scale += (y - mouse_y) / 100.0;
		if (scale < 0.1) scale = 0.1;
		if (scale > 7)	scale = 7;
	}

	mouse_x = x;
	mouse_y = y;

	glutPostRedisplay();
}



// moving



void move() {

	// rotate diamond
	angle = angle + 19.1;

	if (angle > 360) angle -= 360;

	moving_position.x = center_position.x + 28.0f * sin(angle * PI / 180);

	moving_position.z = center_position.z + 28.0f * cos(angle * PI / 180);

	glutPostRedisplay();

}



// text



void renderBitmapString(float x, float y, float z, char *string) {

	char *c;

	glRasterPos3f(x, y, z);   // fonts position

	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}



float ylight = 100.0;
float xlight = 100.0;
bool lightleft = false;

void lightRotation() {
	//Sun Rotation
	if (lightleft) {
		xlight += 0.1;
	}
	else {
		xlight -= 0.1;
	}

	if (xlight >= 100) {
		lightleft = false;
	}
	else if (xlight <= -750) {
		exit(1);
	}
}



// display
int sunMoveFlag = 0;


void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glViewport(0, 0, width, height);
	gluPerspective(45, ratio, 1, 1000);


	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	// lookAt fijo
	gluLookAt(0.0f, 40.0f, 0.0f,
		y_angle, 1.0f, -90.0f,
		0.0f, 1.0f, 0.0f);



	// camera
	glScalef(scale, scale, scale);
	glTranslatef(x_trans, 0.0f, y_trans);
	//glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
	//glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
	
	//Plane
	glPushMatrix();
	glTranslatef(-500, 0, -500);
	glCallList(displayPlaneVer);
	glPopMatrix();



	//Draw Maze

	glPushMatrix();

	glTranslatef(450, 0, 450);

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {

			if (mazeArray[j][i] == 1) {
				glPushMatrix();
				glTranslatef(-(i * 50), -20, -(j * 50));
				glScalef(20, 20, 20);
				glCallList(displayCube);
				glPopMatrix();
			}

			if (mazeArray[j][i] == 2) {
				glPushMatrix();
				glTranslatef(-(i * 50), 0, -(j * 50));
				glCallList(displayBorder);
				glPopMatrix();
			}
		}
	}

	glPopMatrix();

	//Emissive Material Goal
	glPushMatrix();
	glTranslatef(450, 0, 450);
	glTranslatef(-(2 * 50), 0, -(18 * 50));
	glCallList(displayGoal);
	glPopMatrix();

	if (mazeArray[playerY / 50][playerX / 50] == -1) {
		cout << "You Win\n";

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();

		glLoadIdentity();

		glColor3f(0.0, 0.0, 0.0);
		
		renderBitmapString(0.0, 13.0f, 0.0f, "YOU WIN!!!");
		glMatrixMode(GL_PROJECTION);

		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);

		glPopMatrix();
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();



	// texto
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glLoadIdentity();

	glColor3f(1.0, 1.0, 1.0);

	renderBitmapString(0.0, height - 13.0f, 0.0f, "Use [Mouse Left Key] to rotate");

	renderBitmapString(0.0, height - 26.0f, 0.0f, "Use [Mouse Right Key] to scale");

	glMatrixMode(GL_PROJECTION);



	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();



	GLfloat light_position[] = { xlight, ylight , 0, 0 };

	GLfloat light_ambient[] = { 0.0, 0.0, 0.5, 1.0 };

	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };



	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	if (sunMoveFlag >= 1000)
	{
		lightRotation();
		sunMoveFlag = 0;
	}
	else
	{
		sunMoveFlag += 1;
	}

	glutSwapBuffers();

	move();

}

bool checkLocation() {

	bool isSpace = true;

	if (mazeArray[playerY / 50][playerX / 50] > 0) {
		cout << "Collision\n";
		isSpace = false;
	}
	cout << playerX / 50 << " " << playerY / 50 << "\n";
	return isSpace;
}

void playerSControl(int key, int x, int y) {

	int prevX = playerX;
	int prevY = playerY;

	switch (key) {

	case GLUT_KEY_LEFT:

		playerX += 1;
		if (checkLocation()) {
			x_trans += 1;
		}
		else {
			playerX = prevX;
		}
		break;

	case GLUT_KEY_RIGHT:

		playerX -= 1;
		if (checkLocation()) {
			x_trans -= 1;
		}
		else {
			playerX = prevX;
		}
		break;

	case GLUT_KEY_UP:

		playerY += 1;
		if (checkLocation()) {
			y_trans += 1;
		}
		else {
			playerY = prevY;
		}
		break;

	case GLUT_KEY_DOWN:
		
		playerY -= 1;
		if (checkLocation()) {
			y_trans -= 1;
		}
		else {
			playerY = prevY;
		}
		break;

	}

}

void playerControl(unsigned char key, int x, int y){

	int prevX = playerX;
	int prevY = playerY;

	switch (key){

	case 'a':

		playerX += 1;
		if (checkLocation()) {
			x_trans += 1;
		}
		else {
			playerX = prevX;
		}
		break;

	case 'd':

		playerX -= 1;
		if (checkLocation()) {
			x_trans -= 1;
		}
		else {
			playerX = prevX;
		}
		break;

	case 'w':

		playerY += 1;
		if (checkLocation()) {
			y_trans += 1;
		}
		else {
			playerY = prevY;
		}
		break;

	case 's':
		
		playerY -= 1;
		if (checkLocation()) {
			y_trans -= 1;
		}
		else {
			playerY = prevY;
		}
		break;

	}

}



// main



void main(int argc, char* argv[]) {



	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(0, 0);

	glutInitWindowSize(width, height);

	glutCreateWindow("Quiz 7");

	glClearColor(0.2, 0.1, 0.7, 1.0);

	glutReshapeFunc(reshape);

	glutDisplayFunc(display);

	glutIdleFunc(display);

	glutMouseFunc(mouse);

	glutKeyboardFunc(playerControl);

	glutSpecialFunc(playerSControl);

	glutMotionFunc(motion);
	init();
	glutMainLoop();
}