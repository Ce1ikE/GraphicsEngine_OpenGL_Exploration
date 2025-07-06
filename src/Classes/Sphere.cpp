#include "Sphere.h"

// https://gist.github.com/Pikachuxxxx/5c4c490a7d7679824e0e18af42918efc
Sphere::Sphere(int radius, int longitudes, int latitudes)
{
	if (longitudes < 3)
		longitudes = 3;
	if (latitudes < 2)
		latitudes = 2;

	float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
	float deltaLatitude = M_PI / latitudes;
	float deltaLongitude = 2 * M_PI / longitudes;
	float latitudeAngle;
	float longitudeAngle;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Compute all vertices first except normals
    for (int i = 0; i <= latitudes; ++i)
    {
        latitudeAngle = M_PI / 2 - i * deltaLatitude; /* Starting -pi/2 to pi/2 */
        float xy = radius * cosf(latitudeAngle);    /* r * cos(phi) */
        float z = radius * sinf(latitudeAngle);     /* r * sin(phi )*/

        /*
            * We add (latitudes + 1) vertices per longitude because of equator,
            * the North pole and South pole are not counted here, as they overlap.
            * The first and last vertices have same position and normal, but
            * different tex coords.
         */
        for (int j = 0; j <= longitudes; ++j)
        {
            longitudeAngle = j * deltaLongitude;

            Vertex vertex;
            float posx = xy * cosf(longitudeAngle);       /* x = r * cos(phi) * cos(theta)  */
            float posy = xy * sinf(longitudeAngle);       /* y = r * cos(phi) * sin(theta) */
            float posz = z;                               /* z = r * sin(phi) */
            vertex.m_position = glm::vec3(posx, posy, posz);

            // normalized vertex normal
            nx = posx * lengthInv;
            ny = posy * lengthInv;
            nz = posz * lengthInv;
            vertex.m_normal = glm::vec3(nx, ny, nz);

            vertices.push_back(vertex);
        }
    }

    /*
        *  Indices
        *  k1--k1+1
        *  |  / |
        *  | /  |
        *  k2--k2+1
    */
    unsigned int k1, k2;
    for (int i = 0; i < latitudes; ++i)
    {
        k1 = i * (longitudes + 1);
        k2 = k1 + longitudes + 1;
        // 2 Triangles per latitude block excluding the first and last longitudes blocks
        for (int j = 0; j < longitudes; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (latitudes - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

	GameObject::setMesh(new Mesh(vertices, indices));
	GameObject::setMaterial(new Material(ResourceManager::GetShader(STD_SHADER)));
};

Sphere::Sphere(int radius, int longitudes, int latitudes,glm::vec3& cubePosition)
	:Sphere(radius,longitudes,latitudes)
{
	setPosition(cubePosition);
}
