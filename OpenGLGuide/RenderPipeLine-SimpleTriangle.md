Hello, This will be my summary for opengl, enjoy 👍.

>[!note]
>
> A lot of information follows the learnopengl with a little more information I got from searching on my own or knowing from reading books or from school, be free to shit talk on anything here if it's wrong, any critizism is welcome ;)

>[!Warning] Warning for the length of the chapter
>
>This chapter is long as shit because it's VERY important information, if you skip and only copy code you might run into a SUPER specific problem later that is so fuckin random and you have no idea why only to realize it has to do with the pipe line or buffers xd, ask me if you want information on that, It took a lot of time and even AI was stupid to not realize a simple information that could be avoided by just studying a little more.
>
>Overall the information is interesting so have fun instead of burden on this, studying the pipeline is one of the most fundamental things in graphics programing and the ingenuity of people to come up with this is very impressive :).


# Graphics Pipeline 

The graphics pipeline works is the manager which works the part of converting the 3d coordinates that we give it to colored pixels, mainly there's 2 parts:

- 1st -> Transforms 3d coordinates into 2d
- 2nd -> Transforms 2d into colored pixels

The Graphics can be separated into several small parts, each part is very specialized and is done to work very well in parallel, this makes so GPU run with a lot of tiny cores each running the code for these small steps, the code for this small steps is called *Shaders*

Some Shaders can be rewritten (tendency with time is for more and more Shaders be replaceable, vulkan is way more replaceable than opengl and is mostly for that, customizability) and in opengl Shaders are coded in *Opengl shading Language (GLSL)*

The stages can be resumed in:
- Vertex Shader (Can have Shader replaced)
- Geometry Shader (Can have Shader replaced)
- Shape Assembly
- Rasterization
- Fragment Shader (Can have Shader replaced)
- Test and blending

## The Pipeline Stages

As input the pipelines takes 3 3d Coordinates
forming a tringle in an array called *Vertex Data*, a vertex is the collection of data per 3d coordinate using *vertex attributes*, This can contain any data but most of the time it contains 3d position and color.

>[!note]
>
> OpenGl in specific requires you to indicate what's the type of render you want, the hints are called *Primitives* and can be, for example, points, triangles, strip, etc...

### Vertex Shader

Inputs a single vertex. The purpose of this Shader is mainly to pick a 3d coordinates and transform that coordinates into the desired 3d coordinates. Basically applying basic processing on the vertex.

### Primitive/Shape assembly

This stage takes the vertices as inputs and forms one or more primitives and assembles into shapes.

### Geometry Shader

This Shader is optional. It takes a collection of primitives and has the ability to generate other shapes by emitting new vertices forming other primitive.
One use of this one is, for example, inputting a triangle and from the 3 vertices destroying the triangle and forming 3 lines to indicate the normals of the vertices.

### Primitive assembly (Again)

The final shapes are then assembled for the next stage

### Rasterization stage

Takes the shapes given in the Primitive Shader and maps the primitives to corresponding pixels on the screen, this are called fragments, in this process clipping is performed removing fragments outside view for performance.

### Fragment Shader

Calculates the final color for the pixel, usually contains information regarding lights position, color and shadows so it can calculate the color.

### Alpha test and Blending

In this stage the object will pass though the alpha test and bleding where it checks it's depth to understand if it's behind an object and checks it's alpha value to blend objects, this way it can combine multiple triangles.

>[!important]
>
>tessellation stage and transform feedback loop also exist, later I'll explain, stay chill 🙏🏻
>(Render pipe line has so many stages and it's a never ending study, I'll keep it simple just know that this is a STRONG simplification and there are hidden stages).

# Render PipeLine In Code

Opengl does not offer a default vertex and fragment shader, you are required to develop one of your own. Let's start by developing the vertex input for the vertex shader as explained above

## Code

As opengl is a 3d graphics library (As most are) you are required to give 3d coordinates.

vertices are rendered when present in rages between -1 and 1 on all 3 axis (x, y and z)  so if we want coordinates different well have to normalize them.

Because we are manually sending that we can just give values in those ranges, like for example:

```cpp
float vertice[] = {
	-0.5f, 0.5f, 0.0f, // First Vertex
	0.5f, -0.5f, 0.0f, // Second Vertex
	0.0f, 0.5f, 0.0f,  // Third Vertex
}
```
> [!important]
>
>The position on the window is defined by the viewport transform, not global coordinate, do not expect a (1920, 1080) pixel to be on the top of the screen, for that you'll need (1,1) and the viewport does the rest.

>[!note]
>
> Vertices outputed from the vertex shader should be inside this -1 and 1 range, anything that is outside of this range will be discarded in the rasterization stage as explained above :)

### Sending the input to Vertex Shader

So, you want to start the pipeline, the first step? send the vertices to the Vertex Shader, this is done through reserving memory in the graphics in a thing we call *Vertex Buffer Objects (VBO)*. They are able to store relatively big number of vertices (Not anymore since soon enough you'll only be able to buy a shitty GT 710 because of openAI xd, mb back to it). The advantage is that vertices are sent on a batch, all together instead of individually, sending data from the CPU is slow (relatively, of course) so we wasn't to avoid it as much as possible (I can talk about game optimization with this later if yall want) so we want to send as much as possible from the get go. Once in the VBO the vertex Shader has instant access to it.

All objects in opengl are associated to an ID corresponding to the buffer, we can generate one with *glGenBuffers*

```Cpp
unsigned int VBO;
glGenBuffers(1, &VBO);
```

Opengl has multiple type of buffers, VBO is an array buffer, you are allowed to bind multiple buffers, just be aware they have to be from different types, you can bind the buffer with function *glBindBuffer*

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
```

From now on, any alterations to an array buffer will alter the VBO since that's the binded buffer, be aware that further alterations can be done to buffers as long as it's not done to array buffers.

We can now copy our previous manually set vertices to the buffer with the function buffer data as such:
```cpp
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

This functions successfully copied vertices to the GPU (At least if it worked, something could go wrong but that wasn't on your side, presumably).
The first parameter takes the type of the buffer we want to copy to, remember we binded VBO to Array buffer (That's it's type ofc :) ), the second takes the size of the data in bytes, as vertices is an array *sizeof* indicates the size of the array, if it was a pointer we would need extra calculation, as it's not, quite simple. The 3rd argument is the data we want to send, simple.
The fourth data is different, it indicates how to manage the data, there's some possibilites (See [Documentation](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml) to see all) however I'm gonna sugar coat it, Fuck all except 3: 
- GL_STREAM_DRAW: Use this when data doesn't change and is used very little
- GL_STATIC_DRAW: Use this when data doesn't change and is used a lot 
- GL_DYNAMIC_DRAW: Use this when data changes
Every other is for different buffer types so don't matter for now, mostly expect them to follow the same logic (GL_STREAM_READ is to read very little times, STATIC_READ to read a lot, like that). 

Usages are very relatives and most people won't give you a direct answer like  <font color="cyan">"Start using Static exactly after 3. Used stream and read it 4 times? Your pc killed 3 children, good job"</font>.  Personally if I read more than 5 times I switch to static, When I asked AI it told me **1 to 5 draws**, take it with a grain of salt but I believe it's mostly right from searches on forums (I feel old saying this 😛). 

> [!fun fact]
> 
> Are you scared that you might read more than supposed and wrongly used stream instead of static? Be chill because most GPU are smart enough to realize most programmers are morrons (I'm included btw, I did it before xd) and changes it for you believe it or not 🤣. So honestly? don't care too much about it, it's micro optimizations at the end of the day so if you follow the 1-5 rule it's prob fine.



### Vertex Shader


Vertex Shader is the least important out of the 2 mandatory Shaders, however, it's very close (the importance of fragment centers mostly on lightning, which is very important yeah, I can't lie, as much as I want vertex to be more important xd).

The code for a default vertex Shader should look something like (it's glsl, md files don't have coloring for glsl language and I wanted colors, ignore the code snipper saying it's C, even tho it looks like it it's glsl):
```c
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```


GLSL is very similar to C (The syntax is based on it on purpose), the first line indicates the version of GLSL we are working on, 330 means 3.3, 420 4.2 and so on, core means we are using core profile, you can also use Compatibility however my Glad (loader generator, a library) was downloaded as core and most people use that one so I leave it at that.

Next we declare the inputs with the keyword *in*. For now let's stick with just position, we represent it with vectors and we can have: vec1 to vec4, for vertices we can use vec3, let's name it aPos (doesn't matter). We also put layout (location = 0), we'll talk later about this.

> [!note]
> 
> You can access the vectors like: vec.x, vec.y, vec.z and vec.w (This one is not used in the position because it's only 3 values)

For the output we use the predefined variable *gl_Position*, you HAVE to set this variable, since the input variable is vec3 we convert it to vec4 and set vec.w to 1.0f (This is the alpha value).

#### Compile Shader

Since it's pretty simple we can do a goof and just store the whole Shader in a string. Let it be:
```cpp
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
```

In order to use it we have to compile it at runtime (This is for opengl, there's other options).
Now we create a Shader object (objects are referenced by id, like the VBO ;) ), we use *glCreateShader*.

```cpp
unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);
```


Since we are creating a vertex Shader that's what we send in the create Shader ([Documentation](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml)] shows all the types of Shaders you can have, for now only vertex and fragment matter)

Now that we have created the Shader we attach the code we had to the Shader and compile it.
```cpp
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);
```

The *glShaderSource* takes the Shader as it's first argument, how many Strings we're passing as second argument and the code as 3rd argument. The 4th argument is an array that indicates the size of the Strings in the 3rd parameter (It can an array, we just use 1 because we smart :) ), you can just use NULL if the strings terminate in NULL but if you want to indicate the size in some and other don't you can use -1 for that, such as:
```cpp
GLint lengthArray[] = {-1, strlen(var2), strlen(var3)};
```
Honestly I have never seen someone use this but it can be useful some day to someone, idk.

### Fragment Shader

The Fragment Shader calculates the color of the fragment, as explained before. To keep simple our script will only output the a simple flat color, no inputs or variations. The script should look something like:
```cpp
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
```
The fragment requires only one output variable in the form of *vec4*. This is what defines the final color. we can define an output variable with the keyword *out*. you can name this what you want, I like *gl_FragColor*.

>[!tip] Cool Info
>
>You might me curious on why opengl doesn't do like in the vertex shader and just make a variable predefined also in the fragment shader, well my dear reader, this is because it does, or at least it did.
>
>The thing is, in opengl 2 there was a variable called **gl_FragColor** (See why i like it now? :) ), however with advancement on technology GPU now possess a new technique called MRT. Saying it simple it lets you output to multiple variables instead of 1. This let's you do mirrors without rendering 2 twice the scene, every questioned on how games can render 2 scenes so fast? They don't, same thing with real time minimaps, seem too fast to render twice? probably not doing it and using this. At the end this variable had to be scraped to adapt to this new technology, however i like to stick to the old reliable name still.  
>
>See [this video](https://www.youtube.com/watch?v=WQLPZKpnod8&t=528s) if you are curious or search for more, it's a cool techique.

We can compile the Shader the same as we did the Vertex changing only the type on it's creation.

```cpp
unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);
```


Both Shaders should be compiled.

>[!Important]
>
> You probably want to know if there's an error in the compilation of the Shader in the future, [learnopengl](https://learnopengl.com/Getting-started/Hello-Triangle) provides a simple code for that if you want, search it in case you want it, I at least recommend it.


### Shader Program

The program is to simplify the setting up the Shaders. To use the recently compiled Shaders we have to link to the Shader and activate the program when calling render.

To create a Shader program we do:
```cpp
unsigned int shaderProgram;
shaderProgram = glCreateProgram();
```
And now we link the Shaders, this is a hard area because a lot of error of linking occur.
```cpp
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);
```

This should be everything :)

Now that we have linked the Shaders they are no longer needed (unless you want to create another program with the same Shaders) so we can discard them as such:
```cpp
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
```

You can activate your program as:
```cpp
glUseProgram(shaderProgram);
```

Meaning in rendering you can pretty easily change between programs with that :)

### Linking Vertex Attributes

The vertex that are given to the vertex shader can be in any format, thankfully :), this allow for SUCH flexibility. This flexibility however, comes with work to indicate how we want our input read. 

In our case sending the coordinates in vec3 the data is in the format:

- Each position is in a 4 byte floating point value.
- Each position has 3 of the 4 byte values (4 if it was vec4, for example).
- There is 0 space between the values
- The 1st value is at the beginning of the array.

With this we can use the function *glVertexAttribPointer* to set this conditions like this:
```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0); 
```

This functions work like this:
- 1st parameter: Which vertex attribute we want to configure. We use (location = 0) in the vertex shader, that's what we are altering.
- 2nd parameter: This indicates how many values are in the information we are setting, remember we are drawing a triangle so we have 3 values.
- 3rd parameter: Specifies the type of values being sent, in our case we sent float values.
- 4th parameter: This normalizes the values, it's cool for things like color, usually colors can be represented in 255 values but opengl reads 0 to 1 values so you can save it as 255 and then normalize it, this is not applicable in our case so just ignore it.
- 5th parameter: This parameter indicates the amount of space between each of these values, in our case we have 3 floats but later we'll change it, for now it's pretty intuitive.
- 6th parameter: This is where our data starts, we can use the same array for multiple attributes so different starts are needed, well do this later.

Now we can end this by using:
```cpp
glEnableVertexAttribArray(0)
```

This 0 is the index of the attribute of vertex, meaning:
- glVertexAttribPointer -> Indicates How the data should be handled
- glEnableVertexAttribArray -> Tells the Data should start being used

Now after all of this we can basically resume using this as:

```cpp

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);  

glUseProgram(shaderProgram);

/* Draw something */ 
```


Now this is long, very long, Imagine we have 100 of objects, it would take a lot of time to do all this. This is where another thing is introduced.

### Vertex Array Object


VAO are a way of storing this VBO operations, everything that is done after a binding of a VAO is held there and any configuration that you want to return to can just be done by binding it again. 

>[!warning]
>
>In core OpenGl you are required to use them.

To generate a VAO you do more or less the same as a VBO:
```cpp
unsigned int VAO;
glGenVertexArrays(1, &VAO);
```

To use the VAO we call *glBindVertexArray* and from now on everything corresponding to the VBO will be held in the VAO for later.

With all of this, after ALL that, we are Ready, we can draw a triangle!
just use the function:
```cpp
glUseProgram(shaderProgram) // Just to make sure ;)
glBindVertexArray(VAO) // Again, should be binded but just to make sure
lDrawArrays(GL_TRIANGLES, 0, 3);
```

Compiling should result in a triangle.

### Element Buffer Objects

At last, we are gonna talk about a very useful buffer, the *EBO*'s.

Let's say we are gonna do a square, from what we know until now what we can do is:

```cpp
float vertices[] = {
 // first triangle
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f,  0.5f, 0.0f,  // top left 
    // second triangle
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
}
```

If you see carefully we have 2 overlaps in vertices.

To fix this we can use EBO, they are very simple and intuitive.
The first step is to create 2 arrays:
```cpp
float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 
```

The 1st holds the vertices, just like we had before, really simple. The second now, holds the order in which the vertices will be used, in this case the 1st triangles will use the indexes 0, 1 and 3 of the vertex atribute and the second will use 1, 2 and 3.

Now we create the buffer just like the VBO:
``` cpp
unsigned int EBO;
glGenBuffers(1, &EBO);
```

Next just like the VBO, again, we bind it and buffer the data:
```cpp
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```

When drawing we can now call *glDrawElements* instead of *glDrawArrays* to draw with this technique.
```cpp
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

In this draw elements the arguments are:
- 1st parameter: mode we wanna draw in, exactly like the array.
- 2nd parameter: the amount of indices there are, we have 6
- 3rd parameter: the type of the indices, we held it as unsigned int
- 4th parameter: The offset of indices we want, just like in the buffer data, with this we could have, for example, 8 indices and only use the last 6 by giving 2xsizeof(unsigned int) to this parameter.

Notice that draw elements uses the buffer currently in the EBO meaning just like the VBO we can assign the EBO to a VAO to not repeat ourselves too much and simplify our life. 

The whole process should be looking like:
```cpp
/* ... */

glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);  

/* Things :) */

glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);

/* ... */
```