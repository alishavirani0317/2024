from flask import Blueprint, render_template

auth = Blueprint('auth', __name__)

# want to make sure login and signup r able to accept post requests
@auth.route('/login', methods=['GET', 'POST'])

def login():
    return render_template("login.html", boolean=True)

@auth.route('/logout')
def logout():
    return render_template("logout.html")

@auth.route('/sign-up', methods=['GET', 'POST'])
def sign_up():
    return render_template("signup.html")

