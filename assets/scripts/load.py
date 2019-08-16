import memelib as ml

PATH = "../../../assets"

# Images
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
ml.content.load({ 
    "type": "image",
    "name": "icon",
    "file": PATH + "/images/dean.png",
    "flip_v": "False"
    })


# Fonts
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
ml.content.load([
    { "type": "font", "name": "clacon",         "file": PATH + "/fonts/clacon.ttf" },
    { "type": "font", "name": "consolas",       "file": PATH + "/fonts/consolas.ttf" },
    { "type": "font", "name": "lconsole",       "file": PATH + "/fonts/lucida_console.ttf" },
    { "type": "font", "name": "minecraft",      "file": PATH + "/fonts/minecraft.ttf" }
    ])


# Shaders
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
ml.content.load([
    { "type": "shader", "name": "basic",        "file": PATH + "/shaders/basic.shader" },
    { "type": "shader", "name": "normal",       "file": PATH + "/shaders/normal.shader" },
    { "type": "shader", "name": "surface",      "file": PATH + "/shaders/surface.shader" },
    { "type": "shader", "name": "solid",        "file": PATH + "/shaders/solid.shader" },
    { "type": "shader", "name": "skybox",       "file": PATH + "/shaders/skybox.shader" },
    { "type": "shader", "name": "sprites",      "file": PATH + "/shaders/sprites.shader" },
    { "type": "shader", "name": "text",         "file": PATH + "/shaders/text.shader" },
    { "type": "shader", "name": "geometry",     "file": PATH + "/shaders/geometry.shader" },
    {
        "type": "shader",
        "name": "demo_shader",
        "vert": PATH + "/shaders/demo.vs.shader",
        "frag": PATH + "/shaders/demo.fs.shader"
    }
    ])


# Textures
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
ml.content.load([
    { "type": "texture", "name": "dean",        "file": PATH + "/images/dean.png" },
    { "type": "texture", "name": "neutrino",    "file": PATH + "/images/neutrino.png" },
    { "type": "texture", "name": "sanic",       "file": PATH + "/images/sanic.png" },
    { "type": "texture", "name": "borg",        "file": PATH + "/images/borg.png" },
    { "type": "texture", "name": "grass",       "file": PATH + "/images/grass.png" },
    { "type": "texture", "name": "navball",     "file": PATH + "/images/navball.png" },
    { "type": "texture", "name": "buck",        "file": PATH + "/images/buck.png" },
    { "type": "texture", "name": "earth_cm",    "file": PATH + "/textures/earth/earth_cm_2k.png", "format": "RGB" },
    { "type": "texture", "name": "earth_dm",    "file": PATH + "/textures/earth/earth_dm_2k.png", "format": "RGB" },
    { "type": "texture", "name": "earth_hm",    "file": PATH + "/textures/earth/earth_hm_2k.png", "format": "red" },
    { "type": "texture", "name": "earth_lm",    "file": PATH + "/textures/earth/earth_lm_2k.png", "format": "RGB" },
    { "type": "texture", "name": "earth_nm",    "file": PATH + "/textures/earth/earth_nm_2k.png", "format": "RGB" },
    { "type": "texture", "name": "earth_sm",    "file": PATH + "/textures/earth/earth_sm_2k.png", "format": "red" },
    { "type": "texture", "name": "stone_dm",    "file": PATH + "/textures/stone/stone_dm.png" },
    { "type": "texture", "name": "stone_hm",    "file": PATH + "/textures/stone/stone_hm.png" },
    { "type": "texture", "name": "stone_nm",    "file": PATH + "/textures/stone/stone_nm.png" },
    { "type": "texture", "name": "mars_dm",     "file": PATH + "/textures/mars/mars_dm_2k.png", "format": "RGB" },
    { "type": "texture", "name": "mars_nm",     "file": PATH + "/textures/mars/mars_nm_2k.png", "format": "RGB" },
    { "type": "texture", "name": "moon_dm",     "file": PATH + "/textures/moon/moon_dm_2k.png", "format": "RGB" },
    { "type": "texture", "name": "moon_nm",     "file": PATH + "/textures/moon/moon_nm_2k.png", "format": "RGB" },
    ])


# Skyboxes
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
ml.content.load([
    { "type": "image", "name": "star",          "file": PATH + "/skyboxes/star.jpg" },
    { "type": "image", "name": "sunset_right",  "file": PATH + "/skyboxes/sunset/sunset_right.png", "flip_v": "False" },
    { "type": "image", "name": "sunset_left",   "file": PATH + "/skyboxes/sunset/sunset_left.png", "flip_v": "False" },
    { "type": "image", "name": "sunset_top",    "file": PATH + "/skyboxes/sunset/sunset_top.png", "flip_v": "False" },
    { "type": "image", "name": "sunset_bottom", "file": PATH + "/skyboxes/sunset/sunset_bottom.png", "flip_v": "False" },
    { "type": "image", "name": "sunset_front",  "file": PATH + "/skyboxes/sunset/sunset_front.png", "flip_v": "False" },
    { "type": "image", "name": "sunset_back",   "file": PATH + "/skyboxes/sunset/sunset_back.png", "flip_v": "False" },
    ])

ml.content.load({
    "type":     "texture",
    "name":     "starfield",
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
    })

ml.content.load({
    "type":     "texture",
    "name":     "space",
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
    })

ml.content.load({
    "type":     "texture",
    "name":     "sunset",
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
    })


# Models
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
ml.content.load([
    { "type": "model", "name": "cube",          "file": PATH + "/meshes/cube.obj" },
    { "type": "model", "name": "sphere8x6",     "file": PATH + "/meshes/sphere8x6.obj" },
    { "type": "model", "name": "sphere32x24",   "file": PATH + "/meshes/sphere32x24.obj" },
    ])


# Surfaces
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
ml.content.load([
    { "type": "surface", "name": "surf_scene_main", "model": "default_quad", "shader": "surface" },
    { "type": "surface", "name": "surf_scene_post", "model": "default_quad", "shader": "surface" },
    ])


# Materials
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
ml.content.load([
    {
    "type":     "material",
    "name":     "skybox",
    "shader":   "skybox",
    "defs":     "True",
    "file":     PATH + "/materials/skybox.mat"
    },
    {
    "type":     "material",
    "name":     "demo_material",
    "shader":   "demo_shader",
    "defs":     "True",
    "file":     PATH + "/materials/demo.mat"
    }
    ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #