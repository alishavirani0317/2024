# how u initialize flask

from flask import Flask

def create_app():
    app = Flask(__name__) # reps name of file ran
    app.config['SECRET_KEY'] = 'alisha' # encrypts/secures cookies + session data related to website

    from .views import views
    from .auth import auth

    app.register_blueprint(views, url_prefix='/')
    app.register_blueprint(auth, url_prefix='/')

    return app