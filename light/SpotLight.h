#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

class SpotLight
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    float cutOffClose;
    float cutOffFar;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


#endif // !SPOT_LIGHT_H
