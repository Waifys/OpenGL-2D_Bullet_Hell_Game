Hello, This will be my summary for opengl, enjoy 👍.

>[!note]
>
> A lot of information follows the learnopengl with a little more information I got from searching on my own or knowing from reading books or from school, be free to shit talk on anything here if it's wrong, any critizism is welcome ;)

>[!Warning] Warning for the length of the chapter
>
>This chapter is long as shit because it's VERY important information, if you skip and only copy code you might run into a SUPER specific problem later that is so fuckin random and you have no idea why only to realize it has to do with the pipe line or buffers xd, ask me if you want information on that, It took a lot of time and even AI was stupid to not realize a simple information that could be avoided by just studying a little more.
>
>Overall the information is interesting so have fun instead of burden on this, studying the pipeline is one of the most fundamental things in graphics programing and the ingenuity of people to come up with this is very impressive :).

# GLSL

Shader are written in language close to C called *Gl shading language* (GLSL), is tailored for graphics and has a lot of functions that help with vector and matrix manipulation. 

Shaders are written starting with version, inputs, outputs and uniforms followed by the main function. Mostly like this:
```cpp
#version version_number
in type in_name;
in type in_name;

out type out_name;

uniform type uniform_name;

void main()
{
/* Logic */
out_variable_name = result_from_logic;
}
```
At their essence Shaders are just input and output machines, they receive a value and give a value.

When talking about vertex Shader we call vertex attribute to the in variables.
The maximum amount of vertex attributes are defined by hardware, openGL guarantees 16 4-component vertex attributes put a lot of hardware allows for more.
>[!info] Cool Info
>
>You can access this number with:
>```cpp
int nrAttributes;
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
>```
>
>For almost all purposes 16 is more than enough, I've personally almost never go beyond 3-4, sometimes going up to like 7? 16 is a lot.


## Types

GLSL has the normal types like:
- int
- float 
- double
- uint
- bool

And then we have vectors. Vectors are complicated, they don't work like most languages, you can declare a vector as such (n being the size of array, being able to be 2,3 or 4):
- vecn
- bvecn
- ivecn
- uvecn
- bvecn

For most purposes these should be enough. You are able to access these with .x, .y, .z, and .w same with rgba and stpq (these are for coordinates, honestly ).

you can also create vectors from other vectors as such:
```cpp
vec2 vector = vec2(0.5, 0.7);
vec4 result = vec4(vector, 0.0, 0.0);
vec4 other  = vec4(result.xyz, 1.0); 
```


## In/Outs

Shaders work in a sequential (yet parallel) way, meaning the output of each Shader is useful on the next one, the code they do should be on that purpose. You can define the Input of the Shader with the *in* keyword and the output with the *out* keyword.

The vertex Shader, however, is different. As the first in line it receives data straight from memory. The Shader requires extra specification just so it knows what data is which, this is done through *(location = 0)* in the code.
>[!note]
>
>You can also not use location, however, it's much more work and it's unnecessary work.
>You can let opengl give the location and then get it with *glGetAttribLocation* (for readability pls use location, it just makes everything easier, for you and the opengl).

Fragment Shader is also different as it requires at least one output vec4 so opengl knows the color of the fragment. 

To send data from 1 Shader to another then we can just name out variable and in variable in the Shaders, know you cannot return to a Shader in a previous step of the render pipeline.

To send, for example, a color from vertex Shader to fragment Shader you can do:
```cpp
out vec4 vertexColor;
```
in the vertex Shader and:
```cpp
in vec4 vertexColor;
```


