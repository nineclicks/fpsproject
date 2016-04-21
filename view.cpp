#include "view.h"

View::View(Game *g)
{
    InitWindow();
    InitOpenGL();
    game = g;
    zoom = 1.5f;
}

int View::GetWidth()
{
    return width;
}

int View::GetHeight()
{
    return height;
}

void View::InitWindow()
{
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
    std::cout << "\n\tcannot connect to X server\n" << std::endl;
    exit(EXIT_FAILURE);
    }
    root = DefaultRootWindow(dpy);

    XWindowAttributes getWinAttr;
    XGetWindowAttributes(dpy, root, &getWinAttr);

    XGrabKeyboard(dpy, root,
        False, GrabModeAsync, GrabModeAsync, CurrentTime);

    width = getWinAttr.width / 4;
    height = getWinAttr.height / 4;

    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if(vi == NULL) {
    std::cout << "\n\tno appropriate visual found\n" << std::endl;
    exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
    ButtonPress | ButtonReleaseMask |
    PointerMotionMask |
    StructureNotifyMask | SubstructureNotifyMask;
    swa.override_redirect = True;
    
    win = XCreateWindow(dpy, root, 0, 0, width, height,
                        0, vi->depth, InputOutput, vi->visual,
                        CWBorderPixel|CWColormap|CWEventMask|
                        CWOverrideRedirect, &swa);
    
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
    CenterCursor();
}

void View::InitOpenGL()
{
    float dep = 1.5;
    //OpenGL initialization
    glViewport(0, 0, width, height);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    glFrustum(-0.1/height*width, 0.1/height*width, -0.1, 0.1, dep, 200.0);
    //Set the screen background color
    glClearColor(1.0, 1.0, 1.0, 1.0);
    ///////////////////////////////////
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    ShowCursor(0);
}

void View::set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Test");
}

void View::cleanupXWindows(void)
{
    //do not change
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void View::ShowCursor(const int onoff)
{
    // Source: Gordon's asteroids.cpp
    if (onoff) {
        //this removes our own blank cursor.
        XUndefineCursor(dpy, win);
    return;
    }
    //vars to make blank cursor
    Pixmap blank;
    XColor dummy;
    char data[1] = {0};
    Cursor cursor;
    //make a blank cursor
    blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
    if (blank == None)
    std::cout << "error: out of memory." << std::endl;
    cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
    XFreePixmap(dpy, blank);
    //this makes you the cursor. then set it using this function
    XDefineCursor(dpy, win, cursor);
    //after you do not need the cursor anymore use this function.
    //it will undo the last change done by XDefineCursor
    //(thus do only use ONCE XDefineCursor and then XUndefineCursor):
}

void View::CenterCursor()
{
    XWarpPointer(dpy,root,root,0,0,width,height,
                 width/2,height/2);
}

Display *View::GetDisplay()
{
    return dpy;
}