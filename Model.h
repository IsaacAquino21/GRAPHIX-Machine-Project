#pragma once

class Model {
private:
    /* Has to track fullVertexData's length since length may value*/
    int fullVertexLength;

    /* Initialize Texture Classes */
    MyTexture texture0, texture1, normalTexture;

    /* Initialize VAO and VBO id vars */
    GLuint VAO, VBO;

    /* Store vertices */
    std::vector<GLfloat> fullVertexData;

    /* Stores transformations of the model */
    glm::vec3 position, scale, rotation;

    MyShader shader;

public:
    /* Constructor of the Model3D Class (with 2 textures and normal map)
     * > Initializes variables of model then loads the object along with VAO and VBO
     *
     * Parameters:
     * const char* objFilename - Code from our vertex shader file
     * glm::vec3 pos - Default position of Model in X, Y, and Z
     * float s - Default scale in size of Model (same value in X, Y, and Z)
     * glm::vec3 rot - Default rotation of Model in X, Y, and Z axis
     * MyShader shad - model shader
     * MyTexture tex0 - First texture of Model
     * MyTexture tex1 - Second texture of Model (overlay)
     * MyTexture normTex - Normal mapping texture of Model
     */
	Model(const char* objFilename, glm::vec3 pos, float s, glm::vec3 rot, MyShader shad, MyTexture tex0, MyTexture tex1, MyTexture normTex): shader(shad), texture0(tex0), texture1(tex1), normalTexture(normTex) {
        position = pos;
        scale = glm::vec3(s);
        rotation = rot;

        fullVertexLength = 11;

        loadObj(objFilename);
	}

    /* AlternativeConstructor of the Model3D Class (if 2 textures)
     * > Initializes variables of model then loads the object along with VAO and VBO
     *
     * Parameters:
     * const char* objFilename - Code from our vertex shader file
     * glm::vec3 pos - Default position of Model in X, Y, and Z
     * float s - Default scale in size of Model (same value in X, Y, and Z)
     * glm::vec3 rot - Default rotation of Model in X, Y, and Z axis
     * MyShader shad - model shader
     * MyTexture tex0 - First texture of Model
     * MyTexture tex1 - Second texture of Model (overlay)
     */
    Model(const char* objFilename, glm::vec3 pos, float s, glm::vec3 rot, MyShader shad, MyTexture tex0, MyTexture tex1): shader(shad), texture0(tex0), texture1(tex1) {
        position = pos;
        scale = glm::vec3(s);
        rotation = rot;

        fullVertexLength = 8;

        loadObj(objFilename);
    }

    /* AlternativeConstructor of the Model3D Class (if 1 texture)
     * > Initializes variables of model then loads the object along with VAO and VBO
     *
     * Parameters:
     * const char* objFilename - Code from our vertex shader file
     * glm::vec3 pos - Default position of Model in X, Y, and Z
     * float s - Default scale in size of Model (same value in X, Y, and Z)
     * glm::vec3 rot - Default rotation of Model in X, Y, and Z axis
     * MyShader shad - model shader
     * MyTexture tex0 - First texture of Model
     */
    Model(const char* objFilename, glm::vec3 pos, float s, glm::vec3 rot, MyShader shad, MyTexture tex0): shader(shad), texture0(tex0) {
        position = pos;
        scale = glm::vec3(s);
        rotation = rot;

        fullVertexLength = 8;

        loadObj(objFilename);
    }

    /* AlternativeConstructor of the Model3D Class (if no texture)
     * > Initializes variables of model then loads the object along with VAO and VBO
     *
     * Parameters:
     * const char* objFilename - Code from our vertex shader file
     * glm::vec3 pos - Default position of Model in X, Y, and Z
     * float s - Default scale in size of Model (same value in X, Y, and Z)
     * glm::vec3 rot - Default rotation of Model in X, Y, and Z axis
     * MyShader shad - model shader
     */
    Model(const char* objFilename, glm::vec3 pos, float s, glm::vec3 rot, MyShader shad): shader(shad) {
        position = pos;
        scale = glm::vec3(s);
        rotation = rot;

        fullVertexLength = 8;

        loadObj(objFilename);
    }

    void loadObj(const char* objFilename) {
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

            if (fullVertexLength == 11) {
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

                    tangents.push_back(tangent);
                    tangents.push_back(tangent);
                    tangents.push_back(tangent);
                }

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

                if (fullVertexLength == 11) {
                    // Push the Tangents
                    fullVertexData.push_back(tangents[i].x);
                    fullVertexData.push_back(tangents[i].y);
                    fullVertexData.push_back(tangents[i].z);
                }
            }
        }

        /* Generate and assign IDs to VAO, VBO and EBO*/
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

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
            fullVertexLength * sizeof(GL_FLOAT),
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
            fullVertexLength * sizeof(GL_FLOAT),
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
            fullVertexLength * sizeof(GL_FLOAT),
            (void*)uvPtr
        );

        if (fullVertexLength == 11) {
            GLintptr tangentPtr = 8 * sizeof(GL_FLOAT);

            glVertexAttribPointer(
                3, // location
                3, // tangent (X, Y, Z)
                GL_FLOAT,
                GL_FALSE,
                fullVertexLength * sizeof(GL_FLOAT),
                (void*)tangentPtr
            );

        }

        /* Tell the VAO to use the data above */
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        if (fullVertexLength == 11) {
            glEnableVertexAttribArray(3);
        }
    }
    
    /* This method sets the transformation of the model then
     * calls the current VAO and shaderProgram to draw the OBJ 
     */
    void draw() {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = glm::scale(transform, scale);
        transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(0, 1, 0));
        transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(-1, 0, 0));
        transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));

        shader.useProgram();
        shader.setMat4("transform", transform);

        int texIdCount = 0;
        if (texture0.getTexId() != -1) shader.setTexture("tex0", texture0.getTexId(), texIdCount++);
        if (texture1.getTexId() != -1) shader.setTexture("tex1", texture1.getTexId(), texIdCount++);
        if (normalTexture.getTexId() != -1) shader.setTexture("norm_tex", normalTexture.getTexId(), texIdCount++);

        glBindVertexArray(VAO);

        // draws the obj with regards to the operations applied
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / fullVertexLength);
    }

    /* This method is returns the position of the Model */
    glm::vec3 getPosition() {
        return position;
    }

    /* This method is returns the scale in size of the Model */
    glm::vec3 getScale() {
        return scale;
    }

    /* This method is returns the rotation of the Model */
    glm::vec3 getRotation() {
        return rotation;
    }

    /* This method is sets the position of the Model given an X, Y, and Z value */
    void setPosition(float x, float y, float z) {
        position = glm::vec3(x, y, z);
    }

    /* This method is sets the rotation of the Model given an X, Y, and Z value */
    void setRotation(float x, float y, float z) {
        rotation = glm::vec3(x, y, z);
    }

    /* This method Deletes VAO, and VBO buffers */
    void deleteBuffers() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};