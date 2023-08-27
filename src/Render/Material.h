//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  September, 2022
//
//  ------------------------
//  Material.h
//  ------------------------
//
//  A simple class to contain the material properties of an object.
//
///////////////////////////////////////////////////

#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Math/Vector3.h"
#include "RGBAImage.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
class Material
{


public:
    bool setFromFile;
    std::string name;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    Vector3 emissive;
    float shininess;
    //stuff for reflection and refraction
    float reflectivity;
    float indexOfRefraction;
    float transparency;
    RGBAImage *texture;
    bool isLight();
    Material();
    Material(Vector3 ambient, Vector3 diffuse, Vector3 specular, Vector3 emissive, float shininess, std::istream &textureStream);
    Material(Vector3 ambient, Vector3 diffuse, Vector3 specular, Vector3 emissive, float shininess); //no texture in constructor;
    ~Material();
    static std::vector<Material*> readMaterials(std::istream &materialStream);
};

#endif // MATERIAL_H
