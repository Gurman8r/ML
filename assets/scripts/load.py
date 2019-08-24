import memelib as ml

PATH = "../../../assets"

ml.content.load([
# Images
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    {
        "type":     "Image",
        "name":     "icon",
        "file":     PATH + "/images/dean.png",
        "flip_v":   "False"
    },

# Fonts
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Font", "name": "clacon",         "file": PATH + "/fonts/clacon.ttf" },
    { "type": "Font", "name": "consolas",       "file": PATH + "/fonts/consolas.ttf" },
    { "type": "Font", "name": "lconsole",       "file": PATH + "/fonts/lucida_console.ttf" },
    { "type": "Font", "name": "minecraft",      "file": PATH + "/fonts/minecraft.ttf" },

# Shaders
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Shader", "name": "basic",        "file": PATH + "/shaders/basic.shader" },
    { "type": "Shader", "name": "normal",       "file": PATH + "/shaders/normal.shader" },
    { "type": "Shader", "name": "surface",      "file": PATH + "/shaders/surface.shader" },
    { "type": "Shader", "name": "solid",        "file": PATH + "/shaders/solid.shader" },
    { "type": "Shader", "name": "skybox",       "file": PATH + "/shaders/skybox.shader" },
    #{ "type": "Shader", "name": "sprites",      "file": PATH + "/shaders/sprites.shader" },
    #{ "type": "Shader", "name": "text",         "file": PATH + "/shaders/text.shader" },
    #{ "type": "Shader", "name": "geometry",     "file": PATH + "/shaders/geometry.shader" },
    {
        "type": "Shader",
        "name": "demo_shader",
        "vert": PATH + "/shaders/demo.vs.shader",
        "frag": PATH + "/shaders/demo.fs.shader"
    },

# Textures
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Texture", "name": "dean",        "file": PATH + "/images/dean.png",      "format": "RGBA" },
    { "type": "Texture", "name": "neutrino",    "file": PATH + "/images/neutrino.png",  "format": "RGBA" },
    { "type": "Texture", "name": "sanic",       "file": PATH + "/images/sanic.png",     "format": "RGBA" },
    { "type": "Texture", "name": "borg",        "file": PATH + "/images/borg.png",      "format": "RGBA" },
    { "type": "Texture", "name": "grass",       "file": PATH + "/images/grass.png",     "format": "RGBA" },
    { "type": "Texture", "name": "navball",     "file": PATH + "/images/navball.png",   "format": "RGBA" },
    { "type": "Texture", "name": "buck",        "file": PATH + "/images/buck.png",      "format": "RGBA" },

    { "type": "Texture", "name": "earth_cm",    "file": PATH + "/textures/earth/earth_cm_2k.png", "format": "RGB" },
    { "type": "Texture", "name": "earth_dm",    "file": PATH + "/textures/earth/earth_dm_2k.png", "format": "RGB" },
    { "type": "Texture", "name": "earth_hm",    "file": PATH + "/textures/earth/earth_hm_2k.png", "format": "red" },
    { "type": "Texture", "name": "earth_lm",    "file": PATH + "/textures/earth/earth_lm_2k.png", "format": "RGB" },
    { "type": "Texture", "name": "earth_nm",    "file": PATH + "/textures/earth/earth_nm_2k.png", "format": "RGB" },
    { "type": "Texture", "name": "earth_sm",    "file": PATH + "/textures/earth/earth_sm_2k.png", "format": "red" },
    
    { "type": "Texture", "name": "stone_dm",    "file": PATH + "/textures/stone/stone_dm.png", "format": "RGBA" },
    { "type": "Texture", "name": "stone_hm",    "file": PATH + "/textures/stone/stone_hm.png", "format": "RGBA" },
    { "type": "Texture", "name": "stone_nm",    "file": PATH + "/textures/stone/stone_nm.png", "format": "RGBA" },
    
    { "type": "Texture", "name": "mars_dm",     "file": PATH + "/textures/mars/mars_dm_2k.png", "format": "RGB" },
    { "type": "Texture", "name": "mars_nm",     "file": PATH + "/textures/mars/mars_nm_2k.png", "format": "RGB" },
    
    { "type": "Texture", "name": "moon_dm",     "file": PATH + "/textures/moon/moon_dm_2k.png", "format": "RGB" },
    { "type": "Texture", "name": "moon_nm",     "file": PATH + "/textures/moon/moon_nm_2k.png", "format": "RGB" },

# Skyboxes
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Image", "name": "star", "file": PATH + "/skyboxes/star.jpg" },
    {
        "type":     "Texture",
        "name":     "sky_starfield",
        "sampler":  "texture_cube",
        "smooth":   "False",
        "repeat":   "True",
        "format":   "RGB",
        "source":   "images",
        "front":    "star",
        "left":     "star",
        "back":     "star",
        "right":    "star",
        "top":      "star",
        "bottom":   "star"
    },
    {
        "type":     "Texture",
        "name":     "sky_space",
        "sampler":  "texture_cube",
        "smooth":   "False",
        "repeat":   "True",
        "format":   "RGB",
        "source":   "files",
        "front":    PATH + "/skyboxes/space/1.png",
        "left":     PATH + "/skyboxes/space/2.png",
        "back":     PATH + "/skyboxes/space/3.png",
        "right":    PATH + "/skyboxes/space/4.png",
        "top":      PATH + "/skyboxes/space/5.png",
        "bottom":   PATH + "/skyboxes/space/6.png"
    },
    { "type": "Image", "name": "sunset_right",  "file": PATH + "/skyboxes/sunset/sunset_right.png", "flip_v": "False" },
    { "type": "Image", "name": "sunset_left",   "file": PATH + "/skyboxes/sunset/sunset_left.png", "flip_v": "False" },
    { "type": "Image", "name": "sunset_top",    "file": PATH + "/skyboxes/sunset/sunset_top.png", "flip_v": "False" },
    { "type": "Image", "name": "sunset_bottom", "file": PATH + "/skyboxes/sunset/sunset_bottom.png", "flip_v": "False" },
    { "type": "Image", "name": "sunset_front",  "file": PATH + "/skyboxes/sunset/sunset_front.png", "flip_v": "False" },
    { "type": "Image", "name": "sunset_back",   "file": PATH + "/skyboxes/sunset/sunset_back.png", "flip_v": "False" },
    {
        "type":     "Texture",
        "name":     "sky_sunset",
        "sampler":  "texture_cube",
        "smooth":   "False",
        "repeat":   "True",
        "format":   "RGBA",
        "source":   "images",
        "right":    "sunset_right",
        "left":     "sunset_left",
        "top":      "sunset_top",
        "bottom":   "sunset_bottom",
        "front":    "sunset_front",
        "back":     "sunset_back"
    },

# Models
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Model", "name": "cube",          "file": PATH + "/meshes/cube.obj" },
    { "type": "Model", "name": "sphere8x6",     "file": PATH + "/meshes/sphere8x6.obj" },
    { "type": "Model", "name": "sphere32x24",   "file": PATH + "/meshes/sphere32x24.obj" },

    #{ "type": "Model", "name": "cow",           "file": PATH + "/meshes/cow.obj" },
    #{ "type": "Model", "name": "pumpkin",       "file": PATH + "/meshes/pumpkin.obj" },
    #{ "type": "Model", "name": "teapot",        "file": PATH + "/meshes/teapot.obj" },
    #{ "type": "Model", "name": "teddy",         "file": PATH + "/meshes/teddy.obj" },

# Surfaces
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Surface", "name": "surf_scene_main", "model": "default_quad", "shader": "surface" },
    { "type": "Surface", "name": "surf_scene_post", "model": "default_quad", "shader": "surface" },

# Materials
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    {
        "type":     "Material",
        "name":     "skybox",
        "shader":   "skybox",
        "defaults": "True",
        "uniforms": PATH + "/materials/skybox.mat"
    },
    {
        "type":     "Material",
        "name":     "demo_material",
        "shader":   "demo_shader",
        "defaults": "True",
        "uniforms": PATH + "/materials/demo.mat"
    }
    ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #