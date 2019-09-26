import memelib_content
PATH = "../../../assets"
memelib_content.load_all([

# Images
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Image", "name": "icon",              "file": PATH + "/images/dean.png", "flip_v": "False" },
    { "type": "Image", "name": "img_star",          "file": PATH + "/skyboxes/star.jpg" },
    { "type": "Image", "name": "img_sunset_right",  "file": PATH + "/skyboxes/sunset/sunset_right.png", "flip_v": "False" },
    { "type": "Image", "name": "img_sunset_left",   "file": PATH + "/skyboxes/sunset/sunset_left.png", "flip_v": "False" },
    { "type": "Image", "name": "img_sunset_top",    "file": PATH + "/skyboxes/sunset/sunset_top.png", "flip_v": "False" },
    { "type": "Image", "name": "img_sunset_bottom", "file": PATH + "/skyboxes/sunset/sunset_bottom.png", "flip_v": "False" },
    { "type": "Image", "name": "img_sunset_front",  "file": PATH + "/skyboxes/sunset/sunset_front.png", "flip_v": "False" },
    { "type": "Image", "name": "img_sunset_back",   "file": PATH + "/skyboxes/sunset/sunset_back.png", "flip_v": "False" },

# Fonts
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Font", "name": "fnt_clacon",         "file": PATH + "/fonts/clacon.ttf" },
    { "type": "Font", "name": "fnt_consolas",       "file": PATH + "/fonts/consolas.ttf" },
    { "type": "Font", "name": "fnt_lconsole",       "file": PATH + "/fonts/lucida_console.ttf" },
    { "type": "Font", "name": "fnt_minecraft",      "file": PATH + "/fonts/minecraft.ttf" },

# Shaders
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Shader", "name": "gl_basic",            "file": PATH + "/shaders/basic.shader" },
    { "type": "Shader", "name": "gl_normal",           "file": PATH + "/shaders/normal.shader" },
    { "type": "Shader", "name": "gl_surface",          "file": PATH + "/shaders/surface.shader" },
    { "type": "Shader", "name": "gl_solid",            "file": PATH + "/shaders/solid.shader" },
    { "type": "Shader", "name": "gl_skybox",           "file": PATH + "/shaders/skybox.shader" },
    #{ "type": "Shader", "name": "gl_sprites",          "file": PATH + "/shaders/sprites.shader" },
    #{ "type": "Shader", "name": "gl_text",             "file": PATH + "/shaders/text.shader" },
    { "type": "Shader", "name": "gl_geometry",         "file": PATH + "/shaders/geometry.shader" },
    {
        "type": "Shader", 
        "name": "gl_advanced",
        "vert": PATH + "/shaders/advanced.vs.shader",
        "frag": PATH + "/shaders/advanced.fs.shader"
    },

# Textures
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
    { "type": "Texture", "name": "tex_dean",        "file": PATH + "/images/dean.png"       },
    { "type": "Texture", "name": "tex_neutrino",    "file": PATH + "/images/neutrino.png"   },
    { "type": "Texture", "name": "tex_sanic",       "file": PATH + "/images/sanic.png"      },
    { "type": "Texture", "name": "tex_borg",        "file": PATH + "/images/borg.png"       },
    { "type": "Texture", "name": "tex_grass",       "file": PATH + "/images/grass.png"      },
    { "type": "Texture", "name": "tex_navball",     "file": PATH + "/images/navball.png"    },
    { "type": "Texture", "name": "tex_buck",        "file": PATH + "/images/buck.png"       },

    { "type": "Texture", "name": "tex_earth_cm",    "file": PATH + "/textures/earth/earth_cm_2k.png" },
    { "type": "Texture", "name": "tex_earth_dm",    "file": PATH + "/textures/earth/earth_dm_2k.png" },
    { "type": "Texture", "name": "tex_earth_hm",    "file": PATH + "/textures/earth/earth_hm_2k.png" },
    { "type": "Texture", "name": "tex_earth_lm",    "file": PATH + "/textures/earth/earth_lm_2k.png" },
    { "type": "Texture", "name": "tex_earth_nm",    "file": PATH + "/textures/earth/earth_nm_2k.png" },
    { "type": "Texture", "name": "tex_earth_sm",    "file": PATH + "/textures/earth/earth_sm_2k.png" },

    { "type": "Texture", "name": "tex_stone_dm",    "file": PATH + "/textures/stone/stone_dm.png" },
    { "type": "Texture", "name": "tex_stone_hm",    "file": PATH + "/textures/stone/stone_hm.png" },
    { "type": "Texture", "name": "tex_stone_nm",    "file": PATH + "/textures/stone/stone_nm.png" },

    { "type": "Texture", "name": "tex_mars_dm",     "file": PATH + "/textures/mars/mars_dm_2k.png" },
    { "type": "Texture", "name": "tex_mars_nm",     "file": PATH + "/textures/mars/mars_nm_2k.png" },

    { "type": "Texture", "name": "tex_moon_dm",     "file": PATH + "/textures/moon/moon_dm_2k.png" },
    { "type": "Texture", "name": "tex_moon_nm",     "file": PATH + "/textures/moon/moon_nm_2k.png" },

    { "type": "Texture", "name": "tex_icon_add",        "file": PATH + "/ui/Add-Icon.png" },
    { "type": "Texture", "name": "tex_icon_check",      "file": PATH + "/ui/Check-Icon.png" },
    { "type": "Texture", "name": "tex_icon_home",       "file": PATH + "/ui/Home-Icon.png" },
    { "type": "Texture", "name": "tex_icon_lock",       "file": PATH + "/ui/Lock-Icon.png" },
    { "type": "Texture", "name": "tex_icon_power",      "file": PATH + "/ui/Power-Icon.png" },
    { "type": "Texture", "name": "tex_icon_refresh",    "file": PATH + "/ui/Refresh-Icon.png" },
    { "type": "Texture", "name": "tex_icon_remote",     "file": PATH + "/ui/Remote-Icon.png" },
    { "type": "Texture", "name": "tex_icon_remove",     "file": PATH + "/ui/Remove-Icon.png" },
    { "type": "Texture", "name": "tex_icon_search",     "file": PATH + "/ui/Search-Icon.png" },
    { "type": "Texture", "name": "tex_icon_settings",   "file": PATH + "/ui/Settings-Icon.png" },
    { "type": "Texture", "name": "tex_icon_social",     "file": PATH + "/ui/Social-Icon.png" },
    { "type": "Texture", "name": "tex_icon_trash",      "file": PATH + "/ui/Trash-Icon.png" },

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
    { "type": "Model", "name": "obj_teapot",        "file": PATH + "/meshes/teapot.obj" },
    { "type": "Model", "name": "obj_teddy",         "file": PATH + "/meshes/teddy.obj" },
    #{ "type": "Model", "name": "obj_pumpkin",       "file": PATH + "/meshes/pumpkin.obj" },

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
        "name":     "mat_advanced",
        "shader":   "gl_advanced",
        "defaults": "True",
        "uniforms": PATH + "/materials/advanced.mat"
    },
    {
        "type":     "Material",
        "name":     "mat_basic",
        "shader":   "gl_basic",
        "defaults": "True",
        "uniforms": PATH + "/materials/basic.mat"
    },
    {
        "type":     "Material",
        "name":     "mat_geometry",
        "shader":   "gl_geometry",
        "defaults": "False",
        "uniforms": PATH + "/materials/geometry.mat"
    },
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


# Entities
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #
name = "ent_skybox"
if content.load({ "type": "Entity", "name": name }):
    if ecs.add_component(name, "struct ml::Renderer"):
        ecs.renderer_enable(name,  False)
        ecs.renderer_attrib(name, "material", "name", "mat_skybox")
        ecs.renderer_attrib(name, "model", "name", "default_skybox")
        ecs.renderer_attrib(name, "depth", "mask", "False")

name = "ent_earth"
if content.load({ "type": "Entity", "name": name }):
    if ecs.add_component(name, "struct ml::Renderer"):
        ecs.renderer_enable(name,  False)
        ecs.renderer_attrib(name, "material", "name", "mat_advanced")
        ecs.renderer_attrib(name, "model", "name", "obj_sphere32x24")

name = "ent_sanic"
if content.load({ "type": "Entity", "name": name }):
    if ecs.add_component(name, "struct ml::Renderer"):
        ecs.renderer_enable(name, True)
        ecs.renderer_attrib(name, "material", "name", "mat_basic")
        ecs.renderer_attrib(name, "model", "name", "default_quad")
        ecs.renderer_attrib(name, "cull", "enabled", "False")
