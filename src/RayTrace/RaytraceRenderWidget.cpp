#include <math.h>
#include <random>
#include <QTimer>
#include <QDebug>

#include "RaytraceRenderWidget.h"


#define N_THREADS 16
#define N_LOOPS 100
#define N_BOUNCES 5
#define TERMINATION_FACTOR 0.35f


constexpr float bias = (1e-2);
const Vector3 camera = Vector3{0.0f, 0.0f, 0.0f}; // set eye to (0,0,0)
// constructor
RaytraceRenderWidget::RaytraceRenderWidget
        (
                // the geometric object to show
                std::vector<ThreeDModel> *newTexturedObject,
                // the render parameters to use
                RenderParameters *newRenderParameters,
                QWidget *parent
        )
        :
// start by calling inherited constructor with parent widget's pointer
        QOpenGLWidget(parent),
        // then store the pointers that were passed in
        texturedObjects(newTexturedObject),
        renderParameters(newRenderParameters),
        raytraceScene(texturedObjects, renderParameters) { // constructor
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    restartRaytrace = false;
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RaytraceRenderWidget::forceRepaint);
    timer->start(30);
    // leaves nothing to put into the constructor body
} // constructor

void RaytraceRenderWidget::forceRepaint() {
    update();
}

// destructor
RaytraceRenderWidget::~RaytraceRenderWidget() { // destructor
} // destructor

// called when OpenGL context is set up
void RaytraceRenderWidget::initializeGL() { // RaytraceRenderWidget::initializeGL()
} // RaytraceRenderWidget::initializeGL()

// called every time the widget is resized
void RaytraceRenderWidget::resizeGL(int w, int h) { // RaytraceRenderWidget::resizeGL()
    // resize the render image
    frameBuffer.Resize(w, h);
} // RaytraceRenderWidget::resizeGL()

// called every time the widget needs painting
void RaytraceRenderWidget::paintGL() { // RaytraceRenderWidget::paintGL()
    // set background colour to white
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // and display the image
    glDrawPixels(frameBuffer.width, frameBuffer.height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.block);
} // RaytraceRenderWidget::paintGL()


// routine that generates the image
void RaytraceRenderWidget::Raytrace() { // RaytraceRenderWidget::Raytrace()

    restartRaytrace = true;
    if (raytracingThread.joinable())
        raytracingThread.join();
    restartRaytrace = false;

    //To make our lifes easier, lets calculate things on VCS.
    //So we need to process our scene to get a triangle soup in VCS.
    raytraceScene.updateScene();

    frameBuffer.clear(RGBAValue(0.0f, 0.0f, 0.0f, 1.0f));

    raytracingThread = std::thread(&RaytraceRenderWidget::RaytraceThread, this);
    raytracingThread.detach();

} // RaytraceRenderWidget::Raytrace()


void RaytraceRenderWidget::RaytraceThread() {
    int loops = renderParameters->monteCarloEnabled ? N_LOOPS : 1;
    std::cout << "I Will do " << loops << " loops" << std::endl;


    auto lights = renderParameters->lights;
    //Each pixel in parallel using openMP.
    for (int loop = 0; loop < loops; loop++) {
#pragma omp parallel for schedule(dynamic)
        for (int j = 0; j < frameBuffer.height; j++) {
            for (int i = 0; i < frameBuffer.width; i++) {

                // YOUR CODE GOES HERE
                Vector4 color; // return value
                int depth = 20; // use to terminate the recursion

                Vector3 dir_camera =
                        Vector3{float(2 * i) / (frameBuffer.width) - 1.0f, float(2 * j) / (frameBuffer.width) - 1.0f,
                                -1.0f} - camera;// set ray direction

                Ray r_camera(camera, dir_camera, Ray::Type::primary); // starting point, direction , ray tpye

                if (renderParameters->monteCarloEnabled) {// sry I did not complete monte carlo sampling,
                    color = indirectLighting(r_camera, camera, dir_camera, 1.0f, depth);
                } else {
                    color = calculateLight(r_camera, camera, dir_camera, 1.0f,
                                           depth); // calculate your raytraced color here.
                }

                //Scene scene;
                //Gamma correction....
                float gamma = 2.2f;
                //We already calculate everything in float, so we just do gamma correction before putting it integer format.
                color.x = pow(color.x, 1 / gamma) / float(loop + 1);
                color.y = pow(color.y, 1 / gamma) / float(loop + 1);
                color.z = pow(color.z, 1 / gamma) / float(loop + 1);
                frameBuffer[j][i] = ((loop) / float(loop + 1)) * frameBuffer[j][i] +
                                    RGBAValue(color.x * 255.0f, color.y * 255.0f, color.z * 255.0f, 255.0f);
                frameBuffer[j][i].alpha = 255;
            }
        }
        std::cout << " Done a loop!" << std::endl;
        if (restartRaytrace) {
            return;
        }
    }
    std::cout << "Done!" << std::endl;
}

Vector4
RaytraceRenderWidget::directLighting(Scene::CollisionInfo cur_tri, Vector3 o, Vector3 dir, Light *lit, Vector3 n) {
    // load light info
    Vector3 lightLocalPosition = lit->GetPositionCenter().Point();
    Vector3 lightPosition = raytraceScene.getModelview() * lightLocalPosition;

    auto impactPoint = o + dir.unit() * cur_tri.t;
    auto lightVector = lightPosition - impactPoint;
    //light direction
    Vector3 vecLight = (lightVector).unit();
    // ray direction
    Vector3 vecRay = (o - impactPoint).unit();
    //half vector
    Vector3 vecHalf = (vecLight + vecRay).unit();

    auto norm = n.unit();
    float N_dot_H = std::fmax(norm.dot(vecHalf), 0.f);
    float N_dot_L = std::fmax(norm.dot(vecLight), 0.f);
    float distanceSquare = lightVector.dot(lightVector);

    const auto &material = cur_tri.triangle.shared_material;

    // specular color
    Vector3 specular = (material->specular / distanceSquare) * std::pow(N_dot_H, material->shininess);

    // diffuse color
    Vector3 diffuse = (material->diffuse / distanceSquare) * N_dot_L;

    return Vector4(specular + diffuse);
}

Vector4 RaytraceRenderWidget::indirectLighting(Ray r, Vector3 o, Vector3 dir, float cur_ior, int depth) {
    if (!depth)
        return Vector4(0, 0, 0, 0);
    Vector4 color;
    Scene::CollisionInfo triangle = raytraceScene.closestTriangle(r);
    if (triangle.t < 2.00f && triangle.t > 0.00f) {
        // move definitos above options
        Vector3 p = o + dir.unit() * triangle.t; // p = o + td, the intersected point (primary)

        auto &tri_mat = triangle.triangle.shared_material; // alias of triangle materials
        auto &cur_tri = triangle.triangle;  // alias of current triangle

        // hitting point normal
        Vector3 n_0 = cur_tri.normals[0].Vector();
        Vector3 n_1 = cur_tri.normals[1].Vector(); // triangle normal in the point
        Vector3 n_2 = cur_tri.normals[2].Vector(); // triangle normal in the point
        Vector3 n = n_0 * triangle.triangle.Baricentric(p).x + n_1 * triangle.triangle.Baricentric(p).y +
                    n_2 * triangle.triangle.Baricentric(p).z;
        Vector3 b_1, b_2;

        if (n.x > 0.9f)
            Vector3 b_1 = {0, 1, 0};
        else
            Vector3 b_1 = {1, 0, 0};


        float rand_pai = 2 * M_PI * (rand() / float(RAND_MAX)); //[0,2pi]
        float rand_theta = (M_PI / 2) * (rand() / float(RAND_MAX)); //[0,pi/2]

        Vector3 dir_mc = {cos(rand_pai) * sin(rand_theta), sin(rand_pai) * sin(rand_theta),
                          cos(rand_theta)}; // direction of this ray

        dir_mc = Vector3{b_1.x * dir_mc.x + n.x * dir_mc.y + b_2.x * dir_mc.z,
                         b_1.y * dir_mc.x + n.y * dir_mc.y + b_2.y * dir_mc.z,
                         b_1.z * dir_mc.x + n.z * dir_mc.y + b_2.z * dir_mc.z};

        //    printf("depth is %f\n",depth);

        // construct this ray as monto-carlo ray
        Ray r_mc(p, dir_mc.unit(), Ray::Type::secondary); // starting point, direction , ray tpye
        Vector4 light_color = /*emissive(cur_tri) +*/ calculateLight(r_mc, p, dir_mc.unit(), cur_ior, depth - 1);

        Light *indirect_source = new Light(Light::Directional, light_color, Vector4(p), r_mc.direction, Vector4(),
                                           Vector4());

        color = color + directLighting(triangle, o, dir, indirect_source, n);

        color = color / (1 / 2 * M_PI);

        // traverse each light
        for (auto &light: renderParameters->lights) {
            color = color + directLighting(triangle, o, dir, light, n);
        }
        color = color + emissive(cur_tri);
    } else color = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

    return color;
}


Vector4 RaytraceRenderWidget::emissive(Triangle tri) {
    return tri.shared_material->emissive;
}

Vector4 RaytraceRenderWidget::ambient(Triangle tri) {
    return tri.shared_material->ambient;
}

Ray RaytraceRenderWidget::reflect(Vector3 hit_point, Vector3 o, Vector3 dir, Vector3 n) {
    float in_coming_angle = (o - dir.unit()).dot(n.unit()); // in coming angle
    Vector3 dir_reflect = (dir.unit() + in_coming_angle * 2 * n.unit()).unit();

    return Ray(hit_point, dir_reflect, Ray::Type::secondary); // starting point, direction , ray tpye
}

Ray
RaytraceRenderWidget::refract(Scene::CollisionInfo triangle, float cur_ior, Vector3 hit_point, Vector3 o, Vector3 dir,
                              Vector3 n) {
    // load IOR info
    float obj_ior = triangle.triangle.shared_material->indexOfRefraction; // get current obj ior (next ior)
    float div_ior = cur_ior / obj_ior;

    // light in the object!, nothing will happpen! light should go staright!
    if (div_ior == 1.0f) {
        return Ray(hit_point, dir.unit(), Ray::Type::secondary); // starting point, direction , ray tpye
    } else {
        float cos_theta_1 = (o - dir).unit().dot(n.unit());
        float sqr_root = 1 - div_ior * div_ior * (1 - dir.dot(n) * dir.dot(n));
//        if(sqr_root<0) // prevant total internal reflection, it should do reflection! though there is no total internal reflection in this coursework!
//        {
//            float in_coming_angle = (o-dir.unit()).dot(n.unit()); // in coming angle
//            Vector3 dir_reflect  = (dir.unit() + in_coming_angle*2*n.unit()).unit();

//            return Ray(hit_point, dir_reflect, Ray::Type::secondary); // starting point, direction , ray tpye
//        }
        if (cos_theta_1 < 0) // dot product < 0, light goes into the object!
        {
            cos_theta_1 = -cos_theta_1;
        } else {
            div_ior = 1.0f / div_ior;
            n = {-n.x, -n.y, -n.z};
        }
        Vector3 item_1 = div_ior * (dir.unit() + n.unit() * (cos_theta_1)); // n/n_t * (d + n*cos_theta)
        Vector3 item_2 = n * std::sqrt(sqr_root);
        Vector3 dir_refract = (item_1 - item_2).unit();
        return Ray(hit_point, dir_refract, Ray::Type::secondary); // starting point, direction , ray tpye
    }
}

float RaytraceRenderWidget::rand_angle() {
//    srand((unsigned)time(0)); // choose a seed
    float angle = rand() % 180 + rand() / float(RAND_MAX);
//    printf("%f\n",angle);

    if (angle < 0.f || angle > 180.f) return rand_angle();
    else return angle;
}

Vector4 RaytraceRenderWidget::calculateLight(Ray r, Vector3 o, Vector3 dir, float curIOR, int depth) {

    if (!depth)
        return Vector4(0, 0, 0, 0);

    Vector4 color;
    Scene::CollisionInfo collisionInfo = raytraceScene.closestTriangle(r);
    if (!(collisionInfo.t < 2.00f && collisionInfo.t > 0.00f)) return Vector4(0, 0, 0, 0);
    // move definitos above options
    Vector3 hitPoint = o + dir.unit() * collisionInfo.t; // hitPoint = o + td, the intersected point (primary)

    auto &lights = renderParameters->lights; // alias of lights should be used
    auto &material = collisionInfo.triangle.shared_material; // alias of collisionInfo materials
    auto &triangle = collisionInfo.triangle;  // alias of current collisionInfo

    // hitting point normal
    Vector3 n_0 = triangle.normals[0].Vector();
    Vector3 n_1 = triangle.normals[1].Vector(); // collisionInfo normal in the point
    Vector3 n_2 = triangle.normals[2].Vector(); // collisionInfo normal in the point
    Vector3 n = n_0 * triangle.Baricentric(hitPoint).x + n_1 * triangle.Baricentric(hitPoint).y +
                n_2 * triangle.Baricentric(hitPoint).z;

    auto norm = n.unit();

    Vector4 reflectedColor;
    Vector4 refractedColor;

    if (renderParameters->interpolationRendering) {
        color = n_0.wiseAbs() * triangle.Baricentric(hitPoint).x +
                n_1.wiseAbs() * triangle.Baricentric(hitPoint).y +
                n_2.wiseAbs() * triangle.Baricentric(hitPoint).z;
    } else if (renderParameters->phongEnabled) {// now do recursions
        // if enable the reflection option and the object is a mirror
        if (renderParameters->reflectionEnabled && material->reflectivity != 0) {
            Ray rayReflect = reflect(hitPoint, o, dir, n);
            color += material->reflectivity *
                     calculateLight(rayReflect, hitPoint, rayReflect.direction.unit(), curIOR, depth--);
        }
        // when the object is a transparency, refraction will happen
        if (renderParameters->refractionEnabled && (!material->isLight()) && material->transparency != 0) {
            Ray rayRefract = refract(collisionInfo, curIOR, hitPoint, o, dir, n);
            color += material->transparency *
                     calculateLight(rayRefract, hitPoint, rayRefract.direction.unit(), material->indexOfRefraction,
                                    depth--);
        }
        if (renderParameters->fresnelRendering) {
            // load IOR info
            float IORFrom = curIOR;
            float IORTo = material->indexOfRefraction;
            float IORDiv = IORFrom / IORTo;



            // compute cos of in coming light angle
            float L_dot_N = (o - dir).unit().dot(norm);
            L_dot_N = clamp(L_dot_N, -1.f, 1.f);

            if (L_dot_N > 0)
                std::swap(IORFrom, IORTo);

            auto R_dot_N = dir.dot(n);
            auto k = 1.f - std::pow(IORDiv, 2) * (1.f - std::pow(R_dot_N, 2));
            // prevent total internal reflection
            if (k < 0.f && material->reflectivity != 0) {
                Ray ray = reflect(hitPoint, o, dir, n);
                color += calculateLight(ray, hitPoint, ray.direction.unit(), curIOR, depth--);
            } else {
                // compute sin of transmission angle
                float sin_refract = (IORFrom / IORTo) * std::sqrt(std::fmax(0.f, 1.f - L_dot_N * L_dot_N));
                // so get cos
                float cos_refract = std::sqrt(std::fmax(0.f, 1 - sin_refract * sin_refract));

                if (L_dot_N < 0)
                    L_dot_N = -L_dot_N;

                // using fresnel equations to update fresnel cofficents
                float R_r = (IORTo * L_dot_N - IORFrom * cos_refract) / (IORTo * L_dot_N + IORFrom * cos_refract);
                float R_t = (IORFrom * cos_refract - IORTo * L_dot_N) / (IORFrom * cos_refract + IORTo * L_dot_N);

                // compute fresnel cofficents
                float F_r = (R_r * R_r + R_t * R_t) / 2;
                float F_t = 1 - F_r;

                if (material->reflectivity != 0) {
                    Ray ray = reflect(hitPoint, o, dir, n);
                    color += F_r * calculateLight(ray, hitPoint, ray.direction.unit(), curIOR, depth--);
                }
                if (material->transparency != 0) {
                    Ray ray = refract(collisionInfo, curIOR, hitPoint, o, dir, n);
                    color += F_t * calculateLight(ray, hitPoint, ray.direction.unit(), curIOR, depth--);
                }

            }
        }

        // traverse each light
        for (auto &light: lights) {
            if (renderParameters->shadowsEnabled) {
                auto lightPosition = raytraceScene.getModelview() * light->GetPositionCenter().Vector();
                auto lightVector = lightPosition - hitPoint;
                // shadow revelant
                // cast a second ray from the intersected point to towards the hitPoint
                Ray r_shadow(hitPoint, lightVector, Ray::Type::secondary);
                // find the closest collisionInfo along this ray
                Scene::CollisionInfo hit_shadow = raytraceScene.closestTriangle(r_shadow);

                // if the no collisionInfo found, means it did not block, so do direct shading
                // not hit, not in the shadow, do direct shading!
                if (hit_shadow.t >= (lightVector.length() - bias)) {
                    color += reflectedColor + directLighting(collisionInfo, o, dir, light, n);
                }
            } else {
                color += reflectedColor + directLighting(collisionInfo, o, dir, light, n);
            }
        }
        if (renderParameters->monteCarloEnabled) {
            color = color + emissive(triangle);
        } else color = color + emissive(triangle) + ambient(triangle);

    } else { color = Vector4(1.0f, 1.0f, 1.0f, 0.0f); }

    return color;
}

// mouse-handling
void RaytraceRenderWidget::mousePressEvent(QMouseEvent *event) { // RaytraceRenderWidget::mousePressEvent()
    // store the button for future reference
    int whichButton = int(event->button());
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width   
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y()) / size;


    // and we want to force mouse buttons to allow shift-click to be the same as right-click
    unsigned int modifiers = event->modifiers();

    // shift-click (any) counts as right click
    if (modifiers & Qt::ShiftModifier)
        whichButton = Qt::RightButton;

    // send signal to the controller for detailed processing
    emit BeginScaledDrag(whichButton, x, y);
} // RaytraceRenderWidget::mousePressEvent()

void RaytraceRenderWidget::mouseMoveEvent(QMouseEvent *event) { // RaytraceRenderWidget::mouseMoveEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width   
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y()) / size;

    // send signal to the controller for detailed processing
    emit ContinueScaledDrag(x, y);
} // RaytraceRenderWidget::mouseMoveEvent()

void RaytraceRenderWidget::mouseReleaseEvent(QMouseEvent *event) { // RaytraceRenderWidget::mouseReleaseEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width   
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y()) / size;

    // send signal to the controller for detailed processing
    emit EndScaledDrag(x, y);
}

template<typename T>
T RaytraceRenderWidget::clamp(const T &value, const T &low, const T &high) {
    return std::max(low, std::min(value, high));
}
// RaytraceRenderWidget::mouseReleaseEvent()
