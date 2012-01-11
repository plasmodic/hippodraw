def fileList( files ):
    import os
    files = os.popen("ls -1 " + files).readlines()
    return [file.strip() for file in files]
