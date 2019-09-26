# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import ml_content
import ml_io
import ml_window

def load_shader(name, file0 = None, file1 = None, file2 = None):
    if not name: return False
    elif (not file0 and not file1 and not file2): return ml_content.load({ "type": "Shader", "name": name })
    elif (file0 and not file1 and not file2): return ml_content.load({ "type": "Shader", "name": name, "file": file0 })
    elif (file0 and file1 and not file2): return ml_content.load({ "type": "Shader", "name": name, "vert": file0, "frag": file1 })
    elif (file0 and file1 and file2): return ml_content.load({ "type": "Shader", "name": name, "vert": file0, "frag": file1, "geom": file2 })
    else: return False

if ml_window.is_open():
    ml_io.printl("Window Information")
    ml_io.printf(" - Title:    {0}\n", [ ml_window.get_title() ])
    ml_io.printf(" - Position: {0}\n", [ str(ml_window.get_position()) ])
    ml_io.printf(" - Size:     {0}\n", [ str(ml_window.get_size()) ])
    ml_io.printf(" - Cursor:   {0}\n", [ str(ml_window.get_cursor()) ])
    ml_io.printf(" - Time:     {0}\n", [ str(ml_window.get_time()) ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #