#include "mesh.h"
#include <cassert>
#include "includes.h"
#include "shader.h"
#include "extra/textparser.h"

Mesh::Mesh()
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
    indices_vbo_id = 0;
    vao = 0;
}

Mesh::Mesh( const Mesh& m )
{
	vertices = m.vertices;
	normals = m.normals;
	uvs = m.uvs;
	colors = m.colors;
    indices = m.indices;
    vao = m.vao;
}

Mesh::~Mesh()
{
	if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
	if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
	if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
	if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &indices_vbo_id);
    if(vao)glDeleteVertexArrays(1, &vao);
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
    indices.clear();
}

void Mesh::genBuffers() {
    assert(vertices.size() && "No vertices in this mesh");
    

    //delete old
    if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
    if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
    if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    
    //bind vao to store state
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers( 1, &vertices_vbo_id ); //generate one handler (id)
    glBindBuffer( GL_ARRAY_BUFFER, vertices_vbo_id ); //bind the handler
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW ); //upload data
    
    //shader must a_vertex attribute, error if not
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    
    if(normals.size())
    {
        glGenBuffers( 1, &normals_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, normals_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), &normals[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_normal attribute
        glEnableVertexAttribArray(NORMAL_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(NORMAL_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );

    }
    
    if(uvs.size())
    {
        glGenBuffers( 1, &uvs_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, uvs_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 2 * sizeof(float), &uvs[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_uv attribute
        glEnableVertexAttribArray(UV_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(UV_ATTRIBUTE_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if(colors.size())
    {
        glGenBuffers( 1, &colors_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, colors_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 4 * sizeof(float), &colors[0], GL_STATIC_DRAW ); //upload data

        //shader must have a_color attribute
        glEnableVertexAttribArray(COLOR_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(COLOR_ATTRIBUTE_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if (indices.size() == 0){
        //if there is no index buffer, make an 'emergency' serial one here
        for (size_t i = 0; i < vertices.size(); i++) {
            indices.push_back((unsigned int)i);
        }
    }
    
    //create index array
    glGenBuffers(1, &indices_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0); //unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
    
}

void Mesh::render( int primitive, Shader* sh )
{   

    glBindVertexArray(vao);
    
    glDrawElements(primitive, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);

}




void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	indices.clear();

	//create four vertices for corner of quad

	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size, 0, size));
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(size,0,-size) );
	

	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );


	//texture coordinates
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(1,0) );
    
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);


    
    this->genBuffers();
}

bool Mesh::loadASE(const char* filename) {

    vertices.clear();
    normals.clear();
    uvs.clear();
    colors.clear();
    indices.clear();

    TextParser t;
    if (t.create(filename) == false)
        return false;
    
    t.seek("*MESH_NUMVERTEX");
    int num_verts = t.getint();

    t.seek("*MESH_NUMFACES");
    int num_faces = t.getint();

    vector<Vector3> unique_vertices;
    unique_vertices.resize(num_verts);

    for (int i = 0; i < num_verts; i++) {

        t.seek("*MESH_VERTEX");
        int index = t.getint();
        float x = (float)t.getfloat(); //x
        float y = (float)t.getfloat(); //y
        float z = (float)t.getfloat(); //z

        //unique_vertices.push_back(Vector3(x, y, z));
        unique_vertices[index] = Vector3(-x, z, y);
    }

    for (int i = 0; i < num_faces; i++) {

        t.seek("*MESH_FACE");
        int index = t.getint();
        t.seek("A:");
        int a = t.getint(); //a
        t.seek("B:");
        int b = t.getint(); //b
        t.seek("C:");
        int c = t.getint(); //c

        vertices.push_back(unique_vertices[a]);
        vertices.push_back(unique_vertices[b]);
        vertices.push_back(unique_vertices[c]);
    }

    t.seek("*MESH_NUMTVERTEX");
    int num_tverts = t.getint();

    vector<Vector2> unique_tverts;
    unique_tverts.resize(num_tverts);

    for (int i = 0; i < num_tverts; i++) {

        t.seek("*MESH_TVERT");
        int index = t.getint();
        float u = t.getfloat(); //u coordinate of the texture
        float v = t.getfloat(); //v coordinate of the texture

        unique_tverts[index] = Vector2(u, v);
    }

    t.seek("*MESH_NUMTVFACES");
    int num_tvfaces = t.getint();

    for (int i = 0; i < num_tvfaces; i++) {

        t.seek("*MESH_TFACE");
        int index = t.getint();
        int a = t.getint();
        int b = t.getint();
        int c = t.getint();

        uvs.push_back(unique_tverts[a]);
        uvs.push_back(unique_tverts[b]);
        uvs.push_back(unique_tverts[c]);
    }

    for (int i = 0; i < num_faces; i++) {

        for (int u = 0; u < 3; u++) {

            t.seek("*MESH_VERTEXNORMAL");
            int index = t.getint();
            float x = t.getfloat();
            float y = t.getfloat();
            float z = t.getfloat();
            normals.push_back(Vector3(-x, z, y));
        }
    }

    this->genBuffers();
    return true;
}

