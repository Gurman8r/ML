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
    io.printf("Window Title:    \'{0}\'\n", [ window.get_title() ])
    io.printf("Window Position: {0}\n", [ str([ window.get_x(), window.get_y() ]) ])
    io.printf("Window Size:     {0}\n", [ str([ window.get_w(), window.get_h() ]) ])
    io.printf("Cursor Position: {0}\n", [ str([ window.get_cx(), window.get_cy() ]) ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #