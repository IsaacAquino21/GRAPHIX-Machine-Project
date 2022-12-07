#pragma once

class Model {
public:

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
	Model(const char* objFilename) {
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
            8 * sizeof(GL_FLOAT),
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
            8 * sizeof(GL_FLOAT),
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
            8 * sizeof(GL_FLOAT),
            (void*)uvPtr
        );

        /* Tell the VAO to use the data above */
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
	}
    
    /* This method sets the current VAO and shaderProgram then draw the OBJ */
    void draw() {
        glBindVertexArray(VAO);

        // draws the obj with regards to the operations applied
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
    }

    /* This method Deletes VAO, and VBO buffers */
    void deleteBuffers() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};