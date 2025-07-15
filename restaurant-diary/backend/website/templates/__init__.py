# how u initialize flask

from flask import Flask

def create_app():
    app = Flask(__name__) # reps name of file ran
    app.config['SECRET_KEY'] = 'alisha' # encrypts/secures cookies + session data related to website
    return app