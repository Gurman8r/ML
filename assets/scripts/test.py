# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

if ml.window.is_open():
    ml.io.printl("Window Info")
    ml.io.printf(" - Title:    {0}\n", [ ml.window.get_title() ])
    ml.io.printf(" - Position: {0}\n", [ str(ml.window.get_position()) ])
    ml.io.printf(" - Size:     {0}\n", [ str(ml.window.get_size()) ])
    ml.io.printf(" - Cursor:   {0}\n", [ str(ml.window.get_cursor()) ])
    ml.io.printf(" - Time:     {0}\n", [ str(ml.window.get_time()) ])

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #