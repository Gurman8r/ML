import memelib_content as content
PATH = "../../../assets"
content.load_all([

# Images
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Image", "name": "icon",          "file": PATH + "/images/dean.png", "flip_v": "False" },
    { "type": "Image", "name": "star",          "file": PATH + "/skyboxes/star.jpg" },
    { "type": "Image", "name": "sunset_right",  "file": PATH + "/skyboxes/sunset/sunset_right.png", "flip_v": "False" },
    { "type": "Image", "name": "sunset_left",   "file": PATH + "/skyboxes/sunset/sunset_left.png", "flip_v": "False" },
    { "type": "Image", "name": "sunset_top",    "file": PATH + "/skyboxes/sunset/sunset_top.png", "flip_v": "False" },
    { "type": "Image", "name": "sunset_bottom", "file": PATH + "/skyboxes/sunset/sunset_bottom.png", "flip_v": "False" },
    { "type": "Image", "name": "sunset_front",  "file": PATH + "/skyboxes/sunset/sunset_front.png", "flip_v": "False" },
    { "type": "Image", "name": "sunset_back",   "file": PATH + "/skyboxes/sunset/sunset_back.png", "flip_v": "False" },

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
    { "type": "Shader", "name": "geometry",     "file": PATH + "/shaders/geometry.shader" },
    {
        "type": "Shader", 
        "name": "advanced",
        "vert": PATH + "/shaders/advanced.vs.shader",
        "frag": PATH + "/shaders/advanced.fs.shader"
    },

# Textures
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Texture", "name": "dean",        "file": PATH + "/images/dean.png"       },
    { "type": "Texture", "name": "neutrino",    "file": PATH + "/images/neutrino.png"   },
    { "type": "Texture", "name": "sanic",       "file": PATH + "/images/sanic.png"      },
    { "type": "Texture", "name": "borg",        "file": PATH + "/images/borg.png"       },
    { "type": "Texture", "name": "grass",       "file": PATH + "/images/grass.png"      },
    { "type": "Texture", "name": "navball",     "file": PATH + "/images/navball.png"    },
    { "type": "Texture", "name": "buck",        "file": PATH + "/images/buck.png"       },

    { "type": "Texture", "name": "earth_cm",    "file": PATH + "/textures/earth/earth_cm_2k.png" },
    { "type": "Texture", "name": "earth_dm",    "file": PATH + "/textures/earth/earth_dm_2k.png" },
    { "type": "Texture", "name": "earth_hm",    "file": PATH + "/textures/earth/earth_hm_2k.png" },
    { "type": "Texture", "name": "earth_lm",    "file": PATH + "/textures/earth/earth_lm_2k.png" },
    { "type": "Texture", "name": "earth_nm",    "file": PATH + "/textures/earth/earth_nm_2k.png" },
    { "type": "Texture", "name": "earth_sm",    "file": PATH + "/textures/earth/earth_sm_2k.png" },
    { "type": "Texture", "name": "stone_dm",    "file": PATH + "/textures/stone/stone_dm.png" },
    { "type": "Texture", "name": "stone_hm",    "file": PATH + "/textures/stone/stone_hm.png" },
    { "type": "Texture", "name": "stone_nm",    "file": PATH + "/textures/stone/stone_nm.png" },
    { "type": "Texture", "name": "mars_dm",     "file": PATH + "/textures/mars/mars_dm_2k.png" },
    { "type": "Texture", "name": "mars_nm",     "file": PATH + "/textures/mars/mars_nm_2k.png" },
    { "type": "Texture", "name": "moon_dm",     "file": PATH + "/textures/moon/moon_dm_2k.png" },
    { "type": "Texture", "name": "moon_nm",     "file": PATH + "/textures/moon/moon_nm_2k.png" },

    { "type": "Texture", "name": "icon_add",        "file": PATH + "/ui/Add-Icon.png" },
    { "type": "Texture", "name": "icon_check",      "file": PATH + "/ui/Check-Icon.png" },
    { "type": "Texture", "name": "icon_home",       "file": PATH + "/ui/Home-Icon.png" },
    { "type": "Texture", "name": "icon_lock",       "file": PATH + "/ui/Lock-Icon.png" },
    { "type": "Texture", "name": "icon_power",      "file": PATH + "/ui/Power-Icon.png" },
    { "type": "Texture", "name": "icon_refresh",    "file": PATH + "/ui/Refresh-Icon.png" },
    { "type": "Texture", "name": "icon_remote",     "file": PATH + "/ui/Remote-Icon.png" },
    { "type": "Texture", "name": "icon_remove",     "file": PATH + "/ui/Remove-Icon.png" },
    { "type": "Texture", "name": "icon_search",     "file": PATH + "/ui/Search-Icon.png" },
    { "type": "Texture", "name": "icon_settings",   "file": PATH + "/ui/Settings-Icon.png" },
    { "type": "Texture", "name": "icon_social",     "file": PATH + "/ui/Social-Icon.png" },
    { "type": "Texture", "name": "icon_trash",      "file": PATH + "/ui/Trash-Icon.png" },

# Skyboxes
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    {
        "type":     "Texture",
        "name":     "sky_starfield",
        "sampler":  "texture_cube",
        "smooth":   "False",
        "repeat":   "True",
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
        "source":   "files",
        "front":    PATH + "/skyboxes/space/1.png",
        "left":     PATH + "/skyboxes/space/2.png",
        "back":     PATH + "/skyboxes/space/3.png",
        "right":    PATH + "/skyboxes/space/4.png",
        "top":      PATH + "/skyboxes/space/5.png",
        "bottom":   PATH + "/skyboxes/space/6.png"
    },
    {
        "type":     "Texture",
        "name":     "sky_sunset",
        "sampler":  "texture_cube",
        "smooth":   "False",
        "repeat":   "True",
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
    { "type": "Model", "name": "cube_obj",      "file": PATH + "/meshes/cube.obj" },
    { "type": "Model", "name": "sphere8x6",     "file": PATH + "/meshes/sphere8x6.obj" },
    { "type": "Model", "name": "sphere32x24",   "file": PATH + "/meshes/sphere32x24.obj" },
    { "type": "Model", "name": "cow",           "file": PATH + "/meshes/cow.obj" },
    { "type": "Model", "name": "teapot",        "file": PATH + "/meshes/teapot.obj" },
    { "type": "Model", "name": "teddy",         "file": PATH + "/meshes/teddy.obj" },
    #{ "type": "Model", "name": "pumpkin",       "file": PATH + "/meshes/pumpkin.obj" },

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
        "name":     "advanced",
        "shader":   "advanced",
        "defaults": "True",
        "uniforms": PATH + "/materials/advanced.mat"
    },
    {
        "type":     "Material",
        "name":     "basic",
        "shader":   "basic",
        "defaults": "True",
        "uniforms": PATH + "/materials/basic.mat"
    },
    {
        "type":     "Material",
        "name":     "geometry",
        "shader":   "geometry",
        "defaults": "False",
        "uniforms": PATH + "/materials/geometry.mat"
    },
    {
        "type":     "Material",
        "name":     "surface",
        "shader":   "surface",
        "defaults": "False",
        "uniforms": PATH + "/materials/surface.mat"
    },

# Surfaces
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Surface", "name": "surf_scene_main", "model": "default_quad", "material": "surface" },
    { "type": "Surface", "name": "surf_scene_post", "model": "default_quad", "material": "surface" },

# Scripts
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Script", "name": "test_ecs",     "file": PATH + "/scripts/ecs.py" },
    { "type": "Script", "name": "test_lua",     "file": PATH + "/scripts/test.lua" },
    { "type": "Script", "name": "test_python",  "file": PATH + "/scripts/test.py" },

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    ])