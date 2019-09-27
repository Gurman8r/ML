# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

def load_shader(name, file0 = None, file1 = None, file2 = None):
    if not name: return False
    elif (not file0 and not file1 and not file2): return ml.content.load({ "type": "Shader", "name": name })
    elif (file0 and not file1 and not file2): return ml.content.load({ "type": "Shader", "name": name, "file": file0 })
    elif (file0 and file1 and not file2): return ml.content.load({ "type": "Shader", "name": name, "vert": file0, "frag": file1 })
    elif (file0 and file1 and file2): return ml.content.load({ "type": "Shader", "name": name, "vert": file0, "frag": file1, "geom": file2 })
    else: return False

if ml.window.is_open():
    ml.io.printl("Window Information")
    ml.io.printf(" - Title:    {0}\n", [ ml.window.get_title() ])
    ml.io.printf(" - Position: {0}\n", [ str(ml.window.get_position()) ])
    ml.io.printf(" - Size:     {0}\n", [ str(ml.window.get_size()) ])
    ml.io.printf(" - Cursor:   {0}\n", [ str(ml.window.get_cursor()) ])
    ml.io.printf(" - Time:     {0}\n", [ str(ml.window.get_time()) ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #