# store standards routes where users can actually go to 

from flask import Blueprint, render_template, request, flash, jsonify
from flask_login import login_required, current_user
from .models import Note
from . import db
import json

# this file is blueprint of our apps (has a ton of urls in it)

views = Blueprint('views', __name__)

# define route (view)
@views.route('/', methods=['GET', 'POST'])
@login_required # cannot get to home page unless ur logged in
def home(): 
    if request.method == 'POST':
        note = request.form.get('note')
        if len(note) < 1:
            flash('Note is too short!', category='error') 
        else:
            new_note = Note(data=note, user_id=current_user.id)  #providing the schema for the note 
            db.session.add(new_note) #adding the note to the database 
            db.session.commit()
            flash('Note added!', category='success')
            
    return render_template("home.html", user=current_user)
