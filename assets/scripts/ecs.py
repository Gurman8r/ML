import memelib_content

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Component:
    def __init__(self, type):
        self.type = str(type)

class Renderer(Component):
    def __init__(self):
        Component.__init__(self, "struct ml::Renderer")

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

class Entity:
    def __init__(self, name):
        self.name = str(name)
        self.components = list([])
    def create(self):
        return bool(memelib_content.create("Entity", self.name))
    def destroy(self):
        return bool(memelib_content.destroy("Entity", self.name))
    def attach(self, value):
        if isinstance(value, Component):
            if not value in self.components:
                if memelib_content.add_component(self.name, value.type):
                    return value
        return None

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #

if (__name__ == "__main__"):
    e = Entity("MyEntity")
    if e.create():
        r = e.attach(Renderer())

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * #