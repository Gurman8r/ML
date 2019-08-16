import memelib as ml

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

if (__name__ == "__main__"):
    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    PATH = "../../../assets/"

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    ml.content.load("image", "icon", { "file": PATH + "images/dean.png",
        "flip_vertically": "false"
        })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    ml.content.load("font", "clacon",       { "file": PATH + "fonts/clacon.ttf" })
    ml.content.load("font", "consolas",     { "file": PATH + "fonts/consolas.ttf" })
    ml.content.load("font", "lconsole",     { "file": PATH + "fonts/lucida_console.ttf" })
    ml.content.load("font", "minecraft",    { "file": PATH + "fonts/minecraft.ttf" })
    
    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    
    ml.content.load("shader", "basic",      { "file": PATH + "shaders/basic.shader" })
    ml.content.load("shader", "sprites",    { "file": PATH + "shaders/sprites.shader" })
    ml.content.load("shader", "normal",     { "file": PATH + "shaders/normal.shader" })
    ml.content.load("shader", "text",       { "file": PATH + "shaders/text.shader" })
    ml.content.load("shader", "geometry",   { "file": PATH + "shaders/geometry.shader" })
    ml.content.load("shader", "surface",    { "file": PATH + "shaders/surface.shader" })
    ml.content.load("shader", "solid",      { "file": PATH + "shaders/solid.shader" })
    ml.content.load("shader", "skybox",     { "file": PATH + "shaders/skybox.shader" })
    
    ml.content.load("shader", "demo_shader", {
        "vert": PATH + "shaders/demo.vs.shader",
        "frag": PATH + "shaders/demo.fs.shader"
        })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    
    ml.content.load("texture", "dean",      { "file": PATH + "images/dean.png" })
    ml.content.load("texture", "neutrino",  { "file": PATH + "images/neutrino.png" })
    ml.content.load("texture", "sanic",     { "file": PATH + "images/sanic.png" })
    ml.content.load("texture", "borg",      { "file": PATH + "images/borg.png" })
    ml.content.load("texture", "grass",     { "file": PATH + "images/grass.png" })
    ml.content.load("texture", "navball",   { "file": PATH + "images/navball.png" })
    ml.content.load("texture", "buck",      { "file": PATH + "images/buck.png" })
    
    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    ml.content.load("texture", "earth_cm", { "file": PATH + "textures/earth/earth_cm_2k.png",
        "format": "RGB"
        })
    ml.content.load("texture", "earth_dm", { "file": PATH + "textures/earth/earth_dm_2k.png",
        "format": "RGB"
        })
    ml.content.load("texture", "earth_hm", { "file": PATH + "textures/earth/earth_hm_2k.png",
        "format": "red"
        })
    ml.content.load("texture", "earth_lm", { "file": PATH + "textures/earth/earth_lm_2k.png",
        "format": "RGB"
        })
    ml.content.load("texture", "earth_nm", { "file": PATH + "textures/earth/earth_nm_2k.png",
        "format": "RGB"
        })
    ml.content.load("texture", "earth_sm", { "file": PATH + "textures/earth/earth_sm_2k.png",
        "format": "red"
        })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    
    ml.content.load("texture", "stone_dm", { "file": PATH + "textures/stone/stone_dm.png"
       })
    ml.content.load("texture", "stone_hm", { "file": PATH + "textures/stone/stone_hm.png"
       })
    ml.content.load("texture", "stone_nm", { "file": PATH + "textures/stone/stone_nm.png" 
        })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    ml.content.load("texture", "mars_dm", { "file": PATH + "textures/mars/mars_dm_2k.png",
        "format": "RGB" 
        })
    ml.content.load("texture", "mars_nm", { "file": PATH + "textures/mars/mars_nm_2k.png",
        "format": "RGB"
       })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    ml.content.load("texture", "moon_dm", { "file": PATH + "textures/moon/moon_dm_2k.png",
        "format": "RGB" 
        })
    ml.content.load("texture", "moon_nm", { "file": PATH + "textures/moon/moon_nm_2k.png",
        "format": "RGB"
       })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    ml.content.load("image", "star", { "file": PATH + "skyboxes/star.jpg"
       })
    ml.content.load("texture", "starfield", {
        "target":   "texture_cube",
        "smooth":   "false",
        "repeat":   "true",
        "format":   "RGB",
        "source":   "images",
        "front":    "star",
        "left":     "star",
        "back":     "star",
        "right":    "star",
        "top":      "star",
        "bottom":   "star"
        })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    ml.content.load("texture", "space", {
        "target":   "texture_cube",
        "smooth":   "false",
        "repeat":   "true",
        "format":   "RGB",
        "source":   "files",
        "front":    PATH + "skyboxes/space/1.png",
        "left":     PATH + "skyboxes/space/2.png",
        "back":     PATH + "skyboxes/space/3.png",
        "right":    PATH + "skyboxes/space/4.png",
        "top":      PATH + "skyboxes/space/5.png",
        "bottom":   PATH + "skyboxes/space/6.png"
        })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    ml.content.load("image", "sunset_right", { "file": PATH + "skyboxes/sunset/sunset_right.png",
        "flip_vertically": "false"
        })
    ml.content.load("image", "sunset_left", { "file": PATH + "skyboxes/sunset/sunset_left.png",
        "flip_vertically": "false"
        })
    ml.content.load("image", "sunset_top", { "file": PATH + "skyboxes/sunset/sunset_top.png",
        "flip_vertically": "false"
        })
    ml.content.load("image", "sunset_bottom", { "file": PATH + "skyboxes/sunset/sunset_bottom.png",
        "flip_vertically": "false"
        })
    ml.content.load("image", "sunset_front", { "file": PATH + "skyboxes/sunset/sunset_front.png",
        "flip_vertically": "false"
        })
    ml.content.load("image", "sunset_back", { "file": PATH + "skyboxes/sunset/sunset_back.png",
        "flip_vertically": "false"
        })

    ml.content.load("texture", "sunset", {
        "target":   "texture_cube",
        "smooth":   "false",
        "repeat":   "true",
        "format":   "RGBA",
        "source":   "images",
        "right":    "sunset_right",
        "left":     "sunset_left",
        "top":      "sunset_top",
        "bottom":   "sunset_bottom",
        "front":    "sunset_front",
        "back":     "sunset_back"
        })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    ml.content.load("model", "cube",        { "file": PATH + "meshes/cube.obj" })
    ml.content.load("model", "sphere8x6",   { "file": PATH + "meshes/sphere8x6.obj" })
    ml.content.load("model", "sphere32x24", { "file": PATH + "meshes/sphere32x24.obj" })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    ml.content.load("surface", "surf_scene_main", {
        "model": "default_quad",
        "shader": "surface",
        })

    ml.content.load("surface", "surf_scene_post", {
        "model": "default_quad",
        "shader": "surface",
        })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

    ml.content.load("material", "skybox", {
        "shader": "skybox",
        "defs": "true",
        "file": PATH + "materials/skybox.mat"
        })

    ml.content.load("material", "demo_material", {
        "shader": "demo_shader",
        "defs": "true",
        "file": PATH + "materials/demo.mat"
        })

    # * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #


