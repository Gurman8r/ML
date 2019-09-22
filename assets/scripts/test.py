# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

import memelib_io as io
import memelib_window as window

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

if window.is_open():
    io.printl("Window Information")
    io.printf(" - Title:    {0}\n", [ window.get_title() ])
    io.printf(" - Position: {0}\n", [ str(window.get_position()) ])
    io.printf(" - Size:     {0}\n", [ str(window.get_size()) ])
    io.printf(" - Cursor:   {0}\n", [ str(window.get_cursor()) ])
    io.printf(" - Time:     {0}\n", [ str(window.get_time()) ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #