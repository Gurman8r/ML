#shader fragment
#version 460 core

in Vertex { vec3 position; vec4 normal; vec2 texcoord; } V;

out vec4 gl_Color;

uniform struct Camera
{
    vec3    pos;    // Camera Position
    vec3    dir;    // Camera Forward
    float   fov;    // Field of View
    float   near;   // Near Clipping Distance
    float   far;    // Far Clipping Distance
    vec2    view;   // Display Size
} u_camera;

uniform vec4        u_color;        // Main Color
uniform vec3        u_lightPos;     // Light Position
uniform vec4        u_diffuse;      // Diffuse Color
uniform vec4        u_specular;     // Specular Color
uniform vec4        u_ambient;      // Ambient Color
uniform float       u_shininess;    // Shininess
uniform sampler2D   u_texture0;     // Diffuse Texture
uniform sampler2D   u_texture1;     // Specular Texture

void main()
{
    // Ambient
    vec4 ambient = (u_ambient * u_diffuse);

    // Diffuse
    vec3  diff_nml = normalize(V.normal.xyz);
    vec3  diff_dir = normalize(u_lightPos - V.position);
    float diff_amt = max(dot(diff_nml, diff_dir), 0.0);
    vec4  diff_col = u_diffuse * diff_amt;
    vec4  diffuse = diff_col * texture(u_texture0, V.texcoord);

    // Specular
    vec3  spec_nml = normalize(u_camera.pos - V.position);
    vec3  spec_dir = reflect(-diff_dir, diff_nml);
    float spec_amt = pow(max(dot(spec_nml, spec_dir), 0.0), u_shininess);
    vec4  spec_col = u_specular * spec_amt;
    vec4  specular = spec_col * texture(u_texture1, V.texcoord);

    // Output
    gl_Color = u_color * (ambient + diffuse + specular);
}