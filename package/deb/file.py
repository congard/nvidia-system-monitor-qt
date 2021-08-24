class file:
    def __init__(self, dir: str, description: str = "", permission = -1):
        self.dir = dir
        self.description = description
        self.permission = permission

    def get_dir(self):
        return self.dir

    def get_description(self):
        return self.description

    def get_permission(self):
        return self.permission

    def has_description(self):
        return bool(self.description)

    def has_permission(self):
        return self.permission != -1
