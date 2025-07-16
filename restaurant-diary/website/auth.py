from flask import Blueprint, render_template, request, flash, redirect, url_for
from .models import User
from werkzeug.security import generate_password_hash, check_password_hash #secures password
from . import db
from flask_login import login_user, login_required, logout_user, current_user

auth = Blueprint('auth', __name__)

# want to make sure login and signup r able to accept post requests
@auth.route('/login', methods=['GET', 'POST'])

def login():
    # if we r actually signing in (not just getting the page)
    if request.method == 'POST':
        email = request.form.get('email')
        password = request.form.get('password')
        
        # check if user name typed in form, is valid
        # use filter_by to filter for specific cold 
        user = User.query.filter_by(email=email).first()
        if user:
            if check_password_hash(user.password, password):
                flash('Logged in successfully!', category='success')
                # redirect user to home & login user
                login_user(user, remember=True)
                return redirect.url_for('views.home')
            else: 
                flash('Incorrect password, try again.', category='error')
        else:
            flash('Email does not exist.', category='error')


    return render_template("login.html", boolean=True)

@auth.route('/logout')
@login_required # cannot access this page unless user is logged in
def logout():
    logout_user()
    return redirect(url_for('auth.login'))

@auth.route('/sign-up', methods=['GET', 'POST'])

def sign_up():
    if request.method == 'POST':
        email = request.form.get('email')
        first_name = request.form.get('firstName')
        password1 = request.form.get('password1')
        password2 = request.form.get('password2')
        
        existing_user = User.query.filter_by(email=email).first()

        if existing_user:
            flash('Email already exists. Please log in or use another email.', category='error')

        elif len(email) < 4:
            #alert user if something went wrong (message flashing (import flash))
            flash('Email must be greater than 3 characters.', category='error')

        
        elif len(first_name) < 2:
            flash('First name must be greater than 1 character.', category='error')

        elif password1 != password2:
            flash('Passwords do not match.', category='error')
        elif len(password1) < 7:
            flash('Password must be at least 7 characters', category='error')
        else:
            #creating new user
            new_user = User(email=email, first_name=first_name, password=generate_password_hash(password1, method='pbkdf2:sha256'))
            # add this to db
            db.session.add(new_user)
            db.session.commit()
            login_user(new_user, remember=True)
            flash ('Account created!', category='success')
            #redirect user to homepage
            return redirect(url_for('views.home')) # blueprint + func name
        


    return render_template("signup.html", user=current_user)


