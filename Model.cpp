
/********************************************

Course	: TGD2151	Computer Graphics Fundamentals
Session: Trimester 1, 2019/20
ID and	Name	#1 : 1171301301 VASHIEGARAN
Contacts	#1 : 01135617621  cool_vashie@yahoo.com
ID and	Name	#2 : 1151105268 Adam Lai bin Azman Lai
Contacts	#2 : 012-3793720 adamlai999@gmail.com
********************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include "vertex.h"
#include <GL/glui.h>

 #define GLUI_DEF_MAX_ARRAY  30




using namespace std;
int WindowWidth = 600, WindowHeight = 600;
double posX = 0.0, posY = 0.0, posZ = 0.0, posInc = 0.1, angleInc = 2.0;
float Rotx;
double rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;
static double scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
int option = 0, axisornot = 0, colorSeg = 0;
bool mousePanMode=false, mouseZoomMode=false, mouseRotationMode=false;
int mouseX = 0, mouseY = 0;
float eyeX=0, eyeY=0, eyeZ=0, focusX=0, focusY=0, focusZ=0, upX=0, upY=1, upZ=0;
void myInit();
void myDisplayFunc(void);
void switch_models(int index);

vertex Manipulation;
float transX=0, transY=0, transZ=0, transparent=1.0;
bool displayWireFrame = false, displayVertices = false, displayFaces = true;
static int model = 0;
int   wireframe = 0;
int   segments = 8;
int main_window;
GLUI *	Glui;
int	ActiveButton;			// current button that is down
GLuint	AxesList;			// list to hold the axes
int	AxesOn;					// != 0 means to draw the axes
int	DebugOn;				// != 0 means to print debugging info
int	DepthCueOn;				// != 0 means to use intensity depth cueing				// instance of glui window
int	GluiWindow;				// the glut id for the glui window
int	LeftButton;				// either ROTATE or SCALE
GLuint	BoxList;			// object display list
int	MainWindow;				// window id for main graphics window
GLfloat	RotMatrix[4][4];	// set by glui rotation widget
float	Scale, Scale2;		// scaling factors
int	WhichColor;				// index into Colors[ ]
int	WhichProjection;		// ORTHO or PERSP
int	Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;			// rotation angles in degrees
float	TransXYZ[3];		// set by glui translation widgets
const char *GLUITITLE   = { "User Interface Window" };
//Selection for which radio buttons
int radbut=0;
//to check if its ticked or no
int check=0;
//to check reset buttons
int check2=0;
GLUI_Spinner    *spinner;
//Matrix identity for rotations
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//Translation matrix
float obj_pos[] = { 0.0, 0.0, 0.0 };
// minimum allowable scale factor:
const float MINSCALE = { 0.05f };
int countX=0;
float x,y,z;

// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };

enum ButtonVals
{
	RESET,
	QUIT
};


void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != main_window )
    glutSetWindow(main_window);

  glutPostRedisplay();
}


void InitTransparent()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glClearColor(0,0,0,0);
}
void myDisplayFunc(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    GLfloat amb_light[] = { 0.5f,0.5f,0.5f,1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //GLfloat specular[] = { 0.1, 0.1, 0.1, 0.1};
    GLfloat lightpos[] = {0.f,0.f,200.0f,1.0f};

    glLightfv( GL_LIGHT0, GL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    //glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);



         if ( radbut == 0 ) {
      displayVertices = false, displayFaces = false, displayWireFrame = true;
        }
         if(radbut==1)
        {
            displayVertices = true, displayFaces = false, displayWireFrame = false;
        }
        if(radbut==2)
        {
            displayVertices = false, displayFaces = true, displayWireFrame = false;
        }


    //calling rotation matrix
    glMultMatrixf( view_rotate );
    //Translation matrix
    glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2]);



    glScalef(scaleX, scaleY, scaleZ);


    if(axisornot%2 == 1)
       Manipulation.DrawAxis();  //Press

    if(displayWireFrame)
        Manipulation.DisplayWireFrame(transparent, colorSeg);
    if(displayFaces)
        Manipulation.DisplayFaces(transparent, colorSeg);
    if(displayVertices)
        Manipulation.DisplayVertices(colorSeg);
        //Status on displays

    if(check==1)
            Manipulation.DrawAxis();
        // uniformly scale the scene:

	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );
	GLfloat scale2 = 1. + Scale2;		// because glui translation starts at 0.



    glPopMatrix();
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

void viewing()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, focusX, focusY, focusZ, upX, upY, upZ);
}
void mySpecialFunc(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_DOWN  :
        rotateX += angleInc;
        cout<<rotateX<<endl;
        break;
    case GLUT_KEY_UP    :
        rotateX -= angleInc;
         cout<<rotateX<<endl;
        break;
    case GLUT_KEY_RIGHT :
        rotateY += angleInc;
        cout<<rotateY<<endl;
        break;
    case GLUT_KEY_LEFT  :
        rotateY -= angleInc;
        cout<<rotateY<<endl;
        break;
    case GLUT_KEY_HOME  :
        rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;
        posX = 0.0, posY = 0.0, posZ = 0.0;
        scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
        break;
    case GLUT_KEY_F1    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case GLUT_KEY_F2    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    }
    glutPostRedisplay();
}


void myKeyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'c':
        case 'C': colorSeg += 1; break;
        case 'm': model += 1; switch_models(model); break;
        case 'n': model -= 1; switch_models(model); break;
        case 's': axisornot += 1; break;//On or Off the axis
        case 'x': transX +=0.005; break;
        case 'X': transX -=0.005; break;
        case 'y': transY +=0.005; break;
        case 'Y': transY -=0.005; break;
        case 'z': transZ +=0.005; break;
        case 'Z': transZ -=0.005; break;
        case 'v':
        case 'V': displayVertices = true, displayFaces = false; displayWireFrame = false; break;
        case 'f':
        case 'F': displayVertices = false, displayFaces = true; displayWireFrame = false; break;
        case 'w':
        case 'W': displayVertices = false, displayFaces = false; displayWireFrame = true; break;


    }

    glutPostRedisplay();
}
void myReshapeFunc(int width, int height)
{
    WindowWidth  = width;
    WindowHeight = height;
    glViewport(0, 0, width, height);
}
void myMotionFunc(int x, int y)
{
    y = WindowHeight - y;
    if(mousePanMode)
    {
        posX += (x - mouseX)*0.5;
        posY += (y - mouseY)*0.5;
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }
    if(mouseZoomMode)
    {
        GLdouble inc = (mouseY - y)*0.005;
        scaleX += inc;
        scaleY += inc;
        scaleZ += inc;
        mouseY = y;
        glutPostRedisplay();
    }
    if(mouseRotationMode)
    {
        rotateX += (mouseY - y)*0.5;
        rotateY += (x - mouseX)*0.5;
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }

}
void myMouseFunc(int button, int state, int x, int y)
{
    y = WindowHeight - y;
    switch (button)
    {
    case GLUT_MIDDLE_BUTTON:
        if (state==GLUT_DOWN && !mousePanMode)
        {
            mouseX = x;
            mouseY = y;
            mousePanMode = true;
        }
        if (state==GLUT_UP &&  mousePanMode)
        {
            mousePanMode = false;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state==GLUT_DOWN && !mouseZoomMode)
        {
            mouseY = y;
            mouseZoomMode = true;
        }
        if (state==GLUT_UP && mouseZoomMode)
        {
            mouseZoomMode = false;
        }
        break;
    case GLUT_LEFT_BUTTON:
        if (state==GLUT_DOWN && !mouseRotationMode)
        {
            mouseX = x;
            mouseY = y;
            mouseRotationMode = true;
        }
        if (state==GLUT_UP &&  mouseRotationMode)
        {
            mouseRotationMode = false;
        }
        break;
    }
}

void switch_models(int index)
{
    string skeleton_filename;
    string segment_filename;
    string points_filename;

    switch(index)
    {
        case 0:
            points_filename = "data/Animal11Seg.ply";
            segment_filename = "data/Animal11Seg.seg";
            //skeleton_filename = "data/chair.skel";
            eyeX=0, eyeY=0, eyeZ=3, focusX=0, focusY=0, focusZ=0, upX=0.0, upY=1, upZ=0;
            break;

    }
    cout << "Switched model to " << points_filename << endl;
    Manipulation.read3DModel(points_filename.c_str());

}
void myInit()
{
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition(300, 100); // Set top-left position
    glutInitWindowSize(WindowWidth, WindowHeight); //Set width and height
    glutCreateWindow("Model");// Create display window

    switch_models(0);

    InitTransparent();

    viewing();
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glEnable(GL_NORMALIZE);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutDisplayFunc(myDisplayFunc);// Specify the display callback function
    glutReshapeFunc(myReshapeFunc);
    glutKeyboardFunc(myKeyboardFunc);
    glutSpecialFunc(mySpecialFunc);
    glutMouseFunc(myMouseFunc);
    glutMotionFunc(myMotionFunc);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);


    cout<<"------------------------------------------------------------"<<endl;
    cout<<"Instruction:"<<endl;
    cout<<"s - On or Off the Axis"
        <<"\nHome - to reset model"
        <<"\nw or W - to display model WireFrame"
        <<"\nv or V - to display model vertices"
        <<"\nf or F - to display model surfaces"
        <<endl;

    cout<<"------------------------------------------------------------"<<endl;

}

void
Reset( void )
{

	obj_pos[0] = obj_pos[1] = obj_pos[2] = 0.;


    //reset rotation
      rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;

    view_rotate[1]=view_rotate[2]=view_rotate[3]=view_rotate[6]=view_rotate[7]=view_rotate[9]=view_rotate[11]=view_rotate[13]=view_rotate[14]=0.0;
    view_rotate[0]=view_rotate[5]=view_rotate[10]=view_rotate[15]= 1.0;
    view_rotate[4]=rotateX;
    view_rotate[8]=rotateY;
    view_rotate[12]=rotateZ;


    //reset object type
    radbut = 0;

    //reset axis
    check = 0;

}

Buttons( int id )
{
	switch( id )
	{
		case RESET:
			//Reset( );

            Reset( );
			Glui->sync_live( );
			glutSetWindow( MainWindow );
			glutPostRedisplay( );
			break;

		case QUIT:
			// gracefully close the glui window:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:

			Glui->close( );
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Button ID %d\n", id );
	}

}

void Yvalue(int &y)
{
        y = WindowHeight - y;

}

void initGlui(){

    glPushMatrix();

	GLUI_Panel *panel;
	GLUI_RadioGroup *group;
	GLUI_Rotation *rot;
	GLUI_Translation *trans, *scale;
	GLUI_Spinner *Trans_spinner;

	// setup the glui window:
	//glutInitWindowPosition( INIT_WINDOW_SIZE + 50, 0 );
	Glui = GLUI_Master.create_glui( (char *) GLUITITLE );
	Glui->add_statictext( (char *) GLUITITLE );
	Glui->add_separator( );

    GLUI_Panel *obj_panel = new GLUI_Panel( Glui, "Object Type" );
    group = new GLUI_RadioGroup( obj_panel,&radbut,4 );
    new GLUI_RadioButton( group, "Wireframe" );
    new GLUI_RadioButton( group, "verticle" );
    new GLUI_RadioButton( group, "Faces" );

	panel = Glui->add_panel( "Object Transformation" );
        //Create a rotation object for the model
        GLUI_Rotation *view_rot = new GLUI_Rotation(Glui, "Rotate", view_rotate );
        view_rot->set_spin( 1.0 );


		Glui->add_column_to_panel( panel, GLUIFALSE );
		trans = Glui->add_translation_to_panel( panel, "Trans X",  GLUI_TRANSLATION_Y ,&obj_pos[1] );
		trans->set_speed( 0.010f );


		Glui->add_column_to_panel( panel, GLUIFALSE );
		trans = Glui->add_translation_to_panel( panel, "Trans Y", GLUI_TRANSLATION_X,&obj_pos[0] );
		trans->set_speed( 0.05f );

		Glui->add_column_to_panel( panel, GLUIFALSE );
		trans = Glui->add_translation_to_panel( panel, "Trans Z",  GLUI_TRANSLATION_Z , &obj_pos[2] );
		trans->set_speed( 0.05f );


        Trans_spinner=
            Glui->add_spinner_to_panel(panel,"X",GLUI_SPINNER_FLOAT ,&obj_pos[1]);
            Glui->add_spinner_to_panel(panel,"Y",GLUI_SPINNER_FLOAT ,&obj_pos[0]);
            Glui->add_spinner_to_panel(panel,"Z",GLUI_SPINNER_FLOAT ,&obj_pos[2]);

        new GLUI_Checkbox( obj_panel, "Axis", &check );




       // Glui->add_button_to_panel( panel, "Quit", QUIT, (GLUI_Update_CB) Buttons );

        Glui->add_button_to_panel( panel, "Reset", RESET, (GLUI_Update_CB) Buttons );


	// tell glui what graphics window it needs to post a redisplay to:	Glui->set_main_gfx_window( main_window );
	glPopMatrix();
    glutPostRedisplay();
	// set the graphics window's idle function if needed:
    GLUI_Master.set_glutIdleFunc( NULL );
    glutMainLoop();

}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    myInit();
    initGlui();
    glutMainLoop();
    return 0;


}

