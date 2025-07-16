# store standards routes where users can actually go to 

from flask import Blueprint, render_template
from flask_login import login_required, current_user

# this file is blueprint of our apps (has a ton of urls in it)

views = Blueprint('views', __name__)

# define route (view)
@views.route('/')
@login_required # cannot get to home page unless ur logged in
def home(): 
    return render_template("home.html")