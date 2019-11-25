# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
#
#   This script is called once after the window is created and after defaults are generated.
#
#   For more information see:
#   https://github.com/Gurman8r/ML/blob/master/src/ML/Engine/Python.cpp
#   https://github.com/Gurman8r/ML/blob/master/src/ML/Engine/ContentImporter.cpp
#
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import MEMELIB as ml

PATH = ml.prefs.get("Engine", "asset_path", "../../../assets")

ml.content.load_all([

# Images
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "ml::Image", "name": "_app_icon_",        "file": PATH + "/icon.png", "flip_v": "False" },
    { "type": "ml::Image", "name": "img/star",          "file": PATH + "/textures/sky/star.jpg" },
    { "type": "ml::Image", "name": "img/sunset/right",  "file": PATH + "/textures/sky/sunset/sunset_right.png", "flip_v": "False" },
    { "type": "ml::Image", "name": "img/sunset/left",   "file": PATH + "/textures/sky/sunset/sunset_left.png", "flip_v": "False" },
    { "type": "ml::Image", "name": "img/sunset/top",    "file": PATH + "/textures/sky/sunset/sunset_top.png", "flip_v": "False" },
    { "type": "ml::Image", "name": "img/sunset/bottom", "file": PATH + "/textures/sky/sunset/sunset_bottom.png", "flip_v": "False" },
    { "type": "ml::Image", "name": "img/sunset/front",  "file": PATH + "/textures/sky/sunset/sunset_front.png", "flip_v": "False" },
    { "type": "ml::Image", "name": "img/sunset/back",   "file": PATH + "/textures/sky/sunset/sunset_back.png", "flip_v": "False" },

# Fonts
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "ml::Font", "name": "fnt/clacon",         "file": PATH + "/fonts/clacon.ttf" },
    { "type": "ml::Font", "name": "fnt/consolas",       "file": PATH + "/fonts/consolas.ttf" },
    { "type": "ml::Font", "name": "fnt/lconsole",       "file": PATH + "/fonts/lucida_console.ttf" },
    { "type": "ml::Font", "name": "fnt/minecraft",      "file": PATH + "/fonts/minecraft.ttf" },

# Shaders
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "ml::Shader", "name": "gl/2D/basic",          "vert": PATH + "/shaders/2D.vs.shader", "frag": PATH + "/shaders/basic.fs.shader" },
    { "type": "ml::Shader", "name": "gl/2D/boxes",          "vert": PATH + "/shaders/2D.vs.shader", "frag": PATH + "/shaders/boxes.fs.shader" },
    { "type": "ml::Shader", "name": "gl/2D/clouds",         "vert": PATH + "/shaders/2D.vs.shader", "frag": PATH + "/shaders/clouds.fs.shader" },
    { "type": "ml::Shader", "name": "gl/2D/flames",         "vert": PATH + "/shaders/2D.vs.shader", "frag": PATH + "/shaders/flames.fs.shader" },
    { "type": "ml::Shader", "name": "gl/2D/lines",          "vert": PATH + "/shaders/2D.vs.shader", "frag": PATH + "/shaders/lines.fs.shader" },
    { "type": "ml::Shader", "name": "gl/2D/spirals",      	"vert": PATH + "/shaders/2D.vs.shader", "frag": PATH + "/shaders/spirals.fs.shader" },
    { "type": "ml::Shader", "name": "gl/2D/swirly",         "vert": PATH + "/shaders/2D.vs.shader", "frag": PATH + "/shaders/swirly.fs.shader" },
    { "type": "ml::Shader", "name": "gl/2D/sparkles",       "vert": PATH + "/shaders/2D.vs.shader", "frag": PATH + "/shaders/sparkles.fs.shader" },
    { "type": "ml::Shader", "name": "gl/2D/hypertunnel",    "vert": PATH + "/shaders/2D.vs.shader", "frag": PATH + "/shaders/hypertunnel.fs.shader" },
    { "type": "ml::Shader", "name": "gl/2D/wrainbow",       "vert": PATH + "/shaders/2D.vs.shader", "frag": PATH + "/shaders/wrainbow.fs.shader" },
    { "type": "ml::Shader", "name": "gl/3D/basic",          "vert": PATH + "/shaders/3D.vs.shader", "frag": PATH + "/shaders/basic.fs.shader" },
    { "type": "ml::Shader", "name": "gl/3D/phong",          "vert": PATH + "/shaders/3D.vs.shader", "frag": PATH + "/shaders/phong.fs.shader" },
    { "type": "ml::Shader", "name": "gl/3D/skybox",         "vert": PATH + "/shaders/skybox.vs.shader", "frag": PATH + "/shaders/skybox.fs.shader" },
    { "type": "ml::Shader", "name": "gl/util/pipeline",     "file": PATH + "/shaders/pipeline.shader" },
    #{ "type": "ml::Shader", "name": "gl/test/geometry",     "file": PATH + "/shaders/geometry.shader" },

# Textures
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "ml::Texture", "name": "tex/neutrino",        "file": PATH + "/textures/neutrino.png" },
    { "type": "ml::Texture", "name": "tex/sanic",           "file": PATH + "/textures/sanic.png" },
    { "type": "ml::Texture", "name": "tex/borg",            "file": PATH + "/textures/borg.png" },
	{ "type": "ml::Texture", "name": "tex/doot",            "file": PATH + "/textures/doot.png" },
    { "type": "ml::Texture", "name": "tex/navball",         "file": PATH + "/textures/navball.png" },
    #{ "type": "ml::Texture", "name": "tex/earth_dm_10k",    "file": PATH + "/textures/earth_dm_10k.jpg" },
    #{ "type": "ml::Texture", "name": "tex/earth_sm_10k",    "file": PATH + "/textures/earth_sm_10k.jpg" },
    { "type": "ml::Texture", "name": "tex/earth_cm",        "file": PATH + "/textures/earth/earth_cm_2k.png" },
    { "type": "ml::Texture", "name": "tex/earth_dm",        "file": PATH + "/textures/earth/earth_dm_2k.png" },
    { "type": "ml::Texture", "name": "tex/earth_hm",        "file": PATH + "/textures/earth/earth_hm_2k.png" },
    { "type": "ml::Texture", "name": "tex/earth_lm",        "file": PATH + "/textures/earth/earth_lm_2k.png" },
    { "type": "ml::Texture", "name": "tex/earth_nm",        "file": PATH + "/textures/earth/earth_nm_2k.png" },
    { "type": "ml::Texture", "name": "tex/earth_sm",        "file": PATH + "/textures/earth/earth_sm_2k.png" },
    { "type": "ml::Texture", "name": "tex/stone_dm",        "file": PATH + "/textures/stone/stone_dm.png" },
    { "type": "ml::Texture", "name": "tex/stone_hm",        "file": PATH + "/textures/stone/stone_hm.png" },
    { "type": "ml::Texture", "name": "tex/stone_nm",        "file": PATH + "/textures/stone/stone_nm.png" },
    { "type": "ml::Texture", "name": "tex/mars_dm",         "file": PATH + "/textures/mars/mars_dm_2k.png" },
    { "type": "ml::Texture", "name": "tex/mars_nm",         "file": PATH + "/textures/mars/mars_nm_2k.png" },
    { "type": "ml::Texture", "name": "tex/moon_dm",         "file": PATH + "/textures/moon/moon_dm_2k.png" },
    { "type": "ml::Texture", "name": "tex/moon_nm",         "file": PATH + "/textures/moon/moon_nm_2k.png" },
    { "type": "ml::Texture", "name": "tex/icon_add",        "file": PATH + "/textures/ui/Add-Icon.png" },
    { "type": "ml::Texture", "name": "tex/icon_check",      "file": PATH + "/textures/ui/Check-Icon.png" },
    { "type": "ml::Texture", "name": "tex/icon_home",       "file": PATH + "/textures/ui/Home-Icon.png" },
    { "type": "ml::Texture", "name": "tex/icon_lock",       "file": PATH + "/textures/ui/Lock-Icon.png" },
    { "type": "ml::Texture", "name": "tex/icon_power",      "file": PATH + "/textures/ui/Power-Icon.png" },
    { "type": "ml::Texture", "name": "tex/icon_refresh",    "file": PATH + "/textures/ui/Refresh-Icon.png" },
    { "type": "ml::Texture", "name": "tex/icon_remote",     "file": PATH + "/textures/ui/Remote-Icon.png" },
    { "type": "ml::Texture", "name": "tex/icon_remove",     "file": PATH + "/textures/ui/Remove-Icon.png" },
    { "type": "ml::Texture", "name": "tex/icon_search",     "file": PATH + "/textures/ui/Search-Icon.png" },
    { "type": "ml::Texture", "name": "tex/icon_settings",   "file": PATH + "/textures/ui/Settings-Icon.png" },
    { "type": "ml::Texture", "name": "tex/icon_social",     "file": PATH + "/textures/ui/Social-Icon.png" },
    { "type": "ml::Texture", "name": "tex/icon_trash",      "file": PATH + "/textures/ui/Trash-Icon.png" },

# Skyboxes
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    {
        "type":     "ml::Texture",
        "name":     "tex/sky/starfield",
        "sampler":  "texture_cube",
        "smooth":   "False",
        "repeat":   "True",
        "source":   "images",
        "front":    "img/star",
        "left":     "img/star",
        "back":     "img/star",
        "right":    "img/star",
        "top":      "img/star",
        "bottom":   "img/star"
    },
    {
        "type":     "ml::Texture",
        "name":     "tex/sky/space",
        "sampler":  "texture_cube",
        "smooth":   "False",
        "repeat":   "True",
        "source":   "files",
        "front":    PATH + "/textures/sky/space/1.png",
        "left":     PATH + "/textures/sky/space/2.png",
        "back":     PATH + "/textures/sky/space/3.png",
        "right":    PATH + "/textures/sky/space/4.png",
        "top":      PATH + "/textures/sky/space/5.png",
        "bottom":   PATH + "/textures/sky/space/6.png"
    },
    {
        "type":     "ml::Texture",
        "name":     "tex/sky/sunset",
        "sampler":  "texture_cube",
        "smooth":   "False",
        "repeat":   "True",
        "source":   "images",
        "right":    "img/sunset/right",
        "left":     "img/sunset/left",
        "top":      "img/sunset/top",
        "bottom":   "img/sunset/bottom",
        "front":    "img/sunset/front",
        "back":     "img/sunset/back"
    },

# Models
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "ml::Model", "name": "obj/cube",          "file": PATH + "/meshes/cube.obj" },
    { "type": "ml::Model", "name": "obj/monkey",        "file": PATH + "/meshes/monkey.obj" },
    { "type": "ml::Model", "name": "obj/sphere8x6",     "file": PATH + "/meshes/sphere8x6.obj" },
    { "type": "ml::Model", "name": "obj/sphere32x24",   "file": PATH + "/meshes/sphere32x24.obj" },
    #{ "type": "ml::Model", "name": "obj/cessna",        "file": PATH + "/meshes/cessna.obj" },
    #{ "type": "ml::Model", "name": "obj/minicooper",    "file": PATH + "/meshes/minicooper.obj" },
    { "type": "ml::Model", "name": "obj/cow",           "file": PATH + "/meshes/cow.obj" },
    { "type": "ml::Model", "name": "obj/teapot",        "file": PATH + "/meshes/teapot.obj" },
    { "type": "ml::Model", "name": "obj/teddy",         "file": PATH + "/meshes/teddy.obj" },

# Materials
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    {
        "type": "ml::Material",
        "name": "mat/demo",
        "defaults": "False",
        "uniforms": PATH + "/materials/demo.mat"
    },
    {
        "type": "ml::Material",
        "name": "mat/2D/basic",
        "defaults": "True",
        "uniforms": PATH + "/materials/basic.2D.mat"
    },
    {
        "type": "ml::Material",
        "name": "mat/3D/basic",
        "defaults": "True",
        "uniforms": PATH + "/materials/basic.3D.mat"
    },
    {
        "type": "ml::Material",
        "name": "mat/3D/phong",
        "defaults": "True",
        "uniforms": PATH + "/materials/phong.3D.mat"
    },
    {
        "type": "ml::Material",
        "name": "mat/3D/skybox",
        "defaults": "True",
        "uniforms": PATH + "/materials/skybox.3D.mat"
    },
    {
        "type": "ml::Material",
        "name": "mat/util/pipeline",
        "defaults": "False",
        "uniforms": PATH + "/materials/pipeline.mat"
    },
    #{
    #    "type": "ml::Material",
    #    "name": "mat/geometry",
    #    "defaults": "False",
    #    "uniforms": PATH + "/materials/geometry.mat"
    #},

# RenderTextures
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { 
        "type":     "ml::RenderTexture", 
        "name":     "render/main", 
        "model":    "obj/default/quad", 
        "material": "mat/util/pipeline",
        "shader":   "gl/util/pipeline",
    },
    { 
        "type":     "ml::RenderTexture", 
        "name":     "render/post", 
        "model":    "obj/default/quad", 
        "material": "mat/util/pipeline",
        "shader":   "gl/util/pipeline",
    },

# Scripts
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "ml::Script", "name": "scr_test_lua", "file": PATH + "/scripts/test.lua" },
    { "type": "ml::Script", "name": "scr_test_python", "file": PATH + "/scripts/test.py" },

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    ])

# Camera
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
name = "_camera_"
if ml.content.load({ "type": "ml::Entity", "name": name }):
    if ml.ecs.add_component(name, "ml::Camera"):
        ml.ecs.camera_attr(name,    "self",     "enabled",      "True")
        ml.ecs.camera_attr(name,    "self",     "clearflags",   "SolidColor")
        ml.ecs.camera_attr(name,    "self",     "projection",   "Perspective")
        ml.ecs.camera_attr(name,    "self",     "background",   "0 0 0 1")
        ml.ecs.camera_attr(name,    "self",     "position",     "0 0 3")
        ml.ecs.camera_attr(name,    "self",     "direction",    "0 0 -1")
        ml.ecs.camera_attr(name,    "self",     "fov",          "45.0")
        ml.ecs.camera_attr(name,    "self",     "near",         "0.001")
        ml.ecs.camera_attr(name,    "self",     "far",          "1000.0")
        ml.ecs.camera_attr(name,    "self",     "viewport",     "0 1920 0 1080")

# Skybox
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
name = "_skybox_"
if ml.content.load({ "type": "ml::Entity", "name": name }):
    if ml.ecs.add_component(name, "ml::Transform"):
        ml.ecs.transform_attr(name, "self",     "enabled",      "True")
        ml.ecs.transform_attr(name, "self",     "rotation",     "0.0 1.0 0.0 0.01")
    if ml.ecs.add_component(name, "ml::Renderer"):
        ml.ecs.renderer_attr(name,  "self",     "enabled",      "False")
        ml.ecs.renderer_attr(name,  "self",     "material",     "mat/3D/skybox")
        ml.ecs.renderer_attr(name,  "self",     "shader",       "gl/3D/skybox")
        ml.ecs.renderer_attr(name,  "self",     "model",        "obj/default/skybox")
        ml.ecs.renderer_attr(name,  "depth",    "mask",         "False")

# Demo A
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
name = "ent_demo_A"
if ml.content.load({ "type": "ml::Entity", "name": name }):
    if ml.ecs.add_component(name, "ml::Renderer"):
        ml.ecs.renderer_attr(name,  "self",     "enabled",      "True")
        ml.ecs.renderer_attr(name,  "self",     "material",     "mat/2D/basic")
        ml.ecs.renderer_attr(name,  "self",     "shader",       "gl/2D/basic")
        ml.ecs.renderer_attr(name,  "self",     "model",        "obj/default/quad")
        ml.ecs.renderer_attr(name,  "cull",     "enabled",      "False")

# Demo B
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
name = "ent_demo_B"
if ml.content.load({ "type": "ml::Entity", "name": name }):
    if ml.ecs.add_component(name, "ml::Transform"):
        ml.ecs.transform_attr(name, "self",     "enabled",      "False")
        ml.ecs.transform_attr(name, "self",     "position",     "0.0 0.0 0.0")
        ml.ecs.transform_attr(name, "self",     "scale",        "0.3 0.3 0.3")
        ml.ecs.transform_attr(name, "self",     "rotation",     "0.0 1.0 0.0 0.25")
    if ml.ecs.add_component(name, "ml::Renderer"):
        ml.ecs.renderer_attr(name,  "self",     "enabled",      "False")
        ml.ecs.renderer_attr(name,  "self",     "material",     "mat/3D/phong")
        ml.ecs.renderer_attr(name,  "self",     "shader",       "gl/3D/phong")
        ml.ecs.renderer_attr(name,  "self",     "model",        "obj/sphere32x24")
        ml.ecs.renderer_attr(name,  "alpha",    "enabled",      "True")
        ml.ecs.renderer_attr(name,  "alpha",    "func",         "GL_GREATER")
        ml.ecs.renderer_attr(name,  "alpha",    "coeff",        "0.001")
        ml.ecs.renderer_attr(name,  "blend",    "enabled",      "True")
        ml.ecs.renderer_attr(name,  "blend",    "sfactorRGB",   "GL_SRC_ALPHA")
        ml.ecs.renderer_attr(name,  "blend",    "sfactorAlpha", "GL_ONE_MINUS_SRC_ALPHA")
        ml.ecs.renderer_attr(name,  "blend",    "dfactorRGB",   "GL_SRC_ALPHA")
        ml.ecs.renderer_attr(name,  "blend",    "dfactorAlpha", "GL_ONE_MINUS_SRC_ALPHA")
        ml.ecs.renderer_attr(name,  "cull",     "enabled",      "True")
        ml.ecs.renderer_attr(name,  "cull",     "mode",         "GL_BACK")
        ml.ecs.renderer_attr(name,  "depth",    "enabled",      "True")
        ml.ecs.renderer_attr(name,  "depth",    "func",         "GL_LESS")
        ml.ecs.renderer_attr(name,  "depth",    "mask",         "True")

if ml.config.is_debug(): # switch target entities
    ml.prefs.set("Noobs", "target_entity", "ent_demo_B")
    ml.ecs.renderer_attr("ent_demo_A", "self", "enabled", "False")
    ml.ecs.renderer_attr("ent_demo_B", "self", "enabled", "True")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #