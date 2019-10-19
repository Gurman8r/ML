import MEMELIB as ml
PATH = "../../../assets"
ml.content.load_all([

# Images
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Image", "name": "_app_icon_",        "file": PATH + "/icon.png", "flip_v": "False" },
    { "type": "Image", "name": "img_star",          "file": PATH + "/textures/sky/star.jpg" },
    { "type": "Image", "name": "img_sunset_right",  "file": PATH + "/textures/sky/sunset/sunset_right.png", "flip_v": "False" },
    { "type": "Image", "name": "img_sunset_left",   "file": PATH + "/textures/sky/sunset/sunset_left.png", "flip_v": "False" },
    { "type": "Image", "name": "img_sunset_top",    "file": PATH + "/textures/sky/sunset/sunset_top.png", "flip_v": "False" },
    { "type": "Image", "name": "img_sunset_bottom", "file": PATH + "/textures/sky/sunset/sunset_bottom.png", "flip_v": "False" },
    { "type": "Image", "name": "img_sunset_front",  "file": PATH + "/textures/sky/sunset/sunset_front.png", "flip_v": "False" },
    { "type": "Image", "name": "img_sunset_back",   "file": PATH + "/textures/sky/sunset/sunset_back.png", "flip_v": "False" },

# Fonts
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Font", "name": "fnt_clacon",         "file": PATH + "/fonts/clacon.ttf" },
    { "type": "Font", "name": "fnt_consolas",       "file": PATH + "/fonts/consolas.ttf" },
    { "type": "Font", "name": "fnt_lconsole",       "file": PATH + "/fonts/lucida_console.ttf" },
    { "type": "Font", "name": "fnt_minecraft",      "file": PATH + "/fonts/minecraft.ttf" },

# Shaders
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    {
        "type": "Shader", 
        "name": "gl_advanced",
        "vert": PATH + "/shaders/advanced.vs.shader",
        "frag": PATH + "/shaders/advanced.fs.shader"
    },
    { "type": "Shader", "name": "gl_basic",         "file": PATH + "/shaders/basic.shader" },
    { "type": "Shader", "name": "gl_boxes",      	"file": PATH + "/shaders/boxes.shader" },
    { "type": "Shader", "name": "gl_flames",        "file": PATH + "/shaders/flames.shader" },
    #{ "type": "Shader", "name": "gl_geometry",      "file": PATH + "/shaders/geometry.shader" },
    { "type": "Shader", "name": "gl_lines",         "file": PATH + "/shaders/lines.shader" },
    { "type": "Shader", "name": "gl_skybox",        "file": PATH + "/shaders/skybox.shader" },
    { "type": "Shader", "name": "gl_spirals",      	"file": PATH + "/shaders/spirals.shader" },
    { "type": "Shader", "name": "gl_surface",       "file": PATH + "/shaders/surface.shader" },
    { "type": "Shader", "name": "gl_swirly",        "file": PATH + "/shaders/swirly.shader" },
    { "type": "Shader", "name": "gl_sparkles",      "file": PATH + "/shaders/sparkles.shader" },
    { "type": "Shader", "name": "gl_hypertunnel",   "file": PATH + "/shaders/hypertunnel.shader" },
    { "type": "Shader", "name": "gl_wrainbow",      "file": PATH + "/shaders/wrainbow.shader" },

# Textures
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Texture", "name": "tex_neutrino",        "file": PATH + "/textures/neutrino.png" },
    { "type": "Texture", "name": "tex_sanic",           "file": PATH + "/textures/sanic.png" },
    { "type": "Texture", "name": "tex_borg",            "file": PATH + "/textures/borg.png" },
	{ "type": "Texture", "name": "tex_doot",            "file": PATH + "/textures/doot.png" },
    { "type": "Texture", "name": "tex_navball",         "file": PATH + "/textures/navball.png" },
    { "type": "Texture", "name": "tex_earth_cm",        "file": PATH + "/textures/earth/earth_cm_2k.png" },
    { "type": "Texture", "name": "tex_earth_dm",        "file": PATH + "/textures/earth/earth_dm_2k.png" },
    { "type": "Texture", "name": "tex_earth_hm",        "file": PATH + "/textures/earth/earth_hm_2k.png" },
    { "type": "Texture", "name": "tex_earth_lm",        "file": PATH + "/textures/earth/earth_lm_2k.png" },
    { "type": "Texture", "name": "tex_earth_nm",        "file": PATH + "/textures/earth/earth_nm_2k.png" },
    { "type": "Texture", "name": "tex_earth_sm",        "file": PATH + "/textures/earth/earth_sm_2k.png" },
    { "type": "Texture", "name": "tex_stone_dm",        "file": PATH + "/textures/stone/stone_dm.png" },
    { "type": "Texture", "name": "tex_stone_hm",        "file": PATH + "/textures/stone/stone_hm.png" },
    { "type": "Texture", "name": "tex_stone_nm",        "file": PATH + "/textures/stone/stone_nm.png" },
    { "type": "Texture", "name": "tex_mars_dm",         "file": PATH + "/textures/mars/mars_dm_2k.png" },
    { "type": "Texture", "name": "tex_mars_nm",         "file": PATH + "/textures/mars/mars_nm_2k.png" },
    { "type": "Texture", "name": "tex_moon_dm",         "file": PATH + "/textures/moon/moon_dm_2k.png" },
    { "type": "Texture", "name": "tex_moon_nm",         "file": PATH + "/textures/moon/moon_nm_2k.png" },
    { "type": "Texture", "name": "tex_icon_add",        "file": PATH + "/textures/ui/Add-Icon.png" },
    { "type": "Texture", "name": "tex_icon_check",      "file": PATH + "/textures/ui/Check-Icon.png" },
    { "type": "Texture", "name": "tex_icon_home",       "file": PATH + "/textures/ui/Home-Icon.png" },
    { "type": "Texture", "name": "tex_icon_lock",       "file": PATH + "/textures/ui/Lock-Icon.png" },
    { "type": "Texture", "name": "tex_icon_power",      "file": PATH + "/textures/ui/Power-Icon.png" },
    { "type": "Texture", "name": "tex_icon_refresh",    "file": PATH + "/textures/ui/Refresh-Icon.png" },
    { "type": "Texture", "name": "tex_icon_remote",     "file": PATH + "/textures/ui/Remote-Icon.png" },
    { "type": "Texture", "name": "tex_icon_remove",     "file": PATH + "/textures/ui/Remove-Icon.png" },
    { "type": "Texture", "name": "tex_icon_search",     "file": PATH + "/textures/ui/Search-Icon.png" },
    { "type": "Texture", "name": "tex_icon_settings",   "file": PATH + "/textures/ui/Settings-Icon.png" },
    { "type": "Texture", "name": "tex_icon_social",     "file": PATH + "/textures/ui/Social-Icon.png" },
    { "type": "Texture", "name": "tex_icon_trash",      "file": PATH + "/textures/ui/Trash-Icon.png" },

# Skyboxes
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    {
        "type":     "Texture",
        "name":     "sky_starfield",
        "sampler":  "texture_cube",
        "smooth":   "False",
        "repeat":   "True",
        "source":   "images",
        "front":    "img_star",
        "left":     "img_star",
        "back":     "img_star",
        "right":    "img_star",
        "top":      "img_star",
        "bottom":   "img_star"
    },
    {
        "type":     "Texture",
        "name":     "sky_space",
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
        "type":     "Texture",
        "name":     "sky_sunset",
        "sampler":  "texture_cube",
        "smooth":   "False",
        "repeat":   "True",
        "source":   "images",
        "right":    "img_sunset_right",
        "left":     "img_sunset_left",
        "top":      "img_sunset_top",
        "bottom":   "img_sunset_bottom",
        "front":    "img_sunset_front",
        "back":     "img_sunset_back"
    },

# Models
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Model", "name": "obj_cube",          "file": PATH + "/meshes/cube.obj" },
    { "type": "Model", "name": "obj_sphere8x6",     "file": PATH + "/meshes/sphere8x6.obj" },
    { "type": "Model", "name": "obj_sphere32x24",   "file": PATH + "/meshes/sphere32x24.obj" },
    { "type": "Model", "name": "obj_cow",           "file": PATH + "/meshes/cow.obj" },
    { "type": "Model", "name": "obj_monkey",        "file": PATH + "/meshes/monkey.obj" },
    { "type": "Model", "name": "obj_teapot",        "file": PATH + "/meshes/teapot.obj" },
    { "type": "Model", "name": "obj_teddy",         "file": PATH + "/meshes/teddy.obj" },

# Materials
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    {
        "type":     "Material",
        "name":     "mat_skybox",
        "shader":   "gl_skybox",
        "defaults": "True",
        "uniforms": PATH + "/materials/skybox.mat"
    },
    {
        "type":     "Material",
        "name":     "mat_3d",
        "shader":   "gl_advanced",
        "defaults": "True",
        "uniforms": PATH + "/materials/advanced.mat"
    },
    {
        "type":     "Material",
        "name":     "mat_2d",
        "shader":   "gl_basic",
        "defaults": "True",
        "uniforms": PATH + "/materials/basic.mat"
    },
    #{
    #    "type":     "Material",
    #    "name":     "mat_geometry",
    #    "shader":   "gl_geometry",
    #    "defaults": "False",
    #    "uniforms": PATH + "/materials/geometry.mat"
    #},
    {
        "type":     "Material",
        "name":     "mat_surface",
        "shader":   "gl_surface",
        "defaults": "False",
        "uniforms": PATH + "/materials/surface.mat"
    },

# Surfaces
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Surface", "name": "surf_scene_main", "model": "default_quad", "material": "mat_surface" },
    { "type": "Surface", "name": "surf_scene_post", "model": "default_quad", "material": "mat_surface" },

# Scripts
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Script", "name": "scr_test_lua",     "file": PATH + "/scripts/test.lua" },
    { "type": "Script", "name": "scr_test_python",  "file": PATH + "/scripts/test.py" },

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    ])

# Camera
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
name = "_ent_camera_"
if ml.content.load({ "type": "Entity", "name": name }):
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
name = "_ent_skybox_"
if ml.content.load({ "type": "Entity", "name": name }):
    if ml.ecs.add_component(name, "ml::Renderer"):
        ml.ecs.renderer_attr(name,  "self",     "enabled",      "False")
        ml.ecs.renderer_attr(name,  "self",     "material",     "mat_skybox")
        ml.ecs.renderer_attr(name,  "self",     "model",        "default_skybox")
        ml.ecs.renderer_attr(name,  "depth",    "mask",         "False")

# Basic
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
name = "ent_2d_demo"
if ml.content.load({ "type": "Entity", "name": name }):
    if ml.ecs.add_component(name, "ml::Renderer"):
        ml.ecs.renderer_attr(name,  "self",     "enabled",      "True")
        ml.ecs.renderer_attr(name,  "self",     "material",     "mat_2d")
        ml.ecs.renderer_attr(name,  "self",     "model",        "default_quad")
        ml.ecs.renderer_attr(name,  "self",     "shader",       "gl_basic")
        ml.ecs.renderer_attr(name,  "cull",     "enabled",      "False")

# Advanced
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
name = "ent_3d_demo"
if ml.content.load({ "type": "Entity", "name": name }):
    if ml.ecs.add_component(name, "ml::Transform"):
        ml.ecs.transform_attr(name, "self",     "position",     "0.0 0.0 0.0")
        ml.ecs.transform_attr(name, "self",     "scale",        "0.3 0.3 0.3")
        ml.ecs.transform_attr(name, "self",     "rotation",     "0.0 1.0 0.0")
    if ml.ecs.add_component(name, "ml::Renderer"):
        ml.ecs.renderer_attr(name,  "self",     "enabled",      "False")
        ml.ecs.renderer_attr(name,  "self",     "material",     "mat_3d")
        ml.ecs.renderer_attr(name,  "self",     "model",        "obj_cow")
        ml.ecs.renderer_attr(name,  "alpha",    "enabled",      "True")
        ml.ecs.renderer_attr(name,  "alpha",    "predicate",    "GL_GREATER")
        ml.ecs.renderer_attr(name,  "alpha",    "coeff",        "0.01")
        ml.ecs.renderer_attr(name,  "blend",    "enabled",      "True")
        ml.ecs.renderer_attr(name,  "blend",    "srcRGB",       "GL_SRC_ALPHA")
        ml.ecs.renderer_attr(name,  "blend",    "srcAlpha",     "GL_ONE_MINUS_SRC_ALPHA")
        ml.ecs.renderer_attr(name,  "blend",    "dstRGB",       "GL_SRC_ALPHA")
        ml.ecs.renderer_attr(name,  "blend",    "dstAlpha",     "GL_ONE_MINUS_SRC_ALPHA")
        ml.ecs.renderer_attr(name,  "cull",     "enabled",      "True")
        ml.ecs.renderer_attr(name,  "cull",     "face",         "GL_BACK")
        ml.ecs.renderer_attr(name,  "depth",    "enabled",      "True")
        ml.ecs.renderer_attr(name,  "depth",    "predicate",    "GL_LESS")
        ml.ecs.renderer_attr(name,  "depth",    "mask",         "True")

if 0:
    ml.prefs.set("Noobs", "demo_entity", "ent_3d_demo")
    ml.ecs.renderer_attr("ent_2d_demo", "self", "enabled", "False")
    ml.ecs.renderer_attr("ent_3d_demo", "self", "enabled", "True")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #