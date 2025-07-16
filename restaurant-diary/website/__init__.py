# how u initialize flask
from flask import Flask
from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy() # define new databse
DB_NAME = "databse.db"


def create_app():
    app = Flask(__name__) # reps name of file ran
    app.config['SECRET_KEY'] = 'alisha' # encrypts/secures cookies + session data related to website
    app.config['SQLALCHEMY_DATABASE_URI'] = f'sqlite:///{DB_NAME}'
    db.init_app(app)
    

    from .views import views
    from .auth import auth

    app.register_blueprint(views, url_prefix='/')
    app.register_blueprint(auth, url_prefix='/')

    return app