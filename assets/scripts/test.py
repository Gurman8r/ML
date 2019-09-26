# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

def load_shader(name, file0 = None, file1 = None, file2 = None):
    if not name: return False
    elif (not file0 and not file1 and not file2): return content.load({ "type": "Shader", "name": name })
    elif (file0 and not file1 and not file2): return content.load({ "type": "Shader", "name": name, "file": file0 })
    elif (file0 and file1 and not file2): return content.load({ "type": "Shader", "name": name, "vert": file0, "frag": file1 })
    elif (file0 and file1 and file2): return content.load({ "type": "Shader", "name": name, "vert": file0, "frag": file1, "geom": file2 })
    else: return False

if window.is_open():
    io.printl("Window Information")
    io.printf(" - Title:    {0}\n", [ window.get_title() ])
    io.printf(" - Position: {0}\n", [ str(window.get_position()) ])
    io.printf(" - Size:     {0}\n", [ str(window.get_size()) ])
    io.printf(" - Cursor:   {0}\n", [ str(window.get_cursor()) ])
    io.printf(" - Time:     {0}\n", [ str(window.get_time()) ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #