#include <vector>
#include "imathvec.h"
#include "glut.h"
#include<iostream>
#include<fstream>
#include<string>
#include "Quiz4Material.h"

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

float ylight = 100.0;
float xlight = 100.0;
bool lightleft = false;

GLuint displayDiamondFace, displayDiamondVer, displayPlaneFace, displayPlaneVer, displayMeshVer, displayMeshFace, displayObjFace, displayObjVer;

//PLAYER MOVEMENT
float playerX =  100;
float playerY =  1;
float playerZ =  0;
float playerZAccel = 0.01;

//Camera Movement
float cameraX = 100;
float cameraY = 15;
float cameraZ = 0;

vector<GLuint> activeMats;

// controling parameters
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 1.0;
float x_angle = 0.0;
float y_angle = 0.0;

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
bool normalBool = false;
bool lightBool = false;
/* ---------------------------------------- BEGIN:MESH CODE ---------------------------------------- */

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
	if (!part_string.empty())
		stlv_string.push_back(part_string);
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

//Calculate Normals Per Face
void calculateNormalPerFace(Mesh* mesh)
{
	Vec3<float> v1, v2, v3, v4, v5;

	mesh->dot_normal.clear();
	mesh->face_index_normal.clear();

	for (float i = 0; i < mesh->face_index_vertex.size(); i += 3)
	{
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

//Calculate Normals Per Vertex
void calculateNormalPerVertex(Mesh* mesh)
{
	vector<int> face_index_normalPerFace = mesh->face_index_normal;
	vector<Vec3f> dot_normalPerFace = mesh->dot_normal;

	mesh->dot_normal.clear();
	mesh->face_index_normal.clear();
	Vec3<float> suma; suma.x = 0; suma.y = 0; suma.z = 0;

	for (unsigned int val = 0; val < mesh->dot_vertex.size(); val++)
	{
		mesh->dot_normal.push_back(suma);
	}

	for (long pos = 0; pos < mesh->face_index_vertex.size(); pos++)
	{
		mesh->dot_normal[mesh->face_index_vertex[pos]] += dot_normalPerFace[face_index_normalPerFace[pos]];
	}

	for (unsigned int val = 0; val < mesh->dot_normal.size(); val++)
	{
		mesh->dot_normal[val] = mesh->dot_normal[val].normalize();
	}

	for (unsigned int pos = 0; pos < mesh->face_index_vertex.size(); pos++)
	{
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

// draw
GLuint meshToDisplayList(Mesh* m, int id, GLuint mat = NULL) {
	GLuint listID = glGenLists(id);
	glNewList(listID, GL_COMPILE);
	if (mat != NULL)
		glCallList(mat);

	glBegin(GL_TRIANGLES);

	for (unsigned int i = 0; i < m->face_index_vertex.size(); i++) {

		if (!m->dot_normal.empty() && !m->face_index_normal.empty())
			glNormal3fv(&m->dot_normal[m->face_index_normal[i]].x);
		if (!m->dot_texture.empty() && !m->face_index_texture.empty())
			glTexCoord2fv(&m->dot_texture[m->face_index_texture[i]].x);
		// color
		Vec3f offset = (m->dot_vertex[m->face_index_vertex[i]]);
		//
		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glVertex3fv(&m->dot_vertex[m->face_index_vertex[i]].x);
	}

	glEnd();
	glEndList();
	return listID;
}
/* ----------------------------------------- END:MESH CODE  ----------------------------------------- */


Mesh* mesh1 = createPlane(1000, 1000, 40);
//Mesh* mesh2 = loadFile("imp_fly_tiefighter.obj");
Mesh* mesh2 = loadFile("C:/Users/Austin/Desktop/ASUFall16/SER431/Examples/mesh/data/Charizard.obj");


// init
void init() {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	activeMats = loadMat();


	calculateNormalPerFace(mesh1);

	displayPlaneFace = meshToDisplayList(mesh1, 7, activeMats[2]);
	displayMeshFace = meshToDisplayList(mesh2, 8, activeMats[1]);

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
		//if (y_angle >= 0)
		//{
			y_angle += (float(x - mouse_x) / width) *360.0;
			cout << y_angle << endl;
		//}
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

/*
// moving
void move() {
	// rotate diamond
	angle = angle + 19.1;
	if (angle > 360) angle -= 360;
	moving_position.x = center_position.x + 28.0f * sin(angle * PI / 180);
	moving_position.z = center_position.z + 28.0f * cos(angle * PI / 180);
	glutPostRedisplay();
}
*/

// text
void renderBitmapString(float x, float y, float z, char *string) {
	char *c;
	glRasterPos3f(x, y, z);   // fonts position
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

//Light Rotation
void lightRotation() {
	//Sun Rotation
	if (lightleft) {
		xlight += 1.0;
	}
	else {
		xlight -= 1.0;
	}
	if (xlight == 100) {
		lightleft = false;
	}
	else if (xlight == -100) {
		lightleft = true;
	}
}

// display
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(3, ratio, 1, 1000);

	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	// camera
	//glScalef(scale, scale, scale);
	glRotatef(x_angle*0.1, 1.0f, 0.0f, 0.0f);
	glRotatef(y_angle*0.1, 0.0f, 1.0f, 0.0f);
	//glTranslatef(0.0f, 0.0f, 0.0f);


	//glCallList(2);

	//LOOK AT
	gluLookAt(cameraX, cameraY+5, cameraZ -4,
			  playerX, cameraY+5, playerZ,
			  0.0f, 1.0f, 0.0f);

	//FLOOR
	glPushMatrix();
	glTranslatef(-500, 0, -500);
	if (!normalBool) {
		glCallList(displayPlaneFace);
	}
	else if (normalBool) {
		glCallList(displayPlaneVer);
	}
	glPopMatrix();
	

	//PLAYER
	glPushMatrix();
	//glRotatef(angle_animated_object, 0, 1, 0);
	//playerZ = playerZ + playerZAccel;
	glTranslatef(playerX, playerY, playerZ);
	glCallList(displayMeshFace);
	glPopMatrix();

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

	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	if (!lightBool) {
		light_position[0] = { 100 };
		light_position[1] = { 100 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	else if (lightBool) {
		lightRotation();
	}

	glutSwapBuffers();
	//move();

}


//--------------------------PLAYER MOVEMENT------------------------------//
//left/right
void movePlayerX(float i)
{
	playerX = playerX + i;
	//angle_animated_object += (i*10);
	cameraX = playerX;
}

//up/down
//void movePlayerY(float i)
//{
//	playerY = playerY + i;
//}

//speed
void movePlayerZpos(float i)
{
	/*
	if (playerZAccel <= 0.8)
	{
		playerZAccel += i;
	}

	if (playerZAccel >= 0.8)
	{
		playerZAccel = 0.8;
	}

	if (playerZAccel == 0.00000000000000)
	{
		playerZAccel = 0.2;
	}
	*/
	playerZ = playerZ + i;
	cameraZ = playerZ;

}


void movePlayerZneg(float i)
{
	/*
	if (playerZAccel >= -0.2)
	{
		playerZAccel -= i;
	}

	if (playerZAccel <= -0.2)
	{
		playerZAccel = -0.2;
	}

	if (playerZAccel == 0.00000000000000)
	{
		playerZAccel = 0.2;
	}
	*/
	playerZ = playerZ - i;
	cameraZ = playerZ;

}

//------------------------------PLAYER MOVEMENT END-------------------------//


//------------------------------KEYBOARD INPUT------------------------------//
void inputKey(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'a': movePlayerX(-10); break;
	case 'd': movePlayerX( 10); break;
	case 'w': movePlayerZpos(10); break;
	case 's': movePlayerZneg(10); break;
	}
}
//------------------------------KEYBOARD INPUT END---------------------------//



/*
enum MENU_TYPE
{
	PERFACE,
	PERVERTEX,
	STATIC,
	MOVING
};


void processMenu(int item)
{
	switch (item)
	{
	case PERFACE:
		normalBool = 0;
		break;
	case PERVERTEX:
		normalBool = 1;
		calculateNormalPerVertex(mesh2);
		calculateNormalPerVertex(mesh3);
		calculateNormalPerVertex(mesh5);
		calculateNormalPerVertex(mesh6);
		glutPostRedisplay();
		break;
	case STATIC:
		lightBool = 0;
		break;
	case MOVING:
		lightBool = 1;
		break;
	default:
	{              }
	break;
	}
}
*/

/*
void createGLUTMenus() {
	int menu;
	// create the menu and
	// "processMenuEvents" will handle the events

	menu = glutCreateMenu(processMenu);

	glutAddMenuEntry("PERFACE", PERFACE);
	glutAddMenuEntry("PERVERTEX", PERVERTEX);
	glutAddMenuEntry("Static", STATIC);
	glutAddMenuEntry("Moving", MOVING);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
*/

// main
void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Mesh Data Structure");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(inputKey);

	//createGLUTMenus();

	init();

	glutMainLoop();
}