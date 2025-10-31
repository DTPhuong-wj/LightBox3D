
#ifndef LIGHT_H
#define LIGHT_H

void initLighting();
void setupMainLight();
void setupLampLight();
void setupSpotLight();
void setupAmbientLight();
void updateLighting(float deltaTime);
void setupMaterial(float r, float g, float b, float shininess);
void setupWoodMaterial();
void setupMetalMaterial();
void setupPlasticMaterial();
void setupGlassMaterial();
void setupEmissiveMaterial(float brightness);
void toggleMainLight();
void toggleLampLight();
void toggleSpotLight();
void toggleAmbientLight();
void adjustLampIntensity(float delta);
void setTimeOfDay(float hour);

extern bool mainLightOn;
extern bool lampLightOn;
extern bool ambientLightOn;
extern bool spotLightOn;
extern float lampIntensity;
extern float timeOfDay;

#endif
