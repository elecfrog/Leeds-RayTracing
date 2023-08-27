#include "Scene.h"
#include <limits>

Scene::Scene(std::vector<ThreeDModel> *texobjs,RenderParameters *renderp)
{
    objects = texobjs;
    rp = renderp;

    //Ka, Kd, Ks, Ke
    Vector3 ambient = Vector3(0.5f, 0.5f, 0.5f);
    Vector3 diffuse = Vector3(0.5f, 0.5f, 0.5f);
    Vector3 specular = Vector3(0.5f, 0.5f, 0.5f);
    Vector3 emissive = Vector3(0, 0, 0);
    float shininess = 1.0f;

    default_mat = new Material(ambient,diffuse,specular,emissive,shininess);
}

Matrix4 Scene::getModelview()
{
    //TODO: Grab all the necessary matrices to build your modelview. Sliders, Arcball, centering.
    Matrix4 result;
    result.SetIdentity();
    Matrix4 mat_trans;
    float x = rp->xTranslate;
    float y = rp->yTranslate;
    float z = rp->zTranslate - 1;
    Vector3 t(x, y, z);
    mat_trans.SetTranslation(t);
    return result * mat_trans * rp->rotationMatrix;
}

void Scene::updateScene()
{
    triangles.clear();

    for (int i = 0;i< int(objects->size());i++)
    {
        typedef unsigned int uint;
        ThreeDModel obj = objects->at(uint(i));

        // loop through the faces: note that they may not be triangles, which complicates life
        for (unsigned int face = 0; face < obj.faceVertices.size(); face++)
        { // per face
            // on each face, treat it as a triangle fan starting with the first vertex on the face
            for (unsigned int triangle = 0; triangle < obj.faceVertices[face].size() - 2; triangle++)
            { // per triangle
                // now do a loop over three vertices
                Triangle t;
                for (unsigned int vertex = 0; vertex < 3; vertex++)
                { // per vertex
                    // we always use the face's vertex 0
                    uint faceVertex = 0;
                    // so if it isn't 0, we want to add the triangle base ID
                    if (vertex != 0)
                        faceVertex = triangle + vertex;

                    //this is our vertex before any transformations. (world space)
                    Vector4 v =  Vector4(
                                obj.vertices[obj.faceVertices   [face][faceVertex]].x,
                            obj.vertices[obj.faceVertices   [face][faceVertex]].y,
                            obj.vertices[obj.faceVertices   [face][faceVertex]].z
                            );

                    //This will start working when you write the method above!
                    v = getModelview()*v;

                    t.verts[vertex] = v;

                    Vector4 n =  Vector4(
                                obj.normals[obj.faceNormals   [face][faceVertex]].x,
                            obj.normals[obj.faceNormals   [face][faceVertex]].y,
                            obj.normals[obj.faceNormals   [face][faceVertex]].z,
                            0.0f);

                    n = getModelview()*n;
                    t.normals[vertex] = n;

                    Vector3 tex = Vector3(
                                obj.textureCoords[obj.faceTexCoords[face][faceVertex]].x,
                            obj.textureCoords[obj.faceTexCoords[face][faceVertex]].y,
                            0.0f
                            );
                    t.uvs[vertex] = tex;


                    t.colors[vertex] = Vector3(0.7f, 0.7f, 0.7f);

                } // per vertex
                t.validate();
                if(obj.material== nullptr){
                    t.shared_material = default_mat;
                }else{
                    t.shared_material = obj.material;
                }
                triangles.push_back(t);
            } // per triangle
        } // per face
    }//per object
}

Scene::CollisionInfo Scene::closestTriangle(Ray r)
{
    //TODO: method to find the closest triangle!
    float min_t = std::numeric_limits<float>::max();
    int tri_index = 0;
    for(int index = 0; index<this->triangles.size();++index)
    {
        if (this->triangles[index].intersect(r)<min_t&&this->triangles[index].intersect(r)>0.001)
        {
                min_t = this->triangles[index].intersect(r);
                tri_index = index;
        }
    }
    return {this->triangles[tri_index],min_t};
}


