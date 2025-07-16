# how u initialize flask
from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from os import path

db = SQLAlchemy() # define new databse
DB_NAME = "database.db"


def create_app():
    app = Flask(__name__) # reps name of file ran
    app.config['SECRET_KEY'] = 'alisha' # encrypts/secures cookies + session data related to website
    app.config['SQLALCHEMY_DATABASE_URI'] = f'sqlite:///{DB_NAME}'
    db.init_app(app)
    

    from .views import views
    from .auth import auth

    app.register_blueprint(views, url_prefix='/')
    app.register_blueprint(auth, url_prefix='/')

    # create db
    from .models import User, Note 

    create_database(app)
    
    return app

def create_database(app):
    # check if db exists (if not we will create it)
    if not path.exists('website/' + DB_NAME):
        with app.app_context():
            db.create_all()
            print('Created Database!')