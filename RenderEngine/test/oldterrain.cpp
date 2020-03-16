
void CreateMeshQuad(float point1, float point2, float point3, float point4, float offsetX, float offsetZ, float quadSize, unsigned int indexOffset, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    glm::vec3 normal1 = glm::cross(glm::vec3(offsetX, point2, offsetZ + quadSize) - glm::vec3(offsetX, point1, offsetZ), glm::vec3(offsetX + quadSize, point4, offsetZ) - glm::vec3(offsetX, point1, offsetZ));
    normal1 = glm::normalize(normal1);
    normal1 = glm::vec3(0, 1, 0);
    vertices.emplace_back(
        offsetX, point1, offsetZ,
        normal1.x, normal1.y, normal1.z,
        0, 0
    );
    vertices.emplace_back(
        offsetX, point2, offsetZ + quadSize,
        normal1.x, normal1.y, normal1.z,
        0, 1.0f
    );
    vertices.emplace_back(
        offsetX + quadSize, point4, offsetZ,
        normal1.x, normal1.y, normal1.z,
        1.0f, 0
    );
    glm::vec3 normal2 = glm::cross(glm::vec3(offsetX + quadSize, point4, offsetZ) - glm::vec3(offsetX + quadSize, point3, offsetZ + quadSize), glm::vec3(offsetX, point2, offsetZ + quadSize) - glm::vec3(offsetX + quadSize, point3, offsetZ + quadSize));
    normal2 = glm::normalize(normal2);
    normal2 = glm::vec3(0, 1, 0);
    vertices.emplace_back(
        offsetX, point2, offsetZ + quadSize,
        normal2.x, normal2.y, normal2.z,
        0, 1.0f
    );
    vertices.emplace_back(
        offsetX + quadSize, point3, offsetZ + quadSize,
        normal2.x, normal2.y, normal2.z,
        1, 1
    );
    vertices.emplace_back(
        offsetX + quadSize, point4, offsetZ,
        normal2.x, normal2.y, normal2.z,
        1.0f, 0
    );

    indices.emplace_back(indexOffset + 0);
    indices.emplace_back(indexOffset + 1);
    indices.emplace_back(indexOffset + 2);

    indices.emplace_back(indexOffset + 3);
    indices.emplace_back(indexOffset + 4);
    indices.emplace_back(indexOffset + 5);
}

// t is a value that goes from 0 to 1 to interpolate in a C1 continuous way across uniformly sampled data points.
// when t is 0, this will return B.  When t is 1, this will return C.  Inbetween values will return an interpolation
// between B and C.  A and B are used to calculate slopes at the edges.
float CubicHermite(float A, float B, float C, float D, float t)
{
    /*float a = -A / 2.0f + (3.0f * B) / 2.0f - (3.0f * C) / 2.0f + D / 2.0f;
    float b = A - (5.0f * B) / 2.0f + 2.0f * C - D / 2.0f;
    float c = -A / 2.0f + C / 2.0f;
    float d = B;

    return a * t * t * t + b * t * t + c * t + d;
    */

    return Math::lerp(B, C, t);
}

float CurvedLerp(float a, float b, float t) {
    //return Math::lerp(a, b, pow(t, 2.0f));
    return Math::lerp(a, b, t);
}

// 2 - - - 3
// |       |
// |       |
// |       |
// 1 - - - 4
void CreateTerrainCellMesh(int point1, int point2, int point3, int point4, int point11, int point22, int point33, int point44, float offsetX, float offsetZ, float cellBaseSize, int subdivisions, unsigned int& indexOffset, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    float quadSize = cellBaseSize / (float)subdivisions;

    for (int vertical = 0; vertical < subdivisions; vertical++) {
        // Create the vertical interpolated points
        float verticalSliceStepLower = (float)vertical / (float)subdivisions;
        float verticalSliceStepUpper = (float)(vertical + 1) / (float)subdivisions;

        float verticalSliceStepLower1 = (float)(vertical - 1) / (float)subdivisions;
        float verticalSliceStepUpper1 = (float)(vertical + 2) / (float)subdivisions;


        float leftVerticalPointLower = CubicHermite((float)point11, (float)point1, (float)point2, (float)point22, verticalSliceStepLower);
        float leftVerticalPointUpper = CubicHermite((float)point11, (float)point1, (float)point2, (float)point22, verticalSliceStepUpper);
        float rightVerticalPointLower = CubicHermite((float)point44, (float)point4, (float)point3, (float)point33, verticalSliceStepLower);
        float rightVerticalPointUpper = CubicHermite((float)point44, (float)point4, (float)point3, (float)point33, verticalSliceStepUpper);

        float leftVerticalPointLower1 = CubicHermite((float)point11, (float)point1, (float)point2, (float)point22, verticalSliceStepLower1);
        float leftVerticalPointUpper1 = CubicHermite((float)point11, (float)point1, (float)point2, (float)point22, verticalSliceStepUpper1);
        float rightVerticalPointLower1 = CubicHermite((float)point44, (float)point4, (float)point3, (float)point33, verticalSliceStepLower1);
        float rightVerticalPointUpper1 = CubicHermite((float)point44, (float)point4, (float)point3, (float)point33, verticalSliceStepUpper1);


        float verticalQuadOffset = quadSize * (float)vertical + offsetZ;
        for (int horizontal = 0; horizontal < subdivisions; horizontal++) {
            float horizontalQuadOffset = quadSize * (float)horizontal + offsetX;

            float horizontalSliceStepLower = (float)horizontal / (float)subdivisions;
            float horizontalSliceStepUpper = (float)(horizontal + 1) / (float)subdivisions;

            float localPoint1 = CubicHermite(leftVerticalPointLower1, leftVerticalPointLower, rightVerticalPointLower, rightVerticalPointLower1, horizontalSliceStepLower);
            float localPoint4 = CubicHermite(leftVerticalPointLower1, leftVerticalPointLower, rightVerticalPointLower, rightVerticalPointLower1, horizontalSliceStepUpper);
            float localPoint2 = CubicHermite(leftVerticalPointUpper1, leftVerticalPointUpper, rightVerticalPointUpper, rightVerticalPointUpper1, horizontalSliceStepLower);
            float localPoint3 = CubicHermite(leftVerticalPointUpper1, leftVerticalPointUpper, rightVerticalPointUpper, rightVerticalPointUpper1, horizontalSliceStepUpper);
            CreateMeshQuad(localPoint1, localPoint2, localPoint3, localPoint4, horizontalQuadOffset, verticalQuadOffset, quadSize, indexOffset, vertices, indices);
            indexOffset += 6;
        }
    }
}

std::shared_ptr<Mesh> CreateTerrainMesh(const int terrainPoints[], const int terrainSize, const int terrainResolution) {
    auto mat = std::make_shared<PBRMaterial>();
    auto mesh = std::make_shared<Mesh>(mat);
    // Vertices per cell = 6*terrainResolution
    // Cells (terrainSize-1)*(terrainSize-1)
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    int cells = (terrainSize - 1) * (terrainSize - 1);
    int verticesPerCell = (terrainResolution * terrainResolution) * 6;
    int vertexCount = cells * verticesPerCell;
    Console::LogInfo("Predicting %d vertices %d, %d", vertexCount, cells, verticesPerCell);
    vertices.reserve(vertexCount);
    indices.reserve(vertexCount);
    unsigned int indexOffset = 0;
    float cellBaseSize = 2.5f;
    float offsetX = 0.0f;
    float offsetZ = 0.0f;
    for (int z = 1; z < terrainSize - 2; z++) {
        for (int x = 1; x < terrainSize - 2; x++) {
            int baseIndex = (z * terrainSize) + x;
            int base2Index = ((z - 1) * terrainSize) + x - 1;
            int upperRow = ((z + 1) * terrainSize) + x;
            int upper2Row = ((z + 2) * terrainSize) + x - 1;
            int point1 = terrainPoints[baseIndex];
            int point2 = terrainPoints[upperRow];
            int point3 = terrainPoints[upperRow + 1];
            int point4 = terrainPoints[baseIndex + 1];
            int point11 = terrainPoints[base2Index];
            int point22 = terrainPoints[upper2Row];
            int point33 = terrainPoints[upper2Row + 2];
            int point44 = terrainPoints[base2Index + 2];
            CreateTerrainCellMesh(point1, point2, point3, point4, point11, point22, point33, point44, offsetX, offsetZ, cellBaseSize, terrainResolution, indexOffset, vertices, indices);
            offsetX += cellBaseSize;
        }
        offsetX = 0.0f;
        offsetZ += cellBaseSize;
    }
    Console::LogInfo("Final vertex count is %d", vertices.size());
    mesh->SetVertices(&vertices.at(0), vertices.size());
    mesh->SetIndices(&indices.at(0), indices.size());
    vertices.clear();
    indices.clear();
    return mesh;
}