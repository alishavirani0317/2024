from website import create_app
#we can import this bc website is a python package (__init__.py)

app = create_app()

if __name__ == '__main__':
    # only if we run this file (NOT IMPORT IT), line 9 will run 
    app.run(debug=True) # run flask application, start up web server
    #debug=True -> everytime we make a change it automatically reruns web server
     