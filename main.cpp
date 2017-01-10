#include <iostream>
#include "FreeImage.h"
#include <glm/glm.hpp>
#include "readFile.h"

using namespace std;
using namespace glm;

const int BitsPerPixel = 24;

struct camera {
  vec3 location;
  vec3 lookAt;
  vec3 up;
  int fovDegrees;
};

struct scene {
  int width;
  int height;
  int maxBounces;
};

struct sphere {
  vec3 center;
  float r;
};

struct color {
  float red, green, blue;
};

struct ray {
  vec3 origin, direction;
};

struct light {
  vec3 color;
  vec3 direction;
  vec3 location;
};

vec3 calcNormalForSphere(sphere *sph, vec3 &poi) {
  return (sph->center - poi) / sph->r;
}
  
int intersect(sphere *sph, ray *r, float *t) {
  vec3 oc = r->origin - sph->center;
  float b = 2*dot(oc, r->direction);
  float c = dot(oc, oc) - sph->r * sph->r;
  float disc = b*b-4*c;
  if (disc < 0) return 0;
  else {
    disc = sqrt(disc);
    float t0 = -b - disc;
    float t1 = -b + disc;
    *t = (t0 < t1) ? t0 : t1;
    return 1;
  }
}

void writeImage(int width, int height, string name) {
  FIBITMAP *bitmap = FreeImage_Allocate(width, height, BitsPerPixel);
  RGBQUAD color;
  if (!bitmap) {
    cout << "Error creating bitmap\n";
    exit(1);
  }
  
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      color.rgbRed = 0;
      color.rgbGreen = (double)i / width * 255.;
      color.rgbBlue = (double)j / height * 255.;
      FreeImage_SetPixelColor(bitmap, i, j, &color);
    }
  }
  
  if (FreeImage_Save(FIF_PNG, bitmap, name.c_str(), 0)) {
    cout << "Image successfully saved to " << name << endl;
  }
}

void writePlainImage(FIBITMAP *bitmap, string name) {
  if (FreeImage_Save(FIF_PNG, bitmap, name.c_str(), 0)) {
    cout << "Saved image to " << name << endl;
  } else {
    cerr << "Failed to save image to " << name << endl;
  }
}

int main(int argc, char **argv) {
  scene sce;
  sce.width = 400;
  sce.height = 400;
  sce.maxBounces = 5;
  
  for (int i = 0; i < argc; i++) {
    cout << "arg: " << i << " is : " << argv[i] << endl;
  }

  light li;
  li.location = vec3(sce.width, 0, 50);
  li.color = vec3(.9, .9, .9);

  char* filename = (char*) "Makefile";
  parseFile(filename);
  
  FreeImage_Initialise();
  cout << "FreeImage_" << FreeImage_GetVersion() << "\n";
  cout << FreeImage_GetCopyrightMessage() << "\n\n";
  // writeImage(200, 200, "test.png");
  FIBITMAP *bitmap = FreeImage_Allocate(sce.width, sce.height, BitsPerPixel);
  
  sphere sph;
  sph.center = vec3(sce.width/(float)2, sce.height/(float)2, 0);
  sph.r = 50;
  RGBQUAD bgColor = {248, 248, 248};

  RGBQUAD red;
  red.rgbRed = 255;
  red.rgbGreen = 0.;
  red.rgbBlue = 0.;

  vec3 ambientColor = vec3(.1, .1, .1);
  
  for (int y = 0; y < sce.height; y++) {
    for (int x = 0; x < sce.width; x++) {
      ray r;
      r.origin = vec3(x, y , 0);
      r.direction = vec3(0, 0, 1);
      
      float t = 20000;
      if (intersect(&sph, &r, &t)) {
	// point of intersection
	vec3 poi = r.origin +  r.direction * t;

	vec3 light1 = li.location - poi;
	vec3 normalOfSphere = calcNormalForSphere(&sph, poi);
	float dt = dot(normalize(light1), normalize(normalOfSphere));
	  
	RGBQUAD finalColor;
	finalColor.rgbRed = red.rgbRed * dt;
	finalColor.rgbGreen = red.rgbGreen * dt;
	finalColor.rgbBlue = red.rgbBlue * dt;
        if (dt < 0) {
	  finalColor.rgbRed = 0;
	  finalColor.rgbGreen = 0 ;
	  finalColor.rgbBlue = 0;
	}
	
	FreeImage_SetPixelColor(bitmap, x, y, &finalColor);
      }
      else {
	FreeImage_SetPixelColor(bitmap, x, y, &bgColor);
      }
    }
  }
  writePlainImage(bitmap, "test.png");
  FreeImage_DeInitialise();
  return 0;
}
