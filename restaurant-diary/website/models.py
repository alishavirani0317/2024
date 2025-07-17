#database models go here
from . import db  
from flask_login import UserMixin
from sqlalchemy.sql import func

class RestaurantEntry(db.Model):
    #all notes need to look like this
    id = db.Column(db.Integer, primary_key=True)
    restaurant_name = db.Column(db.String(255), nullable=False)
    rating = db.Column(db.Integer, nullable=False)
    notes = db.Column(db.String(10000))
    visit_date = db.Column(db.DateTime, default=func.now())
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'))


class User(db.Model, UserMixin):
    #all users need to look like this
    # define all cols we want to have stored in this table
    id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(150), unique=True) # no email can have same email as other user
    password = db.Column(db.String(150))
    first_name = db.Column(db.String(150))
    entries = db.relationship('RestuarantEntry') # everytime we can make a note, add into this users' notes relationship that notes id
