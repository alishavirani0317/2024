#database models go here
from . import db  
from flask_login import UserMixin
from sqlalchemy.sql import func

class Note(db.Model):
    #all notes need to look like this
    id = db.Column(db.Integer, primary_key=True)
    data = db.Column(db.String(10000))
    date = db.Column(db.DateTime(timezone=True), default= func.now())
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'))

class User(db.Model, UserMixin):
    #all users need to look like this
    # define all cols we want to have stored in this table
    id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(150), unique=True) # no email can have same email as other user
    password = db.Column(db.String(150))
    first_name = db.Column(db.String(150))
    notes = db.relationship('Note') # everytime we can make a note, add into this users' notes relationship that notes id
