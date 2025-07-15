# store standards routes where users can actually go to 

from flask import Blueprint
# this file is blueprint of our apps (has a ton of urls in it)

views = Blueprint('views', __name__)

# define route (view)
@views.route('/')
def home():
    return "<h1> Test </h1>"