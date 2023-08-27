//////////////////////////////////////////////////////////////////////
//
//	University of Leeds
//	COMP 5812M Foundations of Modelling & Rendering
//	User Interface for Coursework
//
//	September, 2020
//
//  -----------------------------
//  Raytrace Render Widget
//  -----------------------------
//
//	Provides a widget that displays a fixed image
//	Assumes that the image will be edited (somehow) when Render() is called
//  
////////////////////////////////////////////////////////////////////////

// include guard
#ifndef RAYTRACE_RENDER_WIDGET_H
#define RAYTRACE_RENDER_WIDGET_H

#include <vector>
#include <mutex>
#include <algorithm>

// include the relevant QT headers
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <thread>

// and include all of our own headers that we need
#include "../Render/ThreeDModel.h"
#include "../RenderParameters.h"
#include "../Math/Ray.h"
#include "../Render/Scene.h"

// class for a render widget with arcball linked to an external arcball widget
class RaytraceRenderWidget : public QOpenGLWidget										
	{ // class RaytraceRenderWidget
	Q_OBJECT
	private:	
	// the geometric object to be rendered
    std::vector<ThreeDModel> *texturedObjects;

	// the render parameters to use
	RenderParameters *renderParameters;

	// An image to use as a framebuffer
	RGBAImage frameBuffer;

    //A friendly Scene representation that we control
    Scene raytraceScene;
    Scene&rs = raytraceScene;

    std::thread raytracingThread;

	public:
	// constructor
	RaytraceRenderWidget
			(
	 		// the geometric object to show
            std::vector<ThreeDModel> 		*newTexturedObject,
			// the render parameters to use
			RenderParameters 	*newRenderParameters,
			// parent widget in visual hierarchy
			QWidget 			*parent
			);
	
	// destructor
	~RaytraceRenderWidget();
			
	protected:
	// called when OpenGL context is set up
	void initializeGL();
	// called every time the widget is resized
	void resizeGL(int w, int h);
	// called every time the widget needs painting
	void paintGL();
	
	// mouse-handling
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

	// these signals are needed to support shared arcball control
	public:

    // routine that generates the image
    void Raytrace();
    //threading stuff
    void RaytraceThread();

    Vector4 directLighting(Scene::CollisionInfo cur_tri, Vector3 o, Vector3 dir, Light* lit, Vector3 n);
//    Vector4 indirectLighting(Scene::CollisionInfo cur_tri, Vector3 o, Vector3 dir, Vector3 n, int loops, int depth, float cur_ior);
    Vector4 ambient(Triangle tri);
    Vector4 emissive(Triangle tri);
//    Vector4 specular(Scene::CollisionInfo cur_tri, Vector3 o, Vector3 dir, Light* lit, Vector3 n);
//    Vector4 diffuse(Scene::CollisionInfo cur_tri, Vector3 o, Vector3 dir, Light* lit, Vector3 n);

    Ray reflect(Vector3 hit_point, Vector3 o, Vector3 dir, Vector3 n); // starting point, direction, normal
    Ray refract(Scene::CollisionInfo triangle, float cur_ior, Vector3 hit_point, Vector3 o, Vector3 dir, Vector3 n); // starting point, direction, normal
    Ray refract_1(Scene::CollisionInfo triangle, float cur_ior, Vector3 hit_point, Vector3 o, Vector3 dir, Vector3 n); // starting point, direction, normal

    float rand_angle();
    Vector4 calculateLight(Ray r, Vector3 o, Vector3 dir, float curIOR, int depth); // ray, starting point, ray direction
    Vector4 indirectLighting(Ray r, Vector3 o, Vector3 dir, float cur_ior, int depth); // ray, starting point, ray direction

    std::vector<Light*> monto_sources;

    template<typename T>
    T clamp(const T &value, const T &low, const T &high);

private:

    void forceRepaint();
    std::mutex drawingLock;
    std::mutex raytraceAgainLock;
    bool restartRaytrace;

	signals:
	// these are general purpose signals, which scale the drag to 
	// the notional unit sphere and pass it to the controller for handling
	void BeginScaledDrag(int whichButton, float x, float y);
	// note that Continue & End assume the button has already been set
	void ContinueScaledDrag(float x, float y);
	void EndScaledDrag(float x, float y);



	}; // class RaytraceRenderWidget

#endif
