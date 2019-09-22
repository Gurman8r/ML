# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import memelib_config   as config
import memelib_content  as content
import memelib_ecs      as ecs
import memelib_io       as io
import memelib_plugins  as plugins
import memelib_prefs    as prefs
import memelib_window   as window

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

if window.is_open():
    io.printl("Window Debug Information")
    io.printf(" - Title:    {0}\n", [ window.get_title() ])
    io.printf(" - Position: {0}\n", [ str(window.get_position()) ])
    io.printf(" - Size:     {0}\n", [ str(window.get_size()) ])
    io.printf(" - Cursor:   {0}\n", [ str(window.get_cursor()) ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #