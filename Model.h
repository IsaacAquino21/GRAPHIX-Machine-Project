#pragma once

class Model {
public:
    int index;

    /* Initialize VAO and VBO id vars */
    GLuint VAO, VBO;

    /* Store vertices */
    std::vector<GLfloat> fullVertexData;

    /* Constructor of the Model3D Class
     * > Initializes shader of program then loads the object along with VAO and VBO
     *
     * Parameters:
     * const char* objFilename - Code from our vertex shader file
     */
	Model(const char* objFilename, int i) {
        index = i;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning, error;

        tinyobj::attrib_t attributes;

        bool success = tinyobj::LoadObj(
            &attributes,
            &shapes,
            &materials,
            &warning,
            &error,
            objFilename
        );

        for (int n = 0; n < shapes.size(); n++) {
            /* Get the tangents and bitangents */
            std::vector<glm::vec3> tangents;
            std::vector<glm::vec3> bitangents;

            for (int i = 0; i < shapes[n].mesh.indices.size(); i += 3) {
                tinyobj::index_t vData1 = shapes[n].mesh.indices[i];
                tinyobj::index_t vData2 = shapes[n].mesh.indices[i + 1];
                tinyobj::index_t vData3 = shapes[n].mesh.indices[i + 2];

                glm::vec3 v1 = glm::vec3(
                    attributes.vertices[(vData1.vertex_index * 3)],
                    attributes.vertices[(vData1.vertex_index * 3) + 1],
                    attributes.vertices[(vData1.vertex_index * 3) + 2]
                );

                glm::vec3 v2 = glm::vec3(
                    attributes.vertices[(vData2.vertex_index * 3)],
                    attributes.vertices[(vData2.vertex_index * 3) + 1],
                    attributes.vertices[(vData2.vertex_index * 3) + 2]
                );

                glm::vec3 v3 = glm::vec3(
                    attributes.vertices[(vData3.vertex_index * 3)],
                    attributes.vertices[(vData3.vertex_index * 3) + 1],
                    attributes.vertices[(vData3.vertex_index * 3) + 2]
                );

                glm::vec2 uv1 = glm::vec2(
                    attributes.texcoords[(vData1.texcoord_index * 2)],
                    attributes.texcoords[(vData1.texcoord_index * 2) + 1]
                );

                glm::vec2 uv2 = glm::vec2(
                    attributes.texcoords[(vData2.texcoord_index * 2)],
                    attributes.texcoords[(vData2.texcoord_index * 2) + 1]
                );

                glm::vec2 uv3 = glm::vec2(
                    attributes.texcoords[(vData3.texcoord_index * 2)],
                    attributes.texcoords[(vData3.texcoord_index * 2) + 1]
                );

                // edges of the triangle
                glm::vec3 deltaPos1 = v2 - v1;
                glm::vec3 deltaPos2 = v3 - v1;

                // uv delta
                glm::vec2 deltaUV1 = uv2 - uv1;
                glm::vec2 deltaUV2 = uv3 - uv1;

                float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
                glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
                glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.y) * r;

                tangents.push_back(tangent);
                tangents.push_back(tangent);
                tangents.push_back(tangent);

                bitangents.push_back(bitangent);
                bitangents.push_back(bitangent);
                bitangents.push_back(bitangent);
            }

            for (int i = 0; i < shapes[n].mesh.indices.size(); i++) {
                // Assign the Index data for easy access
                tinyobj::index_t vData = shapes[n].mesh.indices[i];

                int vertexIndex = vData.vertex_index * 3;

                // Push the X position of the vertex
                fullVertexData.push_back(attributes.vertices[vertexIndex]);

                // Push the Y position of the vertex
                fullVertexData.push_back(attributes.vertices[vertexIndex + 1]);

                // Push the Z position of the vertex
                fullVertexData.push_back(attributes.vertices[vertexIndex + 2]);

                int normalsIndex = vData.normal_index * 3;

                // Push the X position of the vertex
                fullVertexData.push_back(attributes.normals[normalsIndex]);

                // Push the Y position of the vertex
                fullVertexData.push_back(attributes.normals[normalsIndex + 1]);

                // Push the Z position of the vertex
                fullVertexData.push_back(attributes.normals[normalsIndex + 2]);

                int texCoordsIndex = vData.texcoord_index * 2;

                // Push the U of the Tex Coords
                fullVertexData.push_back(attributes.texcoords[texCoordsIndex]);

                // Push the V of the Tex Coords
                fullVertexData.push_back(attributes.texcoords[texCoordsIndex + 1]);

                // Push the Tangents
                fullVertexData.push_back(tangents[i].x);
                fullVertexData.push_back(tangents[i].y);
                fullVertexData.push_back(tangents[i].z);

                // Push the Bitangents
                fullVertexData.push_back(bitangents[i].x);
                fullVertexData.push_back(bitangents[i].y);
                fullVertexData.push_back(bitangents[i].z);
            }
        }

        /* Generate and assign IDs to VAO, VBO and EBO*/
        glGenVertexArrays(1 + i, &VAO);
        glGenBuffers(1 + i, &VBO);

        /* Create an array of buffer to store the vertex positions */
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        /* Convert array to buffer data, then assign it to VBO */
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(GL_FLOAT) * fullVertexData.size(),
            fullVertexData.data(),
            GL_STATIC_DRAW
        );

        /* Tell the VAO how to interpret the VBO */
        glVertexAttribPointer(
            0, // location
            3, // (X, Y, Z)
            GL_FLOAT,
            GL_FALSE,
            // Our vertex now has 5 floats in it (X, Y, Z, U, V)
            14 * sizeof(GL_FLOAT),
            (void*)0
        );

        // Since our Normals starts at index 5
        GLintptr normalsPtr = 3 * sizeof(GL_FLOAT);

        glVertexAttribPointer(
            1, // location
            3, // (X, Y, Z)
            GL_FLOAT,
            GL_FALSE,
            // Our vertex now has 5 floats in it (X, Y, Z, U, V)
            14 * sizeof(GL_FLOAT),
            (void*)normalsPtr
        );

        // Since our UV starts at index 2
        GLintptr uvPtr = 6 * sizeof(GL_FLOAT);

        glVertexAttribPointer(
            2, // location
            2, // (U, V)
            GL_FLOAT,
            GL_FALSE,
            // Our vertex now has 5 floats in it (X, Y, Z, U, V)
            14 * sizeof(GL_FLOAT),
            (void*)uvPtr
        );

        GLintptr tangentPtr = 8 * sizeof(GL_FLOAT);

        glVertexAttribPointer(
            3, // location
            3, // tangent (X, Y, Z)
            GL_FLOAT,
            GL_FALSE,
            14 * sizeof(GL_FLOAT),
            (void*)tangentPtr
        );

        GLintptr bitangentPtr = 11 * sizeof(GL_FLOAT);

        glVertexAttribPointer(
            4, // location
            3, // bitangent(X, Y, Z)
            GL_FLOAT,
            GL_FALSE,
            14 * sizeof(GL_FLOAT),
            (void*)bitangentPtr
        );

        /* Tell the VAO to use the data above */
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
	}
    
    /* This method sets the current VAO and shaderProgram then draw the OBJ */
    void draw() {
        glBindVertexArray(VAO);

        // draws the obj with regards to the operations applied
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 14);
    }

    /* This method Deletes VAO, and VBO buffers */
    void deleteBuffers() {
        glDeleteVertexArrays(1 + index, &VAO);
        glDeleteBuffers(1 + index, &VBO);
    }
};